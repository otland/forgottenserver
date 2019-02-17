/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "server.h"

#include "game.h"

#include "iomarket.h"

#include "configmanager.h"
#include "scriptmanager.h"
#include "rsa.h"
#include "protocolold.h"
#include "protocollogin.h"
#include "protocolstatus.h"
#include "databasemanager.h"
#include "scheduler.h"
#include "databasetasks.h"
#include <fstream>

DatabaseTasks g_databaseTasks;
Dispatcher g_dispatcher;
Scheduler g_scheduler;

Game g_game;
ConfigManager g_config;
Monsters g_monsters;
Vocations g_vocations;
RSA g_RSA;

std::mutex g_loaderLock;
std::condition_variable g_loaderSignal;
std::unique_lock<std::mutex> g_loaderUniqueLock(g_loaderLock);

void startupErrorMessage(const std::string& errorStr)
{
	std::cout << "> ERROR: " << errorStr << std::endl;
	g_loaderSignal.notify_all();
}

void mainLoader(int argc, char* argv[], ServiceManager* services);

[[noreturn]] void badAllocationHandler()
{
	// Use functions that only use stack allocation
	puts("Allocation failed, server out of memory.\nDecrease the size of your map or compile in 64 bits mode.\n");
	getchar();
	exit(-1);
}

int main(int argc, char* argv[])
{
	// Setup bad allocation handler
	std::set_new_handler(badAllocationHandler);

	ServiceManager serviceManager;

	g_dispatcher.start();
	g_scheduler.start();

	g_dispatcher.addTask(createTask(std::bind(mainLoader, argc, argv, &serviceManager)));

	g_loaderSignal.wait(g_loaderUniqueLock);

	if (serviceManager.is_running()) {
		std::cout << ">> " << g_config.getString(ConfigManager::SERVER_NAME) << " Server Online!" << std::endl << std::endl;
		serviceManager.run();
	} else {
		std::cout << ">> No services running. The server is NOT online." << std::endl;
		g_scheduler.shutdown();
		g_databaseTasks.shutdown();
		g_dispatcher.shutdown();
	}

	g_scheduler.join();
	g_databaseTasks.join();
	g_dispatcher.join();
	return 0;
}

void mainLoader(int, char*[], ServiceManager* services)
{
	//dispatcher thread
	g_game.setGameState(GAME_STATE_STARTUP);

	srand(static_cast<unsigned int>(OTSYS_TIME()));
#ifdef _WIN32
	SetConsoleTitle(STATUS_SERVER_NAME);
#endif
	std::cout << STATUS_SERVER_NAME << " - Version " << STATUS_SERVER_VERSION << std::endl;
	std::cout << "Compiled with " << BOOST_COMPILER << std::endl;
	std::cout << "Compiled on " << __DATE__ << ' ' << __TIME__ << " for platform ";

#if defined(__amd64__) || defined(_M_X64)
	std::cout << "x64" << std::endl;
#elif defined(__i386__) || defined(_M_IX86) || defined(_X86_)
	std::cout << "x86" << std::endl;
#elif defined(__arm__)
	std::cout << "ARM" << std::endl;
#else
	std::cout << "unknown" << std::endl;
#endif
	std::cout << std::endl;

	std::cout << "A server developed by " << STATUS_SERVER_DEVELOPERS << std::endl;
	std::cout << "Visit our forum for updates, support, and resources: http://otland.net/." << std::endl;
	std::cout << std::endl;

	// check if config.lua or config.lua.dist exist
	std::ifstream c_test("./config.lua");
	if (!c_test.is_open()) {
		std::ifstream config_lua_dist("./config.lua.dist");
		if (config_lua_dist.is_open()) {
			std::cout << ">> copying config.lua.dist to config.lua" << std::endl;
			std::ofstream config_lua("config.lua");
			config_lua << config_lua_dist.rdbuf();
			config_lua.close();
			config_lua_dist.close();
		}
	} else {
		c_test.close();
	}

	// read global config
	std::cout << ">> Loading config" << std::endl;
	if (!g_config.load()) {
		startupErrorMessage("Unable to load config.lua!");
		return;
	}

#ifdef _WIN32
	const std::string& defaultPriority = g_config.getString(ConfigManager::DEFAULT_PRIORITY);
	if (strcasecmp(defaultPriority.c_str(), "high") == 0) {
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	} else if (strcasecmp(defaultPriority.c_str(), "above-normal") == 0) {
		SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
	}
#endif

	//set RSA key
	try {
		g_RSA.loadPEM("key.pem");
	} catch(const std::exception& e) {
		startupErrorMessage(e.what());
		return;
	}

	std::cout << ">> Establishing database connection..." << std::flush;

	if (!Database::getInstance().connect()) {
		startupErrorMessage("Failed to connect to database.");
		return;
	}

	std::cout << " MySQL " << Database::getClientVersion() << std::endl;

	// run database manager
	std::cout << ">> Running database manager" << std::endl;

	if (!DatabaseManager::isDatabaseSetup()) {
		startupErrorMessage("The database you have specified in config.lua is empty, please import the schema.sql to your database.");
		return;
	}
	g_databaseTasks.start();

	DatabaseManager::updateDatabase();

	if (g_config.getBoolean(ConfigManager::OPTIMIZE_DATABASE) && !DatabaseManager::optimizeTables()) {
		std::cout << "> No tables were optimized." << std::endl;
	}

	//load vocations
	std::cout << ">> Loading vocations" << std::endl;
	if (!g_vocations.loadFromXml()) {
		startupErrorMessage("Unable to load vocations!");
		return;
	}

	// load item data
	std::cout << ">> Loading items" << std::endl;
	if (!Item::items.loadFromOtb("data/items/items.otb")) {
		startupErrorMessage("Unable to load items (OTB)!");
		return;
	}

	if (!Item::items.loadFromXml()) {
		startupErrorMessage("Unable to load items (XML)!");
		return;
	}

	std::cout << ">> Loading script systems" << std::endl;
	if (!ScriptingManager::getInstance().loadScriptSystems()) {
		startupErrorMessage("Failed to load script systems");
		return;
	}

	std::cout << ">> Loading monsters" << std::endl;
	if (!g_monsters.loadFromXml()) {
		startupErrorMessage("Unable to load monsters!");
		return;
	}

	std::cout << ">> Loading outfits" << std::endl;
	if (!Outfits::getInstance().loadFromXml()) {
		startupErrorMessage("Unable to load outfits!");
		return;
	}

	std::cout << ">> Checking world type... " << std::flush;
	std::string worldType = asLowerCaseString(g_config.getString(ConfigManager::WORLD_TYPE));
	if (worldType == "pvp") {
		g_game.setWorldType(WORLD_TYPE_PVP);
	} else if (worldType == "no-pvp") {
		g_game.setWorldType(WORLD_TYPE_NO_PVP);
	} else if (worldType == "pvp-enforced") {
		g_game.setWorldType(WORLD_TYPE_PVP_ENFORCED);
	} else {
		std::cout << std::endl;

		std::ostringstream ss;
		ss << "> ERROR: Unknown world type: " << g_config.getString(ConfigManager::WORLD_TYPE) << ", valid world types are: pvp, no-pvp and pvp-enforced.";
		startupErrorMessage(ss.str());
		return;
	}
	std::cout << asUpperCaseString(worldType) << std::endl;

	std::cout << ">> Loading map" << std::endl;
	if (!g_game.loadMainMap(g_config.getString(ConfigManager::MAP_NAME))) {
		startupErrorMessage("Failed to load map");
		return;
	}

	std::cout << ">> Initializing gamestate" << std::endl;
	g_game.setGameState(GAME_STATE_INIT);

	// Game client protocols
	services->add<ProtocolGame>(g_config.getNumber(ConfigManager::GAME_PORT));
	services->add<ProtocolLogin>(g_config.getNumber(ConfigManager::LOGIN_PORT));

	// OT protocols
	services->add<ProtocolStatus>(g_config.getNumber(ConfigManager::STATUS_PORT));

	// Legacy login protocol
	services->add<ProtocolOld>(g_config.getNumber(ConfigManager::LOGIN_PORT));

	RentPeriod_t rentPeriod;
	std::string strRentPeriod = asLowerCaseString(g_config.getString(ConfigManager::HOUSE_RENT_PERIOD));

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

	IOMarket::checkExpiredOffers();
	IOMarket::getInstance().updateStatistics();

	std::cout << ">> Loaded all modules, server starting up..." << std::endl;

#ifndef _WIN32
	if (getuid() == 0 || geteuid() == 0) {
		std::cout << "> Warning: " << STATUS_SERVER_NAME << " has been executed as root user, please consider running it as a normal user." << std::endl;
	}
#endif

	g_game.start(services);
	g_game.setGameState(GAME_STATE_NORMAL);
	g_loaderSignal.notify_all();
}
