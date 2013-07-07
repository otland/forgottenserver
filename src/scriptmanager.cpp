//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include "otpch.h"

#include "scriptmanager.h"
#include "luascript.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "actions.h"
#include "talkaction.h"
#include "spells.h"
#include "movement.h"
#include "weapons.h"
#include "creatureevent.h"
#include "globalevent.h"

Actions* g_actions = NULL;
CreatureEvents* g_creatureEvents = NULL;
GlobalEvents* g_globalEvents = NULL;
Spells* g_spells = NULL;
TalkActions* g_talkActions = NULL;
MoveEvents* g_moveEvents = NULL;
Weapons* g_weapons = NULL;

ScriptingManager* ScriptingManager::_instance = NULL;

ScriptingManager::ScriptingManager()
{
	g_weapons = new Weapons();
	g_spells = new Spells();
	g_actions = new Actions();
	g_talkActions = new TalkActions();
	g_moveEvents = new MoveEvents();
	g_creatureEvents = new CreatureEvents();
	g_globalEvents = new GlobalEvents();
}

ScriptingManager::~ScriptingManager()
{
	//
}

ScriptingManager* ScriptingManager::getInstance()
{
	if (_instance == NULL) {
		_instance = new ScriptingManager();
	}

	return _instance;
}

bool ScriptingManager::loadScriptSystems()
{
	if (!g_weapons->loadFromXml()) {
		std::cout << "> ERROR: Unable to load Weapons!" << std::endl;
		return false;
	}

	g_weapons->loadDefaults();

	if (!g_spells->loadFromXml()) {
		std::cout << "> ERROR: Unable to load Spells!" << std::endl;
		return false;
	}

	if (!g_actions->loadFromXml()) {
		std::cout << "> ERROR: Unable to load Actions!" << std::endl;
		return false;
	}

	if (!g_talkActions->loadFromXml()) {
		std::cout << "> ERROR: Unable to load TalkActions!" << std::endl;
		return false;
	}

	if (!g_moveEvents->loadFromXml()) {
		std::cout << "> ERROR: Unable to load MoveEvents!" << std::endl;
		return false;
	}

	if (!g_creatureEvents->loadFromXml()) {
		std::cout << "> ERROR: Unable to load CreatureEvents!" << std::endl;
		return false;
	}

	if (!g_globalEvents->loadFromXml()) {
		std::cout << "> ERROR: Unable to load GlobalEVents!" << std::endl;
		return false;
	}

	return true;
}
