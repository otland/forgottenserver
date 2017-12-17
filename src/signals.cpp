/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017 Mark Samman <mark.samman@gmail.com>
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
#include <csignal>

#include "signals.h"
#include "tasks.h"
#include "game.h"
#include "actions.h"
#include "configmanager.h"
#include "spells.h"
#include "talkaction.h"
#include "movement.h"
#include "weapons.h"
#include "raids.h"
#include "quests.h"
#include "mounts.h"
#include "globalevent.h"
#include "monster.h"
#include "events.h"


extern Dispatcher g_dispatcher;

extern ConfigManager g_config;
extern Actions* g_actions;
extern Monsters g_monsters;
extern TalkActions* g_talkActions;
extern MoveEvents* g_moveEvents;
extern Spells* g_spells;
extern Weapons* g_weapons;
extern Game g_game;
extern CreatureEvents* g_creatureEvents;
extern GlobalEvents* g_globalEvents;
extern Events* g_events;
extern Chat* g_chat;
extern LuaEnvironment g_luaEnvironment;

using ErrorCode = boost::system::error_code;

Signals::Signals(boost::asio::io_service& service) :
	set(service)
{
	set.add(SIGINT);
	set.add(SIGTERM);
#ifndef _WIN32
	set.add(SIGUSR1);
	set.add(SIGHUP);
#endif

	asyncWait();
}

void Signals::asyncWait()
{
	set.async_wait([this] (ErrorCode err, int signal) {
		if (err) {
			std::cerr << "Signal handling error: "  << err.message() << std::endl;
			return;
		}
		dispatchSignalHandler(signal);
		asyncWait();
	});
}

void Signals::dispatchSignalHandler(int signal)
{
	switch(signal) {
		case SIGINT: //Shuts the server down
			g_dispatcher.addTask(createTask(sigintHandler));
			break;
		case SIGTERM: //Shuts the server down
			g_dispatcher.addTask(createTask(sigtermHandler));
			break;
#ifndef _WIN32
		case SIGHUP: //Reload config/data
			g_dispatcher.addTask(createTask(sighupHandler));
			break;
		case SIGUSR1: //Saves game state
			g_dispatcher.addTask(createTask(sigusr1Handler));
			break;
#endif
		default:
			break;
	}
}

void Signals::sigtermHandler()
{
	//Dispatcher thread
	std::cout << "SIGTERM received, shutting game server down..." << std::endl;
	g_game.setGameState(GAME_STATE_SHUTDOWN);
}

void Signals::sigusr1Handler()
{
	//Dispatcher thread
	std::cout << "SIGUSR1 received, saving the game state..." << std::endl;
	g_game.saveGameState();
}

void Signals::sighupHandler()
{
	//Dispatcher thread
	std::cout << "SIGHUP received, reloading config files..." << std::endl;

	g_actions->reload();
	std::cout << "Reloaded actions." << std::endl;

	g_config.reload();
	std::cout << "Reloaded config." << std::endl;

	g_creatureEvents->reload();
	std::cout << "Reloaded creature scripts." << std::endl;

	g_moveEvents->reload();
	std::cout << "Reloaded movements." << std::endl;

	Npcs::reload();
	std::cout << "Reloaded npcs." << std::endl;

	g_game.raids.reload();
	g_game.raids.startup();
	std::cout << "Reloaded raids." << std::endl;

	g_spells->reload();
	std::cout << "Reloaded monsters." << std::endl;

	g_monsters.reload();
	std::cout << "Reloaded spells." << std::endl;

	g_talkActions->reload();
	std::cout << "Reloaded talk actions." << std::endl;

	Item::items.reload();
	std::cout << "Reloaded items." << std::endl;

	g_weapons->reload();
	g_weapons->loadDefaults();
	std::cout << "Reloaded weapons." << std::endl;

	g_game.quests.reload();
	std::cout << "Reloaded quests." << std::endl;

	g_game.mounts.reload();
	std::cout << "Reloaded mounts." << std::endl;

	g_globalEvents->reload();
	std::cout << "Reloaded globalevents." << std::endl;

	g_events->load();
	std::cout << "Reloaded events." << std::endl;

	g_chat->load();
	std::cout << "Reloaded chatchannels." << std::endl;

	g_luaEnvironment.loadFile("data/global.lua");
	std::cout << "Reloaded global.lua." << std::endl;

	lua_gc(g_luaEnvironment.getLuaState(), LUA_GCCOLLECT, 0);
}

void Signals::sigintHandler()
{
	//Dispatcher thread
	std::cout << "SIGINT received, shutting game server down..." << std::endl;
	g_game.setGameState(GAME_STATE_SHUTDOWN);
}
