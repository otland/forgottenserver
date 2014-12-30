/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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
	for (const auto& it : m_creatureEvents) {
		delete it.second;
	}
}

void CreatureEvents::clear()
{
	//clear creature events
	for (const auto& it : m_creatureEvents) {
		it.second->clearEvent();
	}

	//clear lua state
	m_scriptInterface.reInitState();
}

LuaScriptInterface& CreatureEvents::getScriptInterface()
{
	return m_scriptInterface;
}

std::string CreatureEvents::getScriptBaseName() const
{
	return "creaturescripts";
}

Event* CreatureEvents::getEvent(const std::string& nodeName)
{
	if (strcasecmp(nodeName.c_str(), "event") != 0) {
		return nullptr;
	}
	return new CreatureEvent(&m_scriptInterface);
}

bool CreatureEvents::registerEvent(Event* event, const pugi::xml_node&)
{
	CreatureEvent* creatureEvent = reinterpret_cast<CreatureEvent*>(event);
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
	return nullptr;
}

bool CreatureEvents::playerLogin(Player* player) const
{
	//fire global event if is registered
	for (const auto& it : m_creatureEvents) {
		if (it.second->getEventType() == CREATURE_EVENT_LOGIN) {
			if (!it.second->executeOnLogin(player)) {
				return false;
			}
		}
	}
	return true;
}

bool CreatureEvents::playerLogout(Player* player) const
{
	//fire global event if is registered
	for (const auto& it : m_creatureEvents) {
		if (it.second->getEventType() == CREATURE_EVENT_LOGOUT) {
			if (!it.second->executeOnLogout(player)) {
				return false;
			}
		}
	}
	return true;
}

bool CreatureEvents::playerAdvance(Player* player, skills_t skill, uint32_t oldLevel,
                                       uint32_t newLevel)
{
	for (const auto& it : m_creatureEvents) {
		if (it.second->getEventType() == CREATURE_EVENT_ADVANCE) {
			if (!it.second->executeAdvance(player, skill, oldLevel, newLevel)) {
				return false;
			}
		}
	}
	return true;
}

/////////////////////////////////////

CreatureEvent::CreatureEvent(LuaScriptInterface* _interface) :
	Event(_interface)
{
	m_type = CREATURE_EVENT_NONE;
	m_isLoaded = false;
}

bool CreatureEvent::configureEvent(const pugi::xml_node& node)
{
	// Name that will be used in monster xml files and
	// lua function to register events to reference this event
	pugi::xml_attribute nameAttribute = node.attribute("name");
	if (!nameAttribute) {
		std::cout << "[Error - CreatureEvent::configureEvent] Missing name for creature event" << std::endl;
		return false;
	}

	m_eventName = nameAttribute.as_string();

	pugi::xml_attribute typeAttribute = node.attribute("type");
	if (!typeAttribute) {
		std::cout << "[Error - CreatureEvent::configureEvent] Missing type for creature event: " << m_eventName << std::endl;
		return false;
	}

	std::string tmpStr = asLowerCaseString(typeAttribute.as_string());
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
	} else if (tmpStr == "modalwindow") {
		m_type = CREATURE_EVENT_MODALWINDOW;
	} else if (tmpStr == "textedit") {
		m_type = CREATURE_EVENT_TEXTEDIT;
	} else if (tmpStr == "healthchange") {
		m_type = CREATURE_EVENT_HEALTHCHANGE;
	} else if (tmpStr == "manachange") {
		m_type = CREATURE_EVENT_MANACHANGE;
	} else if (tmpStr == "extendedopcode") {
		m_type = CREATURE_EVENT_EXTENDED_OPCODE;
	} else {
		std::cout << "[Error - CreatureEvent::configureEvent] Invalid type for creature event: " << m_eventName << std::endl;
		return false;
	}

	m_isLoaded = true;
	return true;
}

std::string CreatureEvent::getScriptEventName() const
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

		case CREATURE_EVENT_MODALWINDOW:
			return "onModalWindow";

		case CREATURE_EVENT_TEXTEDIT:
			return "onTextEdit";

		case CREATURE_EVENT_HEALTHCHANGE:
			return "onHealthChange";

		case CREATURE_EVENT_MANACHANGE:
			return "onManaChange";

		case CREATURE_EVENT_EXTENDED_OPCODE:
			return "onExtendedOpcode";

		case CREATURE_EVENT_NONE:
		default:
			return std::string();
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
	m_scriptInterface = nullptr;
	m_scripted = false;
	m_isLoaded = false;
}

bool CreatureEvent::executeOnLogin(Player* player)
{
	//onLogin(player)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeOnLogin] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	LuaScriptInterface::pushUserdata(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");
	return m_scriptInterface->callFunction(1);
}

bool CreatureEvent::executeOnLogout(Player* player)
{
	//onLogout(player)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeOnLogout] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	LuaScriptInterface::pushUserdata(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");
	return m_scriptInterface->callFunction(1);
}

bool CreatureEvent::executeOnThink(Creature* creature, uint32_t interval)
{
	//onThink(creature, interval)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeOnThink] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	lua_pushnumber(L, interval);

	return m_scriptInterface->callFunction(2);
}

bool CreatureEvent::executeOnPrepareDeath(Creature* creature, Creature* killer)
{
	//onPrepareDeath(creature, killer)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeOnPrepareDeath] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);

	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);

	if (killer) {
		LuaScriptInterface::pushUserdata<Creature>(L, killer);
		LuaScriptInterface::setCreatureMetatable(L, -1, killer);
	} else {
		lua_pushnil(L);
	}

	return m_scriptInterface->callFunction(2);
}

bool CreatureEvent::executeOnDeath(Creature* creature, Item* corpse, Creature* killer, Creature* mostDamageKiller, bool lastHitUnjustified, bool mostDamageUnjustified)
{
	//onDeath(creature, corpse, lasthitkiller, mostdamagekiller, lasthitunjustified, mostdamageunjustified)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeOnDeath] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	lua_pushnumber(L, env->addThing(corpse));

	if (killer) {
		LuaScriptInterface::pushUserdata<Creature>(L, killer);
		LuaScriptInterface::setCreatureMetatable(L, -1, killer);
	} else {
		lua_pushnil(L);
	}

	if (mostDamageKiller) {
		LuaScriptInterface::pushUserdata<Creature>(L, mostDamageKiller);
		LuaScriptInterface::setCreatureMetatable(L, -1, mostDamageKiller);
	} else {
		lua_pushnil(L);
	}

	LuaScriptInterface::pushBoolean(L, lastHitUnjustified);
	LuaScriptInterface::pushBoolean(L, mostDamageUnjustified);

	return m_scriptInterface->callFunction(6);
}

bool CreatureEvent::executeAdvance(Player* player, skills_t skill, uint32_t oldLevel,
                                       uint32_t newLevel)
{
	//onAdvance(player, skill, oldLevel, newLevel)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeAdvance] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	LuaScriptInterface::pushUserdata(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");
	lua_pushnumber(L, static_cast<uint32_t>(skill));
	lua_pushnumber(L, oldLevel);
	lua_pushnumber(L, newLevel);

	return m_scriptInterface->callFunction(4);
}

bool CreatureEvent::executeOnKill(Creature* creature, Creature* target)
{
	//onKill(creature, target)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeOnKill] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	LuaScriptInterface::pushUserdata<Creature>(L, target);
	LuaScriptInterface::setCreatureMetatable(L, -1, target);
	return m_scriptInterface->callFunction(2);
}

void CreatureEvent::executeModalWindow(Player* player, uint32_t modalWindowId, uint8_t buttonId, uint8_t choiceId)
{
	//onModalWindow(player, modalWindowId, buttonId, choiceId)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeModalWindow] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_scriptId);

	LuaScriptInterface::pushUserdata(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");

	lua_pushnumber(L, modalWindowId);
	lua_pushnumber(L, buttonId);
	lua_pushnumber(L, choiceId);

	m_scriptInterface->callVoidFunction(4);
}

bool CreatureEvent::executeTextEdit(Player* player, Item* item, const std::string& text)
{
	//onTextEdit(player, item, text)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeTextEdit] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_scriptId);

	LuaScriptInterface::pushUserdata(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");

	LuaScriptInterface::pushThing(L, item);
	LuaScriptInterface::pushString(L, text);

	return m_scriptInterface->callFunction(3);
}

void CreatureEvent::executeHealthChange(Creature* creature, Creature* attacker, CombatDamage& damage)
{
	//onHealthChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeHealthChange] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_scriptId);

	LuaScriptInterface::pushUserdata(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	if (attacker) {
		LuaScriptInterface::pushUserdata(L, attacker);
		LuaScriptInterface::setCreatureMetatable(L, -1, attacker);
	} else {
		lua_pushnil(L);
	}

	lua_pushnumber(L, damage.primary.value);
	lua_pushnumber(L, damage.primary.type);
	lua_pushnumber(L, damage.secondary.value);
	lua_pushnumber(L, damage.secondary.type);
	lua_pushnumber(L, damage.origin);

	if (m_scriptInterface->protectedCall(L, 7, 4) != 0) {
		LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
	} else {
		damage.primary.value = std::abs(LuaScriptInterface::getNumber<int32_t>(L, -4));
		damage.primary.type = LuaScriptInterface::getNumber<CombatType_t>(L, -3);
		damage.secondary.value = std::abs(LuaScriptInterface::getNumber<int32_t>(L, -2));
		damage.secondary.type = LuaScriptInterface::getNumber<CombatType_t>(L, -1);

		lua_pop(L, 4);
		if (damage.primary.type != COMBAT_HEALING) {
			damage.primary.value = -damage.primary.value;
			damage.secondary.value = -damage.secondary.value;
		}
	}

	m_scriptInterface->resetScriptEnv();
}

void CreatureEvent::executeManaChange(Creature* creature, Creature* attacker, int32_t& manaChange, CombatOrigin origin)
{
	//onManaChange(creature, attacker, manaChange, origin)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeManaChange] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_scriptId);

	LuaScriptInterface::pushUserdata(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	if (attacker) {
		LuaScriptInterface::pushUserdata(L, attacker);
		LuaScriptInterface::setCreatureMetatable(L, -1, attacker);
	} else {
		lua_pushnil(L);
	}

	lua_pushnumber(L, manaChange);
	lua_pushnumber(L, origin);

	if (m_scriptInterface->protectedCall(L, 4, 1) != 0) {
		LuaScriptInterface::reportError(nullptr, LuaScriptInterface::getString(L, -1));
	} else {
		manaChange = LuaScriptInterface::getNumber<int32_t>(L, -1);
	}
	lua_pop(L, 1);

	m_scriptInterface->resetScriptEnv();
}

void CreatureEvent::executeExtendedOpcode(Player* player, uint8_t opcode, const std::string& buffer)
{
	//onExtendedOpcode(player, opcode, buffer)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CreatureEvent::executeExtendedOpcode] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);

	LuaScriptInterface::pushUserdata<Player>(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");

	lua_pushnumber(L, opcode);
	LuaScriptInterface::pushString(L, buffer);

	m_scriptInterface->callVoidFunction(3);
}
