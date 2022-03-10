// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

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
#include "script.h"

Actions* g_actions = nullptr;
CreatureEvents* g_creatureEvents = nullptr;
Chat* g_chat = nullptr;
Events* g_events = nullptr;
GlobalEvents* g_globalEvents = nullptr;
Spells* g_spells = nullptr;
TalkActions* g_talkActions = nullptr;
MoveEvents* g_moveEvents = nullptr;
Weapons* g_weapons = nullptr;
Scripts* g_scripts = nullptr;

extern LuaEnvironment g_luaEnvironment;

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
	delete g_scripts;
}

bool ScriptingManager::loadScriptSystems()
{
	if (g_luaEnvironment.loadFile("data/global.lua") == -1) {
		std::cout << "[Warning - ScriptingManager::loadScriptSystems] Can not load data/global.lua" << std::endl;
	}

	g_scripts = new Scripts();
	std::cout << ">> Loading lua libs" << std::endl;
	if (!g_scripts->loadScripts("scripts/lib", true, false)) {
		std::cout << "> ERROR: Unable to load lua libs!" << std::endl;
		return false;
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
