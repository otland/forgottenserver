// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "signals.h"

#include "actions.h"
#include "configmanager.h"
#include "databasetasks.h"
#include "events.h"
#include "game.h"
#include "globalevent.h"
#include "monsters.h"
#include "mounts.h"
#include "movement.h"
#include "scheduler.h"
#include "spells.h"
#include "talkaction.h"
#include "tasks.h"
#include "weapons.h"

#include <csignal>

extern Scheduler g_scheduler;
extern DatabaseTasks g_databaseTasks;
extern Dispatcher g_dispatcher;

extern Actions* g_actions;
extern Monsters g_monsters;
extern TalkActions* g_talkActions;
extern MoveEvents* g_moveEvents;
extern Spells* g_spells;
extern Weapons* g_weapons;
extern Game g_game;
extern CreatureEvents* g_creatureEvents;
extern GlobalEvents* g_globalEvents;
extern Chat* g_chat;
extern LuaEnvironment g_luaEnvironment;

namespace {

#ifndef _WIN32
void sigusr1Handler()
{
	// Dispatcher thread
	std::cout << "SIGUSR1 received, saving the game state..." << std::endl;
	g_globalEvents->save();
	g_game.saveGameState();
}

void sighupHandler()
{
	// Dispatcher thread
	std::cout << "SIGHUP received, reloading config files..." << std::endl;

	g_actions->reload();
	std::cout << "Reloaded actions." << std::endl;

	ConfigManager::load();
	std::cout << "Reloaded config." << std::endl;

	g_creatureEvents->reload();
	std::cout << "Reloaded creature scripts." << std::endl;

	g_moveEvents->reload();
	std::cout << "Reloaded movements." << std::endl;

	Npcs::reload();
	std::cout << "Reloaded npcs." << std::endl;

	g_monsters.reload();
	std::cout << "Reloaded monsters." << std::endl;

	g_spells->reload();
	std::cout << "Reloaded spells." << std::endl;

	g_talkActions->reload();
	std::cout << "Reloaded talk actions." << std::endl;

	Item::items.reload();
	std::cout << "Reloaded items." << std::endl;

	g_weapons->reload();
	g_weapons->loadDefaults();
	std::cout << "Reloaded weapons." << std::endl;

	g_game.mounts.reload();
	std::cout << "Reloaded mounts." << std::endl;

	g_globalEvents->reload();
	std::cout << "Reloaded globalevents." << std::endl;

	tfs::events::reload();
	std::cout << "Reloaded events." << std::endl;

	g_chat->load();
	std::cout << "Reloaded chatchannels." << std::endl;

	g_luaEnvironment.loadFile("data/global.lua");
	std::cout << "Reloaded global.lua." << std::endl;

	lua_gc(g_luaEnvironment.getLuaState(), LUA_GCCOLLECT, 0);
}
#else
void sigbreakHandler()
{
	// Dispatcher thread
	std::cout << "SIGBREAK received, shutting game server down..." << std::endl;
	g_game.setGameState(GAME_STATE_SHUTDOWN);
}
#endif

void sigtermHandler()
{
	// Dispatcher thread
	std::cout << "SIGTERM received, shutting game server down..." << std::endl;
	g_game.setGameState(GAME_STATE_SHUTDOWN);
}

void sigintHandler()
{
	// Dispatcher thread
	std::cout << "SIGINT received, shutting game server down..." << std::endl;
	g_game.setGameState(GAME_STATE_SHUTDOWN);
}

// On Windows this function does not need to be signal-safe,
// as it is called in a new thread.
// https://github.com/otland/forgottenserver/pull/2473
void dispatchSignalHandler(int signal)
{
	switch (signal) {
		case SIGINT: // Shuts the server down
			g_dispatcher.addTask(sigintHandler);
			break;
		case SIGTERM: // Shuts the server down
			g_dispatcher.addTask(sigtermHandler);
			break;
#ifndef _WIN32
		case SIGHUP: // Reload config/data
			g_dispatcher.addTask(sighupHandler);
			break;
		case SIGUSR1: // Saves game state
			g_dispatcher.addTask(sigusr1Handler);
			break;
#else
		case SIGBREAK: // Shuts the server down
			g_dispatcher.addTask(sigbreakHandler);
			// hold the thread until other threads end
			g_scheduler.join();
			g_databaseTasks.join();
			g_dispatcher.join();
			break;
#endif
		default:
			break;
	}
}

} // namespace

Signals::Signals(boost::asio::io_context& ioc) : set(ioc)
{
	set.add(SIGINT);
	set.add(SIGTERM);
#ifndef _WIN32
	set.add(SIGUSR1);
	set.add(SIGHUP);
#else
	// This must be a blocking call as Windows calls it in a new thread and terminates the process when the handler
	// returns (or after 5 seconds, whichever is earlier). On Windows it is called in a new thread.
	signal(SIGBREAK, dispatchSignalHandler);
#endif

	asyncWait();
}

void Signals::asyncWait()
{
	set.async_wait([this](const boost::system::error_code& err, int signal) {
		if (err) {
			std::cerr << "Signal handling error: " << err.message() << std::endl;
			return;
		}
		dispatchSignalHandler(signal);
		asyncWait();
	});
}
