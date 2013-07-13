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

#include "creatureevent.h"
#include "tools.h"
#include "player.h"

CreatureEvents::CreatureEvents() :
	m_scriptInterface("CreatureScript Interface")
{
	m_scriptInterface.initState();
}

CreatureEvents::~CreatureEvents()
{
	CreatureEventList::iterator it;

	for (it = m_creatureEvents.begin(); it != m_creatureEvents.end(); ++it) {
		delete it->second;
	}

	m_creatureEvents.clear();
}

void CreatureEvents::clear()
{
	//clear creature events
	for (CreatureEventList::iterator it = m_creatureEvents.begin(); it != m_creatureEvents.end(); ++it) {
		it->second->clearEvent();
	}

	//clear lua state
	m_scriptInterface.reInitState();
}

LuaScriptInterface& CreatureEvents::getScriptInterface()
{
	return m_scriptInterface;
}

std::string CreatureEvents::getScriptBaseName()
{
	return "creaturescripts";
}

Event* CreatureEvents::getEvent(const std::string& nodeName)
{
	if (asLowerCaseString(nodeName) == "event") {
		return new CreatureEvent(&m_scriptInterface);
	}

	return NULL;
}

bool CreatureEvents::registerEvent(Event* event, xmlNodePtr p)
{
	CreatureEvent* creatureEvent = dynamic_cast<CreatureEvent*>(event);

	if (!creatureEvent) {
		return false;
	}

	if (creatureEvent->getEventType() == CREATURE_EVENT_NONE) {
		std::cout << "Error: [CreatureEvents::registerEvent] Trying to register event without type!" << std::endl;
		return false;
	}

	CreatureEvent* oldEvent = getEventByName(creatureEvent->getName(), false);

	if (oldEvent) {
		//if there was an event with the same that is not loaded
		//(happens when realoading), it is reused
		if (!oldEvent->isLoaded() && oldEvent->getEventType() == creatureEvent->getEventType()) {
			oldEvent->copyEvent(creatureEvent);
		}

		return false;
	} else {
		//if not, register it normally
		m_creatureEvents[creatureEvent->getName()] = creatureEvent;
		return true;
	}
}

CreatureEvent* CreatureEvents::getEventByName(const std::string& name, bool forceLoaded /*= true*/)
{
	CreatureEventList::iterator it = m_creatureEvents.find(name);

	if (it != m_creatureEvents.end()) {
		if (!forceLoaded || it->second->isLoaded()) {
			return it->second;
		}
	}

	return NULL;
}

uint32_t CreatureEvents::playerLogin(Player* player)
{
	//fire global event if is registered
	for (CreatureEventList::iterator it = m_creatureEvents.begin(); it != m_creatureEvents.end(); ++it) {
		if (it->second->getEventType() == CREATURE_EVENT_LOGIN) {
			if (!it->second->executeOnLogin(player)) {
				return 0;
			}
		}
	}

	return 1;
}

uint32_t CreatureEvents::playerLogout(Player* player)
{
	//fire global event if is registered
	for (CreatureEventList::iterator it = m_creatureEvents.begin(); it != m_creatureEvents.end(); ++it) {
		if (it->second->getEventType() == CREATURE_EVENT_LOGOUT) {
			if (!it->second->executeOnLogout(player)) {
				return 0;
			}
		}
	}

	return 1;
}

uint32_t CreatureEvents::playerAdvance(Player* player, skills_t skill, uint32_t oldLevel,
                                       uint32_t newLevel)
{
	for (CreatureEventList::iterator it = m_creatureEvents.begin(); it != m_creatureEvents.end(); ++it) {
		if (it->second->getEventType() == CREATURE_EVENT_ADVANCE) {
			if (!it->second->executeAdvance(dynamic_cast<Creature*>(player), skill, oldLevel, newLevel)) {
				return 0;
			}
		}
	}

	return 1;
}

/////////////////////////////////////

CreatureEvent::CreatureEvent(LuaScriptInterface* _interface) :
	Event(_interface)
{
	m_type = CREATURE_EVENT_NONE;
	m_isLoaded = false;
}

bool CreatureEvent::configureEvent(xmlNodePtr p)
{
	std::string str;

	//Name that will be used in monster xml files and
	// lua function to register events to reference this event
	if (readXMLString(p, "name", str)) {
		m_eventName = str;
	} else {
		std::cout << "Error: [CreatureEvent::configureEvent] No name for creature event." << std::endl;
		return false;
	}

	if (readXMLString(p, "type", str)) {
		std::string tmpStr = asLowerCaseString(str);

		if (tmpStr == "login") {
			m_type = CREATURE_EVENT_LOGIN;
		} else if (tmpStr == "logout") {
			m_type = CREATURE_EVENT_LOGOUT;
		} else if (tmpStr == "think") {
			m_type = CREATURE_EVENT_THINK;
		} else if (tmpStr == "preparedeath") {
			m_type = CREATURE_EVENT_PREPAREDEATH;
		} else if (tmpStr == "death") {
			m_type = CREATURE_EVENT_DEATH;
		} else if (tmpStr == "kill") {
			m_type = CREATURE_EVENT_KILL;
		} else if (tmpStr == "advance") {
			m_type = CREATURE_EVENT_ADVANCE;
		} else {
			std::cout << "[Error - CreatureEvent::configureEvent] No valid type for creature event." << str << std::endl;
			return false;
		}
	} else {
		std::cout << "[Error - CreatureEvent::configureEvent] No type for creature event."  << std::endl;
		return false;
	}

	m_isLoaded = true;
	return true;
}

std::string CreatureEvent::getScriptEventName()
{
	//Depending on the type script event name is different
	switch (m_type) {
		case CREATURE_EVENT_LOGIN:
			return "onLogin";

		case CREATURE_EVENT_LOGOUT:
			return "onLogout";

		case CREATURE_EVENT_THINK:
			return "onThink";

		case CREATURE_EVENT_PREPAREDEATH:
			return "onPrepareDeath";

		case CREATURE_EVENT_DEATH:
			return "onDeath";

		case CREATURE_EVENT_KILL:
			return "onKill";

		case CREATURE_EVENT_ADVANCE:
			return "onAdvance";

		case CREATURE_EVENT_NONE:
		default:
			return "";
	}
}

void CreatureEvent::copyEvent(CreatureEvent* creatureEvent)
{
	m_scriptId = creatureEvent->m_scriptId;
	m_scriptInterface = creatureEvent->m_scriptInterface;
	m_scripted = creatureEvent->m_scripted;
	m_isLoaded = creatureEvent->m_isLoaded;
}

void CreatureEvent::clearEvent()
{
	m_scriptId = 0;
	m_scriptInterface = NULL;
	m_scripted = false;
	m_isLoaded = false;
}

uint32_t CreatureEvent::executeOnLogin(Player* player)
{
	//onLogin(cid)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_scriptId, m_scriptInterface);
		env->setRealPos(player->getPosition());

		lua_State* L = m_scriptInterface->getLuaState();

		m_scriptInterface->pushFunction(m_scriptId);
		lua_pushnumber(L, player->getID());

		bool result = m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();

		return result;
	} else {
		std::cout << "[Error - CreatureEvent::executeOnLogin] Call stack overflow." << std::endl;
		return 0;
	}
}

uint32_t CreatureEvent::executeOnLogout(Player* player)
{
	//onLogout(cid)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_scriptId, m_scriptInterface);
		env->setRealPos(player->getPosition());

		lua_State* L = m_scriptInterface->getLuaState();

		m_scriptInterface->pushFunction(m_scriptId);
		lua_pushnumber(L, player->getID());

		bool result = m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();

		return result;
	} else {
		std::cout << "[Error - CreatureEvent::executeOnLogout] Call stack overflow." << std::endl;
		return 0;
	}
}

uint32_t CreatureEvent::executeOnThink(Creature* creature, uint32_t interval)
{
	//onThink(cid, interval)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_scriptId, m_scriptInterface);
		env->setRealPos(creature->getPosition());

		lua_State* L = m_scriptInterface->getLuaState();

		m_scriptInterface->pushFunction(m_scriptId);
		lua_pushnumber(L, creature->getID());
		lua_pushnumber(L, interval);

		bool result = m_scriptInterface->callFunction(2);
		m_scriptInterface->releaseScriptEnv();

		return result;
	} else {
		std::cout << "[Error - CreatureEvent::executeOnThink] Call stack overflow." << std::endl;
		return 0;
	}
}

uint32_t CreatureEvent::executeOnPrepareDeath(Player* player, Creature* killer)
{
	//onPrepareDeath(cid, killer)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_scriptId, m_scriptInterface);
		env->setRealPos(player->getPosition());

		uint32_t killercid;
		if (killer) {
			killercid = killer->getID();
		} else {
			killercid = 0;
		}

		lua_State* L = m_scriptInterface->getLuaState();

		m_scriptInterface->pushFunction(m_scriptId);
		lua_pushnumber(L, player->getID());
		lua_pushnumber(L, killercid);

		bool result = m_scriptInterface->callFunction(2);
		m_scriptInterface->releaseScriptEnv();

		return result;
	} else {
		std::cout << "[Error - CreatureEvent::executeOnPrepareDeath] Call stack overflow." << std::endl;
		return 0;
	}
}

uint32_t CreatureEvent::executeOnDeath(Creature* creature, Item* corpse, Creature* killer, Creature* mostDamageKiller, bool lastHitUnjustified, bool mostDamageUnjustified)
{
	//onDeath(cid, corpse, lasthitkiller, mostdamagekiller, lasthitunjustified, mostdamageunjustified)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_scriptId, m_scriptInterface);
		env->setRealPos(creature->getPosition());

		uint32_t corpseid = env->addThing(corpse);

		uint32_t killercid;
		if (killer) {
			killercid = killer->getID();
		} else {
			killercid = 0;
		}

		uint32_t mostdamagekillercid;
		if (mostDamageKiller) {
			mostdamagekillercid = mostDamageKiller->getID();
		} else {
			mostdamagekillercid = 0;
		}

		lua_State* L = m_scriptInterface->getLuaState();

		m_scriptInterface->pushFunction(m_scriptId);
		lua_pushnumber(L, creature->getID());
		lua_pushnumber(L, corpseid);
		lua_pushnumber(L, killercid);
		lua_pushnumber(L, mostdamagekillercid);
		lua_pushnumber(L, lastHitUnjustified);
		lua_pushnumber(L, mostDamageUnjustified);

		bool result = m_scriptInterface->callFunction(6);
		m_scriptInterface->releaseScriptEnv();

		return result;
	} else {
		std::cout << "[Error - CreatureEvent::executeOnDeath] Call stack overflow." << std::endl;
		return 0;
	}
}

uint32_t CreatureEvent::executeAdvance(Creature* creature, skills_t skill, uint32_t oldLevel,
                                       uint32_t newLevel)
{
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_scriptId, m_scriptInterface);
		env->setRealPos(creature->getPosition());

		lua_State* L = m_scriptInterface->getLuaState();

		m_scriptInterface->pushFunction(m_scriptId);
		lua_pushnumber(L, creature->getID());
		lua_pushnumber(L, static_cast<uint32_t>(skill));
		lua_pushnumber(L, oldLevel);
		lua_pushnumber(L, newLevel);

		bool result = m_scriptInterface->callFunction(4);
		m_scriptInterface->releaseScriptEnv();
		return result;
	} else {
		std::cout << "[Error - CreatureEvent::executeAdvance] Call stack overflow." << std::endl;
		return 0;
	}
}

uint32_t CreatureEvent::executeOnKill(Creature* creature, Creature* target)
{
	//onKill(cid, target)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_scriptId, m_scriptInterface);
		env->setRealPos(creature->getPosition());

		lua_State* L = m_scriptInterface->getLuaState();

		m_scriptInterface->pushFunction(m_scriptId);
		lua_pushnumber(L, creature->getID());
		lua_pushnumber(L, target->getID());

		bool result = m_scriptInterface->callFunction(2);
		m_scriptInterface->releaseScriptEnv();

		return result;
	} else {
		std::cout << "[Error - CreatureEvent::executeOnKill] Call stack overflow." << std::endl;
		return 0;
	}
}
