// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "otserv.h"

#include "configmanager.h"
#include "databasemanager.h"
#include "databasetasks.h"
#include "game.h"
#include "http/http.h"
#include "iomarket.h"
#include "logger.h"
#include "monsters.h"
#include "outfit.h"
#include "protocollogin.h"
#include "protocolold.h"
#include "protocolstatus.h"
#include "rsa.h"
#include "scheduler.h"
#include "script.h"
#include "scriptmanager.h"
#include "server.h"

#include <fstream>

#if __has_include("gitmetadata.h")
#include "gitmetadata.h"
#endif

DatabaseTasks g_databaseTasks;
Dispatcher g_dispatcher;
Scheduler g_scheduler;

Game g_game;
Monsters g_monsters;
Vocations g_vocations;
extern Scripts* g_scripts;

std::mutex g_loaderLock;
std::condition_variable g_loaderSignal;
std::unique_lock<std::mutex> g_loaderUniqueLock(g_loaderLock);

namespace {

void startupErrorMessage(const std::string& errorStr)
{
	getLogger().error("{}", errorStr);
	g_loaderSignal.notify_all();
}

void mainLoader(ServiceManager* services)
{
	// dispatcher thread
	g_game.setGameState(GAME_STATE_STARTUP);
	if (!initLogger(LogLevel::INFO)) {
		startupErrorMessage("Failed to initialize logger!");
		return;
	}

	setupLoggerSignalHandlers();

	srand(static_cast<unsigned int>(OTSYS_TIME()));
#ifdef _WIN32
	SetConsoleTitle(STATUS_SERVER_NAME);

	// fixes a problem with escape characters not being processed in Windows consoles
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
#endif

	printServerVersion();

	// check if config.lua or config.lua.dist exist
	const std::string& configFile = getString(ConfigManager::CONFIG_FILE);
	std::ifstream c_test("./" + configFile);
	if (!c_test.is_open()) {
		std::ifstream config_lua_dist("./config.lua.dist");
		if (config_lua_dist.is_open()) {
			getLogger().info("copying config.lua.dist to {}", configFile);
			std::ofstream config_lua(configFile);
			config_lua << config_lua_dist.rdbuf();
			config_lua.close();
			config_lua_dist.close();
		}
	} else {
		c_test.close();
	}

	// read global config
	getLogger().info("Loading config");
	if (!ConfigManager::load()) {
		startupErrorMessage("Unable to load " + configFile + "!");
		return;
	}
	getLogger().setLevel(parseLogLevel(getString(ConfigManager::LOG_LEVEL)));

#ifdef _WIN32
	const std::string& defaultPriority = getString(ConfigManager::DEFAULT_PRIORITY);
	if (caseInsensitiveEqual(defaultPriority, "high")) {
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	} else if (caseInsensitiveEqual(defaultPriority, "above-normal")) {
		SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
	}
#endif

	// set RSA key
	getLogger().info("Loading RSA key");
	try {
		std::ifstream key{"key.pem"};
		std::string pem{std::istreambuf_iterator<char>{key}, std::istreambuf_iterator<char>{}};
		tfs::rsa::loadPEM(pem);
	} catch (const std::exception& e) {
		startupErrorMessage(e.what());
		return;
	}


	if (!Database::getInstance().connect()) {
		startupErrorMessage("Failed to connect to database.");
		return;
	}
	getLogger().info("Establishing database connection...,MySQL {}", Database::getClientVersion());

	// run database manager
	getLogger().info("Running database manager");

	if (!DatabaseManager::isDatabaseSetup()) {
		startupErrorMessage(
		    "The database you have specified in config.lua is empty, please import the schema.sql to your database.");
		return;
	}
	g_databaseTasks.start();

	DatabaseManager::updateDatabase();

	if (getBoolean(ConfigManager::OPTIMIZE_DATABASE) && !DatabaseManager::optimizeTables()) {
		getLogger().info("  >> No tables were optimized.");
	}

	// load vocations
	getLogger().info("Loading vocations");
	if (std::ifstream is{"data/XML/vocations.xml"}; !g_vocations.loadFromXml(is, "data/XML/vocations.xml")) {
		startupErrorMessage("Unable to load vocations!");
		return;
	}

	// load item data
	if (!Item::items.loadFromOtb("data/items/items.otb")) {
		startupErrorMessage("Unable to load items (OTB)!");
		return;
	}
	getLogger().info("Loading items... OTB v{}.{}.{}", Item::items.majorVersion, Item::items.minorVersion, Item::items.buildNumber);

	if (!Item::items.loadFromXml()) {
		startupErrorMessage("Unable to load items (XML)!");
		return;
	}

	getLogger().info("Loading script systems");
	if (!ScriptingManager::getInstance().loadScriptSystems()) {
		startupErrorMessage("Failed to load script systems");
		return;
	}

	getLogger().info("Loading lua scripts");
	if (!g_scripts->loadScripts("scripts", false, false)) {
		startupErrorMessage("Failed to load lua scripts");
		return;
	}

	getLogger().info("Loading lua monsters");
	if (!g_monsters.loadFromXml()) {
		startupErrorMessage("Unable to load monsters!");
		return;
	}
	getLogger().info("Loading lua monsters");
	if (!g_scripts->loadScripts("monster", false, false)) {
		startupErrorMessage("Failed to load lua monsters");
		return;
	}

	getLogger().info("Loading outfits");
	if (!Outfits::getInstance().loadFromXml()) {
		startupErrorMessage("Unable to load outfits!");
		return;
	}

	std::string worldType = boost::algorithm::to_lower_copy(getString(ConfigManager::WORLD_TYPE));
	if (worldType == "pvp") {
		g_game.setWorldType(WORLD_TYPE_PVP);
	} else if (worldType == "no-pvp") {
		g_game.setWorldType(WORLD_TYPE_NO_PVP);
	} else if (worldType == "pvp-enforced") {
		g_game.setWorldType(WORLD_TYPE_PVP_ENFORCED);
	} else {
		std::cout << std::endl;
		startupErrorMessage(
		    fmt::format("Unknown world type: {:s}, valid world types are: pvp, no-pvp and pvp-enforced.",
		                getString(ConfigManager::WORLD_TYPE)));
		return;
	}
	getLogger().info("Checking world type... {}", boost::algorithm::to_upper_copy(worldType));

	getLogger().info("Loading map");
	if (!g_game.loadMainMap(getString(ConfigManager::MAP_NAME))) {
		startupErrorMessage("Failed to load map");
		return;
	}

	getLogger().info("Initializing gamestate");
	g_game.setGameState(GAME_STATE_INIT);

	// Game client protocols
	services->add<ProtocolGame>(static_cast<uint16_t>(getNumber(ConfigManager::GAME_PORT)));
	services->add<ProtocolLogin>(static_cast<uint16_t>(getNumber(ConfigManager::LOGIN_PORT)));

	// OT protocols
	services->add<ProtocolStatus>(static_cast<uint16_t>(getNumber(ConfigManager::STATUS_PORT)));

	// Legacy login protocol
	services->add<ProtocolOld>(static_cast<uint16_t>(getNumber(ConfigManager::LOGIN_PORT)));

#ifdef HTTP
	// HTTP server
	tfs::http::start(getBoolean(ConfigManager::BIND_ONLY_GLOBAL_ADDRESS), getString(ConfigManager::IP),
	                 getNumber(ConfigManager::HTTP_PORT), getNumber(ConfigManager::HTTP_WORKERS));
#endif

	RentPeriod_t rentPeriod;
	std::string strRentPeriod = boost::algorithm::to_lower_copy(getString(ConfigManager::HOUSE_RENT_PERIOD));

	if (strRentPeriod == "yearly") {
		rentPeriod = RENTPERIOD_YEARLY;
	} else if (strRentPeriod == "weekly") {
		rentPeriod = RENTPERIOD_WEEKLY;
	} else if (strRentPeriod == "monthly") {
		rentPeriod = RENTPERIOD_MONTHLY;
	} else if (strRentPeriod == "daily") {
		rentPeriod = RENTPERIOD_DAILY;
	} else {
		rentPeriod = RENTPERIOD_NEVER;
	}

	g_game.map.houses.payHouses(rentPeriod);

	tfs::iomarket::checkExpiredOffers();
	tfs::iomarket::updateStatistics();

	getLogger().info("Loaded all modules, server starting up...");

#ifndef _WIN32
	if (getuid() == 0 || geteuid() == 0) {
		getLogger().critical("Warning: {} has been executed as root user, please consider running it as a normal user.", STATUS_SERVER_NAME);
	}
#endif

	g_game.start(services);
	g_game.setGameState(GAME_STATE_NORMAL);
	g_loaderSignal.notify_all();
}

[[noreturn]] void badAllocationHandler()
{
	// Use functions that only use stack allocation
	puts("Allocation failed, server out of memory.\nDecrease the size of your map or compile in 64 bits mode.\n");
	getchar();
	exit(-1);
}

} // namespace

void startServer()
{
	// Setup bad allocation handler
	std::set_new_handler(badAllocationHandler);

	ServiceManager serviceManager;

	g_dispatcher.start();
	g_scheduler.start();

	g_dispatcher.addTask([services = &serviceManager]() { mainLoader(services); });

	g_loaderSignal.wait(g_loaderUniqueLock);

	if (serviceManager.is_running()) {
		getLogger().info("{}  Server Online!", getString(ConfigManager::SERVER_NAME));
		serviceManager.run();
	} else {
		getLogger().error("No services running. The server is NOT online.");
		g_scheduler.shutdown();
		g_databaseTasks.shutdown();
		g_dispatcher.shutdown();
	}

	g_scheduler.join();
	g_databaseTasks.join();
	g_dispatcher.join();
}

void printServerVersion()
{
#if defined(GIT_RETRIEVED_STATE) && GIT_RETRIEVED_STATE
	getLogger().info("{} - Version {}", STATUS_SERVER_NAME, GIT_DESCRIBE);
	getLogger().info("Git SHA1 {} dated {}", GIT_SHORT_SHA1, GIT_COMMIT_DATE_ISO8601);
#if GIT_IS_DIRTY
	getLogger().info("*** DIRTY - NOT OFFICIAL RELEASE ***");
#endif
#else
	getLogger().info("{} - Version {}", STATUS_SERVER_NAME, STATUS_SERVER_VERSION);
#endif
	std::cout << std::endl;

	getLogger().info("Compiled with {}", BOOST_COMPILER);
	getLogger().info("Compiled on {} {}", __DATE__, __TIME__);
#if defined(__amd64__) || defined(_M_X64)
	getLogger().info("for platform: x64");
#elif defined(__i386__) || defined(_M_IX86) || defined(_X86_)
	getLogger().info("for platform: x86");
#elif defined(__arm__)
	getLogger().info("for platform: ARM");
#else
	getLogger().warn("for platform: unknown");
#endif
#if defined(LUAJIT_VERSION)
	getLogger().info("Linked with {} for Lua support", LUAJIT_VERSION);
#else
	getLogger().info("Linked with {} for Lua support", LUA_RELEASE);
#endif
	std::cout << std::endl;

	getLogger().info("A server developed by {}", STATUS_SERVER_DEVELOPERS);
	getLogger().info("Visit our forum for updates, support, and resources: https://otland.net/.");
}
