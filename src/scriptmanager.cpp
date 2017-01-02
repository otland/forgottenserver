/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#include "scriptmanager.h"

#include "actions.h"
#include "chat.h"
#include "talkaction.h"
#include "spells.h"
#include "movement.h"
#include "weapons.h"
#include "globalevent.h"
#include "events.h"
#include "configmanager.h"
#include "game.h"

Actions* g_actions = nullptr;
CreatureEvents* g_creatureEvents = nullptr;
Chat* g_chat = nullptr;
Events* g_events = nullptr;
GlobalEvents* g_globalEvents = nullptr;
Spells* g_spells = nullptr;
TalkActions* g_talkActions = nullptr;
MoveEvents* g_moveEvents = nullptr;
Weapons* g_weapons = nullptr;

extern LuaEnvironment g_luaEnvironment;
extern ConfigManager g_config;
extern Game g_game;
extern Monsters g_monsters;

ScriptingManager::~ScriptingManager()
{
	delete g_events;
	delete g_weapons;
	delete g_spells;
	delete g_actions;
	delete g_talkActions;
	delete g_moveEvents;
	delete g_chat;
	delete g_creatureEvents;
	delete g_globalEvents;
}

bool ScriptingManager::loadScriptSystems()
{
	if (g_luaEnvironment.loadFile("data/global.lua") == -1) {
		std::cout << "[Warning - ScriptingManager::loadScriptSystems] Can not load data/global.lua" << std::endl;
	}

	g_chat = new Chat();

	g_weapons = new Weapons();
	if (!g_weapons->loadFromXml()) {
		std::cout << "> ERROR: Unable to load weapons!" << std::endl;
		return false;
	}

	g_weapons->loadDefaults();

	g_spells = new Spells();
	if (!g_spells->loadFromXml()) {
		std::cout << "> ERROR: Unable to load spells!" << std::endl;
		return false;
	}

	g_actions = new Actions();
	if (!g_actions->loadFromXml()) {
		std::cout << "> ERROR: Unable to load actions!" << std::endl;
		return false;
	}

	g_talkActions = new TalkActions();
	if (!g_talkActions->loadFromXml()) {
		std::cout << "> ERROR: Unable to load talk actions!" << std::endl;
		return false;
	}

	g_moveEvents = new MoveEvents();
	if (!g_moveEvents->loadFromXml()) {
		std::cout << "> ERROR: Unable to load move events!" << std::endl;
		return false;
	}

	g_creatureEvents = new CreatureEvents();
	if (!g_creatureEvents->loadFromXml()) {
		std::cout << "> ERROR: Unable to load creature events!" << std::endl;
		return false;
	}

	g_globalEvents = new GlobalEvents();
	if (!g_globalEvents->loadFromXml()) {
		std::cout << "> ERROR: Unable to load global events!" << std::endl;
		return false;
	}

	g_events = new Events();
	if (!g_events->load()) {
		std::cout << "> ERROR: Unable to load events!" << std::endl;
		return false;
	}

	return true;
}

void ScriptingManager::reloadInterface(ReloadInterface_t interfaceId) {
	switch (interfaceId) {
		case ACTIONS:
			g_actions->reload();
			break;

		case CONFIG:
			g_config.reload();
			break;

		case CREATUREEVENTS:
			g_creatureEvents->reload();
			break;

		case MOVEEVENTS:
			g_moveEvents->reload();
			break;

		case NPCS:
			Npcs::reload();
			break;

		case RAIDS:
			g_game.raids.reload();
			g_game.raids.startup();
			break;

		case SPELLS:
			g_spells->reload();

		case MONSTERS:
			g_monsters.reload();
			break;

		case TALKACTIONS:
			g_talkActions->reload();
			break;

		case ITEMS:
			Item::items.reload();
			break;

		case WEAPONS:
			g_weapons->reload();
			g_weapons->loadDefaults();
			break;

		case QUESTS:
			g_game.quests.reload();
			break;

		case MOUNTS:
			g_game.mounts.reload();
			break;

		case GLOBALEVENTS:
			g_globalEvents->reload();
			break;

		case EVENTS:
			g_events->load();
			break;

		case CHAT:
			g_chat->load();
			break;

		case GLOBAL:
			g_luaEnvironment.loadFile("data/global.lua");
			break;

		default:
			break;
	}
}