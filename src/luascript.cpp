/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#include <string>
#include <iostream>
#include <iomanip>

#include "luascript.h"
#include "chat.h"
#include "player.h"
#include "item.h"
#include "game.h"
#include "house.h"
#include "housetile.h"
#include "status.h"
#include "combat.h"
#include "spells.h"
#include "condition.h"
#include "monsters.h"
#include "baseevents.h"
#include "iologindata.h"
#include "configmanager.h"
#include "town.h"
#include "vocation.h"
#include "teleport.h"
#include "ban.h"
#include "mounts.h"
#include "databasemanager.h"
#include "beds.h"

#include <boost/range/adaptor/reversed.hpp>

extern Chat g_chat;
extern Game g_game;
extern Monsters g_monsters;
extern ConfigManager g_config;
extern Vocations g_vocations;
extern Spells* g_spells;
extern LuaEnvironment g_luaEnvironment;

enum {
	EVENT_ID_LOADING = 1,
	EVENT_ID_USER = 1000,
};

ScriptEnvironment::ThingMap ScriptEnvironment::m_globalMap;

ScriptEnvironment::AreaMap ScriptEnvironment::m_areaMap;
uint32_t ScriptEnvironment::m_lastAreaId = 0;

ScriptEnvironment::CombatMap ScriptEnvironment::m_combatMap;
uint32_t ScriptEnvironment::m_lastCombatId = 0;

ScriptEnvironment::ConditionMap ScriptEnvironment::m_conditionMap;
uint32_t ScriptEnvironment::m_lastConditionId = 0;

ScriptEnvironment::DBResultMap ScriptEnvironment::m_tempResults;
uint32_t ScriptEnvironment::m_lastResultId = 0;

ScriptEnvironment::StorageMap ScriptEnvironment::m_globalStorageMap;

ScriptEnvironment::TempItemListMap ScriptEnvironment::m_tempItems;

ScriptEnvironment::ScriptEnvironment()
{
	m_curNpc = nullptr;
	resetEnv();
	m_lastUID = 70000;
}

ScriptEnvironment::~ScriptEnvironment()
{
	resetEnv();
}

void ScriptEnvironment::resetEnv()
{
	m_scriptId = 0;
	m_callbackId = 0;
	m_timerEvent = false;
	m_interface = nullptr;
	m_localMap.clear();

	for (const auto& it : m_tempItems) {
		for (Item* item : it.second) {
			if (item->getParent() == VirtualCylinder::virtualCylinder) {
				g_game.ReleaseItem(item);
			}
		}
	}
	m_tempItems.clear();

	Database* db = Database::getInstance();
	for (const auto& it : m_tempResults) {
		db->freeResult(it.second);
	}
	m_tempResults.clear();
}

void ScriptEnvironment::freeCombatObjects()
{
	for (const auto& it : m_combatMap) {
		delete it.second;
	}
}

void ScriptEnvironment::freeAreaObjects()
{
	for (const auto& it : m_areaMap) {
		delete it.second;
	}
}

void ScriptEnvironment::freeConditionObjects()
{
	for (const auto& it : m_conditionMap) {
		delete it.second;
	}
}

bool ScriptEnvironment::saveGameState()
{
	if (!g_config.getBoolean(ConfigManager::SAVE_GLOBAL_STORAGE)) {
		return true;
	}

	Database* db = Database::getInstance();

	db->executeQuery("TRUNCATE TABLE `global_storage`");

	DBInsert stmt;
	stmt.setQuery("INSERT INTO `global_storage` (`key`, `value`) VALUES ");

	std::ostringstream query;
	for (const auto& it : m_globalStorageMap) {
		query << it.first << ',' << it.second;
		if (!stmt.addRow(query)) {
			return false;
		}
	}
	return stmt.execute();
}

bool ScriptEnvironment::loadGameState()
{
	if (!g_config.getBoolean(ConfigManager::SAVE_GLOBAL_STORAGE)) {
		return true;
	}

	Database* db = Database::getInstance();

	DBResult* result = db->storeQuery("SELECT `key`, `value` FROM `global_storage`");
	if (result) {
		do {
			m_globalStorageMap[result->getDataInt("key")] = result->getDataInt("value");
		} while (result->next());
		db->freeResult(result);
	}
	return true;
}

bool ScriptEnvironment::setCallbackId(int32_t callbackId, LuaScriptInterface* scriptInterface)
{
	if (m_callbackId == 0) {
		m_callbackId = callbackId;
		m_interface = scriptInterface;
		return true;
	} else {
		//nested callbacks are not allowed
		if (m_interface) {
			m_interface->reportErrorFunc("Nested callbacks!");
		}

		return false;
	}
}

void ScriptEnvironment::getEventInfo(int32_t& scriptId, std::string& desc, LuaScriptInterface*& scriptInterface, int32_t& callbackId, bool& timerEvent) const
{
	scriptId = m_scriptId;
	desc = m_eventdesc;
	scriptInterface = m_interface;
	callbackId = m_callbackId;
	timerEvent = m_timerEvent;
}

void ScriptEnvironment::addUniqueThing(Thing* thing)
{
	Item* item = thing->getItem();
	if (item && item->getUniqueId() != 0) {
		uint16_t uid = item->getUniqueId();
		auto it = m_globalMap.find(uid);
		if (it == m_globalMap.end()) {
			m_globalMap[uid] = thing;
		} else {
			std::cout << "Duplicate uniqueId " << uid << std::endl;
		}
	}
}

void ScriptEnvironment::removeUniqueThing(Thing* thing)
{
	Item* item = thing->getItem();
	if (item && item->getUniqueId() != 0) {
		auto it = m_globalMap.find(item->getUniqueId());
		if (it != m_globalMap.end()) {
			m_globalMap.erase(it);
		}
	}
}

uint32_t ScriptEnvironment::addThing(Thing* thing)
{
	if (!thing || thing->isRemoved()) {
		return 0;
	}

	Creature* creature = thing->getCreature();
	if (creature) {
		return creature->getID();
	}

	Item* item = thing->getItem();
	if (item) {
		uint16_t uid = item->getUniqueId();
		if (uid > 0) {
			bool isOnTile = false;

			const Cylinder* parent = item->getParent();
			if (item->getTile() == parent) {
				isOnTile = true;
			} else if (parent) {
				const Container* parentContainer = parent->getContainer();
				if (parentContainer && parentContainer->getID() == ITEM_BROWSEFIELD) {
					isOnTile = true;
				}
			}

			if (isOnTile) {
				if (m_localMap.find(uid) != m_localMap.end()) {
					return uid;
				}

				m_localMap[uid] = thing;
				return uid;
			}
		}
	}

	for (const auto& it : m_localMap) {
		if (it.second == thing) {
			return it.first;
		}
	}

	++m_lastUID;
	m_localMap[m_lastUID] = thing;
	return m_lastUID;
}

void ScriptEnvironment::insertThing(uint32_t uid, Thing* thing)
{
	if (m_localMap.find(uid) == m_localMap.end()) {
		m_localMap[uid] = thing;
	} else {
		std::cout << std::endl << "Lua Script Error: Thing uid already taken.";
	}
}

Thing* ScriptEnvironment::getThingByUID(uint32_t uid)
{
	if (uid >= 0x10000000) {
		Thing* thing = g_game.getCreatureByID(uid);
		if (thing && !thing->isRemoved()) {
			return thing;
		}
	} else {
		auto it = m_localMap.find(uid);
		if (it != m_localMap.end()) {
			Thing* thing = it->second;
			if (thing && !thing->isRemoved()) {
				return thing;
			}
		}

		it = m_globalMap.find(uid);
		if (it != m_globalMap.end()) {
			Thing* thing = it->second;
			if (thing && !thing->isRemoved()) {
				return thing;
			}
		}
	}
	return nullptr;
}

Item* ScriptEnvironment::getItemByUID(uint32_t uid)
{
	Thing* thing = getThingByUID(uid);
	if (!thing) {
		return nullptr;
	}
	return thing->getItem();
}

Container* ScriptEnvironment::getContainerByUID(uint32_t uid)
{
	Item* item = getItemByUID(uid);
	if (!item) {
		return nullptr;
	}
	return item->getContainer();
}

void ScriptEnvironment::removeItemByUID(uint32_t uid)
{
	auto it = m_localMap.find(uid);
	if (it != m_localMap.end()) {
		m_localMap.erase(it);
	}

	it = m_globalMap.find(uid);
	if (it != m_globalMap.end()) {
		m_globalMap.erase(it);
	}
}

uint32_t ScriptEnvironment::addCombatArea(AreaCombat* area)
{
	uint32_t newAreaId = m_lastAreaId + 1;
	m_areaMap[newAreaId] = area;

	m_lastAreaId++;
	return newAreaId;
}

AreaCombat* ScriptEnvironment::getCombatArea(uint32_t areaId)
{
	auto it = m_areaMap.find(areaId);
	if (it == m_areaMap.end()) {
		return nullptr;
	}
	return it->second;
}

uint32_t ScriptEnvironment::addCombatObject(Combat* combat)
{
	uint32_t newCombatId = m_lastCombatId + 1;
	m_combatMap[newCombatId] = combat;

	m_lastCombatId++;
	return newCombatId;
}

Combat* ScriptEnvironment::getCombatObject(uint32_t combatId)
{
	CombatMap::iterator it = m_combatMap.find(combatId);
	if (it == m_combatMap.end()) {
		return nullptr;
	}
	return it->second;
}

uint32_t ScriptEnvironment::addConditionObject(Condition* condition)
{
	uint32_t newConditionId = m_lastConditionId + 1;
	m_conditionMap[newConditionId] = condition;

	m_lastConditionId++;
	return m_lastConditionId;
}

Condition* ScriptEnvironment::getConditionObject(uint32_t conditionId)
{
	ConditionMap::iterator it = m_conditionMap.find(conditionId);
	if (it == m_conditionMap.end()) {
		return nullptr;
	}
	return it->second;
}

void ScriptEnvironment::addTempItem(ScriptEnvironment* env, Item* item)
{
	m_tempItems[env].push_back(item);
}

void ScriptEnvironment::removeTempItem(ScriptEnvironment* env, Item* item)
{
	ItemList& itemList = m_tempItems[env];

	auto it = std::find(itemList.begin(), itemList.end(), item);
	if (it != itemList.end()) {
		itemList.erase(it);
	}
}

void ScriptEnvironment::removeTempItem(Item* item)
{
	for (auto& it : m_tempItems) {
		ItemList& itemList = it.second;
		auto it_ = std::find(itemList.begin(), itemList.end(), item);
		if (it_ != itemList.end()) {
			itemList.erase(it_);
			break;
		}
	}
}

uint32_t ScriptEnvironment::addResult(DBResult* res)
{
	m_tempResults[++m_lastResultId] = res;
	return m_lastResultId;
}

bool ScriptEnvironment::removeResult(uint32_t id)
{
	auto it = m_tempResults.find(id);
	if (it == m_tempResults.end()) {
		return false;
	}

	Database::getInstance()->freeResult(it->second);
	m_tempResults.erase(it);
	return true;
}

DBResult* ScriptEnvironment::getResultByID(uint32_t id)
{
	auto it = m_tempResults.find(id);
	if (it == m_tempResults.end()) {
		return nullptr;
	}
	return it->second;
}

void ScriptEnvironment::addGlobalStorageValue(const uint32_t key, const int32_t value)
{
	m_globalStorageMap[key] = value;
}

bool ScriptEnvironment::getGlobalStorageValue(const uint32_t key, int32_t& value) const
{
	StorageMap::const_iterator it = m_globalStorageMap.find(key);
	if (it == m_globalStorageMap.end()) {
		value = 0;
		return false;
	}

	value = it->second;
	return true;
}

std::string LuaScriptInterface::getErrorDesc(ErrorCode_t code)
{
	switch (code) {
		case LUA_ERROR_PLAYER_NOT_FOUND:
			return "Player not found";

		case LUA_ERROR_CREATURE_NOT_FOUND:
			return "Creature not found";

		case LUA_ERROR_ITEM_NOT_FOUND:
			return "Item not found";

		case LUA_ERROR_THING_NOT_FOUND:
			return "Thing not found";

		case LUA_ERROR_TILE_NOT_FOUND:
			return "Tile not found";

		case LUA_ERROR_HOUSE_NOT_FOUND:
			return "House not found";

		case LUA_ERROR_COMBAT_NOT_FOUND:
			return "Combat not found";

		case LUA_ERROR_CONDITION_NOT_FOUND:
			return "Condition not found";

		case LUA_ERROR_AREA_NOT_FOUND:
			return "Area not found";

		case LUA_ERROR_CONTAINER_NOT_FOUND:
			return "Container not found";

		case LUA_ERROR_VARIANT_NOT_FOUND:
			return "Variant not found";

		case LUA_ERROR_VARIANT_UNKNOWN:
			return "Unknown variant type";

		case LUA_ERROR_SPELL_NOT_FOUND:
			return "Spell not found";

		default:
			return "Bad error code";
	}
}

ScriptEnvironment LuaScriptInterface::m_scriptEnv[16];
int32_t LuaScriptInterface::m_scriptEnvIndex = -1;

LuaScriptInterface::LuaScriptInterface(const std::string& interfaceName)
{
	if (!g_luaEnvironment.getLuaState()) {
		g_luaEnvironment.initState();
	}

	m_eventTableRef = -1;
	m_luaState = nullptr;
	m_interfaceName = interfaceName;
}

LuaScriptInterface::~LuaScriptInterface()
{
	closeState();
}

bool LuaScriptInterface::reInitState()
{
	closeState();
	return initState();
}

/// Same as lua_pcall, but adds stack trace to error strings in called function.
int32_t LuaScriptInterface::protectedCall(lua_State* L, int32_t nargs, int32_t nresults)
{
	int32_t error_index = lua_gettop(L) - nargs;
	lua_pushcfunction(L, luaErrorHandler);
	lua_insert(L, error_index);

	int32_t ret = lua_pcall(L, nargs, nresults, error_index);
	lua_remove(L, error_index);
	return ret;
}

int32_t LuaScriptInterface::loadFile(const std::string& file, Npc* npc /* = nullptr*/)
{
	//loads file as a chunk at stack top
	int32_t ret = luaL_loadfile(m_luaState, file.c_str());
	if (ret != 0) {
		m_lastLuaError = popString(m_luaState);
		return -1;
	}

	//check that it is loaded as a function
	if (!isFunction(m_luaState, -1)) {
		return -1;
	}

	m_loadingFile = file;

	if (!reserveScriptEnv()) {
		return -1;
	}

	ScriptEnvironment* env = getScriptEnv();
	env->setScriptId(EVENT_ID_LOADING, this);
	env->setNpc(npc);

	//execute it
	ret = protectedCall(m_luaState, 0, 0);
	if (ret != 0) {
		reportError(nullptr, popString(m_luaState));
		resetScriptEnv();
		return -1;
	}

	resetScriptEnv();
	return 0;
}

int32_t LuaScriptInterface::getEvent(const std::string& eventName)
{
	//get our events table
	lua_rawgeti(m_luaState, LUA_REGISTRYINDEX, m_eventTableRef);
	if (!isTable(m_luaState, -1)) {
		lua_pop(m_luaState, 1);
		return -1;
	}

	//get current event function pointer
	lua_getglobal(m_luaState, eventName.c_str());
	if (!isFunction(m_luaState, -1)) {
		lua_pop(m_luaState, 2);
		return -1;
	}

	//save in our events table
	lua_pushnumber(m_luaState, m_runningEventId);
	lua_pushvalue(m_luaState, -2);
	lua_rawset(m_luaState, -4);
	lua_pop(m_luaState, 2);

	//reset global value of this event
	lua_pushnil(m_luaState);
	lua_setglobal(m_luaState, eventName.c_str());

	m_cacheFiles[m_runningEventId] = m_loadingFile + ":" + eventName;
	return m_runningEventId++;
}

const std::string& LuaScriptInterface::getFileById(int32_t scriptId)
{
	if (scriptId == EVENT_ID_LOADING) {
		return m_loadingFile;
	}

	auto it = m_cacheFiles.find(scriptId);
	if (it == m_cacheFiles.end()) {
		static const std::string& unk = "(Unknown scriptfile)";
		return unk;
	}
	return it->second;
}

std::string LuaScriptInterface::getStackTrace(const std::string& error_desc)
{
	lua_getglobal(m_luaState, "debug");
	if (!isTable(m_luaState, -1)) {
		lua_pop(m_luaState, 1);
		return error_desc;
	}

	lua_getfield(m_luaState, -1, "traceback");
	if (!isFunction(m_luaState, -1)) {
		lua_pop(m_luaState, 1);
		return error_desc;
	}

	pushString(m_luaState, error_desc);
	lua_call(m_luaState, 1, 1);
	return popString(m_luaState);
}

void LuaScriptInterface::reportError(const char* function, const std::string& error_desc, bool stack_trace/* = false*/)
{
	int32_t scriptId;
	int32_t callbackId;
	bool timerEvent;
	std::string event_desc;
	LuaScriptInterface* scriptInterface;
	getScriptEnv()->getEventInfo(scriptId, event_desc, scriptInterface, callbackId, timerEvent);

	std::cout << std::endl << "Lua Script Error: ";

	if (scriptInterface) {
		std::cout << '[' << scriptInterface->getInterfaceName() << "] " << std::endl;

		if (timerEvent) {
			std::cout << "in a timer event called from: " << std::endl;
		}

		if (callbackId) {
			std::cout << "in callback: " << scriptInterface->getFileById(callbackId) << std::endl;
		}

		std::cout << scriptInterface->getFileById(scriptId) << std::endl;
	}

	if (!event_desc.empty()) {
		std::cout << "Event: " << event_desc << std::endl;
	}

	if (function) {
		std::cout << function << "(). ";
	}

	if (stack_trace && scriptInterface) {
		std::cout << scriptInterface->getStackTrace(error_desc) << std::endl;
	} else {
		std::cout << error_desc << std::endl;
	}
}

bool LuaScriptInterface::pushFunction(int32_t functionId)
{
	lua_rawgeti(m_luaState, LUA_REGISTRYINDEX, m_eventTableRef);
	if (!isTable(m_luaState, -1)) {
		return false;
	}
	
	lua_pushnumber(m_luaState, functionId);
	lua_rawget(m_luaState, -2);
	lua_remove(m_luaState, -2);
	return isFunction(m_luaState, -1);
}

bool LuaScriptInterface::initState()
{
	m_luaState = g_luaEnvironment.getLuaState();
	if (!m_luaState) {
		return false;
	}

	lua_newtable(m_luaState);
	m_eventTableRef = luaL_ref(m_luaState, LUA_REGISTRYINDEX);
	m_runningEventId = EVENT_ID_USER;
	return true;
}

bool LuaScriptInterface::closeState()
{
	if (!m_luaState) {
		return false;
	}

	m_cacheFiles.clear();
	if (m_eventTableRef != -1) {
		luaL_unref(m_luaState, LUA_REGISTRYINDEX, m_eventTableRef);
		m_eventTableRef = -1;
	}

	m_luaState = nullptr;
	return true;
}

int32_t LuaScriptInterface::luaErrorHandler(lua_State* L)
{
	const std::string& errorMessage = popString(L);
	pushString(L, getScriptEnv()->getScriptInterface()->getStackTrace(errorMessage));
	return 1;
}

bool LuaScriptInterface::callFunction(uint32_t nParams)
{
	bool result = false;
	int32_t size0 = lua_gettop(m_luaState);

	int32_t ret = protectedCall(m_luaState, nParams, 1);
	if (ret != 0) {
		LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(m_luaState));
	} else {
		result = LuaScriptInterface::popBoolean(m_luaState);
	}

	if ((lua_gettop(m_luaState) + (int)nParams + 1) != size0) {
		LuaScriptInterface::reportError(nullptr, "Stack size changed!");
	}

	resetScriptEnv();
	return result;
}

void LuaScriptInterface::pushVariant(lua_State* L, const LuaVariant& var)
{
	lua_newtable(L);
	setField(L, "type", var.type);

	switch (var.type) {
		case VARIANT_NUMBER:
			setField(L, "number", var.number);
			break;
		case VARIANT_STRING:
			setField(L, "string", var.text);
			break;
		case VARIANT_TARGETPOSITION:
		case VARIANT_POSITION: {
			lua_pushstring(L, "pos");
			pushPosition(L, var.pos);
			lua_settable(L, -3);
			break;
		}
		case VARIANT_NONE:
			break;
	}
}

void LuaScriptInterface::pushThing(lua_State* L, Thing* thing, uint32_t uid)
{
	lua_newtable(L);

	if (thing && thing->getItem()) {
		const Item* item = thing->getItem();
		setField(L, "uid", uid);
		setField(L, "itemid", item->getID());

		if (item->hasSubType()) {
			setField(L, "type", item->getSubType());
		} else {
			setField(L, "type", 0);
		}

		setField(L, "actionid", item->getActionId());
	} else if (thing && thing->getCreature()) {
		const Creature* creature = thing->getCreature();
		setField(L, "uid", uid);
		setField(L, "itemid", 1);

		uint8_t type;
		if (creature->getPlayer()) {
			type = 1;
		} else if (creature->getMonster()) {
			type = 2;
		} else {
			type = 3;    //npc
		}

		setField(L, "type", type);
		setField(L, "actionid", 0);
	} else {
		setField(L, "uid", 0);
		setField(L, "itemid", 0);
		setField(L, "type", 0);
		setField(L, "actionid", 0);
	}
}

void LuaScriptInterface::pushPosition(lua_State* L, const PositionEx& position)
{
	lua_newtable(L);
	setField(L, "z", position.z);
	setField(L, "y", position.y);
	setField(L, "x", position.x);
	setField(L, "stackpos", position.stackpos);
}

void LuaScriptInterface::pushPosition(lua_State* L, const Position& position, uint32_t stackpos)
{
	lua_newtable(L);
	setField(L, "z", position.z);
	setField(L, "y", position.y);
	setField(L, "x", position.x);
	setField(L, "stackpos", stackpos);
}

void LuaScriptInterface::pushString(lua_State* L, const std::string& value)
{
	lua_pushlstring(L, value.c_str(), value.length());
}

void LuaScriptInterface::pushCallback(lua_State* L, int32_t callback)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, callback);
}

LuaVariant LuaScriptInterface::popVariant(lua_State* L)
{
	LuaVariant var;
	switch (var.type = static_cast<LuaVariantType_t>(popField<uint32_t>(L, "type"))) {
		case VARIANT_NUMBER: {
			var.number = popField<uint32_t>(L, "number");
			break;
		}

		case VARIANT_STRING: {
			var.text = popFieldString(L, "string");
			break;
		}

		case VARIANT_POSITION:
		case VARIANT_TARGETPOSITION: {
			lua_getfield(L, -1, "pos");
			popPosition(L, var.pos);
			break;
		}

		default: {
			var.type = VARIANT_NONE;
			break;
		}
	}

	lua_pop(L, 1);
	return var;
}

void LuaScriptInterface::popPosition(lua_State* L, PositionEx& position)
{
	position.x = popField<uint16_t>(L, "x");
	position.y = popField<uint16_t>(L, "y");
	position.z = popField<uint8_t>(L, "z");
	position.stackpos = popField<int32_t>(L, "stackpos");
	lua_pop(L, 1);
}

void LuaScriptInterface::popPosition(lua_State* L, Position& position, uint32_t& stackpos)
{
	position.x = popField<uint16_t>(L, "x");
	position.y = popField<uint16_t>(L, "y");
	position.z = popField<uint8_t>(L, "z");
	stackpos = popField<int32_t>(L, "stackpos");
	lua_pop(L, 1);
}

uint32_t LuaScriptInterface::popNumber(lua_State* L)
{
	if (lua_gettop(L) == 0) {
		return 0;
	}

	uint32_t number = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return number;
}

double LuaScriptInterface::popFloatNumber(lua_State* L)
{
	if (lua_gettop(L) == 0) {
		return 0.0;
	}

	double number = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return number;
}

std::string LuaScriptInterface::popString(lua_State* L)
{
	if (lua_gettop(L) == 0) {
		return "";
	}

	std::string str;

	size_t len;
	const char* c_str = lua_tolstring(L, -1, &len);
	if (c_str && len > 0) {
		str.assign(c_str, len);
	}

	lua_pop(L, 1);
	return str;
}

int32_t LuaScriptInterface::popCallback(lua_State* L)
{
	return luaL_ref(L, LUA_REGISTRYINDEX);
}

bool LuaScriptInterface::popBoolean(lua_State* L)
{
	if (lua_gettop(L) == 0) {
		return false;
	}

	bool value = lua_toboolean(L, -1) != 0;
	lua_pop(L, 1);
	return value;
}

// Metatables
void LuaScriptInterface::setMetatable(lua_State* L, int32_t index, const std::string& string)
{
	luaL_getmetatable(L, string.c_str());
	lua_setmetatable(L, index - 1);
}

void LuaScriptInterface::setItemMetatable(lua_State* L, int32_t index, const Item* item)
{
	if (item->getContainer()) {
		luaL_getmetatable(L, "Container");
	} else {
		luaL_getmetatable(L, "Item");
	}
	lua_setmetatable(L, index - 1);
}

void LuaScriptInterface::setCreatureMetatable(lua_State* L, int32_t index, const Creature* creature)
{
	if (creature->getPlayer()) {
		luaL_getmetatable(L, "Player");
	} else if (creature->getMonster()) {
		luaL_getmetatable(L, "Monster");
	} else {
		luaL_getmetatable(L, "Npc");
	}
	lua_setmetatable(L, index - 1);
}

// Get
std::string LuaScriptInterface::getString(lua_State* L, int32_t arg)
{
	std::string str;
	size_t len;

	const char* c_str = lua_tolstring(L, arg, &len);
	if (c_str && len > 0) {
		str.assign(c_str, len);
	}
	return str;
}

bool LuaScriptInterface::getBoolean(lua_State* L, int32_t arg)
{
	return lua_toboolean(L, arg) != 0;
}

Position LuaScriptInterface::getPosition(lua_State* L, int32_t arg, uint32_t& stackpos)
{
	Position position;
	position.x = getField<uint16_t>(L, arg, "x");
	position.y = getField<uint16_t>(L, arg, "y");
	position.z = getField<uint8_t>(L, arg, "z");

	lua_getfield(L, arg, "stackpos");
	if (isNil(L, -1)) {
		stackpos = 0;
	} else {
		stackpos = getNumber<int32_t>(L, -1);
	}
	
	lua_pop(L, 4);
	return position;
}

Position LuaScriptInterface::getPosition(lua_State* L, int32_t arg)
{
	Position position;
	position.x = getField<uint16_t>(L, arg, "x");
	position.y = getField<uint16_t>(L, arg, "y");
	position.z = getField<uint8_t>(L, arg, "z");

	lua_pop(L, 3);
	return position;
}

Thing* LuaScriptInterface::getThing(lua_State* L, int32_t arg)
{
	Thing* thing;
	if (lua_getmetatable(L, arg) != 0) {
		lua_rawgeti(L, -1, 't');
		switch(getNumber<uint32_t>(L, -1)) {
			case LuaData_Item:
				thing = getUserdata<Item>(L, arg);
				break;
			case LuaData_Container:
				thing = getUserdata<Container>(L, arg);
				break;
			case LuaData_Player:
				thing = getUserdata<Player>(L, arg);
				break;
			case LuaData_Monster:
				thing = getUserdata<Monster>(L, arg);
				break;
			case LuaData_Npc:
				thing = getUserdata<Npc>(L, arg);
				break;
			default:
				thing = nullptr;
				break;
		}
		lua_pop(L, 2);
	} else {
		thing = getScriptEnv()->getThingByUID(getNumber<uint32_t>(L, arg));
	}
	return thing;
}

Creature* LuaScriptInterface::getCreature(lua_State* L, int32_t arg)
{
	if (isUserdata(L, arg)) {
		return getUserdata<Creature>(L, arg);
	}
	return g_game.getCreatureByID(getNumber<uint32_t>(L, arg));
}

Player* LuaScriptInterface::getPlayer(lua_State* L, int32_t arg)
{
	if (isUserdata(L, arg)) {
		return getUserdata<Player>(L, arg);
	}
	return g_game.getPlayerByID(getNumber<uint32_t>(L, arg));
}

std::string LuaScriptInterface::getFieldString(lua_State* L, const std::string& key)
{
	lua_getfield(L, -1, key.c_str());
	return getString(L, -1);
}

bool LuaScriptInterface::getFieldBoolean(lua_State* L, const std::string& key)
{
	lua_getfield(L, -1, key.c_str());
	return getBoolean(L, -1);
}

// Other
int32_t LuaScriptInterface::getStackTop(lua_State* L)
{
	return lua_gettop(L);
}

// Is
bool LuaScriptInterface::isNil(lua_State* L, int32_t arg)
{
	return lua_isnil(L, arg);
}

bool LuaScriptInterface::isNumber(lua_State* L, int32_t arg)
{
	return lua_isnumber(L, arg) != 0;
}

bool LuaScriptInterface::isString(lua_State* L, int32_t arg)
{
	return lua_isstring(L, arg) != 0;
}

bool LuaScriptInterface::isBoolean(lua_State* L, int32_t arg)
{
	return lua_isboolean(L, arg);
}

bool LuaScriptInterface::isTable(lua_State* L, int32_t arg)
{
	return lua_istable(L, arg);
}

bool LuaScriptInterface::isFunction(lua_State* L, int32_t arg)
{
	return lua_isfunction(L, arg);
}

bool LuaScriptInterface::isUserdata(lua_State* L, int32_t arg)
{
	return lua_isuserdata(L, arg) != 0;
}

// Pop
Outfit_t LuaScriptInterface::popOutfit(lua_State* L)
{
	Outfit_t outfit;
	outfit.lookMount = popField<uint16_t>(L, "lookMount");
	outfit.lookAddons = popField<uint8_t>(L, "lookAddons");

	outfit.lookFeet = popField<uint8_t>(L, "lookFeet");
	outfit.lookLegs = popField<uint8_t>(L, "lookLegs");
	outfit.lookBody = popField<uint8_t>(L, "lookBody");
	outfit.lookHead = popField<uint8_t>(L, "lookHead");

	outfit.lookTypeEx = popField<uint16_t>(L, "lookTypeEx");
	outfit.lookType = popField<uint16_t>(L, "lookType");

	lua_pop(L, 1);
	return outfit;
}

// Push
void LuaScriptInterface::pushBoolean(lua_State* L, bool value)
{
	lua_pushboolean(L, value ? 1 : 0);
}

void LuaScriptInterface::pushNil(lua_State* L)
{
	lua_pushnil(L);
}

void LuaScriptInterface::pushMetaPosition(lua_State* L, const PositionEx& position)
{
	lua_newtable(L);

	pushNumber(L, position.x);
	lua_setfield(L, -2, "x");

	pushNumber(L, position.y);
	lua_setfield(L, -2, "y");

	pushNumber(L, position.z);
	lua_setfield(L, -2, "z");

	pushNumber(L, position.stackpos);
	lua_setfield(L, -2, "stackpos");

	setMetatable(L, -1, "Position");
}

void LuaScriptInterface::pushMetaPosition(lua_State* L, const Position& position, uint32_t stackpos)
{
	lua_newtable(L);

	pushNumber(L, position.x);
	lua_setfield(L, -2, "x");

	pushNumber(L, position.y);
	lua_setfield(L, -2, "y");

	pushNumber(L, position.z);
	lua_setfield(L, -2, "z");

	pushNumber(L, stackpos);
	lua_setfield(L, -2, "stackpos");

	setMetatable(L, -1, "Position");
}

void LuaScriptInterface::pushOutfit(lua_State* L, const Outfit_t& outfit)
{
	lua_newtable(L);

	pushNumber(L, outfit.lookType);
	lua_setfield(L, -2, "lookType");

	pushNumber(L, outfit.lookTypeEx);
	lua_setfield(L, -2, "lookTypeEx");

	pushNumber(L, outfit.lookHead);
	lua_setfield(L, -2, "lookHead");

	pushNumber(L, outfit.lookBody);
	lua_setfield(L, -2, "lookBody");

	pushNumber(L, outfit.lookLegs);
	lua_setfield(L, -2, "lookLegs");

	pushNumber(L, outfit.lookFeet);
	lua_setfield(L, -2, "lookFeet");

	pushNumber(L, outfit.lookAddons);
	lua_setfield(L, -2, "lookAddons");

	pushNumber(L, outfit.lookMount);
	lua_setfield(L, -2, "lookMount");
}

//
void LuaScriptInterface::setField(lua_State* L, const char* index, double val)
{
	lua_pushstring(L, index);
	lua_pushnumber(L, val);
	lua_settable(L, -3);
}

void LuaScriptInterface::setField(lua_State* L, const char* index, const std::string& val)
{
	lua_pushstring(L, index);
	pushString(L, val);
	lua_settable(L, -3);
}

void LuaScriptInterface::setFieldBool(lua_State* L, const char* index, bool val)
{
	lua_pushstring(L, index);
	pushBoolean(L, val);
	lua_settable(L, -3);
}

bool LuaScriptInterface::popFieldBoolean(lua_State* L, const std::string& key)
{
	lua_getfield(L, -1, key.c_str());
	return popBoolean(L);
}

std::string LuaScriptInterface::popFieldString(lua_State* L, const std::string& key)
{
	lua_getfield(L, -1, key.c_str());
	return popString(L);
}

void LuaScriptInterface::registerFunctions()
{
	//getPlayerFood(cid)
	lua_register(m_luaState, "getPlayerFood", LuaScriptInterface::luaGetPlayerFood);

	//getPlayerFlagValue(cid, flag)
	lua_register(m_luaState, "getPlayerFlagValue", LuaScriptInterface::luaGetPlayerFlagValue);

	//playerLearnInstantSpell(cid, name)
	lua_register(m_luaState, "playerLearnInstantSpell", LuaScriptInterface::luaPlayerLearnInstantSpell);

	//canPlayerLearnInstantSpell(cid, name)
	lua_register(m_luaState, "canPlayerLearnInstantSpell", LuaScriptInterface::luaCanPlayerLearnInstantSpell);

	//getPlayerLearnedInstantSpell(cid, name)
	lua_register(m_luaState, "getPlayerLearnedInstantSpell", LuaScriptInterface::luaGetPlayerLearnedInstantSpell);

	//getPlayerInstantSpellCount(cid)
	lua_register(m_luaState, "getPlayerInstantSpellCount", LuaScriptInterface::luaGetPlayerInstantSpellCount);

	//getPlayerInstantSpellInfo(cid, index)
	lua_register(m_luaState, "getPlayerInstantSpellInfo", LuaScriptInterface::luaGetPlayerInstantSpellInfo);

	//getInstantSpellInfoByName(cid, name)
	lua_register(m_luaState, "getInstantSpellInfoByName", LuaScriptInterface::luaGetInstantSpellInfoByName);

	//getInstantSpellWords(name)
	lua_register(m_luaState, "getInstantSpellWords", LuaScriptInterface::luaGetInstantSpellWords);

	//getGlobalStorageValue(valueid)
	lua_register(m_luaState, "getGlobalStorageValue", LuaScriptInterface::luaGetGlobalStorageValue);

	//setGlobalStorageValue(valueid, newvalue)
	lua_register(m_luaState, "setGlobalStorageValue", LuaScriptInterface::luaSetGlobalStorageValue);

	//getOnlinePlayers()
	lua_register(m_luaState, "getOnlinePlayers", LuaScriptInterface::luaGetOnlinePlayers);

	//getTileHouseInfo(pos)
	//0 no house. != 0 house id
	lua_register(m_luaState, "getTileHouseInfo", LuaScriptInterface::luaGetTileHouseInfo);

	//getItemRWInfo(uid)
	lua_register(m_luaState, "getItemRWInfo", LuaScriptInterface::luaGetItemRWInfo);

	//getThingfromPos(pos)
	lua_register(m_luaState, "getThingfromPos", LuaScriptInterface::luaGetThingfromPos);

	//getThing(uid)
	lua_register(m_luaState, "getThing", LuaScriptInterface::luaGetThing);

	//getThingPos(uid)
	lua_register(m_luaState, "getThingPos", LuaScriptInterface::luaGetThingPos);

	//doPlayerFeed(cid, food)
	lua_register(m_luaState, "doPlayerFeed", LuaScriptInterface::luaDoFeedPlayer);

	//doPlayerSendCancel(cid, text)
	lua_register(m_luaState, "doPlayerSendCancel", LuaScriptInterface::luaDoPlayerSendCancel);

	//doPlayerSendDefaultCancel(cid, ReturnValue)
	lua_register(m_luaState, "doPlayerSendDefaultCancel", LuaScriptInterface::luaDoSendDefaultCancel);

	//doTeleportThing(cid, newpos, <optional> pushmove)
	lua_register(m_luaState, "doTeleportThing", LuaScriptInterface::luaDoTeleportThing);

	//doPlayerChangeName(cid, newName)
	lua_register(m_luaState, "doPlayerChangeName", LuaScriptInterface::luaDoPlayerChangeName);

	//doChangeTypeItem(uid, newtype)
	lua_register(m_luaState, "doChangeTypeItem", LuaScriptInterface::luaDoChangeTypeItem);

	//doSetItemText(uid, text)
	lua_register(m_luaState, "doSetItemText", LuaScriptInterface::luaDoSetItemText);

	//doSetItemSpecialDescription(uid, desc)
	lua_register(m_luaState, "doSetItemSpecialDescription", LuaScriptInterface::luaDoSetItemSpecialDescription);

	//doSendAnimatedText(pos, text, color)
	lua_register(m_luaState, "doSendAnimatedText", LuaScriptInterface::luaDoSendAnimatedText);

	//doPlayerAddSkillTry(cid, skillid, n)
	lua_register(m_luaState, "doPlayerAddSkillTry", LuaScriptInterface::luaDoPlayerAddSkillTry);

	//doPlayerAddMana(cid, mana[, animationOnLoss])
	lua_register(m_luaState, "doPlayerAddMana", LuaScriptInterface::luaDoPlayerAddMana);

	//doPlayerAddManaSpent(cid, mana)
	lua_register(m_luaState, "doPlayerAddManaSpent", LuaScriptInterface::luaDoPlayerAddManaSpent);

	//doPlayerAddItem(uid, itemid, <optional: default: 1> count/subtype)
	//doPlayerAddItem(cid, itemid, <optional: default: 1> count, <optional: default: 1> canDropOnMap, <optional: default: 1>subtype)
	//Returns uid of the created item
	lua_register(m_luaState, "doPlayerAddItem", LuaScriptInterface::luaDoPlayerAddItem);

	//doPlayerSendTextMessage(cid, MessageClasses, message[, position, value, color])
	lua_register(m_luaState, "doPlayerSendTextMessage", LuaScriptInterface::luaDoPlayerSendTextMessage);

	//doSendTutorial(cid, tutorialid)
	lua_register(m_luaState, "doSendTutorial", LuaScriptInterface::luaDoSendTutorial);

	//doAddMapMark(cid, pos, type, <optional> description)
	lua_register(m_luaState, "doAddMapMark", LuaScriptInterface::luaDoAddMark);

	//doDecayItem(uid)
	lua_register(m_luaState, "doDecayItem", LuaScriptInterface::luaDoDecayItem);

	//doCreateItem(itemid, type/count, pos)
	//Returns uid of the created item, only works on tiles.
	lua_register(m_luaState, "doCreateItem", LuaScriptInterface::luaDoCreateItem);

	//doCreateItemEx(itemid, <optional> count/subtype)
	lua_register(m_luaState, "doCreateItemEx", LuaScriptInterface::luaDoCreateItemEx);

	//doTileAddItemEx(pos, uid)
	lua_register(m_luaState, "doTileAddItemEx", LuaScriptInterface::luaDoTileAddItemEx);

	//doRelocate(pos, posTo)
	//Moves all moveable objects from pos to posTo
	lua_register(m_luaState, "doRelocate", LuaScriptInterface::luaDoRelocate);

	//doCreateTeleport(itemid, topos, createpos)
	lua_register(m_luaState, "doCreateTeleport", LuaScriptInterface::luaDoCreateTeleport);

	//doCreateNpc(name, pos)
	lua_register(m_luaState, "doCreateNpc", LuaScriptInterface::luaDoCreateNpc);

	//doSummonCreature(name, pos)
	lua_register(m_luaState, "doSummonCreature", LuaScriptInterface::luaDoSummonCreature);

	//doConvinceCreature(cid, target)
	lua_register(m_luaState, "doConvinceCreature", LuaScriptInterface::luaDoConvinceCreature);

	//getMonsterTargetList(cid)
	lua_register(m_luaState, "getMonsterTargetList", LuaScriptInterface::luaGetMonsterTargetList);

	//getMonsterFriendList(cid)
	lua_register(m_luaState, "getMonsterFriendList", LuaScriptInterface::luaGetMonsterFriendList);

	//doSetMonsterTarget(cid, target)
	lua_register(m_luaState, "doSetMonsterTarget", LuaScriptInterface::luaDoSetMonsterTarget);

	//doMonsterChangeTarget(cid)
	lua_register(m_luaState, "doMonsterChangeTarget", LuaScriptInterface::luaDoMonsterChangeTarget);

	//doAddCondition(cid, condition)
	lua_register(m_luaState, "doAddCondition", LuaScriptInterface::luaDoAddCondition);

	//doRemoveCondition(cid, type[, subId])
	lua_register(m_luaState, "doRemoveCondition", LuaScriptInterface::luaDoRemoveCondition);

	//doMoveCreature(cid, direction)
	lua_register(m_luaState, "doMoveCreature", LuaScriptInterface::luaDoMoveCreature);

	//doPlayerRemoveItem(cid, itemid, count, <optional> subtype, <optional> ignoreEquipped)
	lua_register(m_luaState, "doPlayerRemoveItem", LuaScriptInterface::luaDoPlayerRemoveItem);

	//doPlayerAddExp(cid, exp, <optional> usemultiplier, <optional> sendtext)
	lua_register(m_luaState, "doPlayerAddExp", LuaScriptInterface::luaDoPlayerAddExp);

	//doPlayerSetGuildLevel(cid, level)
	lua_register(m_luaState, "doPlayerSetGuildLevel", LuaScriptInterface::luaDoPlayerSetGuildLevel);

	//doPlayerSetGuildNick(cid, nick)
	lua_register(m_luaState, "doPlayerSetGuildNick", LuaScriptInterface::luaDoPlayerSetGuildNick);

	//doSetCreatureLight(cid, lightLevel, lightColor, time)
	lua_register(m_luaState, "doSetCreatureLight", LuaScriptInterface::luaDoSetCreatureLight);

	//doSetCreatureDropLoot(cid, doDrop)
	lua_register(m_luaState, "doSetCreatureDropLoot", LuaScriptInterface::luaDoSetCreatureDropLoot);

	//getPlayerParty(cid)
	lua_register(m_luaState, "getPlayerParty", LuaScriptInterface::luaGetPlayerParty);

	//doPlayerJoinParty(cid, leaderId)
	lua_register(m_luaState, "doPlayerJoinParty", LuaScriptInterface::luaDoPlayerJoinParty);

	//getPartyMembers(leaderId)
	lua_register(m_luaState, "getPartyMembers", LuaScriptInterface::luaGetPartyMembers);

	//getCreatureSummons(cid)
	//returns a table with all the summons of the creature
	lua_register(m_luaState, "getCreatureSummons", LuaScriptInterface::luaGetCreatureSummons);

	//getSpectators(centerPos, rangex, rangey, multifloor, onlyPlayers)
	lua_register(m_luaState, "getSpectators", LuaScriptInterface::luaGetSpectators);

	//getCreatureCondition(cid, condition[, subId])
	lua_register(m_luaState, "getCreatureCondition", LuaScriptInterface::luaGetCreatureCondition);

	//isValidUID(uid)
	lua_register(m_luaState, "isValidUID", LuaScriptInterface::luaIsValidUID);

	//isPlayerGhost(cid)
	lua_register(m_luaState, "isPlayerGhost", LuaScriptInterface::luaIsPlayerGhost);

	//isPlayerPzLocked(cid)
	lua_register(m_luaState, "isPlayerPzLocked", LuaScriptInterface::luaIsPlayerPzLocked);

	//isDepot(uid)
	lua_register(m_luaState, "isDepot", LuaScriptInterface::luaIsDepot);

	//isCorpse(uid)
	lua_register(m_luaState, "isCorpse", LuaScriptInterface::luaIsCorpse);

	//isMovable(uid)
	lua_register(m_luaState, "isMovable", LuaScriptInterface::luaIsMoveable);
	//isMoveable(uid)
	lua_register(m_luaState, "isMoveable", LuaScriptInterface::luaIsMoveable);

	//getPlayerGUIDByName(name)
	lua_register(m_luaState, "getPlayerGUIDByName", LuaScriptInterface::luaGetPlayerGUIDByName);

	//doAddContainerItem(uid, itemid, <optional> count/subtype)
	lua_register(m_luaState, "doAddContainerItem", LuaScriptInterface::luaDoAddContainerItem);

	//getDepotId(uid)
	lua_register(m_luaState, "getDepotId", LuaScriptInterface::luaGetDepotId);

	//getHouseRent(houseid)
	lua_register(m_luaState, "getHouseRent", LuaScriptInterface::luaGetHouseRent);

	//getHouseAccessList(houseid, listid)
	lua_register(m_luaState, "getHouseAccessList", LuaScriptInterface::luaGetHouseAccessList);

	//getHouseByPlayerGUID(playerGUID)
	lua_register(m_luaState, "getHouseByPlayerGUID", LuaScriptInterface::luaGetHouseByPlayerGUID);

	//setHouseAccessList(houseid, listid, listtext)
	lua_register(m_luaState, "setHouseAccessList", LuaScriptInterface::luaSetHouseAccessList);

	//setHouseOwner(houseid, ownerGUID)
	lua_register(m_luaState, "setHouseOwner", LuaScriptInterface::luaSetHouseOwner);

	//getWorldType()
	lua_register(m_luaState, "getWorldType", LuaScriptInterface::luaGetWorldType);

	//getWorldTime()
	lua_register(m_luaState, "getWorldTime", LuaScriptInterface::luaGetWorldTime);

	//getWorldLight()
	lua_register(m_luaState, "getWorldLight", LuaScriptInterface::luaGetWorldLight);

	//getWorldCreatures(type)
	//0 players, 1 monsters, 2 npcs, 3 all
	lua_register(m_luaState, "getWorldCreatures", LuaScriptInterface::luaGetWorldCreatures);

	//getWorldUpTime()
	lua_register(m_luaState, "getWorldUpTime", LuaScriptInterface::luaGetWorldUpTime);

	//broadcastMessage(message, type)
	lua_register(m_luaState, "broadcastMessage", LuaScriptInterface::luaBroadcastMessage);

	//createCombatArea( {area}, <optional> {extArea} )
	lua_register(m_luaState, "createCombatArea", LuaScriptInterface::luaCreateCombatArea);

	//createConditionObject(type)
	lua_register(m_luaState, "createConditionObject", LuaScriptInterface::luaCreateConditionObject);

	//setCombatArea(combat, area)
	lua_register(m_luaState, "setCombatArea", LuaScriptInterface::luaSetCombatArea);

	//setCombatCondition(combat, condition)
	lua_register(m_luaState, "setCombatCondition", LuaScriptInterface::luaSetCombatCondition);

	//setCombatParam(combat, key, value)
	lua_register(m_luaState, "setCombatParam", LuaScriptInterface::luaSetCombatParam);

	//setConditionParam(condition, key, value)
	lua_register(m_luaState, "setConditionParam", LuaScriptInterface::luaSetConditionParam);

	//addDamageCondition(condition, rounds, time, value)
	lua_register(m_luaState, "addDamageCondition", LuaScriptInterface::luaAddDamageCondition);

	//addOutfitCondition(condition, lookTypeEx, lookType, lookHead, lookBody, lookLegs, lookFeet)
	lua_register(m_luaState, "addOutfitCondition", LuaScriptInterface::luaAddOutfitCondition);

	//setCombatCallBack(combat, key, function_name)
	lua_register(m_luaState, "setCombatCallback", LuaScriptInterface::luaSetCombatCallBack);

	//setCombatFormula(combat, type, mina, minb, maxa, maxb)
	lua_register(m_luaState, "setCombatFormula", LuaScriptInterface::luaSetCombatFormula);

	//setConditionFormula(combat, mina, minb, maxa, maxb)
	lua_register(m_luaState, "setConditionFormula", LuaScriptInterface::luaSetConditionFormula);

	//doCombat(cid, combat, param)
	lua_register(m_luaState, "doCombat", LuaScriptInterface::luaDoCombat);

	//createCombatObject()
	lua_register(m_luaState, "createCombatObject", LuaScriptInterface::luaCreateCombatObject);

	//doAreaCombatHealth(cid, type, pos, area, min, max, effect)
	lua_register(m_luaState, "doAreaCombatHealth", LuaScriptInterface::luaDoAreaCombatHealth);

	//doTargetCombatHealth(cid, target, type, min, max, effect)
	lua_register(m_luaState, "doTargetCombatHealth", LuaScriptInterface::luaDoTargetCombatHealth);

	//doAreaCombatMana(cid, pos, area, min, max, effect)
	lua_register(m_luaState, "doAreaCombatMana", LuaScriptInterface::luaDoAreaCombatMana);

	//doTargetCombatMana(cid, target, min, max, effect)
	lua_register(m_luaState, "doTargetCombatMana", LuaScriptInterface::luaDoTargetCombatMana);

	//doAreaCombatCondition(cid, pos, area, condition, effect)
	lua_register(m_luaState, "doAreaCombatCondition", LuaScriptInterface::luaDoAreaCombatCondition);

	//doTargetCombatCondition(cid, target, condition, effect)
	lua_register(m_luaState, "doTargetCombatCondition", LuaScriptInterface::luaDoTargetCombatCondition);

	//doAreaCombatDispel(cid, pos, area, type, effect)
	lua_register(m_luaState, "doAreaCombatDispel", LuaScriptInterface::luaDoAreaCombatDispel);

	//doTargetCombatDispel(cid, target, type, effect)
	lua_register(m_luaState, "doTargetCombatDispel", LuaScriptInterface::luaDoTargetCombatDispel);

	//doChallengeCreature(cid, target)
	lua_register(m_luaState, "doChallengeCreature", LuaScriptInterface::luaDoChallengeCreature);

	//numberToVariant(number)
	lua_register(m_luaState, "numberToVariant", LuaScriptInterface::luaNumberToVariant);

	//stringToVariant(string)
	lua_register(m_luaState, "stringToVariant", LuaScriptInterface::luaStringToVariant);

	//positionToVariant(pos)
	lua_register(m_luaState, "positionToVariant", LuaScriptInterface::luaPositionToVariant);

	//targetPositionToVariant(pos)
	lua_register(m_luaState, "targetPositionToVariant", LuaScriptInterface::luaTargetPositionToVariant);

	//variantToNumber(var)
	lua_register(m_luaState, "variantToNumber", LuaScriptInterface::luaVariantToNumber);

	//variantToString(var)
	lua_register(m_luaState, "variantToString", LuaScriptInterface::luaVariantToString);

	//variantToPosition(var)
	lua_register(m_luaState, "variantToPosition", LuaScriptInterface::luaVariantToPosition);

	//doChangeSpeed(cid, delta)
	lua_register(m_luaState, "doChangeSpeed", LuaScriptInterface::luaDoChangeSpeed);

	//doSetMonsterOutfit(cid, name, time)
	lua_register(m_luaState, "doSetMonsterOutfit", LuaScriptInterface::luaSetMonsterOutfit);

	//doSetItemOutfit(cid, item, time)
	lua_register(m_luaState, "doSetItemOutfit", LuaScriptInterface::luaSetItemOutfit);

	//doSetCreatureOutfit(cid, outfit, time)
	lua_register(m_luaState, "doSetCreatureOutfit", LuaScriptInterface::luaSetCreatureOutfit);

	//getCreatureOutfit(cid)
	lua_register(m_luaState, "getCreatureOutfit", LuaScriptInterface::luaGetCreatureOutfit);

	//getCreatureSpeed(cid)
	lua_register(m_luaState, "getCreatureSpeed", LuaScriptInterface::luaGetCreatureSpeed);

	//getCreatureBaseSpeed(cid)
	lua_register(m_luaState, "getCreatureBaseSpeed", LuaScriptInterface::luaGetCreatureBaseSpeed);

	//getItemWeightByUID(uid)
	lua_register(m_luaState, "getItemWeightByUID", LuaScriptInterface::luaGetItemWeightByUID);

	//hasProperty(uid, prop)
	lua_register(m_luaState, "hasProperty", LuaScriptInterface::luaHasProperty);

	//getFluidSourceType(type)
	lua_register(m_luaState, "getFluidSourceType", LuaScriptInterface::luaGetFluidSourceType);

	//isInArray(array, value)
	lua_register(m_luaState, "isInArray", LuaScriptInterface::luaIsInArray);

	//addEvent(callback, delay, ...)
	lua_register(m_luaState, "addEvent", LuaScriptInterface::luaAddEvent);

	//stopEvent(eventid)
	lua_register(m_luaState, "stopEvent", LuaScriptInterface::luaStopEvent);

	//doPlayerPopupFYI(cid, message)
	lua_register(m_luaState, "doPlayerPopupFYI", LuaScriptInterface::luaDoPlayerPopupFYI);

	//mayNotMove(cid, value)
	lua_register(m_luaState, "mayNotMove", LuaScriptInterface::luaMayNotMove);

	//saveServer()
	//saveData()
	lua_register(m_luaState, "saveServer", LuaScriptInterface::luaSaveServer);
	lua_register(m_luaState, "saveData", LuaScriptInterface::luaSaveServer);

	//refreshMap()
	lua_register(m_luaState, "refreshMap", LuaScriptInterface::luaRefreshMap);

	//cleanMap()
	lua_register(m_luaState, "cleanMap", LuaScriptInterface::luaCleanMap);

	//getPlayersByAccountNumber(accountNumber)
	lua_register(m_luaState, "getPlayersByAccountNumber", LuaScriptInterface::luaGetPlayersByAccountNumber);

	//getAccountNumberByPlayerName(name)
	lua_register(m_luaState, "getAccountNumberByPlayerName", LuaScriptInterface::luaGetAccountNumberByPlayerName);

	//getPlayersByIPAddress(ip[, mask = 0xFFFFFFFF])
	lua_register(m_luaState, "getPlayersByIPAddress", LuaScriptInterface::luaGetPlayersByIPAddress);

	//debugPrint(text)
	lua_register(m_luaState, "debugPrint", LuaScriptInterface::luaDebugPrint);

	//isInWar(cid, target)
	lua_register(m_luaState, "isInWar", LuaScriptInterface::luaIsInWar);

	//doPlayerSetOfflineTrainingSkill(cid, skill)
	lua_register(m_luaState, "doPlayerSetOfflineTrainingSkill", LuaScriptInterface::luaDoPlayerSetOfflineTrainingSkill);

	//getWaypointPosition(name)
	lua_register(m_luaState, "getWaypointPosition", LuaScriptInterface::luaGetWaypointPosition);

	//doWaypointAddTemporial(name, pos)
	lua_register(m_luaState, "doWaypointAddTemporial", LuaScriptInterface::luaDoWaypointAddTemporial);

	//sendChannelMessage(channelId, type, message)
	lua_register(m_luaState, "sendChannelMessage", LuaScriptInterface::luaSendChannelMessage);

	//sendGuildChannelMessage(guildId, type, message)
	lua_register(m_luaState, "sendGuildChannelMessage", LuaScriptInterface::luaSendGuildChannelMessage);

#ifndef __LUAJIT__
	//bit operations for Lua, based on bitlib project release 24
	//bit.bnot, bit.band, bit.bor, bit.bxor, bit.lshift, bit.rshift
	luaL_register(m_luaState, "bit", LuaScriptInterface::luaBitReg);
#endif

	//configManager table
	luaL_register(m_luaState, "configManager", LuaScriptInterface::luaConfigManagerTable);

	//db table
	luaL_register(m_luaState, "db", LuaScriptInterface::luaDatabaseTable);

	//result table
	luaL_register(m_luaState, "result", LuaScriptInterface::luaResultTable);
	
	/* New functions */
	//registerClass(className, baseClass, newFunction)
	//registerClassMethod(className, functionName, function)
	//registerMetaMethod(className, functionName, function)
	//registerGlobalMethod(functionName, function)

	// _G
	registerGlobalMethod("isType", LuaScriptInterface::luaIsType);

	// Position
	registerClass("Position", "", LuaScriptInterface::luaPositionCreate);
	registerMetaMethod("Position", "__add", LuaScriptInterface::luaPositionAdd);
	registerMetaMethod("Position", "__sub", LuaScriptInterface::luaPositionSub);
	registerMetaMethod("Position", "__eq", LuaScriptInterface::luaPositionCompare);

	registerClassMethod("Position", "getTile", LuaScriptInterface::luaPositionGetTile);
	registerClassMethod("Position", "getDistance", LuaScriptInterface::luaPositionGetDistance);
	registerClassMethod("Position", "isSightClear", LuaScriptInterface::luaPositionIsSightClear);

	registerClassMethod("Position", "sendMagicEffect", LuaScriptInterface::luaPositionSendMagicEffect);
	registerClassMethod("Position", "sendDistanceEffect", LuaScriptInterface::luaPositionSendDistanceEffect);

	// Tile
	registerClass("Tile", "", LuaScriptInterface::luaTileCreate);
	registerClassMethod("Tile", "getPosition", LuaScriptInterface::luaTileGetPosition);

	registerClassMethod("Tile", "getGround", LuaScriptInterface::luaTileGetGround);
	registerClassMethod("Tile", "getThing", LuaScriptInterface::luaTileGetThing);
	registerClassMethod("Tile", "getThingCount", LuaScriptInterface::luaTileGetThingCount);

	registerClassMethod("Tile", "getTopTopItem", LuaScriptInterface::luaTileGetTopTopItem);
	registerClassMethod("Tile", "getTopDownItem", LuaScriptInterface::luaTileGetTopDownItem);
	registerClassMethod("Tile", "getFieldItem", LuaScriptInterface::luaTileGetFieldItem);
	
	registerClassMethod("Tile", "getItemById", LuaScriptInterface::luaTileGetItemById);
	registerClassMethod("Tile", "getItemByType", LuaScriptInterface::luaTileGetItemByType);
	registerClassMethod("Tile", "getItemByTopOrder", LuaScriptInterface::luaTileGetItemByTopOrder);

	registerClassMethod("Tile", "getBottomCreature", LuaScriptInterface::luaTileGetBottomCreature);
	registerClassMethod("Tile", "getTopCreature", LuaScriptInterface::luaTileGetTopCreature);
	registerClassMethod("Tile", "getBottomVisibleCreature", LuaScriptInterface::luaTileGetBottomVisibleCreature);
	registerClassMethod("Tile", "getTopVisibleCreature", LuaScriptInterface::luaTileGetTopVisibleCreature);

	registerClassMethod("Tile", "getItems", LuaScriptInterface::luaTileGetItems);
	registerClassMethod("Tile", "getItemCount", LuaScriptInterface::luaTileGetItemCount);
	registerClassMethod("Tile", "getDownItemCount", LuaScriptInterface::luaTileGetDownItemCount);
	registerClassMethod("Tile", "getTopItemCount", LuaScriptInterface::luaTileGetTopItemCount);

	registerClassMethod("Tile", "getCreatures", LuaScriptInterface::luaTileGetCreatures);
	registerClassMethod("Tile", "getCreatureCount", LuaScriptInterface::luaTileGetCreatureCount);

	registerClassMethod("Tile", "hasProperty", LuaScriptInterface::luaTileHasProperty);
	registerClassMethod("Tile", "hasFlag", LuaScriptInterface::luaTileHasFlag);
	
	registerClassMethod("Tile", "queryAdd", LuaScriptInterface::luaTileQueryAdd);

	// NetworkMessage
	registerClass("NetworkMessage", "", LuaScriptInterface::luaNetworkMessageCreate);
	registerMetaMethod("NetworkMessage", "__gc", LuaScriptInterface::luaNetworkMessageDelete);

	registerClassMethod("NetworkMessage", "getByte", LuaScriptInterface::luaNetworkMessageGetByte);
	registerClassMethod("NetworkMessage", "getU16", LuaScriptInterface::luaNetworkMessageGetU16);
	registerClassMethod("NetworkMessage", "getU32", LuaScriptInterface::luaNetworkMessageGetU32);
	registerClassMethod("NetworkMessage", "getU64", LuaScriptInterface::luaNetworkMessageGetU64);
	registerClassMethod("NetworkMessage", "getString", LuaScriptInterface::luaNetworkMessageGetString);
	registerClassMethod("NetworkMessage", "getPosition", LuaScriptInterface::luaNetworkMessageGetPosition);

	registerClassMethod("NetworkMessage", "addByte", LuaScriptInterface::luaNetworkMessageAddByte);
	registerClassMethod("NetworkMessage", "addU16", LuaScriptInterface::luaNetworkMessageAddU16);
	registerClassMethod("NetworkMessage", "addU32", LuaScriptInterface::luaNetworkMessageAddU32);
	registerClassMethod("NetworkMessage", "addU64", LuaScriptInterface::luaNetworkMessageAddU64);
	registerClassMethod("NetworkMessage", "addString", LuaScriptInterface::luaNetworkMessageAddString);
	registerClassMethod("NetworkMessage", "addPosition", LuaScriptInterface::luaNetworkMessageAddPosition);
	registerClassMethod("NetworkMessage", "addDouble", LuaScriptInterface::luaNetworkMessageAddDouble);
	registerClassMethod("NetworkMessage", "addItem", LuaScriptInterface::luaNetworkMessageAddItem);
	registerClassMethod("NetworkMessage", "addItemId", LuaScriptInterface::luaNetworkMessageAddItemId);

	registerClassMethod("NetworkMessage", "reset", LuaScriptInterface::luaNetworkMessageReset);
	registerClassMethod("NetworkMessage", "skipBytes", LuaScriptInterface::luaNetworkMessageSkipBytes);
	registerClassMethod("NetworkMessage", "sendToPlayer", LuaScriptInterface::luaNetworkMessageSendToPlayer);

	// ModalWindow
	registerClass("ModalWindow", "", LuaScriptInterface::luaModalWindowCreate);
	registerMetaMethod("ModalWindow", "__gc", LuaScriptInterface::luaModalWindowDelete);

	registerClassMethod("ModalWindow", "getId", LuaScriptInterface::luaModalWindowGetId);
	registerClassMethod("ModalWindow", "getTitle", LuaScriptInterface::luaModalWindowGetTitle);
	registerClassMethod("ModalWindow", "getMessage", LuaScriptInterface::luaModalWindowGetMessage);

	registerClassMethod("ModalWindow", "getButtonCount", LuaScriptInterface::luaModalWindowGetButtonCount);
	registerClassMethod("ModalWindow", "getChoiceCount", LuaScriptInterface::luaModalWindowGetChoiceCount);

	registerClassMethod("ModalWindow", "addButton", LuaScriptInterface::luaModalWindowAddButton);
	registerClassMethod("ModalWindow", "addChoice", LuaScriptInterface::luaModalWindowAddChoice);

	registerClassMethod("ModalWindow", "getDefaultEnterButton", LuaScriptInterface::luaModalWindowGetDefaultEnterButton);
	registerClassMethod("ModalWindow", "setDefaultEnterButton", LuaScriptInterface::luaModalWindowSetDefaultEnterButton);

	registerClassMethod("ModalWindow", "getDefaultEscapeButton", LuaScriptInterface::luaModalWindowGetDefaultEscapeButton);
	registerClassMethod("ModalWindow", "setDefaultEscapeButton", LuaScriptInterface::luaModalWindowSetDefaultEscapeButton);

	registerClassMethod("ModalWindow", "hasPriority", LuaScriptInterface::luaModalWindowHasPriority);
	registerClassMethod("ModalWindow", "setPriority", LuaScriptInterface::luaModalWindowSetPriority);

	registerClassMethod("ModalWindow", "sendToPlayer", LuaScriptInterface::luaModalWindowSendToPlayer);

	// Item
	registerClass("Item", "", LuaScriptInterface::luaItemCreate);
	
	registerClassMethod("Item", "isCreature", LuaScriptInterface::luaItemIsCreature);
	registerClassMethod("Item", "isItem", LuaScriptInterface::luaItemIsItem);

	registerClassMethod("Item", "getId", LuaScriptInterface::luaItemGetId);

	registerClassMethod("Item", "clone", LuaScriptInterface::luaItemClone);
	registerClassMethod("Item", "split", LuaScriptInterface::luaItemSplit);
	registerClassMethod("Item", "remove", LuaScriptInterface::luaItemRemove);

	registerClassMethod("Item", "getUniqueId", LuaScriptInterface::luaItemGetUniqueId);
	registerClassMethod("Item", "getActionId", LuaScriptInterface::luaItemGetActionId);
	registerClassMethod("Item", "setActionId", LuaScriptInterface::luaItemSetActionId);

	registerClassMethod("Item", "getCount", LuaScriptInterface::luaItemGetCount);
	registerClassMethod("Item", "getCharges", LuaScriptInterface::luaItemGetCharges);
	registerClassMethod("Item", "getFluidType", LuaScriptInterface::luaItemGetFluidType);

	registerClassMethod("Item", "hasSubType", LuaScriptInterface::luaItemHasSubType);
	registerClassMethod("Item", "getSubType", LuaScriptInterface::luaItemGetSubType);

	registerClassMethod("Item", "getName", LuaScriptInterface::luaItemGetName);
	registerClassMethod("Item", "getPluralName", LuaScriptInterface::luaItemGetPluralName);
	registerClassMethod("Item", "getArticle", LuaScriptInterface::luaItemGetArticle);

	registerClassMethod("Item", "getPosition", LuaScriptInterface::luaItemGetPosition);
	registerClassMethod("Item", "getTile", LuaScriptInterface::luaItemGetTile);

	registerClassMethod("Item", "getAttribute", LuaScriptInterface::luaItemGetAttribute);
	registerClassMethod("Item", "setAttribute", LuaScriptInterface::luaItemSetAttribute);
	registerClassMethod("Item", "removeAttribute", LuaScriptInterface::luaItemRemoveAttribute);

	registerClassMethod("Item", "moveTo", LuaScriptInterface::luaItemMoveTo);
	registerClassMethod("Item", "transform", LuaScriptInterface::luaItemTransform);

	// Container
	registerClass("Container", "Item", LuaScriptInterface::luaContainerCreate);

	registerClassMethod("Container", "getSize", LuaScriptInterface::luaContainerGetSize);
	registerClassMethod("Container", "getCapacity", LuaScriptInterface::luaContainerGetCapacity);
	registerClassMethod("Container", "getEmptySlots", LuaScriptInterface::luaContainerGetEmptySlots);

	registerClassMethod("Container", "getItem", LuaScriptInterface::luaContainerGetItem);
	registerClassMethod("Container", "hasItem", LuaScriptInterface::luaContainerHasItem);
	registerClassMethod("Container", "addItem", LuaScriptInterface::luaContainerAddItem);
	registerClassMethod("Container", "addItemEx", LuaScriptInterface::luaContainerAddItemEx);

	// Creature
	registerClass("Creature", "", LuaScriptInterface::luaCreatureCreate);
	
	registerClassMethod("Creature", "registerEvent", LuaScriptInterface::luaCreatureRegisterEvent);
	registerClassMethod("Creature", "unregisterEvent", LuaScriptInterface::luaCreatureUnregisterEvent);

	registerClassMethod("Creature", "isCreature", LuaScriptInterface::luaCreatureIsCreature);
	registerClassMethod("Creature", "isPlayer", LuaScriptInterface::luaCreatureIsPlayer);
	registerClassMethod("Creature", "isMonster", LuaScriptInterface::luaCreatureIsMonster);
	registerClassMethod("Creature", "isNpc", LuaScriptInterface::luaCreatureIsNpc);
	registerClassMethod("Creature", "isItem", LuaScriptInterface::luaCreatureIsItem);

	registerClassMethod("Creature", "getId", LuaScriptInterface::luaCreatureGetId);
	registerClassMethod("Creature", "getName", LuaScriptInterface::luaCreatureGetName);
	registerClassMethod("Creature", "getTarget", LuaScriptInterface::luaCreatureGetTarget);
	registerClassMethod("Creature", "getMaster", LuaScriptInterface::luaCreatureGetMaster);
	
	registerClassMethod("Creature", "getLight", LuaScriptInterface::luaCreatureGetLight);
	registerClassMethod("Creature", "setLight", LuaScriptInterface::luaCreatureSetLight);

	registerClassMethod("Creature", "getPosition", LuaScriptInterface::luaCreatureGetPosition);
	registerClassMethod("Creature", "getTile", LuaScriptInterface::luaCreatureGetTile);
	registerClassMethod("Creature", "getDirection", LuaScriptInterface::luaCreatureGetDirection);
	registerClassMethod("Creature", "setDirection", LuaScriptInterface::luaCreatureSetDirection);

	registerClassMethod("Creature", "getHealth", LuaScriptInterface::luaCreatureGetHealth);
	registerClassMethod("Creature", "addHealth", LuaScriptInterface::luaCreatureAddHealth);
	registerClassMethod("Creature", "getMaxHealth", LuaScriptInterface::luaCreatureGetMaxHealth);
	registerClassMethod("Creature", "setMaxHealth", LuaScriptInterface::luaCreatureSetMaxHealth);

	registerClassMethod("Creature", "getMana", LuaScriptInterface::luaCreatureGetMana);
	registerClassMethod("Creature", "addMana", LuaScriptInterface::luaCreatureAddMana);
	registerClassMethod("Creature", "getMaxMana", LuaScriptInterface::luaCreatureGetMaxMana);
	registerClassMethod("Creature", "setMaxMana", LuaScriptInterface::luaCreatureSetMaxMana);

	registerClassMethod("Creature", "getOutfit", LuaScriptInterface::luaCreatureGetOutfit);
	registerClassMethod("Creature", "setOutfit", LuaScriptInterface::luaCreatureSetOutfit);

	registerClassMethod("Creature", "remove", LuaScriptInterface::luaCreatureRemove);
	registerClassMethod("Creature", "teleportTo", LuaScriptInterface::luaCreatureTeleportTo);
	registerClassMethod("Creature", "say", LuaScriptInterface::luaCreatureSay);

	registerClassMethod("Creature", "getDamageMap", LuaScriptInterface::luaCreatureGetDamageMap);

	// Player
	registerClass("Player", "Creature", LuaScriptInterface::luaPlayerCreate);

	registerClassMethod("Player", "isPlayer", LuaScriptInterface::luaPlayerIsPlayer);

	registerClassMethod("Player", "getGuid", LuaScriptInterface::luaPlayerGetGuid);
	registerClassMethod("Player", "getIp", LuaScriptInterface::luaPlayerGetIp);
	registerClassMethod("Player", "getAccountType", LuaScriptInterface::luaPlayerGetAccountType);
	registerClassMethod("Player", "getLastLoginSaved", LuaScriptInterface::luaPlayerGetLastLoginSaved);

	registerClassMethod("Player", "getFreeCapacity", LuaScriptInterface::luaPlayerGetFreeCapacity);
	registerClassMethod("Player", "getDepotItems", LuaScriptInterface::luaPlayerGetDepotItems);
	
	registerClassMethod("Player", "getSkull", LuaScriptInterface::luaPlayerGetSkull);
	registerClassMethod("Player", "setSkull", LuaScriptInterface::luaPlayerSetSkull);
	registerClassMethod("Player", "getSkullTime", LuaScriptInterface::luaPlayerGetSkullTime);
	registerClassMethod("Player", "setSkullTime", LuaScriptInterface::luaPlayerSetSkullTime);
	registerClassMethod("Player", "getDeathPenalty", LuaScriptInterface::luaPlayerGetDeathPenalty);

	registerClassMethod("Player", "getExperience", LuaScriptInterface::luaPlayerGetExperience);
	registerClassMethod("Player", "addExperience", LuaScriptInterface::luaPlayerAddExperience);
	registerClassMethod("Player", "getLevel", LuaScriptInterface::luaPlayerGetLevel);

	registerClassMethod("Player", "getMagicLevel", LuaScriptInterface::luaPlayerGetMagicLevel);
	registerClassMethod("Player", "getManaSpent", LuaScriptInterface::luaPlayerGetManaSpent);
	registerClassMethod("Player", "addManaSpent", LuaScriptInterface::luaPlayerAddManaSpent);

	registerClassMethod("Player", "getSkillLevel", LuaScriptInterface::luaPlayerGetSkillLevel);
	registerClassMethod("Player", "getSkillPercent", LuaScriptInterface::luaPlayerGetSkillPercent);
	registerClassMethod("Player", "getSkillTries", LuaScriptInterface::luaPlayerGetSkillTries);
	registerClassMethod("Player", "addSkillTries", LuaScriptInterface::luaPlayerAddSkillTries);
	
	registerClassMethod("Player", "getItemCount", LuaScriptInterface::luaPlayerGetItemCount);
	registerClassMethod("Player", "getItemById", LuaScriptInterface::luaPlayerGetItemById);

	registerClassMethod("Player", "getVocation", LuaScriptInterface::luaPlayerGetVocation);
	registerClassMethod("Player", "setVocation", LuaScriptInterface::luaPlayerSetVocation);

	registerClassMethod("Player", "getSex", LuaScriptInterface::luaPlayerGetSex);
	registerClassMethod("Player", "setSex", LuaScriptInterface::luaPlayerSetSex);

	registerClassMethod("Player", "getTown", LuaScriptInterface::luaPlayerGetTown);
	registerClassMethod("Player", "setTown", LuaScriptInterface::luaPlayerSetTown);

	registerClassMethod("Player", "getGuild", LuaScriptInterface::luaPlayerGetGuild);
	registerClassMethod("Player", "setGuild", LuaScriptInterface::luaPlayerSetGuild);

	registerClassMethod("Player", "getGuildLevel", LuaScriptInterface::luaPlayerGetGuildLevel);
	registerClassMethod("Player", "getGuildNick", LuaScriptInterface::luaPlayerGetGuildNick);

	registerClassMethod("Player", "getGroup", LuaScriptInterface::luaPlayerGetGroup);
	registerClassMethod("Player", "setGroup", LuaScriptInterface::luaPlayerSetGroup);

	registerClassMethod("Player", "getStamina", LuaScriptInterface::luaPlayerGetStamina);

	registerClassMethod("Player", "getSoul", LuaScriptInterface::luaPlayerGetSoul);
	registerClassMethod("Player", "addSoul", LuaScriptInterface::luaPlayerAddSoul);
	registerClassMethod("Player", "getMaxSoul", LuaScriptInterface::luaPlayerGetMaxSoul);

	registerClassMethod("Player", "getBankBalance", LuaScriptInterface::luaPlayerGetBankBalance);
	registerClassMethod("Player", "setBankBalance", LuaScriptInterface::luaPlayerSetBankBalance);

	registerClassMethod("Player", "getStorageValue", LuaScriptInterface::luaPlayerGetStorageValue);
	registerClassMethod("Player", "setStorageValue", LuaScriptInterface::luaPlayerSetStorageValue);

	registerClassMethod("Player", "addItem", LuaScriptInterface::luaPlayerAddItem);
	registerClassMethod("Player", "addItemEx", LuaScriptInterface::luaPlayerAddItemEx);
	registerClassMethod("Player", "removeItem", LuaScriptInterface::luaPlayerRemoveItem);

	registerClassMethod("Player", "getMoney", LuaScriptInterface::luaPlayerGetMoney);
	registerClassMethod("Player", "addMoney", LuaScriptInterface::luaPlayerAddMoney);
	registerClassMethod("Player", "removeMoney", LuaScriptInterface::luaPlayerRemoveMoney);

	registerClassMethod("Player", "showTextDialog", LuaScriptInterface::luaPlayerShowTextDialog);

	registerClassMethod("Player", "sendTextMessage", LuaScriptInterface::luaPlayerSendTextMessage);
	registerClassMethod("Player", "sendChannelMessage", LuaScriptInterface::luaPlayerSendChannelMessage);
	registerClassMethod("Player", "channelSay", LuaScriptInterface::luaPlayerChannelSay);
	registerClassMethod("Player", "openChannel", LuaScriptInterface::luaPlayerOpenChannel);

	registerClassMethod("Player", "getSlotItem", LuaScriptInterface::luaPlayerGetSlotItem);
	
	registerClassMethod("Player", "addOutfit", LuaScriptInterface::luaPlayerAddOutfit);
	registerClassMethod("Player", "addOutfitAddon", LuaScriptInterface::luaPlayerAddOutfitAddon);
	registerClassMethod("Player", "removeOutfit", LuaScriptInterface::luaPlayerRemoveOutfit);
	registerClassMethod("Player", "removeOutfitAddon", LuaScriptInterface::luaPlayerRemoveOutfitAddon);
	registerClassMethod("Player", "hasOutfit", LuaScriptInterface::luaPlayerHasOutfit);
	registerClassMethod("Player", "sendOutfitWindow", LuaScriptInterface::luaPlayerSendOutfitWindow);

	registerClassMethod("Player", "addMount", LuaScriptInterface::luaPlayerAddMount);
	registerClassMethod("Player", "removeMount", LuaScriptInterface::luaPlayerRemoveMount);
	registerClassMethod("Player", "hasMount", LuaScriptInterface::luaPlayerHasMount);
	
	registerClassMethod("Player", "getPremiumDays", LuaScriptInterface::luaPlayerGetPremiumDays);
	registerClassMethod("Player", "addPremiumDays", LuaScriptInterface::luaPlayerAddPremiumDays);
	registerClassMethod("Player", "removePremiumDays", LuaScriptInterface::luaPlayerRemovePremiumDays);

	registerClassMethod("Player", "hasBlessing", LuaScriptInterface::luaPlayerHasBlessing);
	registerClassMethod("Player", "addBlessing", LuaScriptInterface::luaPlayerAddBlessing);
	registerClassMethod("Player", "removeBlessing", LuaScriptInterface::luaPlayerRemoveBlessing);
	
	registerClassMethod("Player", "save", LuaScriptInterface::luaPlayerSave);

	// Monster
	registerClass("Monster", "Creature", LuaScriptInterface::luaMonsterCreate);

	registerClassMethod("Monster", "isMonster", LuaScriptInterface::luaMonsterIsMonster);

	// Npc
	registerClass("Npc", "Creature", LuaScriptInterface::luaNpcCreate);

	registerClassMethod("Npc", "isNpc", LuaScriptInterface::luaNpcIsNpc);

	// Guild
	registerClass("Guild", "", LuaScriptInterface::luaGuildCreate);
	registerClassMethod("Guild", "getId", LuaScriptInterface::luaGuildGetId);
	registerClassMethod("Guild", "getName", LuaScriptInterface::luaGuildGetName);
	registerClassMethod("Guild", "getMembersOnline", LuaScriptInterface::luaGuildGetMembersOnline);

	registerClassMethod("Guild", "addMember", LuaScriptInterface::luaGuildAddMember);
	registerClassMethod("Guild", "removeMember", LuaScriptInterface::luaGuildRemoveMember);

	registerClassMethod("Guild", "addRank", LuaScriptInterface::luaGuildAddRank);
	registerClassMethod("Guild", "getRankById", LuaScriptInterface::luaGuildGetRankById);
	registerClassMethod("Guild", "getRankByLevel", LuaScriptInterface::luaGuildGetRankByLevel);
	
	registerClassMethod("Guild", "getMotd", LuaScriptInterface::luaGuildGetMotd);
	registerClassMethod("Guild", "setMotd", LuaScriptInterface::luaGuildSetMotd);

	// Group
	registerClass("Group", "", LuaScriptInterface::luaGroupCreate);
	registerClassMethod("Group", "getId", LuaScriptInterface::luaGroupGetId);
	registerClassMethod("Group", "getName", LuaScriptInterface::luaGroupGetName);
	registerClassMethod("Group", "getFlags", LuaScriptInterface::luaGroupGetFlags);
	registerClassMethod("Group", "getAccess", LuaScriptInterface::luaGroupGetAccess);
	registerClassMethod("Group", "getMaxDepotItems", LuaScriptInterface::luaGroupGetMaxDepotItems);
	registerClassMethod("Group", "getMaxVipEntries", LuaScriptInterface::luaGroupGetMaxVipEntries);

	// Vocation
	registerClass("Vocation", "", LuaScriptInterface::luaVocationCreate);
	registerClassMethod("Vocation", "getId", LuaScriptInterface::luaVocationGetId);
	registerClassMethod("Vocation", "getClientId", LuaScriptInterface::luaVocationGetClientId);
	registerClassMethod("Vocation", "getName", LuaScriptInterface::luaVocationGetName);
	registerClassMethod("Vocation", "getDescription", LuaScriptInterface::luaVocationGetDescription);
	
	registerClassMethod("Vocation", "getRequiredSkillTries", LuaScriptInterface::luaVocationGetRequiredSkillTries);
	registerClassMethod("Vocation", "getRequiredManaSpent", LuaScriptInterface::luaVocationGetRequiredManaSpent);

	registerClassMethod("Vocation", "getCapacityGain", LuaScriptInterface::luaVocationGetCapacityGain);
	
	registerClassMethod("Vocation", "getHealthGain", LuaScriptInterface::luaVocationGetHealthGain);
	registerClassMethod("Vocation", "getHealthGainTicks", LuaScriptInterface::luaVocationGetHealthGainTicks);
	registerClassMethod("Vocation", "getHealthGainAmount", LuaScriptInterface::luaVocationGetHealthGainAmount);

	registerClassMethod("Vocation", "getManaGain", LuaScriptInterface::luaVocationGetManaGain);
	registerClassMethod("Vocation", "getManaGainTicks", LuaScriptInterface::luaVocationGetManaGainTicks);
	registerClassMethod("Vocation", "getManaGainAmount", LuaScriptInterface::luaVocationGetManaGainTicks);
	
	registerClassMethod("Vocation", "getMaxSoul", LuaScriptInterface::luaVocationGetMaxSoul);
	registerClassMethod("Vocation", "getSoulGainTicks", LuaScriptInterface::luaVocationGetSoulGainTicks);
	
	registerClassMethod("Vocation", "getAttackSpeed", LuaScriptInterface::luaVocationGetAttackSpeed);
	registerClassMethod("Vocation", "getBaseSpeed", LuaScriptInterface::luaVocationGetBaseSpeed);

	registerClassMethod("Vocation", "getDemotion", LuaScriptInterface::luaVocationGetDemotion);
	registerClassMethod("Vocation", "getPromotion", LuaScriptInterface::luaVocationGetPromotion);

	// Town
	registerClass("Town", "", LuaScriptInterface::luaTownCreate);
	registerClassMethod("Town", "getId", LuaScriptInterface::luaTownGetId);
	registerClassMethod("Town", "getName", LuaScriptInterface::luaTownGetName);
	registerClassMethod("Town", "getTemplePosition", LuaScriptInterface::luaTownGetTemplePosition);

	// House
	registerClass("House", "", LuaScriptInterface::luaHouseCreate);
	
	registerClassMethod("House", "getId", LuaScriptInterface::luaHouseGetId);
	registerClassMethod("House", "getName", LuaScriptInterface::luaHouseGetName);
	registerClassMethod("House", "getTown", LuaScriptInterface::luaHouseGetTown);
	registerClassMethod("House", "getOwnerGuid", LuaScriptInterface::luaHouseGetOwnerGuid);
	registerClassMethod("House", "getExitPosition", LuaScriptInterface::luaHouseGetExitPosition);
	
	registerClassMethod("House", "getBeds", LuaScriptInterface::luaHouseGetBeds);
	registerClassMethod("House", "getBedCount", LuaScriptInterface::luaHouseGetBedCount);
	
	registerClassMethod("House", "getDoors", LuaScriptInterface::luaHouseGetDoors);
	registerClassMethod("House", "getDoorCount", LuaScriptInterface::luaHouseGetDoorCount);
	
	registerClassMethod("House", "getTiles", LuaScriptInterface::luaHouseGetTiles);
	registerClassMethod("House", "getTileCount", LuaScriptInterface::luaHouseGetTileCount);

	// ItemType
	registerClass("ItemType", "", LuaScriptInterface::luaItemTypeCreate);
	
	registerClassMethod("ItemType", "isCorpse", LuaScriptInterface::luaItemTypeIsCorpse);
	registerClassMethod("ItemType", "isDoor", LuaScriptInterface::luaItemTypeIsDoor);
	registerClassMethod("ItemType", "isContainer", LuaScriptInterface::luaItemTypeIsContainer);
	registerClassMethod("ItemType", "isFluidContainer", LuaScriptInterface::luaItemTypeIsFluidContainer);
	registerClassMethod("ItemType", "isMovable", LuaScriptInterface::luaItemTypeIsMovable);
	registerClassMethod("ItemType", "isRune", LuaScriptInterface::luaItemTypeIsRune);
	registerClassMethod("ItemType", "isStackable", LuaScriptInterface::luaItemTypeIsStackable);
	
	registerClassMethod("ItemType", "getType", LuaScriptInterface::luaItemTypeGetType);
	registerClassMethod("ItemType", "getId", LuaScriptInterface::luaItemTypeGetId);
	registerClassMethod("ItemType", "getName", LuaScriptInterface::luaItemTypeGetName);
	registerClassMethod("ItemType", "getPluralName", LuaScriptInterface::luaItemTypeGetPluralName);
	registerClassMethod("ItemType", "getArticle", LuaScriptInterface::luaItemTypeGetArticle);
	registerClassMethod("ItemType", "getDescription", LuaScriptInterface::luaItemTypeGetDescription);

	registerClassMethod("ItemType", "getFluidSource", LuaScriptInterface::luaItemTypeGetFluidSource);
	registerClassMethod("ItemType", "getCapacity", LuaScriptInterface::luaItemTypeGetCapacity);
	registerClassMethod("ItemType", "getWeight", LuaScriptInterface::luaItemTypeGetWeight);
}

void LuaScriptInterface::registerClass(const std::string& className, const std::string& baseClass, lua_CFunction newFunction/* = nullptr*/)
{
	// className = {}
	lua_newtable(m_luaState);
	lua_pushvalue(m_luaState, -1);
	lua_setglobal(m_luaState, className.c_str());
	int32_t methods = getStackTop(m_luaState);

	// methodsTable = {}
	lua_newtable(m_luaState);
	int32_t methodsTable = getStackTop(m_luaState);

	if (newFunction) {
		// className.new = newFunction
		// className.__call = newFunction
		lua_pushcfunction(m_luaState, newFunction);
		lua_pushvalue(m_luaState, -1);
		lua_setfield(m_luaState, methods, "new");
		lua_setfield(m_luaState, methodsTable, "__call");
	}

	uint32_t parents = 0;
	if (!baseClass.empty()) {
		lua_getglobal(m_luaState, baseClass.c_str());
		lua_rawgeti(m_luaState, -1, 'p');
		parents = popNumber(m_luaState) + 1;
		lua_setfield(m_luaState, methodsTable, "__index");
	}

	// setmetatable(className, methodsTable)
	lua_setmetatable(m_luaState, methods);

	// className.metatable = {}
	luaL_newmetatable(m_luaState, className.c_str());
	int32_t metatable = getStackTop(m_luaState);

	// className.metatable.__metatable = className
	lua_pushvalue(m_luaState, methods);
	lua_setfield(m_luaState, metatable, "__metatable");

	// className.metatable.__index = className
	lua_pushvalue(m_luaState, methods);
	lua_setfield(m_luaState, metatable, "__index");

	// className.metatable['h'] = hash
	lua_pushnumber(m_luaState, std::hash<std::string>()(className));
	lua_rawseti(m_luaState, metatable, 'h');

	// className.metatable['p'] = parents
	lua_pushnumber(m_luaState, parents);
	lua_rawseti(m_luaState, metatable, 'p');

	// className.metatable['t'] = type
	if (className == "Item") {
		lua_pushnumber(m_luaState, LuaData_Item);
	} else if (className == "Container") {
		lua_pushnumber(m_luaState, LuaData_Container);
	} else if (className == "Player") {
		lua_pushnumber(m_luaState, LuaData_Player);
	} else if (className == "Monster") {
		lua_pushnumber(m_luaState, LuaData_Monster);
	} else if (className == "Npc") {
		lua_pushnumber(m_luaState, LuaData_Npc);
	} else {
		lua_pushnumber(m_luaState, LuaData_Unknown);
	}
	lua_rawseti(m_luaState, metatable, 't');

	// pop className, className.metatable
	lua_pop(m_luaState, 2);
}

void LuaScriptInterface::registerClassMethod(const std::string& className, const std::string& methodName, lua_CFunction func)
{
	// className.methodName = func
	lua_getglobal(m_luaState, className.c_str());
	lua_pushcfunction(m_luaState, func);
	lua_setfield(m_luaState, -2, methodName.c_str());

	// pop className
	lua_pop(m_luaState, 1);
}

void LuaScriptInterface::registerMetaMethod(const std::string& className, const std::string& methodName, lua_CFunction func)
{
	// className.metatable.methodName = func
	luaL_getmetatable(m_luaState, className.c_str());
	lua_pushcfunction(m_luaState, func);
	lua_setfield(m_luaState, -2, methodName.c_str());

	// pop className.metatable
	lua_pop(m_luaState, 1);
}

void LuaScriptInterface::registerGlobalMethod(const std::string& functionName, lua_CFunction func)
{
	// _G[functionName] = func
	lua_pushcfunction(m_luaState, func);
	lua_setglobal(m_luaState, functionName.c_str());
}

int32_t LuaScriptInterface::internalGetPlayerInfo(lua_State* L, PlayerInfo_t info)
{
	uint32_t cid = popNumber(L);

	const Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		pushBoolean(L, false);
		return 1;
	}

	int32_t value;
	switch (info) {
		case PlayerInfoFood: {
			Condition* condition = player->getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT);
			if (condition) {
				value = condition->getTicks() / 1000;
			} else {
				value = 0;
			}
			break;
		}

		case PlayerInfoPzLock:
			pushBoolean(L, player->isPzLocked());
			return 1;

		case PlayerInfoGhostStatus:
			pushBoolean(L, player->isInGhostMode());
			return 1;

		default:
		{
			std::ostringstream ss;
			ss << "Unknown player info: " << info;
			reportErrorFunc(ss.str());
			value = 0;
			break;
		}
	}

	lua_pushnumber(L, value);
	return 1;
}
//getPlayer[Info](uid)
int32_t LuaScriptInterface::luaGetPlayerFood(lua_State* L)
{
	return internalGetPlayerInfo(L, PlayerInfoFood);
}
int32_t LuaScriptInterface::luaIsPlayerPzLocked(lua_State* L)
{
	return internalGetPlayerInfo(L, PlayerInfoPzLock);
}
int32_t LuaScriptInterface::luaIsPlayerGhost(lua_State* L)
{
	return internalGetPlayerInfo(L, PlayerInfoGhostStatus);
}

int32_t LuaScriptInterface::luaGetPlayerFlagValue(lua_State* L)
{
	//getPlayerFlagValue(cid, flag)
	uint32_t flagindex = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		if (flagindex < PlayerFlag_LastFlag) {
			pushBoolean(L, player->hasFlag((PlayerFlags)flagindex));
		} else {
			reportErrorFunc("No valid flag index.");
			pushBoolean(L, false);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerLearnInstantSpell(lua_State* L)
{
	//playerLearnInstantSpell(cid, name)
	std::string spellName = popString(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	InstantSpell* spell = g_spells->getInstantSpellByName(spellName);
	if (!spell) {
		std::string error_str = (std::string)"Spell \"" + spellName + (std::string)"\" not found";
		reportErrorFunc(error_str);
		pushBoolean(L, false);
		return 1;
	}

	player->learnInstantSpell(spell->getName());
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaCanPlayerLearnInstantSpell(lua_State* L)
{
	//canPlayerLearnInstantSpell(cid, name)
	std::string spellName = popString(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	InstantSpell* spell = g_spells->getInstantSpellByName(spellName);
	if (!spell) {
		std::string error_str = (std::string)"Spell \"" + spellName + (std::string)"\" not found";
		reportErrorFunc(error_str);
		pushBoolean(L, false);
		return 1;
	}

	if (!player->hasFlag(PlayerFlag_IgnoreSpellCheck)) {
		if (player->getLevel() < spell->getLevel()) {
			pushBoolean(L, false);
			return 1;
		}

		if (player->getMagicLevel() < spell->getMagicLevel()) {
			pushBoolean(L, false);
			return 1;
		}
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetPlayerLearnedInstantSpell(lua_State* L)
{
	//getPlayerLearnedInstantSpell(cid, name)
	std::string spellName = popString(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	InstantSpell* spell = g_spells->getInstantSpellByName(spellName);
	if (!spell) {
		std::string error_str = (std::string)"Spell \"" + spellName + (std::string)"\" not found";
		reportErrorFunc(error_str);
		pushBoolean(L, false);
		return 1;
	}

	if (!player->hasLearnedInstantSpell(spellName)) {
		pushBoolean(L, false);
		return 1;
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetPlayerInstantSpellCount(lua_State* L)
{
	//getPlayerInstantSpellCount(cid)
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		lua_pushnumber(L, g_spells->getInstantSpellCount(player));
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetPlayerInstantSpellInfo(lua_State* L)
{
	//getPlayerInstantSpellInfo(cid, index)
	uint32_t index = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	InstantSpell* spell = g_spells->getInstantSpellByIndex(player, index);
	if (!spell) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_SPELL_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	lua_newtable(L);
	setField(L, "name", spell->getName());
	setField(L, "words", spell->getWords());
	setField(L, "level", spell->getLevel());
	setField(L, "mlevel", spell->getMagicLevel());
	setField(L, "mana", spell->getManaCost(player));
	setField(L, "manapercent", spell->getManaPercent());
	return 1;
}

int32_t LuaScriptInterface::luaGetInstantSpellInfoByName(lua_State* L)
{
	//getInstantSpellInfoByName(cid, name)
	std::string spellName = popString(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (!player && cid != 0) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	InstantSpell* spell = g_spells->getInstantSpellByName(spellName);
	if (!spell) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_SPELL_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	lua_newtable(L);
	setField(L, "name", spell->getName());
	setField(L, "words", spell->getWords());
	setField(L, "level", spell->getLevel());
	setField(L, "mlevel", spell->getMagicLevel());
	setField(L, "mana", (player != nullptr ? spell->getManaCost(player) : 0));
	setField(L, "manapercent", spell->getManaPercent());
	return 1;
}

int32_t LuaScriptInterface::luaGetInstantSpellWords(lua_State* L)
{
	//getInstantSpellWords(name)
	std::string spellName = popString(L);

	InstantSpell* spell = g_spells->getInstantSpellByName(spellName);
	if (!spell) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_SPELL_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	pushString(L, spell->getWords());
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerRemoveItem(lua_State* L)
{
	//doPlayerRemoveItem(cid, itemid, count, <optional> subtype, <optional> ignoreEquipped)
	int32_t parameters = lua_gettop(L);

	bool ignoreEquipped = false;
	if (parameters > 4) {
		ignoreEquipped = popBoolean(L);
	}

	int32_t subType = -1;
	if (parameters > 3) {
		subType = popNumber(L);
	}

	uint32_t count = popNumber(L);
	uint16_t itemId = popNumber<uint16_t>(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		pushBoolean(L, player->removeItemOfType(itemId, count, subType, ignoreEquipped));
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}

	return 1;
}

int32_t LuaScriptInterface::luaDoFeedPlayer(lua_State* L)
{
	//doFeedPlayer(cid, food)
	int32_t food = popNumber<int32_t>(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->addDefaultRegeneration(food * 1000);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}

	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerSendCancel(lua_State* L)
{
	//doPlayerSendCancel(cid, text)
	std::string text = popString(L);
	uint32_t cid = popNumber(L);

	const Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->sendCancel(text);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoSendDefaultCancel(lua_State* L)
{
	//doPlayerSendDefaultCancel(cid, ReturnValue)
	ReturnValue ret = (ReturnValue)popNumber(L);
	uint32_t cid = popNumber(L);

	const Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->sendCancelMessage(ret);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoTeleportThing(lua_State* L)
{
	//doTeleportThing(cid, newpos, <optional> pushmove)
	int32_t parameters = lua_gettop(L);

	bool pushMovement = false;
	if (parameters > 2) {
		pushMovement = popBoolean(L);
	}

	PositionEx pos;
	popPosition(L, pos);
	uint32_t uid = popNumber(L);

	Thing* thing = getScriptEnv()->getThingByUID(uid);
	if (!thing) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Position oldPos = thing->getPosition();
	if (g_game.internalTeleport(thing, pos, pushMovement) != RET_NOERROR) {
		pushBoolean(L, false);
		return 1;
	}

	if (!pushMovement) {
		Creature* creature = thing->getCreature();
		if (creature) {
			if (oldPos.x == pos.x) {
				if (oldPos.y < pos.y) {
					g_game.internalCreatureTurn(creature, SOUTH);
				} else {
					g_game.internalCreatureTurn(creature, NORTH);
				}
			} else if (oldPos.x > pos.x) {
				g_game.internalCreatureTurn(creature, WEST);
			} else if (oldPos.x < pos.x) {
				g_game.internalCreatureTurn(creature, EAST);
			}
		}
	}
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoCreateNpc(lua_State* L)
{
	//doCreateNpc(name, pos)
	PositionEx pos;
	popPosition(L, pos);
	const std::string& name = popString(L);

	Npc* npc = Npc::createNpc(name);
	if (!npc) {
		pushBoolean(L, false);
		return 1;
	}

	// Place the npc
	if (g_game.placeCreature(npc, pos)) {
		npc->setMasterPos(npc->getPosition());
		pushBoolean(L, true);
	} else {
		delete npc;
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoChangeTypeItem(lua_State* L)
{
	//doChangeTypeItem(uid,new_type)
	int32_t subtype = popNumber<int32_t>(L);
	uint32_t uid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Item* item = env->getItemByUID(uid);
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Item* newItem = g_game.transformItem(item, item->getID(), subtype);

	if (item->isRemoved()) {
		env->removeItemByUID(uid);
	}

	if (newItem && newItem != item) {
		env->insertThing(uid, newItem);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerAddSkillTry(lua_State* L)
{
	//doPlayerAddSkillTry(uid,skillid,n)
	uint32_t n = popNumber(L);
	uint32_t skillid = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->addSkillAdvance((skills_t)skillid, n);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerAddMana(lua_State* L)
{
	//doPlayerAddMana(uid, mana[, animationOnLoss])
	int32_t parameters = lua_gettop(L);

	bool animationOnLoss;
	if (parameters > 2) {
		animationOnLoss = popBoolean(L);
	} else {
		animationOnLoss = false;
	}

	int32_t manaChange = popNumber<int32_t>(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		if (!animationOnLoss && manaChange < 0) {
			player->changeMana(manaChange);
		} else {
			g_game.combatChangeMana(nullptr, player, manaChange);
		}

		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerChangeName(lua_State* L)
{
	uint32_t guid = popNumber(L);
	std::string newName = popString(L);

	if (IOLoginData::getInstance()->changeName(guid, newName)) {
		if (House* house = Houses::getInstance().getHouseByPlayerId(guid)) {
			house->updateDoorDescription();
		}

		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerAddManaSpent(lua_State* L)
{
	//doPlayerAddManaSpent(cid,mana)
	uint32_t mana = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->addManaSpent(mana);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerAddItem(lua_State* L)
{
	//doPlayerAddItem(cid, itemid, <optional: default: 1> count/subtype, <optional: default: 1> canDropOnMap)
	//doPlayerAddItem(cid, itemid, <optional: default: 1> count, <optional: default: 1> canDropOnMap, <optional: default: 1>subtype)
	int32_t parameters = lua_gettop(L);

	int32_t subType = 1;
	if (parameters > 4) {
		subType = popNumber(L);
	}

	bool canDropOnMap = true;
	if (parameters > 3) {
		canDropOnMap = popBoolean(L);
	}

	uint32_t count = 1;
	if (parameters > 2) {
		count = popNumber(L);
	}

	uint32_t itemId = popNumber(L);
	uint32_t cid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();
	Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	const ItemType& it = Item::items[itemId];

	int32_t itemCount = 1;

	if (parameters > 4) {
		//subtype already supplied, count then is the amount
		itemCount = std::max<int32_t>(1, count);
	} else if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = (int32_t)std::ceil((float)count / 100);
		}

		subType = count;
	} else {
		itemCount = std::max<int32_t>(1, count);
	}

	while (itemCount > 0) {
		int32_t stackCount = subType;
		if (it.stackable && stackCount > 100) {
			stackCount = 100;
		}

		Item* newItem = Item::CreateItem(itemId, stackCount);
		if (!newItem) {
			reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}

		if (it.stackable) {
			subType -= stackCount;
		}

		ReturnValue ret = g_game.internalPlayerAddItem(player, newItem, canDropOnMap);
		if (ret != RET_NOERROR) {
			delete newItem;
			pushBoolean(L, false);
			return 1;
		}

		if (--itemCount == 0) {
			if (newItem->getParent()) {
				uint32_t uid = env->addThing(newItem);
				lua_pushnumber(L, uid);
				return 1;
			} else {
				//stackable item stacked with existing object, newItem will be released
				pushBoolean(L, false);
				return 1;
			}
		}
	}

	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaDoTileAddItemEx(lua_State* L)
{
	//doTileAddItemEx(pos, uid)
	uint32_t uid = popNumber(L);
	PositionEx pos;
	popPosition(L, pos);

	Tile* tile = g_game.getTile(pos.x, pos.y, pos.z);
	if (!tile) {
		std::ostringstream ss;
		ss << pos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	Item* item = getScriptEnv()->getItemByUID(uid);
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	if (item->getParent() != VirtualCylinder::virtualCylinder) {
		reportErrorFunc("Item already has a parent");
		pushBoolean(L, false);
		return 1;
	}

	ReturnValue ret = g_game.internalAddItem(tile, item);
	lua_pushnumber(L, ret);
	return 1;
}

int32_t LuaScriptInterface::luaDoRelocate(lua_State* L)
{
	//doRelocate(pos, posTo)
	//Moves all moveable objects from pos to posTo

	PositionEx toPos;
	popPosition(L, toPos);

	PositionEx fromPos;
	popPosition(L, fromPos);

	Tile* fromTile = g_game.getTile(fromPos.x, fromPos.y, fromPos.z);
	if (!fromTile) {
		std::ostringstream ss;
		ss << fromPos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	Tile* toTile = g_game.getTile(toPos.x, toPos.y, toPos.z);
	if (!toTile) {
		std::ostringstream ss;
		ss << toPos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	if (fromTile != toTile) {
		int32_t thingCount = fromTile->getThingCount();
		for (int32_t i = thingCount - 1; i >= 0; --i) {
			Thing* thing = fromTile->__getThing(i);

			if (thing) {
				if (Item* item = thing->getItem()) {
					const ItemType& it = Item::items[item->getID()];
					if (!it.isGroundTile() && !it.alwaysOnTop && !it.isMagicField() && !it.isDoor()) {
						g_game.internalTeleport(item, toPos, false, FLAG_IGNORENOTMOVEABLE);
					}
				} else if (Creature* creature = thing->getCreature()) {
					g_game.internalTeleport(creature, toPos);
				}
			}
		}
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerSendTextMessage(lua_State* L)
{
	//doPlayerSendTextMessage(cid, MessageClasses, message[, position, value, color])
	int parameters = lua_gettop(L);
	PositionEx position;
	uint32_t value = 0;
	TextColor_t color = TEXTCOLOR_NONE;

	if (parameters > 5) {
		color = (TextColor_t)popNumber(L);
		value = popNumber(L);
		popPosition(L, position);
	}

	std::string text = popString(L);
	uint32_t messageClass = popNumber(L);
	uint32_t cid = popNumber(L);

	const Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	if (parameters > 5) {
		player->sendTextMessage((MessageClasses)messageClass, text, &position, value, color);
	} else {
		player->sendTextMessage((MessageClasses)messageClass, text);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoSendAnimatedText(lua_State* L)
{
	//doSendAnimatedText(pos, text, color)
	popNumber(L);
	popString(L);
	PositionEx pos;
	popPosition(L, pos);

	reportErrorFunc("Deprecated function.");

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoSetCreatureDropLoot(lua_State* L)
{
	//doSetCreatureDropLoot(cid, doDrop)
	bool doDrop = popBoolean(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		creature->setDropLoot(doDrop);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoSendTutorial(lua_State* L)
{
	//doSendTutorial(cid, tutorialid)
	uint32_t tutorial = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	player->sendTutorial(tutorial);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoAddMark(lua_State* L)
{
	//doAddMapMark(cid, pos, type, <optional> description)
	int32_t parameters = lua_gettop(L);
	std::string description;
	Position pos;
	uint32_t stackpos;

	if (parameters > 3) {
		description = popString(L);
	}

	uint32_t type = popNumber(L);
	popPosition(L, pos, stackpos);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	player->sendAddMarker(pos, type, description);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetItemRWInfo(lua_State* L)
{
	//getItemRWInfo(uid)
	uint32_t uid = popNumber(L);

	const Item* item = getScriptEnv()->getItemByUID(uid);
	if (item) {
		uint32_t rwflags = 0;

		if (item->isReadable()) {
			rwflags |= 1;
		}

		if (item->canWriteText()) {
			rwflags |= 2;
		}

		lua_pushnumber(L, rwflags);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoDecayItem(lua_State* L)
{
	//doDecayItem(uid)
	//Note: to stop decay set decayTo = 0 in items.otb
	uint32_t uid = popNumber(L);

	Item* item = getScriptEnv()->getItemByUID(uid);
	if (!item) {
		pushBoolean(L, false);
		return 1;
	}

	g_game.startDecay(item);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetThingfromPos(lua_State* L)
{
	//Consider using getTileItemById/getTileItemByType/getTileThingByPos/getTopCreature instead.

	//getThingfromPos(pos)
	//Note:
	//	stackpos = 255. Get the top thing(item moveable or creature)
	//	stackpos = 254. Get MagicFieldtItem
	//	stackpos = 253. Get the top creature (moveable creature)

	PositionEx pos;
	popPosition(L, pos);

	Tile* tile = g_game.getMap()->getTile(pos);
	if (!tile) {
		pushThing(L, nullptr, 0);
		return 1;
	}

	Thing* thing;
	if (pos.stackpos == 255) {
		thing = tile->getTopCreature();
		if (!thing) {
			Item* item = tile->getTopDownItem();
			if (item && !item->isNotMoveable()) {
				thing = item;
			}
		}
	} else if (pos.stackpos == 254) {
		thing = tile->getFieldItem();
	} else if (pos.stackpos == 253) {
		thing = tile->getTopCreature();
	} else {
		thing = tile->__getThing(pos.stackpos);
	}

	if (!thing) {
		pushThing(L, nullptr, 0);
		return 1;
	}

	pushThing(L, thing, getScriptEnv()->addThing(thing));
	return 1;
}

int32_t LuaScriptInterface::luaDoCreateItem(lua_State* L)
{
	//doCreateItem(itemid, <optional> type/count, pos)
	//Returns uid of the created item, only works on tiles.
	uint32_t parameters = lua_gettop(L);

	PositionEx pos;
	popPosition(L, pos);

	uint32_t count = 1;
	if (parameters > 2) {
		count = popNumber(L);
	}

	uint32_t itemId = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Tile* tile = g_game.getTile(pos.x, pos.y, pos.z);
	if (!tile) {
		std::ostringstream ss;
		ss << pos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	int32_t itemCount = 1;
	int32_t subType = 1;

	const ItemType& it = Item::items[itemId];
	if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = (int32_t)std::ceil((float)count / 100);
		}

		subType = count;
	} else {
		itemCount = std::max<int32_t>(1, count);
	}

	while (itemCount > 0) {
		int32_t stackCount = std::min<int32_t>(100, subType);
		Item* newItem = Item::CreateItem(itemId, stackCount);
		if (!newItem) {
			reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}

		if (it.stackable) {
			subType -= stackCount;
		}

		ReturnValue ret = g_game.internalAddItem(tile, newItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
		if (ret != RET_NOERROR) {
			delete newItem;
			pushBoolean(L, false);
			return 1;
		}

		if (--itemCount == 0) {
			if (newItem->getParent()) {
				uint32_t uid = env->addThing(newItem);
				lua_pushnumber(L, uid);
				return 1;
			} else {
				//stackable item stacked with existing object, newItem will be released
				pushBoolean(L, false);
				return 1;
			}
		}
	}

	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaDoCreateItemEx(lua_State* L)
{
	//doCreateItemEx(itemid, <optional> count/subtype)
	//Returns uid of the created item

	int32_t parameters = lua_gettop(L);

	uint32_t count = 1;
	if (parameters > 1) {
		count = popNumber(L);
	}

	uint32_t itemId = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	const ItemType& it = Item::items[itemId];
	if (it.stackable && count > 100) {
		reportErrorFunc("Stack count cannot be higher than 100.");
		count = 100;
	}

	Item* newItem = Item::CreateItem(itemId, count);
	if (!newItem) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	newItem->setParent(VirtualCylinder::virtualCylinder);
	env->addTempItem(env, newItem);

	uint32_t uid = env->addThing(newItem);
	lua_pushnumber(L, uid);
	return 1;
}

int32_t LuaScriptInterface::luaDoCreateTeleport(lua_State* L)
{
	//doCreateTeleport(itemid, topos, createpos)
	PositionEx createPos;
	popPosition(L, createPos);
	PositionEx toPos;
	popPosition(L, toPos);
	uint32_t itemId = popNumber(L);

	Tile* tile = g_game.getMap()->getTile(createPos);
	if (!tile) {
		std::ostringstream ss;
		ss << createPos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	Item* newItem = Item::CreateItem(itemId);
	if (!newItem) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Teleport* newTeleport = newItem->getTeleport();
	if (!newTeleport) {
		delete newItem;
		reportErrorFunc("Invalid teleport ItemID.");
		pushBoolean(L, false);
		return 1;
	}

	newTeleport->setDestPos(toPos);

	ReturnValue ret = g_game.internalAddItem(tile, newTeleport, INDEX_WHEREEVER, FLAG_NOLIMIT);
	if (ret != RET_NOERROR) {
		delete newItem;
		reportErrorFunc("Can not add Item");
		pushBoolean(L, false);
		return 1;
	}

	if (newItem->getParent()) {
		uint32_t uid = getScriptEnv()->addThing(newItem);
		lua_pushnumber(L, uid);
	} else {
		pushBoolean(L, false);    //stackable item stacked with existing object, newItem will be released
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoSetItemText(lua_State* L)
{
	//doSetItemText(uid, text)
	std::string text = popString(L);
	uint32_t uid = popNumber(L);

	Item* item = getScriptEnv()->getItemByUID(uid);
	if (item) {
		std::string str(text);
		item->setText(str);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoSetItemSpecialDescription(lua_State* L)
{
	//doSetItemSpecialDescription(uid, desc)
	std::string desc = popString(L);
	uint32_t uid = popNumber(L);

	Item* item = getScriptEnv()->getItemByUID(uid);
	if (item) {
		if (desc.empty()) {
			item->resetSpecialDescription();
		} else {
			item->setSpecialDescription(desc);
		}
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetTileHouseInfo(lua_State* L)
{
	//getTileHouseInfo(pos)
	PositionEx pos;
	popPosition(L, pos);

	Tile* tile = g_game.getMap()->getTile(pos);
	if (tile) {
		if (HouseTile* houseTile = dynamic_cast<HouseTile*>(tile)) {
			House* house = houseTile->getHouse();
			if (house) {
				lua_pushnumber(L, house->getHouseId());
			} else {
				pushBoolean(L, false);
			}
		} else {
			pushBoolean(L, false);
		}
	} else {
		std::ostringstream ss;
		ss << pos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoSummonCreature(lua_State* L)
{
	//doSummonCreature(name, pos)
	PositionEx pos;
	popPosition(L, pos);
	std::string name = popString(L);

	Monster* monster = Monster::createMonster(name);
	if (!monster) {
		std::string error_str = (std::string)"Monster name(" + name + (std::string)") not found";
		reportErrorFunc(error_str);
		pushBoolean(L, false);
		return 1;
	}

	if (!g_game.placeCreature(monster, pos)) {
		delete monster;
		pushBoolean(L, false);
		return 1;
	}

	lua_pushnumber(L, monster->getID());
	return 1;
}

int32_t LuaScriptInterface::luaDebugPrint(lua_State* L)
{
	//debugPrint(text)
	std::string text = popString(L);
	reportErrorFunc(text);
	return 0;
}

int32_t LuaScriptInterface::luaGetHouseRent(lua_State* L)
{
	//getHouseRent(houseid)
	uint32_t houseid = popNumber(L);

	House* house = Houses::getInstance().getHouse(houseid);
	if (house) {
		lua_pushnumber(L, house->getRent());
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_HOUSE_NOT_FOUND));
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetHouseAccessList(lua_State* L)
{
	//getHouseAccessList(houseid, listid)
	uint32_t listid = popNumber(L);
	uint32_t houseid = popNumber(L);

	House* house = Houses::getInstance().getHouse(houseid);
	if (house) {
		std::string list;
		if (house->getAccessList(listid, list)) {
			pushString(L, list);
		} else {
			reportErrorFunc("No valid listid.");
			lua_pushnil(L);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_HOUSE_NOT_FOUND));
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetHouseByPlayerGUID(lua_State* L)
{
	//getHouseByPlayerGUID(playerGUID)
	uint32_t guid = popNumber(L);

	House* house = Houses::getInstance().getHouseByPlayerId(guid);
	if (house) {
		lua_pushnumber(L, house->getHouseId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetHouseAccessList(lua_State* L)
{
	//setHouseAccessList(houseid, listid, listtext)
	std::string list = popString(L);
	uint32_t listid = popNumber(L);
	uint32_t houseid = popNumber(L);

	House* house = Houses::getInstance().getHouse(houseid);
	if (house) {
		house->setAccessList(listid, list);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_HOUSE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetHouseOwner(lua_State* L)
{
	//setHouseOwner(houseid, owner)
	uint32_t owner = popNumber(L);
	uint32_t houseid = popNumber(L);

	House* house = Houses::getInstance().getHouse(houseid);

	if (house) {
		house->setHouseOwner(owner);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_HOUSE_NOT_FOUND));
		pushBoolean(L, false);
	}

	return 1;
}

int32_t LuaScriptInterface::luaGetWorldType(lua_State* L)
{
	//getWorldType()
	switch (g_game.getWorldType()) {
		case WORLD_TYPE_NO_PVP:
			lua_pushnumber(L, 1);
			break;
		case WORLD_TYPE_PVP:
			lua_pushnumber(L, 2);
			break;
		case WORLD_TYPE_PVP_ENFORCED:
			lua_pushnumber(L, 3);
			break;
		default:
			pushBoolean(L, false);
			break;
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetWorldTime(lua_State* L)
{
	//getWorldTime()
	uint32_t time = g_game.getLightHour();
	lua_pushnumber(L, time);
	return 1;
}

int32_t LuaScriptInterface::luaGetWorldLight(lua_State* L)
{
	//getWorldLight()
	LightInfo lightInfo;
	g_game.getWorldLightInfo(lightInfo);
	lua_pushnumber(L, lightInfo.level);
	lua_pushnumber(L, lightInfo.color);
	return 2;
}

int32_t LuaScriptInterface::luaGetWorldCreatures(lua_State* L)
{
	//getWorldCreatures(type)
	//0 players, 1 monsters, 2 npcs, 3 all
	uint32_t type = popNumber(L);
	uint32_t value;

	switch (type) {
		case 0:
			value = g_game.getPlayersOnline();
			break;

		case 1:
			value = g_game.getMonstersOnline();
			break;

		case 2:
			value = g_game.getNpcsOnline();
			break;

		case 3:
			value = g_game.getCreaturesOnline();
			break;

		default:
			reportErrorFunc("Wrong creature type.");
			pushBoolean(L, false);
			return 1;
	}

	lua_pushnumber(L, value);
	return 1;
}

int32_t LuaScriptInterface::luaGetWorldUpTime(lua_State* L)
{
	//getWorldUpTime()
	uint32_t uptime = 0;
	Status* status = Status::getInstance();
	if (status) {
		uptime = status->getUptime();
	}

	lua_pushnumber(L, uptime);
	return 1;
}

int32_t LuaScriptInterface::luaBroadcastMessage(lua_State* L)
{
	//broadcastMessage(message, type)
	uint32_t type = MSG_STATUS_WARNING;
	int32_t parameters = lua_gettop(L);
	if (parameters >= 2) {
		type = popNumber(L);
	}

	std::string message = popString(L);
	if (g_game.broadcastMessage(message, (MessageClasses)type)) {
		pushBoolean(L, true);
	} else {
		reportErrorFunc("Bad messageClass type.");
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerAddExp(lua_State* L)
{
	//doPlayerAddExp(cid,exp,usemultiplier,sendtext)
	int32_t parameters = lua_gettop(L);

	bool sendText = false;
	if (parameters > 3) {
		sendText = popBoolean(L);
	}

	bool useMult = false;
	if (parameters > 2) {
		useMult = popBoolean(L);
	}

	uint64_t exp = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		if (exp > 0) {
			player->addExperience(exp, useMult, sendText);
			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetThing(lua_State* L)
{
	//getThing(uid)
	uint32_t uid = popNumber(L);

	Thing* thing = getScriptEnv()->getThingByUID(uid);
	if (thing) {
		pushThing(L, thing, uid);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		pushThing(L, nullptr, 0);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetThingPos(lua_State* L)
{
	//getThingPos(uid)
	uint32_t uid = popNumber(L);

	Position pos;
	uint32_t stackpos = 0;

	Thing* thing = getScriptEnv()->getThingByUID(uid);
	if (thing) {
		pos = thing->getPosition();
		if (Tile* tile = thing->getTile()) {
			stackpos = tile->__getIndexOfThing(thing);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
	}

	pushPosition(L, pos, stackpos);
	return 1;
}

int32_t LuaScriptInterface::luaCreateCombatObject(lua_State* L)
{
	//createCombatObject()
	ScriptEnvironment* env = getScriptEnv();
	if (env->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	uint32_t newCombatId = env->addCombatObject(new Combat);
	lua_pushnumber(L, newCombatId);
	return 1;
}

bool LuaScriptInterface::getArea(lua_State* L, std::list<uint32_t>& list, uint32_t& rows)
{
	pushNil(L);
	for (rows = 0; lua_next(L, -2) != 0; ++rows) {
		if (!isTable(L, -1)) {
			return false;
		}

		pushNil(L);
		while (lua_next(L, -2) != 0) {
			if (!isNumber(L, -1)) {
				return false;
			}
			list.push_back(popNumber(L));
		}

		lua_pop(L, 1);
	}

	lua_pop(L, 1);
	return (rows != 0);
}

int32_t LuaScriptInterface::luaCreateCombatArea(lua_State* L)
{
	//createCombatArea( {area}, <optional> {extArea} )
	ScriptEnvironment* env = getScriptEnv();
	if (env->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	AreaCombat* area = new AreaCombat;

	int32_t parameters = lua_gettop(L);
	if (parameters > 1) {
		//has extra parameter with diagonal area information

		uint32_t rowsExtArea;
		std::list<uint32_t> listExtArea;

		if (!getArea(L, listExtArea, rowsExtArea)) {
			reportErrorFunc("Invalid extended area table.");
			pushBoolean(L, false);
			return 1;
		}

		/*setup all possible rotations*/
		area->setupExtArea(listExtArea, rowsExtArea);
	}

	uint32_t rowsArea = 0;
	std::list<uint32_t> listArea;

	if (!getArea(L, listArea, rowsArea)) {
		reportErrorFunc("Invalid area table.");
		pushBoolean(L, false);
		return 1;
	}

	/*setup all possible rotations*/
	area->setupArea(listArea, rowsArea);
	uint32_t newAreaId = env->addCombatArea(area);

	lua_pushnumber(L, newAreaId);
	return 1;
}

int32_t LuaScriptInterface::luaCreateConditionObject(lua_State* L)
{
	//createConditionObject(type)
	ScriptEnvironment* env = getScriptEnv();
	if (env->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	ConditionType_t type = (ConditionType_t)popNumber(L);

	Condition* condition = Condition::createCondition(CONDITIONID_COMBAT, type, 0, 0);
	if (condition) {
		uint32_t newConditionId = env->addConditionObject(condition);
		lua_pushnumber(L, newConditionId);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetCombatArea(lua_State* L)
{
	//setCombatArea(combat, area)
	uint32_t areaId = popNumber(L);
	uint32_t combatId = popNumber(L);

	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	Combat* combat = ScriptEnvironment::getCombatObject(combatId);
	if (!combat) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	const AreaCombat* area = ScriptEnvironment::getCombatArea(areaId);
	if (!area) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_AREA_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	combat->setArea(new AreaCombat(*area));

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaSetCombatCondition(lua_State* L)
{
	//setCombatCondition(combat, condition)
	uint32_t conditionId = popNumber(L);
	uint32_t combatId = popNumber(L);

	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	Combat* combat = ScriptEnvironment::getCombatObject(combatId);
	if (!combat) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	const Condition* condition = ScriptEnvironment::getConditionObject(conditionId);
	if (!condition) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	combat->setCondition(condition->clone());

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaSetCombatParam(lua_State* L)
{
	//setCombatParam(combat, key, value)
	uint32_t value = popNumber(L);
	CombatParam_t key = (CombatParam_t)popNumber(L);
	uint32_t combatId = popNumber(L);

	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	Combat* combat = ScriptEnvironment::getCombatObject(combatId);
	if (combat) {
		combat->setParam(key, value);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetConditionParam(lua_State* L)
{
	//setConditionParam(condition, key, value)
	int32_t value;
	if (isBoolean(L, -1)) {
		value = popBoolean(L) ? 1 : 0;
	} else {
		value = popNumber<int32_t>(L);
	}
	ConditionParam_t key = (ConditionParam_t)popNumber(L);
	uint32_t conditionId = popNumber(L);

	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	Condition* condition = ScriptEnvironment::getConditionObject(conditionId);
	if (condition) {
		condition->setParam(key, value);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaAddDamageCondition(lua_State* L)
{
	//addDamageCondition(condition, rounds, time, value)
	int32_t value = popNumber<int32_t>(L);
	int32_t time = popNumber<int32_t>(L);
	int32_t rounds = popNumber<int32_t>(L);
	uint32_t conditionId = popNumber(L);

	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	ConditionDamage* condition = dynamic_cast<ConditionDamage*>(ScriptEnvironment::getConditionObject(conditionId));
	if (condition) {
		condition->addDamage(rounds, time, value);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaAddOutfitCondition(lua_State* L)
{
	//addOutfitCondition(condition, lookTypeEx, lookType, lookHead, lookBody, lookLegs, lookFeet[, lookAddons, lookMount])
	Outfit_t outfit;

	int32_t parameters = lua_gettop(L);
	if (parameters > 8) {
		outfit.lookMount = popNumber(L);
	}

	if (parameters > 7) {
		outfit.lookAddons = popNumber(L);
	}

	outfit.lookFeet = popNumber(L);
	outfit.lookLegs = popNumber(L);
	outfit.lookBody = popNumber(L);
	outfit.lookHead = popNumber(L);
	outfit.lookType = popNumber(L);
	outfit.lookTypeEx = popNumber(L);

	uint32_t conditionId = popNumber(L);

	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	ConditionOutfit* condition = dynamic_cast<ConditionOutfit*>(ScriptEnvironment::getConditionObject(conditionId));
	if (condition) {
		condition->addOutfit(outfit);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetCombatCallBack(lua_State* L)
{
	//setCombatCallBack(combat, key, function_name)
	std::string function = popString(L);
	CallBackParam_t key = (CallBackParam_t)popNumber(L);
	uint32_t combatId = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();
	if (env->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	Combat* combat = ScriptEnvironment::getCombatObject(combatId);
	if (!combat) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	LuaScriptInterface* scriptInterface = env->getScriptInterface();

	combat->setCallback(key);

	CallBack* callback = combat->getCallback(key);
	if (!callback) {
		std::ostringstream ss;
		ss << key << " is not a valid callback key.";
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	if (!callback->loadCallBack(scriptInterface, function)) {
		reportErrorFunc("Can not load callback");
		pushBoolean(L, false);
		return 1;
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaSetCombatFormula(lua_State* L)
{
	//setCombatFormula(combat, type, mina, minb, maxa, maxb)
	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	float maxb = (float)popFloatNumber(L);
	float maxa = (float)popFloatNumber(L);
	float minb = (float)popFloatNumber(L);
	float mina = (float)popFloatNumber(L);

	formulaType_t type = (formulaType_t)popNumber(L);
	uint32_t combatId = popNumber(L);

	Combat* combat = ScriptEnvironment::getCombatObject(combatId);
	if (combat) {
		combat->setPlayerCombatValues(type, mina, minb, maxa, maxb);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetConditionFormula(lua_State* L)
{
	//setConditionFormula(condition, mina, minb, maxa, maxb)
	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	double maxb = popFloatNumber(L);
	double maxa = popFloatNumber(L);
	double minb = popFloatNumber(L);
	double mina = popFloatNumber(L);

	uint32_t conditionId = popNumber(L);

	ConditionSpeed* condition = dynamic_cast<ConditionSpeed*>(ScriptEnvironment::getConditionObject(conditionId));
	if (condition) {
		condition->setFormulaVars(mina, minb, maxa, maxb);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoCombat(lua_State* L)
{
	//doCombat(cid, combat, param)
	LuaVariant var = popVariant(L);
	uint32_t combatId = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (cid != 0 && !creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	const Combat* combat = ScriptEnvironment::getCombatObject(combatId);
	if (!combat) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	if (var.type == VARIANT_NONE) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_VARIANT_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	switch (var.type) {
		case VARIANT_NUMBER: {
			Creature* target = g_game.getCreatureByID(var.number);
			if (!target) {
				pushBoolean(L, false);
				return 1;
			}

			if (combat->hasArea()) {
				combat->doCombat(creature, target->getPosition());
				//std::cout << "Combat->hasArea()" << std::endl;
			} else {
				combat->doCombat(creature, target);
			}

			break;
		}

		case VARIANT_POSITION: {
			combat->doCombat(creature, var.pos);
			break;
		}

		case VARIANT_TARGETPOSITION: {
			if (combat->hasArea()) {
				combat->doCombat(creature, var.pos);
			} else {
				combat->postCombatEffects(creature, var.pos);
				g_game.addMagicEffect(var.pos, NM_ME_POFF);
			}
			break;
		}

		case VARIANT_STRING: {
			Player* target = g_game.getPlayerByName(var.text);
			if (!target) {
				pushBoolean(L, false);
				return 1;
			}

			combat->doCombat(creature, target);
			break;
		}

		default: {
			reportErrorFunc(getErrorDesc(LUA_ERROR_VARIANT_UNKNOWN));
			pushBoolean(L, false);
			return 1;
		}
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoAreaCombatHealth(lua_State* L)
{
	//doAreaCombatHealth(cid, type, pos, area, min, max, effect)
	uint8_t effect = popNumber<uint8_t>(L);
	int32_t maxChange = popNumber<int32_t>(L);
	int32_t minChange = popNumber<int32_t>(L);
	uint32_t areaId = popNumber(L);

	PositionEx pos;
	popPosition(L, pos);

	CombatType_t combatType = (CombatType_t)popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = nullptr;
	if (cid != 0) {
		creature = g_game.getCreatureByID(cid);
		if (!creature) {
			reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}
	}

	const AreaCombat* area = getScriptEnv()->getCombatArea(areaId);
	if (area || areaId == 0) {
		CombatParams params;
		params.combatType = combatType;
		params.impactEffect = effect;
		Combat::doCombatHealth(creature, pos, area, minChange, maxChange, params);

		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_AREA_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoTargetCombatHealth(lua_State* L)
{
	//doTargetCombatHealth(cid, target, type, min, max, effect)
	uint8_t effect = popNumber<uint8_t>(L);
	int32_t maxChange = popNumber<int32_t>(L);
	int32_t minChange = popNumber<int32_t>(L);
	CombatType_t combatType = (CombatType_t)popNumber(L);
	uint32_t targetCid = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = nullptr;
	if (cid != 0) {
		creature = g_game.getCreatureByID(cid);
		if (!creature) {
			reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}
	}

	Creature* target = g_game.getCreatureByID(targetCid);
	if (target) {
		CombatParams params;
		params.combatType = combatType;
		params.impactEffect = effect;
		Combat::doCombatHealth(creature, target, minChange, maxChange, params);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoAreaCombatMana(lua_State* L)
{
	//doAreaCombatMana(cid, pos, area, min, max, effect)
	uint8_t effect = popNumber<uint8_t>(L);
	int32_t maxChange = popNumber<int32_t>(L);
	int32_t minChange = popNumber<int32_t>(L);
	uint32_t areaId = popNumber(L);

	PositionEx pos;
	popPosition(L, pos);

	uint32_t cid = popNumber(L);

	Creature* creature = nullptr;
	if (cid != 0) {
		creature = g_game.getCreatureByID(cid);
		if (!creature) {
			reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}
	}

	const AreaCombat* area = getScriptEnv()->getCombatArea(areaId);
	if (area || areaId == 0) {
		CombatParams params;
		params.impactEffect = effect;
		Combat::doCombatMana(creature, pos, area, minChange, maxChange, params);

		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_AREA_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoTargetCombatMana(lua_State* L)
{
	//doTargetCombatMana(cid, target, min, max, effect)
	uint8_t effect = popNumber<uint8_t>(L);
	int32_t maxChange = popNumber<int32_t>(L);
	int32_t minChange = popNumber<int32_t>(L);
	uint32_t targetCid = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (cid != 0 && !creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Creature* target = g_game.getCreatureByID(targetCid);
	if (target) {
		CombatParams params;
		params.impactEffect = effect;
		Combat::doCombatMana(creature, target, minChange, maxChange, params);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoAreaCombatCondition(lua_State* L)
{
	//doAreaCombatCondition(cid, pos, area, condition, effect)
	uint8_t effect = popNumber<uint8_t>(L);
	uint32_t conditionId = popNumber(L);
	uint32_t areaId = popNumber(L);
	PositionEx pos;
	popPosition(L, pos);
	uint32_t cid = popNumber(L);

	Creature* creature = nullptr;
	if (cid != 0) {
		creature = g_game.getCreatureByID(cid);
		if (!creature) {
			reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}
	}

	const Condition* condition = ScriptEnvironment::getConditionObject(conditionId);
	if (condition) {
		const AreaCombat* area = ScriptEnvironment::getCombatArea(areaId);
		if (area || areaId == 0) {
			CombatParams params;
			params.impactEffect = effect;
			params.conditionList.push_back(condition);
			Combat::doCombatCondition(creature, pos, area, params);

			pushBoolean(L, true);
		} else {
			reportErrorFunc(getErrorDesc(LUA_ERROR_AREA_NOT_FOUND));
			pushBoolean(L, false);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoTargetCombatCondition(lua_State* L)
{
	//doTargetCombatCondition(cid, target, condition, effect)
	uint8_t effect = popNumber<uint8_t>(L);
	uint32_t conditionId = popNumber(L);
	uint32_t targetCid = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = nullptr;
	if (cid != 0) {
		creature = g_game.getCreatureByID(cid);
		if (!creature) {
			reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}
	}

	Creature* target = g_game.getCreatureByID(targetCid);
	if (target) {
		const Condition* condition = ScriptEnvironment::getConditionObject(conditionId);
		if (condition) {
			CombatParams params;
			params.impactEffect = effect;
			params.conditionList.push_back(condition);
			Combat::doCombatCondition(creature, target, params);
			pushBoolean(L, true);
		} else {
			reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
			pushBoolean(L, false);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoAreaCombatDispel(lua_State* L)
{
	//doAreaCombatDispel(cid, pos, area, type, effect)
	uint8_t effect = popNumber<uint8_t>(L);
	ConditionType_t dispelType = (ConditionType_t)popNumber(L);
	uint32_t areaId = popNumber(L);
	PositionEx pos;
	popPosition(L, pos);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (cid != 0 && !creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	const AreaCombat* area = getScriptEnv()->getCombatArea(areaId);
	if (area || areaId == 0) {
		CombatParams params;
		params.impactEffect = effect;
		params.dispelType = dispelType;
		Combat::doCombatDispel(creature, pos, area, params);

		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_AREA_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoTargetCombatDispel(lua_State* L)
{
	//doTargetCombatDispel(cid, target, type, effect)
	uint8_t effect = popNumber<uint8_t>(L);
	ConditionType_t dispelType = (ConditionType_t)popNumber(L);
	uint32_t targetCid = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = nullptr;
	if (cid != 0) {
		creature = g_game.getCreatureByID(cid);
		if (!creature) {
			reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}
	}

	Creature* target = g_game.getCreatureByID(targetCid);
	if (target) {
		CombatParams params;
		params.impactEffect = effect;
		params.dispelType = dispelType;
		Combat::doCombatDispel(creature, target, params);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoChallengeCreature(lua_State* L)
{
	//doChallengeCreature(cid, target)
	uint32_t targetCid = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Creature* target = g_game.getCreatureByID(targetCid);
	if (!target) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	target->challengeCreature(creature);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoConvinceCreature(lua_State* L)
{
	//doConvinceCreature(cid, target)
	uint32_t targetCid = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Creature* target = g_game.getCreatureByID(targetCid);
	if (!target) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	target->convinceCreature(creature);
	g_game.updateCreatureType(target);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetMonsterTargetList(lua_State* L)
{
	//getMonsterTargetList(cid)
	uint32_t cid = popNumber(L);

	Monster* monster = g_game.getMonsterByID(cid);
	if (!monster) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	lua_newtable(L);
	uint32_t index = 0;
	const CreatureList& targetList = monster->getTargetList();
	for (Creature* creature : targetList) {
		if (monster->isTarget(creature)) {
			lua_pushnumber(L, ++index);
			lua_pushnumber(L, creature->getID());
			lua_settable(L, -3);
		}
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetMonsterFriendList(lua_State* L)
{
	//getMonsterFriendList(cid)
	uint32_t cid = popNumber(L);

	Monster* monster = g_game.getMonsterByID(cid);
	if (!monster) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	lua_newtable(L);
	uint32_t index = 0;
	const CreatureHashSet& friendList = monster->getFriendList();
	for (Creature* friendCreature : friendList) {
		if (!friendCreature->isRemoved() && friendCreature->getPosition().z == monster->getPosition().z) {
			lua_pushnumber(L, ++index);
			lua_pushnumber(L, friendCreature->getID());
			lua_settable(L, -3);
		}
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoSetMonsterTarget(lua_State* L)
{
	//doSetMonsterTarget(cid, target)
	uint32_t targetCid = popNumber(L);
	uint32_t cid = popNumber(L);

	Monster* monster = g_game.getMonsterByID(cid);
	if (!monster) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Creature* target = g_game.getCreatureByID(targetCid);
	if (!target) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	if (!monster->isSummon()) {
		monster->selectTarget(target);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoMonsterChangeTarget(lua_State* L)
{
	//doMonsterChangeTarget(cid)
	uint32_t cid = popNumber(L);

	Monster* monster = g_game.getMonsterByID(cid);
	if (!monster) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	if (!monster->isSummon()) {
		monster->searchTarget(TARGETSEARCH_RANDOM);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoAddCondition(lua_State* L)
{
	//doAddCondition(cid, condition)
	uint32_t conditionId = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Condition* condition = ScriptEnvironment::getConditionObject(conditionId);
	if (!condition) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	creature->addCondition(condition->clone());
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoRemoveCondition(lua_State* L)
{
	//doRemoveCondition(cid, type[, subId])
	uint32_t subId;
	if (lua_gettop(L) > 2) {
		subId = popNumber(L);
	} else {
		subId = 0;
	}

	ConditionType_t conditionType = (ConditionType_t)popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Condition* condition = creature->getCondition(conditionType, CONDITIONID_COMBAT, subId);
	if (!condition) {
		condition = creature->getCondition(conditionType, CONDITIONID_DEFAULT, subId);
	}

	if (condition) {
		creature->removeCondition(condition);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaNumberToVariant(lua_State* L)
{
	//numberToVariant(number)
	LuaVariant var;
	var.type = VARIANT_NUMBER;
	var.number = popNumber(L);

	LuaScriptInterface::pushVariant(L, var);
	return 1;
}

int32_t LuaScriptInterface::luaStringToVariant(lua_State* L)
{
	//stringToVariant(string)
	LuaVariant var;
	var.type = VARIANT_STRING;
	var.text = popString(L);

	LuaScriptInterface::pushVariant(L, var);
	return 1;
}

int32_t LuaScriptInterface::luaPositionToVariant(lua_State* L)
{
	//positionToVariant(pos)
	LuaVariant var;
	var.type = VARIANT_POSITION;
	popPosition(L, var.pos);

	LuaScriptInterface::pushVariant(L, var);
	return 1;
}

int32_t LuaScriptInterface::luaTargetPositionToVariant(lua_State* L)
{
	//targetPositionToVariant(pos)
	LuaVariant var;
	var.type = VARIANT_TARGETPOSITION;
	popPosition(L, var.pos);

	LuaScriptInterface::pushVariant(L, var);
	return 1;
}

int32_t LuaScriptInterface::luaVariantToNumber(lua_State* L)
{
	//variantToNumber(var)
	LuaVariant var = popVariant(L);

	uint32_t number = 0;

	if (var.type == VARIANT_NUMBER) {
		number = var.number;
	}

	lua_pushnumber(L, number);
	return 1;
}

int32_t LuaScriptInterface::luaVariantToString(lua_State* L)
{
	//variantToString(var)
	LuaVariant var = popVariant(L);
	if (var.type == VARIANT_STRING) {
		pushString(L, var.text);
	} else {
		pushString(L, "");
	}
	return 1;
}

int32_t LuaScriptInterface::luaVariantToPosition(lua_State* L)
{
	//luaVariantToPosition(var)
	LuaVariant var = popVariant(L);

	PositionEx pos(0, 0, 0, 0);
	if (var.type == VARIANT_POSITION || var.type == VARIANT_TARGETPOSITION) {
		pos = var.pos;
	}

	pushPosition(L, pos, pos.stackpos);
	return 1;
}

int32_t LuaScriptInterface::luaDoChangeSpeed(lua_State* L)
{
	//doChangeSpeed(cid, delta)
	int32_t delta = popNumber<int32_t>(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		g_game.changeSpeed(creature, delta);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}

	return 1;
}

int32_t LuaScriptInterface::luaSetCreatureOutfit(lua_State* L)
{
	//doSetCreatureOutfit(cid, outfit, time)
	int32_t time = popNumber<int32_t>(L);
	Outfit_t outfit = popOutfit(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		ReturnValue ret = Spell::CreateIllusion(creature, outfit, time);
		if (ret == RET_NOERROR) {
			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetCreatureOutfit(lua_State* L)
{
	//getCreatureOutfit(cid)
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		const Outfit_t outfit = creature->getCurrentOutfit();

		lua_newtable(L);
		setField(L, "lookType", outfit.lookType);
		setField(L, "lookHead", outfit.lookHead);
		setField(L, "lookBody", outfit.lookBody);
		setField(L, "lookLegs", outfit.lookLegs);
		setField(L, "lookFeet", outfit.lookFeet);
		setField(L, "lookAddons", outfit.lookAddons);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetMonsterOutfit(lua_State* L)
{
	//doSetMonsterOutfit(cid, name, time)
	int32_t time = popNumber<int32_t>(L);
	std::string name = popString(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		ReturnValue ret = Spell::CreateIllusion(creature, name, time);
		if (ret == RET_NOERROR) {
			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetItemOutfit(lua_State* L)
{
	//doSetItemOutfit(cid, item, time)
	int32_t time = popNumber<int32_t>(L);
	uint32_t item = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		ReturnValue ret = Spell::CreateIllusion(creature, item, time);
		if (ret == RET_NOERROR) {
			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}

	return 1;
}

int32_t LuaScriptInterface::luaGetGlobalStorageValue(lua_State* L)
{
	//getGlobalStorageValue(valueid)
	uint32_t key = popNumber(L);

	int32_t value;
	if (getScriptEnv()->getGlobalStorageValue(key, value)) {
		lua_pushnumber(L, value);
	} else {
		lua_pushnumber(L, -1);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetGlobalStorageValue(lua_State* L)
{
	//setGlobalStorageValue(valueid, newvalue)
	int32_t value = popNumber<int32_t>(L);
	uint32_t key = popNumber(L);

	getScriptEnv()->addGlobalStorageValue(key, value);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerSetGuildLevel(lua_State* L)
{
	//doPlayerSetGuildLevel(cid, level)
	uint8_t level = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->setGuildLevel(level);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerSetGuildNick(lua_State* L)
{
	//doPlayerSetGuildNick(cid, nick)
	std::string nick = popString(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->setGuildNick(nick);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoMoveCreature(lua_State* L)
{
	//doMoveCreature(cid, direction)
	uint32_t direction = popNumber(L);
	switch (direction) {
		case NORTH:
		case SOUTH:
		case WEST:
		case EAST:
		case SOUTHWEST:
		case NORTHWEST:
		case NORTHEAST:
		case SOUTHEAST:
			break;
		default:
			reportErrorFunc("No valid direction");
			pushBoolean(L, false);
			return 1;
	}

	uint32_t cid = popNumber(L);
	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		ReturnValue ret = g_game.internalMoveCreature(creature, (Direction)direction, FLAG_NOLIMIT);
		lua_pushnumber(L, ret);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaIsValidUID(lua_State* L)
{
	//isValidUID(uid)
	pushBoolean(L, getScriptEnv()->getThingByUID(popNumber(L)) != nullptr);
	return 1;
}

int32_t LuaScriptInterface::luaIsDepot(lua_State* L)
{
	//isDepot(uid)
	Container* container = getScriptEnv()->getContainerByUID(popNumber(L));
	pushBoolean(L, container && container->getDepotLocker());
	return 1;
}

int32_t LuaScriptInterface::luaIsCorpse(lua_State* L)
{
	//isCorpse(uid)
	Item* item = getScriptEnv()->getItemByUID(popNumber(L));
	if (item) {
		const ItemType& it = Item::items[item->getID()];
		pushBoolean(L, it.corpseType != RACE_NONE);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaIsMoveable(lua_State* L)
{
	//isMoveable(uid)
	//isMovable(uid)
	Thing* thing = getScriptEnv()->getThingByUID(popNumber(L));
	pushBoolean(L, thing && thing->isPushable());
	return 1;
}

int32_t LuaScriptInterface::luaGetPlayersByAccountNumber(lua_State* L)
{
	//getPlayersByAccountNumber(accountNumber)
	uint32_t accno = popNumber(L);

	lua_newtable(L);
	int index = 0;
	for (Player* player : g_game.getPlayersByAccount(accno)) {
		lua_pushnumber(L, ++index);
		lua_pushnumber(L, player->getID());
		lua_settable(L, -3);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetPlayersByIPAddress(lua_State* L)
{
	//getPlayersByIPAddress(ip[, mask])
	int parameters = lua_gettop(L);

	uint32_t mask = 0xFFFFFFFF;

	if (parameters > 1) {
		mask = popNumber(L);
	}

	uint32_t ip = popNumber(L);

	lua_newtable(L);
	int index = 0;
	for (Player* player : g_game.getPlayersByIP(ip, mask)) {
		lua_pushnumber(L, ++index);
		lua_pushnumber(L, player->getID());
		lua_settable(L, -3);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetAccountNumberByPlayerName(lua_State* L)
{
	//getAccountNumberByPlayerName(name)
	std::string name = popString(L);

	Player* player = g_game.getPlayerByName(name);
	uint32_t value = 0;

	if (player) {
		value = player->getAccount();
	} else {
		value = IOLoginData::getInstance()->getAccountNumberByName(name);
	}

	lua_pushnumber(L, value);
	return 1;
}

int32_t LuaScriptInterface::luaGetPlayerGUIDByName(lua_State* L)
{
	//getPlayerGUIDByName(name)
	std::string name = popString(L);
	uint32_t value;

	Player* player = g_game.getPlayerByName(name);
	if (player) {
		value = player->getGUID();
	} else {
		uint32_t guid;
		std::string strName(name);

		if (IOLoginData::getInstance()->getGuidByName(guid, strName)) {
			value = guid;
		} else {
			value = 0;
		}
	}

	lua_pushnumber(L, value);
	return 1;
}

int32_t LuaScriptInterface::luaDoAddContainerItem(lua_State* L)
{
	//doAddContainerItem(uid, itemid, <optional> count/subtype)
	int32_t parameters = lua_gettop(L);

	uint32_t count = 1;
	if (parameters > 2) {
		count = popNumber(L);
	}

	uint16_t itemId = popNumber<uint16_t>(L);
	uint32_t uid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();
	Container* container = env->getContainerByUID(uid);
	if (!container) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONTAINER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	const ItemType& it = Item::items[itemId];

	int32_t itemCount = 1;

	int32_t subType = 1;

	if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = (int32_t)std::ceil((float)count / 100);
		}

		subType = count;
	} else {
		itemCount = std::max<int32_t>(1, count);
	}

	while (itemCount > 0) {
		int32_t stackCount = std::min<int32_t>(100, subType);
		Item* newItem = Item::CreateItem(itemId, stackCount);
		if (!newItem) {
			reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}

		if (it.stackable) {
			subType -= stackCount;
		}

		ReturnValue ret = g_game.internalAddItem(container, newItem);
		if (ret != RET_NOERROR) {
			delete newItem;
			pushBoolean(L, false);
			return 1;
		}

		if (--itemCount == 0) {
			if (newItem->getParent()) {
				lua_pushnumber(L, env->addThing(newItem));
				return 1;
			} else {
				//stackable item stacked with existing object, newItem will be released
				pushBoolean(L, false);
				return 1;
			}
		}
	}

	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaGetDepotId(lua_State* L)
{
	//getDepotId(uid)
	uint32_t uid = popNumber(L);

	Container* container = getScriptEnv()->getContainerByUID(uid);
	if (container) {
		DepotLocker* depotLocker = container->getDepotLocker();
		if (depotLocker) {
			lua_pushnumber(L, depotLocker->getDepotId());
		} else {
			reportErrorFunc("Depot not found");
			pushBoolean(L, false);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONTAINER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetFluidSourceType(lua_State* L)
{
	//getFluidSourceType(type)
	uint32_t type = popNumber(L);

	const ItemType& it = Item::items[type];
	if (it.id != 0) {
		lua_pushnumber(L, it.fluidSource);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaIsInArray(lua_State* L)
{
	//isInArray(array, value)
	if (!isTable(L, 1)) {
		pushBoolean(L, false);
		return 1;
	}
	
	pushNil(L);
	while (lua_next(L, 1)) {
		if (lua_equal(L, 2, -1) != 0) {
			pushBoolean(L, true);
			return 1;
		}
		lua_pop(L, 1);
	}

	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaDoSetCreatureLight(lua_State* L)
{
	//doSetCreatureLight(cid, lightLevel, lightColor, time)
	uint32_t time = popNumber(L);
	uint8_t color = popNumber<uint8_t>(L);
	uint8_t level = popNumber<uint8_t>(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		Condition* condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_LIGHT, time, level | (color << 8));
		creature->addCondition(condition);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerPopupFYI(lua_State* L)
{
	//doPlayerPopupFYI(cid, message)
	std::string message = popString(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->sendFYIBox(message);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMayNotMove(lua_State* L)
{
	//mayNotMove(cid, value)
	bool boolValue = popBoolean(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->mayNotMove = boolValue;
		if (player->mayNotMove) {
			player->onWalkAborted();
		}

		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetItemWeightByUID(lua_State* L)
{
	//getItemWeight(itemid[, precise = true])
	bool precise = true;
	if (lua_gettop(L) > 2) {
		precise = popBoolean(L);
	} else {
		precise = false;
	}

	Item* item = getScriptEnv()->getItemByUID(popNumber(L));
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	double weight = item->getWeight();
	if (precise) {
		std::ostringstream ws;
		ws << std::fixed << std::setprecision(2) << weight;
		weight = atof(ws.str().c_str());
	}

	lua_pushnumber(L, weight);
	return 1;
}

int32_t LuaScriptInterface::luaHasProperty(lua_State* L)
{
	//hasProperty(uid, prop)
	uint32_t prop = popNumber(L);
	uint32_t uid = popNumber(L);

	Item* item = getScriptEnv()->getItemByUID(uid);
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	//Check if the item is a tile, so we can get more accurate properties
	bool hasProp;
	const Tile* itemTile = item->getTile();
	if (itemTile && itemTile->ground == item) {
		hasProp = itemTile->hasProperty((ITEMPROPERTY)prop);
	} else {
		hasProp = item->hasProperty((ITEMPROPERTY)prop);
	}

	pushBoolean(L, hasProp);
	return 1;
}

int32_t LuaScriptInterface::luaGetCreatureSummons(lua_State* L)
{
	//getCreatureSummons(cid)
	//returns a table with all the summons of the creature
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	lua_newtable(L);
	uint32_t index = 0;
	for (Creature* summon : creature->getSummons()) {
		lua_pushnumber(L, ++index);
		lua_pushnumber(L, summon->getID());
		lua_settable(L, -3);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetSpectators(lua_State* L)
{
	//getSpectators(centerPos, rangex, rangey, multifloor, onlyPlayers = false)
	int32_t parameters = lua_gettop(L);

	bool onlyPlayers = false;

	if (parameters > 4) {
		onlyPlayers = popBoolean(L);
	}

	bool multifloor = popBoolean(L);
	uint32_t rangey = popNumber(L);
	uint32_t rangex = popNumber(L);

	PositionEx centerPos;
	popPosition(L, centerPos);

	SpectatorVec list;
	g_game.getSpectators(list, centerPos, multifloor, onlyPlayers, rangex, rangex, rangey, rangey);
	if (list.empty()) {
		lua_pushnil(L);
		return 1;
	}

	lua_newtable(L);
	uint32_t index = 0;
	for (Creature* spectator : list) {
		lua_pushnumber(L, ++index);
		lua_pushnumber(L, spectator->getID());
		lua_settable(L, -3);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetCreatureSpeed(lua_State* L)
{
	//getCreatureSpeed(cid)
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		lua_pushnumber(L, creature->getSpeed());
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetCreatureBaseSpeed(lua_State* L)
{
	//getCreatureBaseSpeed(cid)
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		lua_pushnumber(L, creature->getBaseSpeed());
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaAddEvent(lua_State* L)
{
	//addEvent(callback, delay, ...)
	lua_State* globalState = g_luaEnvironment.getLuaState();
	if (!globalState) {
		reportErrorFunc("No valid script interface!");
		pushBoolean(L, false);
		return 1;
	}

	int32_t parameters = getStackTop(L);
	if (!isFunction(globalState, -parameters)) { //-parameters means the first parameter from left to right
		reportErrorFunc("callback parameter should be a function.");
		pushBoolean(L, false);
		return 1;
	}

	LuaTimerEventDesc eventDesc;
	for (int32_t i = 0; i < parameters - 2; ++i) { //-2 because addEvent needs at least two parameters
		eventDesc.parameters.push_back(luaL_ref(globalState, LUA_REGISTRYINDEX));
	}

	uint32_t delay = std::max<uint32_t>(100, popNumber(globalState));
	eventDesc.function = luaL_ref(globalState, LUA_REGISTRYINDEX);
	eventDesc.scriptId = getScriptEnv()->getScriptId();

	auto& lastTimerEventId = g_luaEnvironment.m_lastEventTimerId;
	eventDesc.eventId = g_scheduler.addEvent(createSchedulerTask(
		delay, boost::bind(&LuaEnvironment::executeTimerEvent, &g_luaEnvironment, lastTimerEventId)
	));

	g_luaEnvironment.m_timerEvents[lastTimerEventId] = eventDesc;
	pushNumber(L, lastTimerEventId++);
	return 1;
}

int32_t LuaScriptInterface::luaStopEvent(lua_State* L)
{
	//stopEvent(eventid)
	uint32_t eventId = popNumber(L);
	if (!g_luaEnvironment.getLuaState()) {
		reportErrorFunc("No valid script interface!");
		pushBoolean(L, false);
		return 1;
	}

	auto& timerEvents = g_luaEnvironment.m_timerEvents;
	auto it = timerEvents.find(eventId);
	if (it == timerEvents.end()) {
		pushBoolean(L, false);
		return 1;
	}

	const LuaTimerEventDesc& timerEventDesc = it->second;
	g_scheduler.stopEvent(timerEventDesc.eventId);

	for (auto parameter : timerEventDesc.parameters) {
		luaL_unref(g_luaEnvironment.m_luaState, LUA_REGISTRYINDEX, parameter);
	}

	luaL_unref(g_luaEnvironment.m_luaState, LUA_REGISTRYINDEX, timerEventDesc.function);
	timerEvents.erase(it);

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetCreatureCondition(lua_State* L)
{
	uint32_t subId;
	if (lua_gettop(L) > 2) {
		subId = popNumber(L);
	} else {
		subId = 0;
	}

	uint32_t condition = popNumber(L);
	uint32_t cid = popNumber(L);

	Creature* creature = g_game.getCreatureByID(cid);
	if (creature) {
		if (creature->hasCondition((ConditionType_t)condition, subId)) {
			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSaveServer(lua_State* L)
{
	g_dispatcher.addTask(
	    createTask(boost::bind(&Game::saveGameState, &g_game)));
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaRefreshMap(lua_State* L)
{
	g_dispatcher.addTask(
	    createTask(boost::bind(&Game::refreshMap, &g_game)));
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaCleanMap(lua_State* L)
{
	g_dispatcher.addTask(
	    createTask(boost::bind(&Game::cleanMap, &g_game)));
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetOnlinePlayers(lua_State* L)
{
	//getOnlinePlayers()
	lua_newtable(L);
	uint32_t index = 0;
	for (const auto& it : g_game.getPlayers()) {
		lua_pushnumber(L, ++index);
		pushString(L, it.second->getName());
		lua_settable(L, -3);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetPlayerParty(lua_State* L)
{
	//getPlayerParty(cid)
	uint32_t cid = popNumber(L);

	if (Player* player = g_game.getPlayerByID(cid)) {
		if (Party* party = player->getParty()) {
			lua_pushnumber(L, party->getLeader()->getID());
		} else {
			lua_pushnil(L);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerJoinParty(lua_State* L)
{
	//doPlayerJoinParty(cid, lid)
	uint32_t leaderId = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* leader = g_game.getPlayerByID(leaderId);
	if (!leader) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	g_game.playerJoinParty(player->getID(), leader->getID());
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetPartyMembers(lua_State* L)
{
	//getPartyMembers(leaderId)
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		Party* party = player->getParty();
		if (party) {
			PlayerVector list = party->getMembers();
			list.push_back(party->getLeader());

			lua_newtable(L);
			uint32_t index = 0;

			for (Player* partyMember : list) {
				lua_pushnumber(L, ++index);
				lua_pushnumber(L, partyMember->getID());
				lua_settable(L, -3);
			}

			return 1;
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
	}

	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaIsInWar(lua_State* L)
{
	//isInWar(cid, target)
	uint32_t target = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Player* targetPlayer = g_game.getPlayerByID(target);
	if (!targetPlayer) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	pushBoolean(L, player->isInWar(targetPlayer));
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerSetOfflineTrainingSkill(lua_State* L)
{
	//doPlayerSetOfflineTrainingSkill(cid, skillid)
	uint32_t skillid = popNumber(L);
	uint32_t cid = popNumber(L);

	Player* player = g_game.getPlayerByID(cid);
	if (player) {
		player->setOfflineTrainingSkill(skillid);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetWaypointPosition(lua_State* L)
{
	//getWaypointPosition(name)
	const std::map<std::string, Position>& waypoints = g_game.getMap()->waypoints;

	auto it = waypoints.find(popString(L));
	if (it != waypoints.end()) {
		pushPosition(L, it->second, 0);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoWaypointAddTemporial(lua_State* L)
{
	//doWaypointAddTemporial(name, pos)
	PositionEx pos;
	popPosition(L, pos);

	g_game.getMap()->waypoints[popString(L)] = pos;
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaSendChannelMessage(lua_State* L)
{
	//sendChannelMessage(channelId, type, message)
	std::string message = popString(L);
	SpeakClasses type = (SpeakClasses)popNumber(L);
	uint32_t channelId = popNumber(L);

	ChatChannel* channel = g_chat.getChannelById(channelId);
	if (channel) {
		channel->sendToAll(message, type);
		pushBoolean(L, true);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSendGuildChannelMessage(lua_State* L)
{
	//sendGuildChannelMessage(guildId, type, message)
	std::string message = popString(L);
	SpeakClasses type = (SpeakClasses)popNumber<uint32_t>(L);
	uint32_t guildId = popNumber(L);

	ChatChannel* channel = g_chat.getGuildChannelById(guildId);
	if (channel) {
		channel->sendToAll(message, type);
		pushBoolean(L, true);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

std::string LuaScriptInterface::escapeString(const std::string& string)
{
	std::string s = string;
	replaceString(s, "\\", "\\\\");
	replaceString(s, "\"", "\\\"");
	replaceString(s, "'", "\\'");
	replaceString(s, "[[", "\\[[");
	return s;
}

#ifndef __LUAJIT__
const luaL_Reg LuaScriptInterface::luaBitReg[] = {
	//{"cast", LuaScriptInterface::luaBitCast},
	{"bnot", LuaScriptInterface::luaBitNot},
	{"band", LuaScriptInterface::luaBitAnd},
	{"bor", LuaScriptInterface::luaBitOr},
	{"bxor", LuaScriptInterface::luaBitXor},
	{"lshift", LuaScriptInterface::luaBitLeftShift},
	{"rshift", LuaScriptInterface::luaBitRightShift},
	// Unsigned
	{"ubnot", LuaScriptInterface::luaBitUNot},
	{"uband", LuaScriptInterface::luaBitUAnd},
	{"ubor", LuaScriptInterface::luaBitUOr},
	{"ubxor", LuaScriptInterface::luaBitUXor},
	{"ulshift", LuaScriptInterface::luaBitULeftShift},
	{"urshift", LuaScriptInterface::luaBitURightShift},
	//{"arshift", LuaScriptInterface::luaBitArithmeticalRightShift},
	{nullptr, nullptr}
};

int32_t LuaScriptInterface::luaBitNot(lua_State* L)
{
	int32_t number = popNumber<int32_t>(L);
	lua_pushnumber(L, ~number);
	return 1;
}

int32_t LuaScriptInterface::luaBitUNot(lua_State* L)
{
	uint32_t number = popNumber(L);
	lua_pushnumber(L, ~number);
	return 1;
}

#define MULTIOP(type, name, op) \
	int32_t LuaScriptInterface::luaBit##name(lua_State* L) \
	{ \
		int32_t n = lua_gettop(L); \
		type w = popNumber<type>(L); \
		for (int32_t i = 2; i <= n; ++i) \
			w op popNumber(L); \
		lua_pushnumber(L, w); \
		return 1; \
	}

MULTIOP(int32_t, And, &= )
MULTIOP(int32_t, Or, |= )
MULTIOP(int32_t, Xor, ^= )
MULTIOP(uint32_t, UAnd, &= )
MULTIOP(uint32_t, UOr, |= )
MULTIOP(uint32_t, UXor, ^= )

#define SHIFTOP(type, name, op) \
	int32_t LuaScriptInterface::luaBit##name(lua_State* L) \
	{ \
		type n2 = popNumber<type>(L), n1 = popNumber<type>(L); \
		lua_pushnumber(L, (n1 op n2)); \
		return 1; \
	}

SHIFTOP(int32_t, LeftShift, << )
SHIFTOP(int32_t, RightShift, >> )
SHIFTOP(uint32_t, ULeftShift, << )
SHIFTOP(uint32_t, URightShift, >> )
#endif

const luaL_Reg LuaScriptInterface::luaConfigManagerTable[] = {
	{"getString", LuaScriptInterface::luaConfigManagerGetString},
	{"getNumber", LuaScriptInterface::luaConfigManagerGetNumber},
	{"getBoolean", LuaScriptInterface::luaConfigManagerGetBoolean},
	{nullptr, nullptr}
};

int32_t LuaScriptInterface::luaConfigManagerGetString(lua_State* L)
{
	pushString(L, g_config.getString(static_cast<ConfigManager::string_config_t>(popNumber(L))));
	return 1;
}

int32_t LuaScriptInterface::luaConfigManagerGetNumber(lua_State* L)
{
	pushNumber(L, g_config.getNumber(static_cast<ConfigManager::integer_config_t>(popNumber(L))));
	return 1;
}

int32_t LuaScriptInterface::luaConfigManagerGetBoolean(lua_State* L)
{
	pushBoolean(L, g_config.getBoolean(static_cast<ConfigManager::boolean_config_t>(popNumber(L))));
	return 1;
}

const luaL_Reg LuaScriptInterface::luaDatabaseTable[] = {
	{"query", LuaScriptInterface::luaDatabaseExecute},
	{"storeQuery", LuaScriptInterface::luaDatabaseStoreQuery},
	{"escapeString", LuaScriptInterface::luaDatabaseEscapeString},
	{"escapeBlob", LuaScriptInterface::luaDatabaseEscapeBlob},
	{"lastInsertId", LuaScriptInterface::luaDatabaseLastInsertId},
	{"connected", LuaScriptInterface::luaDatabaseConnected},
	{"tableExists", LuaScriptInterface::luaDatabaseTableExists},
	{nullptr, nullptr}
};

int32_t LuaScriptInterface::luaDatabaseExecute(lua_State* L)
{
	pushBoolean(L, Database::getInstance()->executeQuery(popString(L)));
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseStoreQuery(lua_State* L)
{
	if (DBResult* res = Database::getInstance()->storeQuery(popString(L))) {
		lua_pushnumber(L, getScriptEnv()->addResult(res));
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseEscapeString(lua_State* L)
{
	pushString(L, Database::getInstance()->escapeString(popString(L)));
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseEscapeBlob(lua_State* L)
{
	uint32_t length = popNumber(L);
	pushString(L, Database::getInstance()->escapeBlob(popString(L).c_str(), length));
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseLastInsertId(lua_State* L)
{
	lua_pushnumber(L, Database::getInstance()->getLastInsertId());
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseConnected(lua_State* L)
{
	pushBoolean(L, Database::getInstance()->isConnected());
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseTableExists(lua_State* L)
{
	pushBoolean(L, DatabaseManager::getInstance()->tableExists(popString(L)));
	return 1;
}

const luaL_Reg LuaScriptInterface::luaResultTable[] = {
	{"getDataInt", LuaScriptInterface::luaResultGetDataInt},
	{"getDataLong", LuaScriptInterface::luaResultGetDataLong},
	{"getDataString", LuaScriptInterface::luaResultGetDataString},
	{"getDataStream", LuaScriptInterface::luaResultGetDataStream},
	{"next", LuaScriptInterface::luaResultNext},
	{"free", LuaScriptInterface::luaResultFree},
	{nullptr, nullptr}
};

int32_t LuaScriptInterface::luaResultGetDataInt(lua_State* L)
{
	const std::string& s = popString(L);

	DBResult* res = getScriptEnv()->getResultByID(popNumber(L));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	lua_pushnumber(L, res->getDataInt(s));
	return 1;
}

int32_t LuaScriptInterface::luaResultGetDataLong(lua_State* L)
{
	const std::string& s = popString(L);

	DBResult* res = getScriptEnv()->getResultByID(popNumber(L));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	lua_pushnumber(L, res->getNumber<int64_t>(s));
	return 1;
}

int32_t LuaScriptInterface::luaResultGetDataString(lua_State* L)
{
	const std::string& s = popString(L);

	DBResult* res = getScriptEnv()->getResultByID(popNumber(L));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	pushString(L, res->getDataString(s));
	return 1;
}

int32_t LuaScriptInterface::luaResultGetDataStream(lua_State* L)
{
	const std::string& s = popString(L);

	DBResult* res = getScriptEnv()->getResultByID(popNumber(L));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	unsigned long length;
	lua_pushstring(L, res->getDataStream(s, length));
	lua_pushnumber(L, length);
	return 2;
}

int32_t LuaScriptInterface::luaResultNext(lua_State* L)
{
	DBResult* res = getScriptEnv()->getResultByID(popNumber(L));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	pushBoolean(L, res->next());
	return 1;
}

int32_t LuaScriptInterface::luaResultFree(lua_State* L)
{
	pushBoolean(L, getScriptEnv()->removeResult(popNumber(L)));
	return 1;
}

// _G
int32_t LuaScriptInterface::luaIsType(lua_State* L)
{
	// isType(derived, base)
	lua_getmetatable(L, -2);
	lua_getmetatable(L, -2);

	lua_rawgeti(L, -2, 'p');
	uint_fast8_t parentsB = getNumber<uint_fast8_t>(L, 1);

	lua_rawgeti(L, -3, 'h');
	size_t hashB = getNumber<size_t>(L, 1);

	lua_rawgeti(L, -3, 'p');
	uint_fast8_t parentsA = getNumber<uint_fast8_t>(L, 1);
	for (uint_fast8_t i = parentsA; i < parentsB; ++i) {
		pushString(L, "__index");
		lua_rawget(L, -4);
		lua_replace(L, -4);
	}

	lua_rawgeti(L, -4, 'h');
	size_t hashA = getNumber<size_t>(L, 1);

	pushBoolean(L, hashA == hashB);
	return 1;
}

// Position
int32_t LuaScriptInterface::luaPositionCreate(lua_State* L)
{
	// Position([x = 0[, y = 0[, z = 0[, stackpos = 0]]]])
	// Position([position])
	// Position.new([x = 0[, y = 0[, z = 0[, stackpos = 0]]]])
	// Position.new([position])
	int32_t parameters = getStackTop(L);
	if (parameters <= 1) {
		pushMetaPosition(L, Position(), 0);
		return 1;
	}

	if (isTable(L, 2)) {
		uint32_t stackpos;
		const Position& position = getPosition(L, 2, stackpos);
		pushMetaPosition(L, position, stackpos);
	} else {
		int32_t stackpos = 0;
		if (parameters >= 5) {
			stackpos = getNumber<int32_t>(L, 5);
		}

		uint8_t z = 0;
		if (parameters >= 4) {
			z = getNumber<uint8_t>(L, 4);
		}

		uint16_t y = 0;
		if (parameters >= 3) {
			y = getNumber<uint16_t>(L, 3);
		}

		uint16_t x = getNumber<uint16_t>(L, 2);
		pushMetaPosition(L, Position(x, y, z), stackpos);
	}

	return 1;
}

int32_t LuaScriptInterface::luaPositionAdd(lua_State* L)
{
	// positionValue = position + positionEx
	uint32_t stackpos;
	const Position& position = getPosition(L, 1, stackpos);

	Position positionEx;
	if (stackpos == 0) {
		positionEx = getPosition(L, 2, stackpos);
	} else {
		positionEx = getPosition(L, 2);
	}

	pushMetaPosition(L, position + positionEx, stackpos);
	return 1;
}

int32_t LuaScriptInterface::luaPositionSub(lua_State* L)
{
	// positionValue = position - positionEx
	uint32_t stackpos;
	const Position& position = getPosition(L, 1, stackpos);

	Position positionEx;
	if (stackpos == 0) {
		positionEx = getPosition(L, 2, stackpos);
	} else {
		positionEx = getPosition(L, 2);
	}

	pushMetaPosition(L, position - positionEx, stackpos);
	return 1;
}

int32_t LuaScriptInterface::luaPositionCompare(lua_State* L)
{
	// position == positionEx
	const Position& positionEx = getPosition(L, 2);
	const Position& position = getPosition(L, 1);
	pushBoolean(L, position == positionEx);
	return 1;
}

int32_t LuaScriptInterface::luaPositionGetTile(lua_State* L)
{
	// position:getTile()
	const Position& position = getPosition(L, 1);

	Tile* tile = g_game.getTile(position);
	if (tile) {
		pushUserdata<Tile>(L, tile);
		setMetatable(L, -1, "Tile");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPositionGetDistance(lua_State* L)
{
	// position:getDistance(positionEx)
	const Position& positionEx = getPosition(L, 2);
	const Position& position = getPosition(L, 1);
	pushNumber(L, std::max<int32_t>(
		std::max<int32_t>(
			std::abs(Position::getDistanceX(position, positionEx)),
			std::abs(Position::getDistanceY(position, positionEx))
		),
		std::abs(Position::getDistanceZ(position, positionEx))
	));
	return 1;
}

int32_t LuaScriptInterface::luaPositionIsSightClear(lua_State* L)
{
	// position:isSightClear(positionEx[, sameFloor = true])
	bool sameFloor = true;
	if (getStackTop(L) >= 3) {
		sameFloor = getBoolean(L, 3);
	}
	const Position& positionEx = getPosition(L, 2);
	const Position& position = getPosition(L, 1);
	pushBoolean(L, g_game.isSightClear(position, positionEx, sameFloor));
	return 1;
}

int32_t LuaScriptInterface::luaPositionSendMagicEffect(lua_State* L)
{
	// position:sendMagicEffect(magicEffect[, player = nullptr])
	SpectatorVec list;
	if (getStackTop(L) >= 3) {
		Player* player = getPlayer(L, 3);
		if (player) {
			list.insert(player);
		}
	}

	MagicEffectClasses magicEffect = static_cast<MagicEffectClasses>(getNumber<uint64_t>(L, 2));
	const Position& position = getPosition(L, 1);
	if (!list.empty()) {
		g_game.addMagicEffect(list, position, magicEffect);
	} else {
		g_game.addMagicEffect(position, magicEffect);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaPositionSendDistanceEffect(lua_State* L)
{
	// position:sendDistanceEffect(positionEx, distanceEffect[, player = nullptr])
	SpectatorVec list;
	if (getStackTop(L) >= 4) {
		Player* player = getPlayer(L, 4);
		if (player) {
			list.insert(player);
		}
	}

	ShootType_t distanceEffect = static_cast<ShootType_t>(getNumber<uint64_t>(L, 3));
	const Position& positionEx = getPosition(L, 2);
	const Position& position = getPosition(L, 1);
	if (!list.empty()) {
		g_game.addDistanceEffect(list, position, positionEx, distanceEffect);
	} else {
		g_game.addDistanceEffect(position, positionEx, distanceEffect);
	}

	pushBoolean(L, true);
	return 1;
}

// Tile
int32_t LuaScriptInterface::luaTileCreate(lua_State* L)
{
	// Tile(x, y, z)
	// Tile(position)
	// Tile.new(x, y, z)
	// Tile.new(position)
	Tile* tile;
	if (isTable(L, 2)) {
		tile = g_game.getTile(getPosition(L, 2));
	} else {
		uint8_t z = getNumber<uint8_t>(L, 4);
		uint16_t y = getNumber<uint16_t>(L, 3);
		uint16_t x = getNumber<uint16_t>(L, 2);
		tile = g_game.getTile(x, y, z);
	}

	if (tile) {
		pushUserdata<Tile>(L, tile);
		setMetatable(L, -1, "Tile");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetPosition(lua_State* L)
{
	// tile:getPosition()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		pushMetaPosition(L, tile->getPosition(), 0);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetGround(lua_State* L)
{
	// tile:getGround()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile && tile->ground) {
		pushUserdata<Item>(L, tile->ground);
		setItemMetatable(L, -1, tile->ground);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetThing(lua_State* L)
{
	// tile:getThing(index)
	int32_t index = getNumber<int32_t>(L, 2);
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		Thing* thing = tile->__getThing(index);
		if (thing) {
			if (Creature* creature = thing->getCreature()) {
				pushUserdata<Creature>(L, creature);
				setCreatureMetatable(L, -1, creature);
			} else if (Item* item = thing->getItem()) {
				pushUserdata<Item>(L, item);
				setItemMetatable(L, -1, item);
			} else {
				pushNil(L);
			}
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetThingCount(lua_State* L)
{
	// tile:getThingCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		pushNumber(L, tile->getThingCount());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetTopTopItem(lua_State* L)
{
	// tile:getTopTopItem()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		Item* item = tile->getTopTopItem();
		if (item) {
			pushUserdata<Item>(L, item);
			setItemMetatable(L, -1, item);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetTopDownItem(lua_State* L)
{
	// tile:getTopDownItem()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		Item* item = tile->getTopDownItem();
		if (item) {
			pushUserdata<Item>(L, item);
			setItemMetatable(L, -1, item);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetFieldItem(lua_State* L)
{
	// tile:getFieldItem()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		Item* item = tile->getFieldItem();
		if (item) {
			pushUserdata<Item>(L, item);
			setItemMetatable(L, -1, item);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItemById(lua_State* L)
{
	// tile:getItemById(itemId[, subType = -1])
	int32_t subType = -1;
	if (getStackTop(L) >= 3) {
		subType = getNumber<int32_t>(L, 3);
	}

	uint16_t itemId = getNumber<uint16_t>(L, 2);
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		Item* item = g_game.findItemOfType(tile, itemId, false, subType);
		if (item) {
			pushUserdata<Item>(L, item);
			setItemMetatable(L, -1, item);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItemByType(lua_State* L)
{
	// tile:getItemByType(itemType)
	ItemTypes_t itemType = static_cast<ItemTypes_t>(getNumber<uint64_t>(L, 2));
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		bool found;
		switch (itemType) {
			case ITEM_TYPE_TELEPORT:
				found = tile->hasFlag(TILESTATE_TELEPORT);
				break;
			case ITEM_TYPE_MAGICFIELD:
				found = tile->hasFlag(TILESTATE_MAGICFIELD);
				break;
			case ITEM_TYPE_MAILBOX:
				found = tile->hasFlag(TILESTATE_MAILBOX);
				break;
			case ITEM_TYPE_TRASHHOLDER:
				found = tile->hasFlag(TILESTATE_TRASHHOLDER);
				break;
			case ITEM_TYPE_BED:
				found = tile->hasFlag(TILESTATE_BED);
				break;
			case ITEM_TYPE_DEPOT:
				found = tile->hasFlag(TILESTATE_DEPOT);
				break;
			default:
				found = true;
				break;
		}

		if (found) {
			if (Item* item = tile->ground) {
				const ItemType& it = Item::items[item->getID()];
				if (it.type == itemType) {
					pushUserdata<Item>(L, item);
					setItemMetatable(L, -1, item);
					return 1;
				}
			}

			if (const TileItemVector* items = tile->getItemList()) {
				for (Item* item : *items) {
					const ItemType& it = Item::items[item->getID()];
					if (it.type == itemType) {
						pushUserdata<Item>(L, item);
						setItemMetatable(L, -1, item);
						return 1;
					}
				}
			}
		}

		pushNil(L);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItemByTopOrder(lua_State* L)
{
	// tile:getItemByTopOrder(topOrder)
	int32_t topOrder = getNumber<int32_t>(L, 2);
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		Item* item = tile->getItemByTopOrder(topOrder);
		if (item) {
			pushUserdata<Item>(L, item);
			setItemMetatable(L, -1, item);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetBottomCreature(lua_State* L)
{
	// tile:getBottomCreature()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		const Creature* creature = tile->getBottomCreature();
		if (creature) {
			pushUserdata<const Creature>(L, creature);
			setCreatureMetatable(L, -1, creature);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetTopCreature(lua_State* L)
{
	// tile:getTopCreature()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		const Creature* creature = tile->getTopCreature();
		if (creature) {
			pushUserdata<const Creature>(L, creature);
			setCreatureMetatable(L, -1, creature);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetBottomVisibleCreature(lua_State* L)
{
	// tile:getBottomVisibleCreature(creature)
	Creature* creature = getCreature(L, 2);
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile && creature) {
		const Creature* visibleCreature = tile->getBottomVisibleCreature(creature);
		if (visibleCreature) {
			pushUserdata<const Creature>(L, visibleCreature);
			setCreatureMetatable(L, -1, visibleCreature);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetTopVisibleCreature(lua_State* L)
{
	// tile:getTopVisibleCreature(creature)
	Creature* creature = getCreature(L, 2);
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile && creature) {
		const Creature* visibleCreature = tile->getTopVisibleCreature(creature);
		if (visibleCreature) {
			pushUserdata<const Creature>(L, visibleCreature);
			setCreatureMetatable(L, -1, visibleCreature);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItems(lua_State* L)
{
	// tile:getItems()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		TileItemVector* itemVector = tile->getItemList();
		if (itemVector) {
			uint32_t i = 0;
			lua_newtable(L);
			for (Item* item : *itemVector) {
				pushNumber(L, ++i);
				pushUserdata<Item>(L, item);
				setItemMetatable(L, -1, item);
				lua_settable(L, -3);
			}
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItemCount(lua_State* L)
{
	// tile:getItemCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		pushNumber(L, tile->getItemCount());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetDownItemCount(lua_State* L)
{
	// tile:getDownItemCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		pushNumber(L, tile->getDownItemCount());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetTopItemCount(lua_State* L)
{
	// tile:getTopItemCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		pushNumber(L, tile->getTopItemCount());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetCreatures(lua_State* L)
{
	// tile:getCreatures()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		CreatureVector* creatureVector = tile->getCreatures();
		if (creatureVector) {
			uint32_t i = 0;
			lua_newtable(L);
			for (Creature* creature : *creatureVector) {
				pushNumber(L, ++i);
				pushUserdata<Creature>(L, creature);
				setCreatureMetatable(L, -1, creature);
				lua_settable(L, -3);
			}
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetCreatureCount(lua_State* L)
{
	// tile:getCreatureCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		pushNumber(L, tile->getCreatureCount());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileHasProperty(lua_State* L)
{
	// tile:hasProperty(property[, item])
	Item* item;
	if (getStackTop(L) >= 3) {
		item = getUserdata<Item>(L, 3);
	} else {
		item = nullptr;
	}

	ITEMPROPERTY property = static_cast<ITEMPROPERTY>(getNumber<uint64_t>(L, 2));
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		if (item) {
			pushBoolean(L, tile->hasProperty(item, property));
		} else {
			pushBoolean(L, tile->hasProperty(property));
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileHasFlag(lua_State* L)
{
	// tile:hasFlag(flag)
	tileflags_t flag = static_cast<tileflags_t>(getNumber<uint64_t>(L, 2));
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		pushBoolean(L, tile->hasFlag(flag));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileQueryAdd(lua_State* L)
{
	// tile:queryAdd(thing[, flags])
	uint32_t flags = 0;
	if (getStackTop(L) >= 3) {
		flags = getNumber<uint32_t>(L, 3);
	}
	Thing* thing = getThing(L, 2);
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile && thing) {
		pushNumber(L, tile->__queryAdd(0, thing, 1, flags));
	} else {
		pushNil(L);
	}
	return 1;
}

// NetworkMessage
int32_t LuaScriptInterface::luaNetworkMessageCreate(lua_State* L)
{
	// NetworkMessage()
	// NetworkMessage.new()
	pushUserdata<NetworkMessage>(L, new NetworkMessage);
	setMetatable(L, -1, "NetworkMessage");
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageDelete(lua_State* L)
{
	NetworkMessage** messagePtr = getRawUserdata<NetworkMessage>(L, 1);
	if (messagePtr && *messagePtr) {
		delete *messagePtr;
		*messagePtr = nullptr;
	}
	return 0;
}

int32_t LuaScriptInterface::luaNetworkMessageGetByte(lua_State* L)
{
	// networkMessage:getByte()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		pushNumber(L, message->GetByte());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageGetU16(lua_State* L)
{
	// networkMessage:getU16()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		pushNumber(L, message->GetU16());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageGetU32(lua_State* L)
{
	// networkMessage:getU32()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		pushNumber(L, message->GetU32());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageGetU64(lua_State* L)
{
	// networkMessage:getU64()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		pushNumber(L, message->GetU64());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageGetString(lua_State* L)
{
	// networkMessage:getString()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		pushString(L, message->GetString());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageGetPosition(lua_State* L)
{
	// networkMessage:getPosition()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		pushMetaPosition(L, message->GetPosition(), 0);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddByte(lua_State* L)
{
	// networkMessage:addByte(number)
	uint8_t number = getNumber<uint8_t>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->AddByte(number);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddU16(lua_State* L)
{
	// networkMessage:addU16(number)
	uint16_t number = getNumber<uint16_t>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->AddU16(number);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddU32(lua_State* L)
{
	// networkMessage:addU32(number)
	uint32_t number = getNumber<uint32_t>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->AddU32(number);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddU64(lua_State* L)
{
	// networkMessage:addU64(number)
	uint64_t number = getNumber<uint64_t>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->AddU64(number);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddString(lua_State* L)
{
	// networkMessage:addString(string)
	const std::string& string = getString(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->AddString(string);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddPosition(lua_State* L)
{
	// networkMessage:addPosition(position)
	const Position& position = getPosition(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->AddPosition(position);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddDouble(lua_State* L)
{
	// networkMessage:addDouble(number)
	double number = getNumber<double>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->AddDouble(number);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddItem(lua_State* L)
{
	// networkMessage:addItem(item[, protocolVersion = CLIENT_VERSION_MIN])
	/*uint16_t protocolVersion = CLIENT_VERSION_MIN;
	if (getStackTop(L) >= 3) {
		protocolVersion = getNumber<uint16_t>(L, 3);
	}*/
	Item* item = getUserdata<Item>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushNil(L);
		return 1;
	}

	if (message) {
		message->AddItem(item);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddItemId(lua_State* L)
{
	// networkMessage:addItemId(itemId[, protocolVersion = CLIENT_VERSION_MIN])
	/*uint16_t protocolVersion = CLIENT_VERSION_MIN;
	if (getStackTop(L) >= 3) {
		protocolVersion = getNumber<uint16_t>(L, 3);
	}*/
	uint16_t itemId = getNumber<uint16_t>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->AddItemId(itemId);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageReset(lua_State* L)
{
	// networkMessage:reset()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageSkipBytes(lua_State* L)
{
	// networkMessage:skipBytes(number)
	int16_t number = getNumber<int16_t>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->SkipBytes(number);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageSendToPlayer(lua_State* L)
{
	// networkMessage:sendToPlayer(player)
	Player* player = getPlayer(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (player) {
		if (message) {
			player->sendNetworkMessage(*message);
		}
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushNil(L);
	}
	return 1;
}

// ModalWindow
int32_t LuaScriptInterface::luaModalWindowCreate(lua_State* L)
{
	// ModalWindow(id, title, message)
	// ModalWindow.new(id, title, message)
	const std::string& message = getString(L, 4);
	const std::string& title = getString(L, 3);
	uint32_t id = getNumber<uint32_t>(L, 2);

	pushUserdata<ModalWindow>(L, new ModalWindow(id, title, message));
	setMetatable(L, -1, "ModalWindow");
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowDelete(lua_State* L)
{
	ModalWindow** windowPtr = getRawUserdata<ModalWindow>(L, 1);
	if (windowPtr && *windowPtr) {
		delete *windowPtr;
		*windowPtr = nullptr;
	}
	return 0;
}

int32_t LuaScriptInterface::luaModalWindowGetId(lua_State* L)
{
	// modalWindow:getId()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		pushNumber(L, window->id);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetTitle(lua_State* L)
{
	// modalWindow:getTitle()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		pushString(L, window->title);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetMessage(lua_State* L)
{
	// modalWindow:getMessage()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		pushString(L, window->message);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetButtonCount(lua_State* L)
{
	// modalWindow:getButtonCount()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		pushNumber(L, window->buttons.size());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetChoiceCount(lua_State* L)
{
	// modalWindow:getChoiceCount()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		pushNumber(L, window->choices.size());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowAddButton(lua_State* L)
{
	// modalWindow:addButton(id, text)
	const std::string& text = getString(L, 3);
	uint8_t id = getNumber<uint8_t>(L, 2);
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->buttons.emplace_back(text, id);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowAddChoice(lua_State* L)
{
	// modalWindow:addChoice(id, text)
	const std::string& text = getString(L, 3);
	uint8_t id = getNumber<uint8_t>(L, 2);
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->choices.emplace_back(text, id);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetDefaultEnterButton(lua_State* L)
{
	// modalWindow:getDefaultEnterButton()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		pushNumber(L, window->defaultEnterButton);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSetDefaultEnterButton(lua_State* L)
{
	// modalWindow:setDefaultEnterButton(buttonId)
	uint8_t buttonId = getNumber<uint8_t>(L, 2);
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->defaultEnterButton = buttonId;
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetDefaultEscapeButton(lua_State* L)
{
	// modalWindow:getDefaultEscapeButton()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		pushNumber(L, window->defaultEscapeButton);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSetDefaultEscapeButton(lua_State* L)
{
	// modalWindow:setDefaultEscapeButton(buttonId)
	uint8_t buttonId = getNumber<uint8_t>(L, 2);
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->defaultEscapeButton = buttonId;
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowHasPriority(lua_State* L)
{
	// modalWindow:hasPriority()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		pushBoolean(L, window->priority);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSetPriority(lua_State* L)
{
	// modalWindow:setPriority(priority)
	bool priority = getBoolean(L, 2);
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->priority = priority;
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSendToPlayer(lua_State* L)
{
	// modalWindow:sendToPlayer(player)
	Player* player = getPlayer(L, 2);
	if (!player) {
		pushNil(L);
		return 1;
	}

	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		if (!player->hasModalWindowOpen(window->id)) {
			player->sendModalWindow(*window);
		}
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

// Item
int32_t LuaScriptInterface::luaItemCreate(lua_State* L)
{
	// Item(uid)
	// Item(itemId, count[, position])
	// Item.new(uid)
	// Item.new(itemId, count[, position])
	int32_t parameters = getStackTop(L);

	uint32_t id = getNumber<uint32_t>(L, 2);

	Item* item;
	ScriptEnvironment* env = getScriptEnv();
	if (parameters >= 3) {
		uint16_t count = getNumber<uint16_t>(L, 3);

		const ItemType& it = Item::items[id];
		if (it.stackable) {
			count = std::min<uint16_t>(count, 100);
		}

		item = Item::CreateItem(id, count);
		if (!item) {
			pushNil(L);
			return 1;
		}

		if (parameters >= 4) {
			const Position& position = getPosition(L, 4);
			Tile* tile = g_game.getTile(position.x, position.y, position.z);
			if (!tile) {
				delete item;
				pushNil(L);
				return 1;
			}

			g_game.internalAddItem(tile, item, INDEX_WHEREEVER, FLAG_NOLIMIT);
		} else {
			item->setParent(VirtualCylinder::virtualCylinder);
			env->addTempItem(env, item);
		}
	} else {
		item = env->getItemByUID(id);
	}

	if (item) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemIsCreature(lua_State* L)
{
	// item:isCreature()
	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaItemIsItem(lua_State* L)
{
	// item:isItem()
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaItemGetId(lua_State* L)
{
	// item:getId()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushNumber(L, item->getID());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemClone(lua_State* L)
{
	// item:clone()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		Item* clone = item->clone();
		if (clone) {
			pushUserdata<Item>(L, clone);
			setItemMetatable(L, -1, clone);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemSplit(lua_State* L)
{
	// item:split([count = 1])
	uint16_t count = 1;
	if (getStackTop(L) >= 2) {
		count = getNumber<uint16_t>(L, 2);
	}
	Item** itemPtr = getRawUserdata<Item>(L, 1);
	if (!itemPtr) {
		pushNil(L);
		return 1;
	}

	if (*itemPtr) {
		Item* item = *itemPtr;
		if (!item->isStackable()) {
			pushNil(L);
			return 1;
		}

		count = std::min<uint16_t>(count, item->getItemCount());
		uint16_t diff = item->getItemCount() - count;

		Item* splitItem = item->clone();
		if (!splitItem) {
			pushNil(L);
			return 1;
		}

		ScriptEnvironment* env = getScriptEnv();
		uint32_t uid = env->addThing(item);

		Item* newItem = g_game.transformItem(item, item->getID(), diff);
		if (item->isRemoved()) {
			env->removeItemByUID(uid);
		}

		if (newItem && newItem != item) {
			env->insertThing(uid, newItem);
		}

		*itemPtr = newItem;

		splitItem->setParent(VirtualCylinder::virtualCylinder);
		env->addTempItem(env, splitItem);

		pushUserdata<Item>(L, splitItem);
		setItemMetatable(L, -1, splitItem);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemRemove(lua_State* L)
{
	// item:remove([count = -1])
	int32_t count = -1;
	if (getStackTop(L) >= 2) {
		count = getNumber<int32_t>(L, 2);
	}

	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushBoolean(L, g_game.internalRemoveItem(item, count) == RET_NOERROR);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetUniqueId(lua_State* L)
{
	// item:getUniqueId()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		uint32_t uniqueId = item->getUniqueId();
		if (uniqueId == 0) {
			uniqueId = getScriptEnv()->addThing(item);
		}
		pushNumber(L, uniqueId);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetActionId(lua_State* L)
{
	// item:getActionId()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushNumber(L, item->getActionId());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemSetActionId(lua_State* L)
{
	// item:setActionId(actionId)
	uint16_t actionId = getNumber<uint16_t>(L, 2);
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		item->setActionId(actionId);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetCount(lua_State* L)
{
	// item:getCount()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushNumber(L, item->getItemCount());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetCharges(lua_State* L)
{
	// item:getCharges()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushNumber(L, item->getCharges());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetFluidType(lua_State* L)
{
	// item:getFluidType()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushNumber(L, item->getFluidType());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemHasSubType(lua_State* L)
{
	// item:hasSubType()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushBoolean(L, item->hasSubType());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetSubType(lua_State* L)
{
	// item:getSubType()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushNumber(L, item->getSubType());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetName(lua_State* L)
{
	// item:getName()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushString(L, item->getName());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetPluralName(lua_State* L)
{
	// item:getPluralName()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushString(L, item->getPluralName());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetArticle(lua_State* L)
{
	// item:getArticle()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushString(L, item->getArticle());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetPosition(lua_State* L)
{
	// item:getPosition()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushMetaPosition(L, item->getPosition(), 0);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetTile(lua_State* L)
{
	// item:getTile()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		Tile* tile = item->getTile();
		if (tile) {
			pushUserdata<Tile>(L, tile);
			setMetatable(L, -1, "Tile");
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetAttribute(lua_State* L)
{
	// item:getAttribute(key)
	itemAttrTypes attribute = ATTR_ITEM_NONE;
	if (isNumber(L, 2)) {
		attribute = static_cast<itemAttrTypes>(getNumber<uint64_t>(L, 2));
	} else if (isString(L, 2)) {
		attribute = stringToItemAttribute(getString(L, 2));
	}
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		if (attribute & 0x7F0013) { // All integer attributes
			pushNumber(L, item->getIntAttr(attribute));
		} else if (attribute & 0x2C) { // All string attributes
			pushString(L, item->getStrAttr(attribute));
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemSetAttribute(lua_State* L)
{
	// item:setAttribute(key, value)
	itemAttrTypes attribute = ATTR_ITEM_NONE;
	if (isNumber(L, 2)) {
		attribute = static_cast<itemAttrTypes>(getNumber<uint64_t>(L, 2));
	} else if (isString(L, 2)) {
		attribute = stringToItemAttribute(getString(L, 2));
	}
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		if (isNumber(L, 3)) {
			if (attribute & 0x7F0013) { // All integer attributes
				item->setIntAttr(attribute, getNumber<int32_t>(L, 3));
				pushBoolean(L, true);
			}
		} else if (isString(L, 3)) {
			if (attribute & 0x2C) { // All string attributes
				item->setStrAttr(attribute, getString(L, 3));
				pushBoolean(L, true);
			}
		} else {
			pushNil(L);
		}
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemRemoveAttribute(lua_State* L)
{
	// item:removeAttribute(key)
	itemAttrTypes attribute = ATTR_ITEM_NONE;
	if (isNumber(L, 2)) {
		attribute = static_cast<itemAttrTypes>(getNumber<uint64_t>(L, 2));
	} else if (isString(L, 2)) {
		attribute = stringToItemAttribute(getString(L, 2));
	}
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		bool ret = attribute != ATTR_ITEM_UNIQUEID;
		if (ret) {
			item->removeAttribute(attribute);
		} else {
			reportErrorFunc("Attempt to erase protected key \"uid\"");
		}
		pushBoolean(L, ret);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemMoveTo(lua_State* L)
{
	// item:moveTo(position)
	const Position& position = getPosition(L, 2);
	Item** itemPtr = getRawUserdata<Item>(L, 1);
	if (!itemPtr) {
		pushNil(L);
		return 1;
	}

	if (*itemPtr) {
		Item* item = *itemPtr;
		if (position == item->getPosition()) {
			pushBoolean(L, true);
		} else if (item->isRemoved()) {
			pushNil(L);
		} else {
			Tile* tile = g_game.getTile(position.x, position.y, position.z);
			if (!tile) {
				pushNil(L);
				return 1;
			}

			if (item->getParent() == VirtualCylinder::virtualCylinder) {
				pushBoolean(L, g_game.internalAddItem(tile, item) == RET_NOERROR);
			} else {
				Item* moveItem = nullptr;
				ReturnValue ret = g_game.internalMoveItem(item->getParent(), tile, INDEX_WHEREEVER, item, item->getItemCount(), &moveItem, FLAG_NOLIMIT | FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE | FLAG_IGNORENOTMOVEABLE);
				if (moveItem) {
					*itemPtr = moveItem;
				}
				pushBoolean(L, ret == RET_NOERROR);
			}
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTransform(lua_State* L)
{
	// item:transform(itemId[, count/subType = -1])
	int32_t count = -1;
	if (getStackTop(L) >= 3) {
		count = getNumber<int32_t>(L, 3);
	}

	uint16_t itemId = getNumber<uint16_t>(L, 2);
	Item** itemPtr = getRawUserdata<Item>(L, 1);
	if (!itemPtr) {
		pushNil(L);
		return 1;
	}

	if (*itemPtr) {
		Item* item = *itemPtr;
		if (item->getID() == itemId) {
			pushBoolean(L, true);
			return 1;
		}

		const ItemType& it = Item::items[itemId];
		if (it.stackable) {
			count = std::min<int32_t>(count, 100);
		}

		ScriptEnvironment* env = getScriptEnv();
		uint32_t uid = env->addThing(item);

		Item* newItem = g_game.transformItem(item, itemId, count);
		if (item->isRemoved()) {
			env->removeItemByUID(uid);
		}

		if (newItem && newItem != item) {
			env->insertThing(uid, newItem);
		}

		*itemPtr = newItem;
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

// Container
int32_t LuaScriptInterface::luaContainerCreate(lua_State* L)
{
	// Container(uid)
	// Container(itemId[, position])
	// Container.new(uid)
	// Container.new(itemId[, position])
	int32_t parameters = getStackTop(L);

	Position position;
	if (parameters >= 3) {
		position = getPosition(L, 3);
	}

	uint32_t id = getNumber<uint32_t>(L, 2);

	Item* item;
	ScriptEnvironment* env = getScriptEnv();
	if (parameters >= 3) {
		item = Item::CreateItem(id, 1);
		if (!item) {
			pushNil(L);
			return 1;
		}

		if (!item->getContainer()) {
			delete item;
			pushNil(L);
			return 1;
		}

		if (parameters >= 4) {
			Tile* tile = g_game.getTile(position.x, position.y, position.z);
			if (!tile) {
				delete item;
				pushNil(L);
				return 1;
			}

			g_game.internalAddItem(tile, item, INDEX_WHEREEVER, FLAG_NOLIMIT);
		} else {
			item->setParent(VirtualCylinder::virtualCylinder);
			env->addTempItem(env, item);
		}
	} else {
		item = env->getContainerByUID(id);
	}

	if (item) {
		pushUserdata<Container>(L, item->getContainer());
		setMetatable(L, -1, "Container");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetSize(lua_State* L)
{
	// container:getSize()
	Container* container = getUserdata<Container>(L, 1);
	if (container) {
		pushNumber(L, container->size());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetCapacity(lua_State* L)
{
	// container:getCapacity()
	Container* container = getUserdata<Container>(L, 1);
	if (container) {
		pushNumber(L, container->capacity());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetEmptySlots(lua_State* L)
{
	// container:getEmptySlots([recursive = false])
	bool recursive = false;
	if (getStackTop(L) >= 2) {
		recursive = getBoolean(L, 2);
	}
	Container* container = getUserdata<Container>(L, 1);
	if (container) {
		uint32_t slots = container->capacity() - container->size();
		if (recursive) {
			for (ContainerIterator cit = container->begin(); cit != container->end(); ++cit) {
				if (Container* tmpContainer = (*cit)->getContainer()) {
					slots += tmpContainer->capacity() - tmpContainer->size();
				}
			}
		}
		pushNumber(L, slots);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetItem(lua_State* L)
{
	// container:getItem(index)
	uint32_t index = getNumber<uint32_t>(L, 2);
	Container* container = getUserdata<Container>(L, 1);
	if (container) {
		Item* item = container->getItemByIndex(index);
		if (item) {
			pushUserdata<Item>(L, item);
			setItemMetatable(L, -1, item);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerHasItem(lua_State* L)
{
	// container:hasItem(item)
	Item* item = getUserdata<Item>(L, 2);
	Container* container = getUserdata<Container>(L, 1);
	if (container) {
		pushBoolean(L, container->isHoldingItem(item));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerAddItem(lua_State* L)
{
	// container:addItem(itemId[, count/subType = 1])
	int32_t parameters = getStackTop(L);

	uint32_t subType = 1;
	if (parameters >= 3) {
		subType = getNumber<uint32_t>(L, 3);
	}

	uint16_t itemId = getNumber<uint16_t>(L, 2);
	Container* container = getUserdata<Container>(L, 1);
	if (!container) {
		pushNil(L);
		return 1;
	}

	Item* item = Item::CreateItem(itemId, std::min<uint32_t>(subType, 100));
	if (!item) {
		pushNil(L);
		return 1;
	}

	ReturnValue ret = g_game.internalAddItem(container, item);
	if (ret != RET_NOERROR || !item->getParent()) {
		delete item;
		pushNil(L);
		return 1;
	}

	pushUserdata<Item>(L, item);
	setItemMetatable(L, -1, item);
	return 1;
}

int32_t LuaScriptInterface::luaContainerAddItemEx(lua_State* L)
{
	// container:addItemEx(item)
	Item* item = getUserdata<Item>(L, 2);
	if (!item) {
		pushNil(L);
		return 1;
	}

	Container* container = getUserdata<Container>(L, 1);
	if (container) {
		if (item->getParent() != VirtualCylinder::virtualCylinder) {
			reportErrorFunc("Item already has a parent");
			pushNil(L);
			return 1;
		}

		ReturnValue ret = g_game.internalAddItem(container, item);
		if (ret == RET_NOERROR) {
			getScriptEnv()->removeTempItem(item);
		}

		pushNumber(L, ret);
	} else {
		pushNil(L);
	}
	return 1;
}

// Creature
int32_t LuaScriptInterface::luaCreatureCreate(lua_State* L)
{
	// Creature(id/name)
	// Creature.new(id/name)
	Creature* creature = nullptr;
	if (isNumber(L, 2)) {
		creature = g_game.getCreatureByID(getNumber<uint32_t>(L, 2));
	} else if (isString(L, 2)) {
		creature = g_game.getCreatureByName(getString(L, 2));
	}

	if (creature) {
		pushUserdata<Creature>(L, creature);
		setCreatureMetatable(L, -1, creature);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureRegisterEvent(lua_State* L)
{
	// creature:registerEvent(name)
	const std::string& name = getString(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		pushBoolean(L, creature->registerCreatureEvent(name));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureUnregisterEvent(lua_State* L)
{
	// creature:unregisterEvent(name)
	const std::string& name = getString(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		pushBoolean(L, creature->unregisterCreatureEvent(name));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureIsCreature(lua_State* L)
{
	// creature:isCreature()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	pushBoolean(L, creature != nullptr);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureIsPlayer(lua_State* L)
{
	// creature:isPlayer()
	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureIsMonster(lua_State* L)
{
	// creature:isMonster()
	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureIsNpc(lua_State* L)
{
	// creature:isNpc()
	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureIsItem(lua_State* L)
{
	// creature:isItem()
	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetId(lua_State* L)
{
	// creature:getId()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushNumber(L, creature->getID());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetName(lua_State* L)
{
	// creature:getName()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushString(L, creature->getName());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetTarget(lua_State* L)
{
	// creature:getTarget()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		Creature* target = creature->getAttackedCreature();
		if (target) {
			pushUserdata<Creature>(L, target);
			setCreatureMetatable(L, -1, target);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetMaster(lua_State* L)
{
	// creature:getMaster()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		Creature* master = creature->getMaster();
		if (master) {
			pushUserdata<Creature>(L, master);
			setCreatureMetatable(L, -1, master);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetLight(lua_State* L)
{
	// creature:getLight()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		LightInfo light;
		creature->getCreatureLight(light);
		pushNumber(L, light.level);
		pushNumber(L, light.color);
		return 2;
	}
	
	pushNil(L);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetLight(lua_State* L)
{
	// creature:setLight(color, level)
	uint32_t level = getNumber<uint32_t>(L, 3);
	uint32_t color = getNumber<uint32_t>(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		LightInfo light;
		light.color = color;
		light.level = level;
		creature->setCreatureLight(light);
		g_game.changeLight(creature);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetPosition(lua_State* L)
{
	// creature:getPosition()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushMetaPosition(L, creature->getPosition(), 0);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetTile(lua_State* L)
{
	// creature:getTile()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		Tile* tile = creature->getTile();
		if (tile) {
			pushUserdata<Tile>(L, tile);
			setMetatable(L, -1, "Tile");
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetDirection(lua_State* L)
{
	// creature:getDirection()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushNumber(L, creature->getDirection());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetDirection(lua_State* L)
{
	// creature:setDirection(direction)
	Direction direction = static_cast<Direction>(getNumber<uint64_t>(L, 2));
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		pushBoolean(L, g_game.internalCreatureTurn(creature, direction));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetHealth(lua_State* L)
{
	// creature:getHealth()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushNumber(L, creature->getHealth());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureAddHealth(lua_State* L)
{
	// creature:addHealth(healthChange)
	int32_t healthChange = getNumber<int32_t>(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		if (healthChange >= 0) {
			g_game.combatChangeHealth(COMBAT_HEALING, nullptr, creature, healthChange);
		} else {
			g_game.combatChangeHealth(COMBAT_UNDEFINEDDAMAGE, nullptr, creature, healthChange);
		}
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetMaxHealth(lua_State* L)
{
	// creature:getMaxHealth()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushNumber(L, creature->getMaxHealth());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetMaxHealth(lua_State* L)
{
	// creature:setMaxHealth(maxHealth)
	uint32_t maxHealth = getNumber<uint32_t>(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		creature->healthMax = maxHealth;
		g_game.addCreatureHealth(creature);

		Player* player = creature->getPlayer();
		if (player) {
			player->sendStats();
		}
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetMana(lua_State* L)
{
	// creature:getMana()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushNumber(L, creature->getMana());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureAddMana(lua_State* L)
{
	// creature:addMana(manaChange[, animationOnLoss = false])
	bool animationOnLoss = true;
	if (getStackTop(L) >= 3) {
		animationOnLoss = getBoolean(L, 3);
	}
	int32_t manaChange = getNumber<int32_t>(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		if (!animationOnLoss && manaChange < 0) {
			creature->changeMana(manaChange);
		} else {
			g_game.combatChangeMana(nullptr, creature, manaChange);
		}
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetMaxMana(lua_State* L)
{
	// creature:getMaxMana()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushNumber(L, creature->getMaxMana());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetMaxMana(lua_State* L)
{
	// creature:setMaxMana(maxMana)
	uint32_t maxMana = getNumber<uint32_t>(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		creature->manaMax = maxMana;

		Player* player = creature->getPlayer();
		if (player) {
			player->sendStats();
		}
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetOutfit(lua_State* L)
{
	// creature:getOutfit()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushOutfit(L, creature->getCurrentOutfit());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetOutfit(lua_State* L)
{
	// creature:setOutfit(outfit)
	Outfit_t outfit = popOutfit(L);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		creature->defaultOutfit = outfit;

		g_game.internalCreatureChangeOutfit(creature, outfit);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureRemove(lua_State* L)
{
	// creature:remove()
	Creature** creaturePtr = getRawUserdata<Creature>(L, 1);
	if (!creaturePtr) {
		pushNil(L);
		return 1;
	}

	Creature* creature = *creaturePtr;
	if (creature) {
		Player* player = creature->getPlayer();
		if (player) {
			player->kickPlayer(true);
		} else {
			g_game.removeCreature(creature);
		}

		*creaturePtr = nullptr;
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureTeleportTo(lua_State* L)
{
	// creature:teleportTo(position[, pushMovement = false])
	bool pushMovement = false;
	if (getStackTop(L) >= 3) {
		pushMovement = getBoolean(L, 3);
	}
	const Position& position = getPosition(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		const Position& oldPosition = creature->getPosition();
		if (g_game.internalTeleport(creature, position, pushMovement) == RET_NOERROR) {
			if (!pushMovement) {
				if (oldPosition.x == position.x) {
					if (oldPosition.y < position.y) {
						g_game.internalCreatureTurn(creature, SOUTH);
					} else {
						g_game.internalCreatureTurn(creature, NORTH);
					}
				} else if (oldPosition.x > position.x) {
					g_game.internalCreatureTurn(creature, WEST);
				} else if (oldPosition.x < position.x) {
					g_game.internalCreatureTurn(creature, EAST);
				}
			}

			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSay(lua_State* L)
{
	// creature:say(text, type[, ghost = false[, target = nullptr[, position]]])
	int32_t parameters = getStackTop(L);

	Position position;
	if (parameters >= 6) {
		position = getPosition(L, 6);
		if (!position.x || !position.y) {
			reportErrorFunc("Invalid position specified.");
			pushBoolean(L, false);
			return 1;
		}
	}

	Creature* target = nullptr;
	if (parameters >= 5) {
		target = getCreature(L, 5);
	}

	bool ghost = false;
	if (parameters >= 4) {
		ghost = getBoolean(L, 4);
	}

	SpeakClasses type = static_cast<SpeakClasses>(getNumber<uint64_t>(L, 3));
	const std::string& text = getString(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		SpectatorVec list;
		if (target) {
			list.insert(target);
		}

		if (position.x != 0) {
			pushBoolean(L, g_game.internalCreatureSay(creature, type, text, ghost, &list, &position));
		} else {
			pushBoolean(L, g_game.internalCreatureSay(creature, type, text, ghost, &list));
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetDamageMap(lua_State* L)
{
	// creature:getDamageMap()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		lua_newtable(L);
		for (auto damageEntry : creature->damageMap) {
			pushNumber(L, damageEntry.first);
			lua_newtable(L);

			pushNumber(L, damageEntry.second.total);
			lua_setfield(L, -2, "total");
			pushNumber(L, damageEntry.second.ticks);
			lua_setfield(L, -2, "ticks");

			lua_settable(L, -3);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

// Player
int32_t LuaScriptInterface::luaPlayerCreate(lua_State* L)
{
	// Player(id/name)
	// Player.new(id/name)
	Player* player = nullptr;
	if (isNumber(L, 2)) {
		player = g_game.getPlayerByID(getNumber<uint32_t>(L, 2));
	} else if (isString(L, 2)) {
		player = g_game.getPlayerByName(getString(L, 2));
	}

	if (player) {
		pushUserdata<Player>(L, player);
		setMetatable(L, -1, "Player");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerIsPlayer(lua_State* L)
{
	// player:isPlayer()
	const Player* player = getUserdata<const Player>(L, 1);
	pushBoolean(L, player != nullptr);
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetGuid(lua_State* L)
{
	// player:getGuid()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getGUID());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetIp(lua_State* L)
{
	// player:getIp()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getIP());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetAccountType(lua_State* L)
{
	// player:getAccountType()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getAccountType());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetLastLoginSaved(lua_State* L)
{
	// player:getLastLoginSaved()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getLastLoginSaved());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetFreeCapacity(lua_State* L)
{
	// player:getFreeCapacity()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getFreeCapacity());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetDepotItems(lua_State* L)
{
	// player:getDepotItems(depotId)
	uint32_t depotId = getNumber<uint32_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		const DepotChest* depotChest = player->getDepotChest(depotId, true);
		if (depotChest) {
			pushNumber(L, depotChest->getItemHoldingCount());
		} else {
			pushBoolean(L, false);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkull(lua_State* L)
{
	// player:getSkull()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getSkull());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetSkull(lua_State* L)
{
	// player:setSkull(skull)
	Skulls_t skull = static_cast<Skulls_t>(getNumber<uint64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setSkull(skull);
		g_game.updatePlayerSkull(player);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkullTime(lua_State* L)
{
	// player:getSkullTime()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getSkullTicks());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetSkullTime(lua_State* L)
{
	// player:setSkullTime(skullTime)
	int64_t skullTime = getNumber<int64_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setSkullTicks(skullTime);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetDeathPenalty(lua_State* L)
{
	// player:getDeathPenalty()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, static_cast<uint32_t>(player->getLostPercent() * 100));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetExperience(lua_State* L)
{
	// player:getExperience()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getExperience());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddExperience(lua_State* L)
{
	// player:addExperience(experience[, useMultiplier = false[, sendText = false]])
	int32_t parameters = getStackTop(L);

	bool sendText = false;
	if (parameters >= 4) {
		sendText = getBoolean(L, 4);
	}

	bool useMultiplier = false;
	if (parameters >= 3) {
		useMultiplier = getBoolean(L, 3);
	}

	int64_t experience = getNumber<int64_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->addExperience(experience, useMultiplier, sendText);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetLevel(lua_State* L)
{
	// player:getLevel()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getLevel());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetMagicLevel(lua_State* L)
{
	// player:getMagicLevel()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getMagicLevel());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetManaSpent(lua_State* L)
{
	// player:getManaSpent()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getSpentMana());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddManaSpent(lua_State* L)
{
	// player:addManaSpent(amount[, useMultiplier = true])
	bool useMultiplier = true;
	if (getStackTop(L) >= 3) {
		useMultiplier = getBoolean(L, 3);
	}
	uint64_t amount = getNumber<uint64_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->addManaSpent(amount, useMultiplier);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkillLevel(lua_State* L)
{
	// player:getSkillLevel(skillType)
	skills_t skillType = static_cast<skills_t>(getNumber<uint64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		pushNumber(L, player->skills[skillType][SKILL_LEVEL]);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkillPercent(lua_State* L)
{
	// player:getSkillPercent(skillType)
	skills_t skillType = static_cast<skills_t>(getNumber<uint64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		pushNumber(L, player->skills[skillType][SKILL_PERCENT]);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkillTries(lua_State* L)
{
	// player:getSkillTries(skillType)
	skills_t skillType = static_cast<skills_t>(getNumber<uint64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		pushNumber(L, player->skills[skillType][SKILL_TRIES]);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddSkillTries(lua_State* L)
{
	// player:addSkillTries(skillType, tries)
	uint32_t tries = getNumber<uint32_t>(L, 3);
	skills_t skillType = static_cast<skills_t>(getNumber<uint64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->addSkillAdvance(skillType, tries);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetItemCount(lua_State* L)
{
	// player:getItemCount(itemId[, subType = -1])
	int32_t subType = -1;
	if (getStackTop(L) >= 3) {
		subType = getNumber<int32_t>(L, 3);
	}
	uint16_t itemId = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->__getItemTypeCount(itemId, subType));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetItemById(lua_State* L)
{
	// player:getItemById(itemId, deepSearch[, subType = -1])
	int32_t subType = -1;
	if (getStackTop(L) >= 4) {
		subType = getNumber<int32_t>(L, 4);
	}
	bool deepSearch = getBoolean(L, 3);
	uint16_t itemId = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		Item* item = g_game.findItemOfType(player, itemId, deepSearch, subType);
		if (item) {
			pushUserdata<Item>(L, item);
			setItemMetatable(L, -1, item);
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetVocation(lua_State* L)
{
	// player:getVocation()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushUserdata<Vocation>(L, player->getVocation());
		setMetatable(L, -1, "Vocation");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetVocation(lua_State* L)
{
	// player:setVocation(vocation)
	Vocation* vocation = getUserdata<Vocation>(L, 2);
	if (!vocation) {
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setVocation(vocation->getId());

		uint32_t promotedVocation = g_vocations.getPromotedVocation(player->getVocationId());
		if (promotedVocation == 0 && player->getVocationId() != promotedVocation) {
			player->addStorageValue(STORAGEVALUE_PROMOTION, 1);
		}

		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSex(lua_State* L)
{
	// player:getSex()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getSex());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetSex(lua_State* L)
{
	// player:setSex(newSex)
	PlayerSex_t newSex = static_cast<PlayerSex_t>(getNumber<uint64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setSex(newSex);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetTown(lua_State* L)
{
	// player:getTown()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushUserdata<Town>(L, player->getTown());
		setMetatable(L, -1, "Town");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetTown(lua_State* L)
{
	// player:setTown(town)
	Town* town = getUserdata<Town>(L, 2);
	if (!town) {
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setTown(town);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetGuild(lua_State* L)
{
	// player:getGuild()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		Guild* guild = player->getGuild();
		if (guild) {
			pushUserdata<Guild>(L, guild);
			setMetatable(L, -1, "Guild");
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetGuildLevel(lua_State* L)
{
	// player:getGuildLevel()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getGuildLevel());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetGuildNick(lua_State* L)
{
	// player:getGuildNick()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushString(L, player->getGuildNick());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetGuild(lua_State* L)
{
	// player:setGuild(guild)
	Guild* guild = getUserdata<Guild>(L, 2);
	if (!guild) {
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setGuild(guild);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetGroup(lua_State* L)
{
	// player:getGroup()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushUserdata<Group>(L, player->getGroup());
		setMetatable(L, -1, "Group");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetGroup(lua_State* L)
{
	// player:setGroup(group)
	Group* group = getUserdata<Group>(L, 2);
	if (!group) {
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setGroup(group);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetStamina(lua_State* L)
{
	// player:getStamina()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getStaminaMinutes());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSoul(lua_State* L)
{
	// player:getSoul()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getPlayerInfo(PLAYERINFO_SOUL));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddSoul(lua_State* L)
{
	// player:addSoul(soulChange)
	int32_t soulChange = getNumber<int32_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->changeSoul(soulChange);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetMaxSoul(lua_State* L)
{
	// player:getMaxSoul()
	Player* player = getUserdata<Player>(L, 1);
	if (player && player->vocation) {
		pushNumber(L, player->vocation->getSoulMax());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetBankBalance(lua_State* L)
{
	// player:getBankBalance()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->getBankBalance());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetBankBalance(lua_State* L)
{
	// player:setBankBalance(bankBalance)
	uint64_t bankBalance = getNumber<uint64_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setBankBalance(bankBalance);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetStorageValue(lua_State* L)
{
	// player:getStorageValue(key)
	uint32_t key = getNumber<uint32_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		int32_t value;
		if (player->getStorageValue(key, value)) {
			pushNumber(L, value);
		} else {
			pushNumber(L, -1);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetStorageValue(lua_State* L)
{
	// player:setStorageValue(key, value)
	int32_t value = getNumber<int32_t>(L, 3);
	uint32_t key = getNumber<uint32_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (IS_IN_KEYRANGE(key, RESERVED_RANGE)) {
		std::ostringstream ss;
		ss << "Accessing reserved range: " << key;
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	if (player) {
		player->addStorageValue(key, value);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddItem(lua_State* L)
{
	// player:addItem(itemId[, count/subType = 1[, canDropOnMap = true[, slot = SLOT_WHEREEVER]]])
	// player:addItem(itemId[, count = 1[, canDropOnMap = true[, subType = 1[, slot = SLOT_WHEREEVER]]]])
	int32_t parameters = getStackTop(L);

	slots_t slot = SLOT_WHEREEVER;
	int32_t subType = 1;
	if (parameters >= 6) {
		slot = static_cast<slots_t>(getNumber<uint64_t>(L, 6));
		subType = getNumber<int32_t>(L, 5);
	} else if (parameters >= 5) {
		slot = static_cast<slots_t>(getNumber<uint64_t>(L, 5));
	}

	bool canDropOnMap = true;
	if (parameters >= 4) {
		canDropOnMap = getBoolean(L, 4);
	}

	uint32_t count = 1;
	if (parameters >= 3) {
		count = getNumber<uint32_t>(L, 3);
	}

	uint16_t itemId = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	const ItemType& it = Item::items[itemId];

	int32_t itemCount = 1;
	if (parameters >= 4) {
		itemCount = std::max<int32_t>(1, count);
	} else if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = std::ceil(count / 100.f);
		}

		subType = count;
	} else {
		itemCount = std::max<int32_t>(1, count);
	}

	bool hasTable = itemCount > 1;
	if (hasTable) {
		lua_newtable(L);
	} else if (itemCount == 0) {
		pushNil(L);
		return 1;
	}

	for (int32_t i = 1; i <= itemCount; ++i) {
		int32_t stackCount = subType;
		if (it.stackable) {
			stackCount = std::min<int32_t>(stackCount, 100);
			subType -= stackCount;
		}

		Item* item = Item::CreateItem(itemId, stackCount);
		if (!item) {
			if (!hasTable) {
				pushNil(L);
			}
			return 1;
		}

		ReturnValue ret = g_game.internalPlayerAddItem(player, item, canDropOnMap, slot);
		if (ret != RET_NOERROR) {
			delete item;
			if (!hasTable) {
				pushNil(L);
			}
			return 1;
		}

		if (hasTable) {
			pushNumber(L, i);
			pushUserdata<Item>(L, item);
			setItemMetatable(L, -1, item);
			lua_settable(L, -3);
		} else {
			pushUserdata<Item>(L, item);
			setItemMetatable(L, -1, item);
		}
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddItemEx(lua_State* L)
{
	// player:addItemEx(item[, canDropOnMap = false[, slot = SLOT_WHEREEVER]])
	int32_t parameters = getStackTop(L);

	slots_t slot = SLOT_WHEREEVER;
	if (parameters >= 4) {
		slot = static_cast<slots_t>(getNumber<uint64_t>(L, 4));
	}

	bool canDropOnMap = false;
	if (parameters >= 3) {
		canDropOnMap = getBoolean(L, 3);
	}

	Item* item = getUserdata<Item>(L, 2);
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		if (item->getParent() != VirtualCylinder::virtualCylinder) {
			reportErrorFunc("Item already has a parent");
			pushBoolean(L, false);
			return 1;
		}

		if (canDropOnMap) {
			pushNumber(L, g_game.internalPlayerAddItem(player, item, true, slot));
		} else {
			pushNumber(L, g_game.internalAddItem(player, item));
		}
	} else {
		pushNil(L);
	}

	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveItem(lua_State* L)
{
	// player:removeItem(itemId, count[, subType = -1[, ignoreEquipped = false]])
	int32_t parameters = getStackTop(L);

	bool ignoreEquipped = false;
	if (parameters >= 5) {
		ignoreEquipped = getBoolean(L, 5);
	}

	int32_t subType = -1;
	if (parameters >= 4) {
		subType = getNumber<int32_t>(L, 4);
	}

	uint32_t count = getNumber<uint32_t>(L, 3);
	uint16_t itemId = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, player->removeItemOfType(itemId, count, subType, ignoreEquipped));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetMoney(lua_State* L)
{
	// player:getMoney()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, g_game.getMoney(player));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddMoney(lua_State* L)
{
	// player:addMoney(money)
	uint64_t money = getNumber<uint64_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		g_game.addMoney(player, money);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveMoney(lua_State* L)
{
	// player:removeMoney(money)
	uint64_t money = getNumber<uint64_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, g_game.removeMoney(player, money));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerShowTextDialog(lua_State* L)
{
	// player:showTextDialog(itemId[, text[, canWrite[, length]]])
	// player:showTextDialog(itemId[, canWrite[, length]])
	int32_t parameters = getStackTop(L);

	int32_t length = -1;
	if (parameters >= 5) {
		length = getNumber<uint32_t>(L, 5);
	}

	bool canWrite = false;
	if (parameters >= 4) {
		if (isBoolean(L, 4)) {
			canWrite = getBoolean(L, 4);
		} else {
			length = getNumber<uint32_t>(L, 4);
		}
	}

	std::string text;
	if (parameters >= 3) {
		if (isBoolean(L, 3)) {
			canWrite = getBoolean(L, 3);
		} else {
			text = getString(L, 3);
		}
	}

	uint16_t itemId = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		Item* item = Item::CreateItem(itemId);
		if (item) {
			if (length < 0) {
				length = item->getMaxWriteLength();
			}

			if (!text.empty()) {
				item->setText(text);
				length = std::max<int32_t>(text.size(), length);
			}

			item->setParent(player);
			player->setWriteItem(item, length);
			player->sendTextWindow(item, length, canWrite);
			pushBoolean(L, true);
		} else {
			reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
			pushBoolean(L, false);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSendTextMessage(lua_State* L)
{
	// player:sendTextMessage(type, text[, position, value = 0, color = TEXTCOLOR_NONE])
	int32_t parameters = getStackTop(L);

	Position position;
	uint32_t value = 0;
	TextColor_t color = TEXTCOLOR_NONE;
	if (parameters >= 6) {
		color = static_cast<TextColor_t>(getNumber<uint64_t>(L, 6));
		value = getNumber<uint32_t>(L, 5);
		position = getPosition(L, 4);
	}

	const std::string& text = getString(L, 3);
	MessageClasses type = static_cast<MessageClasses>(getNumber<uint64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		if (parameters >= 6) {
			player->sendTextMessage(type, text, &position, value, color);
		} else {
			player->sendTextMessage(type, text);
		}
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSendChannelMessage(lua_State* L)
{
	// player:sendChannelMessage(author, text, type, channelId)
	uint16_t channelId = getNumber<uint16_t>(L, 5);
	SpeakClasses type = static_cast<SpeakClasses>(getNumber<uint64_t>(L, 4));
	const std::string& text = getString(L, 3);
	const std::string& author = getString(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->sendChannelMessage(author, text, type, channelId);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerChannelSay(lua_State* L)
{
	// player:channelSay(target, type, text, channelId)
	uint16_t channelId = getNumber<uint16_t>(L, 5);
	const std::string& text = getString(L, 4);
	SpeakClasses type = static_cast<SpeakClasses>(getNumber<uint64_t>(L, 3));
	Creature* target = getCreature(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->sendToChannel(target, type, text, channelId);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerOpenChannel(lua_State* L)
{
	// player:openChannel(channelId)
	uint16_t channelId = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, g_game.playerOpenChannel(player->getID(), channelId));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSlotItem(lua_State* L)
{
	// player:getSlotItem(slot)
	uint32_t slot = getNumber<uint32_t>(L, 2);
	const Player* player = getUserdata<const Player>(L, 1);
	if (player) {
		Thing* thing = player->__getThing(slot);
		if (thing) {
			Item* item = thing->getItem();
			if (item) {
				pushUserdata<Item>(L, item);
				setItemMetatable(L, -1, item);
			} else {
				pushNil(L);
			}
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddOutfit(lua_State* L)
{
	// player:addOutfit(lookType)
	uint16_t lookType = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->addOutfit(lookType, 0);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddOutfitAddon(lua_State* L)
{
	// player:addOutfitAddon(lookType, addon)
	uint8_t addon = getNumber<uint8_t>(L, 3);
	uint16_t lookType = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->addOutfit(lookType, addon);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveOutfit(lua_State* L)
{
	// player:removeOutfit(lookType)
	uint16_t lookType = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, player->removeOutfit(lookType));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveOutfitAddon(lua_State* L)
{
	// player:removeOutfitAddon(lookType, addon)
	uint8_t addon = getNumber<uint8_t>(L, 3);
	uint16_t lookType = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, player->removeOutfitAddon(lookType, addon));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerHasOutfit(lua_State* L)
{
	// player:hasOutfit(lookType[, addon = 0])
	uint8_t addon;
	if (getStackTop(L) >= 3) {
		addon = getNumber<uint8_t>(L, 3);
	} else {
		addon = 0;
	}
	uint16_t lookType = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, player->canWear(lookType, addon));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSendOutfitWindow(lua_State* L)
{
	// player:sendOutfitWindow()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->sendOutfitWindow();
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddMount(lua_State* L)
{
	// player:addMount(mountId)
	uint8_t mountId = getNumber<uint8_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, player->tameMount(mountId));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveMount(lua_State* L)
{
	// player:removeMount(mountId)
	uint8_t mountId = getNumber<uint8_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, player->untameMount(mountId));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerHasMount(lua_State* L)
{
	// player:hasMount(mountId)
	uint8_t mountId = getNumber<uint8_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		Mount* mount = Mounts::getInstance()->getMountByID(mountId);
		if (mount) {
			pushBoolean(L, mount->isTamed(player));
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetPremiumDays(lua_State* L)
{
	// player:getPremiumDays()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushNumber(L, player->premiumDays);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddPremiumDays(lua_State* L)
{
	// player:addPremiumDays(days)
	uint16_t days = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		if (player->premiumDays != 0xFFFF) {
			int32_t addDays = std::min<int32_t>(0xFFFE - player->premiumDays, days);
			if (addDays > 0) {
				player->setPremiumDays(player->premiumDays + addDays);
				IOLoginData::getInstance()->addPremiumDays(player->getAccount(), addDays);
			}
		}
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemovePremiumDays(lua_State* L)
{
	// player:removePremiumDays(days)
	uint16_t days = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		if (player->premiumDays != 0xFFFF) {
			int32_t removeDays = std::min<int32_t>(player->premiumDays, days);
			if (removeDays > 0) {
				player->setPremiumDays(player->premiumDays - removeDays);
				IOLoginData::getInstance()->removePremiumDays(player->getAccount(), removeDays);
			}
		}
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerHasBlessing(lua_State* L)
{
	// player:hasBlessing(blessing)
	int16_t blessing = getNumber<int16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, player->hasBlessing(blessing));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddBlessing(lua_State* L)
{
	// player:addBlessing(blessing)
	int16_t blessing = getNumber<int16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		if (!player->hasBlessing(blessing)) {
			player->addBlessing(1 << blessing);
			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveBlessing(lua_State* L)
{
	// player:removeBlessing(blessing)
	int16_t blessing = getNumber<int16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		if (player->hasBlessing(blessing)) {
			player->removeBlessing(1 << blessing);
			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSave(lua_State* L)
{
	// player:save()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->loginPosition = player->getPosition();
		pushBoolean(L, IOLoginData::getInstance()->savePlayer(player));
	} else {
		pushNil(L);
	}
	return 1;
}

// Monster
int32_t LuaScriptInterface::luaMonsterCreate(lua_State* L)
{
	// Monster(id)
	// Monster.new(id)
	uint32_t id = getNumber<uint32_t>(L, 2);

	Monster* monster = g_game.getMonsterByID(id);
	if (monster) {
		pushUserdata<Monster>(L, monster);
		setMetatable(L, -1, "Monster");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterIsMonster(lua_State* L)
{
	// monster:isMonster()
	const Monster* monster = getUserdata<const Monster>(L, 1);
	pushBoolean(L, monster != nullptr);
	return 1;
}

// Npc
int32_t LuaScriptInterface::luaNpcCreate(lua_State* L)
{
	// Npc([id])
	// Npc.new([id])
	Npc* npc;
	if (getStackTop(L) >= 2) {
		npc = g_game.getNpcByID(getNumber<uint32_t>(L, 2));
	} else {
		npc = getScriptEnv()->getNpc();
	}

	if (npc) {
		pushUserdata<Npc>(L, npc);
		setMetatable(L, -1, "Npc");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNpcIsNpc(lua_State* L)
{
	// npc:isNpc()
	const Npc* npc = getUserdata<const Npc>(L, 1);
	pushBoolean(L, npc != nullptr);
	return 1;
}

// Guild
int32_t LuaScriptInterface::luaGuildCreate(lua_State* L)
{
	// Guild(id)
	// Guild.new(id)
	uint32_t id = getNumber<uint32_t>(L, 2);

	Guild* guild = g_game.getGuild(id);
	if (guild) {
		pushUserdata<Guild>(L, guild);
		setMetatable(L, -1, "Guild");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetId(lua_State* L)
{
	// guild:getId()
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		pushNumber(L, guild->getId());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetName(lua_State* L)
{
	// guild:getName()
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		pushString(L, guild->getName());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetMembersOnline(lua_State* L)
{
	// guild:getMembersOnline()
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		uint32_t i = 0;
		lua_newtable(L);
		for (Player* player : guild->getMembersOnline()) {
			pushNumber(L, ++i);
			pushUserdata<Player>(L, player);
			setMetatable(L, -1, "Player");
			lua_settable(L, -3);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildAddMember(lua_State* L)
{
	// guild:addMember(player)
	Player* player = getPlayer(L, 2);
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		if (player) {
			guild->addMember(player);
			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildRemoveMember(lua_State* L)
{
	// guild:removeMember(player)
	Player* player = getPlayer(L, 2);
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		if (player) {
			guild->removeMember(player);
			pushBoolean(L, true);
		} else {
			pushBoolean(L, false);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildAddRank(lua_State* L)
{
	// guild:addRank(id, name, level)
	uint8_t level = getNumber<uint8_t>(L, 4);
	const std::string& name = getString(L, 3);
	uint32_t id = getNumber<uint32_t>(L, 2);
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		guild->addRank(id, name, level);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetRankById(lua_State* L)
{
	// guild:getRankById(id)
	uint32_t id = getNumber<uint32_t>(L, 2);
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		GuildRank* rank = guild->getRankById(id);
		if (rank) {
			lua_newtable(L);
			pushNumber(L, rank->id);
			lua_setfield(L, -2, "id");

			pushString(L, rank->name);
			lua_setfield(L, -2, "name");

			pushNumber(L, rank->level);
			lua_setfield(L, -2, "level");
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetRankByLevel(lua_State* L)
{
	// guild:getRankByLevel(level)
	uint8_t level = getNumber<uint8_t>(L, 2);
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		GuildRank* rank = guild->getRankByLevel(level);
		if (rank) {
			lua_newtable(L);
			pushNumber(L, rank->id);
			lua_setfield(L, -2, "id");

			pushString(L, rank->name);
			lua_setfield(L, -2, "name");

			pushNumber(L, rank->level);
			lua_setfield(L, -2, "level");
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetMotd(lua_State* L)
{
	// guild:getMotd()
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		pushString(L, guild->getMotd());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildSetMotd(lua_State* L)
{
	// guild:setMotd(motd)
	const std::string& motd = getString(L, 2);
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		guild->setMotd(motd);
		pushBoolean(L, true);
	} else {
		pushNil(L);
	}
	return 1;
}

// Group
int32_t LuaScriptInterface::luaGroupCreate(lua_State* L)
{
	// Group(id)
	// Group.new(id)
	uint32_t id = getNumber<uint32_t>(L, 2);

	Group* group = g_game.getGroup(id);
	if (group) {
		pushUserdata<Group>(L, group);
		setMetatable(L, -1, "Group");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetId(lua_State* L)
{
	// group:getId()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		pushNumber(L, group->id);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetName(lua_State* L)
{
	// group:getName()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		pushString(L, group->name);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetFlags(lua_State* L)
{
	// group:getFlags()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		pushNumber(L, group->flags);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetAccess(lua_State* L)
{
	// group:getAccess()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		pushBoolean(L, group->access);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetMaxDepotItems(lua_State* L)
{
	// group:getMaxDepotItems()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		pushNumber(L, group->maxDepotItems);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetMaxVipEntries(lua_State* L)
{
	// group:getMaxVipEntries()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		pushNumber(L, group->maxVipEntries);
	} else {
		pushNil(L);
	}
	return 1;
}

// Vocation
int32_t LuaScriptInterface::luaVocationCreate(lua_State* L)
{
	// Vocation(id)
	// Vocation.new(id)
	uint32_t id = getNumber<uint32_t>(L, 2);

	Vocation* vocation = g_vocations.getVocation(id);
	if (vocation) {
		pushUserdata<Vocation>(L, vocation);
		setMetatable(L, -1, "Vocation");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetId(lua_State* L)
{
	// vocation:getId()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getId());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetClientId(lua_State* L)
{
	// vocation:getClientId()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getClientId());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetName(lua_State* L)
{
	// vocation:getName()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushString(L, vocation->getVocName());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetDescription(lua_State* L)
{
	// vocation:getDescription()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushString(L, vocation->getVocDescription());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetRequiredSkillTries(lua_State* L)
{
	// vocation:getRequiredSkillTries(skillType, skillLevel)
	int32_t skillLevel = getNumber<int32_t>(L, 3);
	skills_t skillType = static_cast<skills_t>(getNumber<uint64_t>(L, 2));
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getReqSkillTries(skillType, skillLevel));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetRequiredManaSpent(lua_State* L)
{
	// vocation:getRequiredManaSpent(magicLevel)
	uint32_t magicLevel = getNumber<uint32_t>(L, 2);
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getReqMana(magicLevel));
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetCapacityGain(lua_State* L)
{
	// vocation:getCapacityGain()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getCapGain());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetHealthGain(lua_State* L)
{
	// vocation:getHealthGain()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getHPGain());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetHealthGainTicks(lua_State* L)
{
	// vocation:getHealthGainTicks()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getHealthGainTicks());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetHealthGainAmount(lua_State* L)
{
	// vocation:getHealthGainAmount()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getHealthGainAmount());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetManaGain(lua_State* L)
{
	// vocation:getManaGain()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getManaGain());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetManaGainTicks(lua_State* L)
{
	// vocation:getManaGainTicks()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getManaGainTicks());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetManaGainAmount(lua_State* L)
{
	// vocation:getManaGainAmount()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getManaGainAmount());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetMaxSoul(lua_State* L)
{
	// vocation:getMaxSoul()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getSoulMax());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetSoulGainTicks(lua_State* L)
{
	// vocation:getSoulGainTicks()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getSoulGainTicks());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetAttackSpeed(lua_State* L)
{
	// vocation:getAttackSpeed()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getAttackSpeed());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetBaseSpeed(lua_State* L)
{
	// vocation:getBaseSpeed()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		pushNumber(L, vocation->getBaseSpeed());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetDemotion(lua_State* L)
{
	// vocation:getDemotion()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		Vocation* demotedVocation = g_vocations.getVocation(vocation->getFromVocation());
		if (demotedVocation && demotedVocation != vocation) {
			pushUserdata<Vocation>(L, demotedVocation);
			setMetatable(L, -1, "Vocation");
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetPromotion(lua_State* L)
{
	// vocation:getPromotion()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		Vocation* promotedVocation = g_vocations.getVocation(g_vocations.getPromotedVocation(vocation->getId()));
		if (promotedVocation && promotedVocation != vocation) {
			pushUserdata<Vocation>(L, promotedVocation);
			setMetatable(L, -1, "Vocation");
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

// Town
int32_t LuaScriptInterface::luaTownCreate(lua_State* L)
{
	// Town(id or name)
	// Town.new(id or name)
	Town* town;
	if (isNumber(L, 2)) {
		town = Towns::getInstance().getTown(getNumber<uint32_t>(L, 2));
	} else if (isString(L, 2)) {
		town = Towns::getInstance().getTown(getString(L, 2));
	} else {
		town = nullptr;
	}

	if (town) {
		pushUserdata<Town>(L, town);
		setMetatable(L, -1, "Town");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTownGetId(lua_State* L)
{
	// town:getId()
	Town* town = getUserdata<Town>(L, 1);
	if (town) {
		pushNumber(L, town->getID());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTownGetName(lua_State* L)
{
	// town:getName()
	Town* town = getUserdata<Town>(L, 1);
	if (town) {
		pushString(L, town->getName());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTownGetTemplePosition(lua_State* L)
{
	// town:getTemplePosition()
	Town* town = getUserdata<Town>(L, 1);
	if (town) {
		pushMetaPosition(L, town->getTemplePosition(), 0);
	} else {
		pushNil(L);
	}
	return 1;
}

// House
int32_t LuaScriptInterface::luaHouseCreate(lua_State* L)
{
	// House(id)
	// House.new(id)
	House* house = Houses::getInstance().getHouse(getNumber<uint32_t>(L, 2));
	if (house) {
		pushUserdata<House>(L, house);
		setMetatable(L, -1, "House");
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetId(lua_State* L)
{
	// house:getId()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		pushNumber(L, house->getHouseId());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetName(lua_State* L)
{
	// house:getName()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		pushString(L, house->getName());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetTown(lua_State* L)
{
	// house:getTown()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		Town* town = Towns::getInstance().getTown(house->getTownId());
		if (town) {
			pushUserdata<Town>(L, town);
			setMetatable(L, -1, "Town");
		} else {
			pushNil(L);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetOwnerGuid(lua_State* L)
{
	// house:getOwnerGuid()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		pushNumber(L, house->getHouseOwner());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetExitPosition(lua_State* L)
{
	// house:getExitPosition()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		pushMetaPosition(L, house->getEntryPosition(), 0);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetBeds(lua_State* L)
{
	// house:getBeds()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		uint32_t i = 0;
		lua_newtable(L);
		for (BedItem* bedItem : house->getBeds()) {
			pushNumber(L, ++i);
			pushUserdata<Item>(L, bedItem);
			setItemMetatable(L, -1, bedItem);
			lua_settable(L, -3);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetBedCount(lua_State* L)
{
	// house:getBedCount()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		pushNumber(L, house->getBedCount());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetDoors(lua_State* L)
{
	// house:getDoors()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		uint32_t i = 0;
		lua_newtable(L);
		for (Door* door : house->getDoors()) {
			pushNumber(L, ++i);
			pushUserdata<Item>(L, door);
			setItemMetatable(L, -1, door);
			lua_settable(L, -3);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetDoorCount(lua_State* L)
{
	// house:getDoorCount()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		pushNumber(L, house->getDoors().size());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetTiles(lua_State* L)
{
	// house:getTiles()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		uint32_t i = 0;
		lua_newtable(L);
		for (Tile* tile : house->getTiles()) {
			pushNumber(L, ++i);
			pushUserdata<Tile>(L, tile);
			setMetatable(L, -1, "Tile");
			lua_settable(L, -3);
		}
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetTileCount(lua_State* L)
{
	// house:getTileCount()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		pushNumber(L, house->getTiles().size());
	} else {
		pushNil(L);
	}
	return 1;
}

// ItemType
int32_t LuaScriptInterface::luaItemTypeCreate(lua_State* L)
{
	// ItemType(id or name)
	// ItemType.new(id or name)
	uint32_t id = 0;
	if (isNumber(L, 2)) {
		id = getNumber<uint32_t>(L, 2);
	} else if (isString(L, 2)) {
		id = Item::items.getItemIdByName(getString(L, 2));
	}

	const ItemType& itemType = Item::items[id];
	pushUserdata<const ItemType>(L, &itemType);
	setMetatable(L, -1, "ItemType");
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeIsCorpse(lua_State* L)
{
	// itemType:isCorpse()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->corpseType != RACE_NONE);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeIsDoor(lua_State* L)
{
	// itemType:isDoor()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->isDoor());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeIsContainer(lua_State* L)
{
	// itemType:isContainer()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->isContainer());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeIsFluidContainer(lua_State* L)
{
	// itemType:isFluidContainer()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->isFluidContainer());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeIsMovable(lua_State* L)
{
	// itemType:isMovable()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->moveable);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeIsRune(lua_State* L)
{
	// itemType:isRune()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->isRune());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeIsStackable(lua_State* L)
{
	// itemType:isStackable()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->stackable);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetType(lua_State* L)
{
	// itemType:getType()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushNumber(L, itemType->type);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetId(lua_State* L)
{
	// itemType:getId()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushNumber(L, itemType->id);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetName(lua_State* L)
{
	// itemType:getName()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushString(L, itemType->name);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetPluralName(lua_State* L)
{
	// itemType:getPluralName()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushString(L, itemType->getPluralName());
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetArticle(lua_State* L)
{
	// itemType:getArticle()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushString(L, itemType->article);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetDescription(lua_State* L)
{
	// itemType:getDescription()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushString(L, itemType->description);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetFluidSource(lua_State* L)
{
	// itemType:getFluidSource()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushNumber(L, itemType->fluidSource);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetCapacity(lua_State* L)
{
	// itemType:getCapacity()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushNumber(L, itemType->maxItems);
	} else {
		pushNil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetWeight(lua_State* L)
{
	// itemType:getWeight([count = 1[, precise = true]])
	int32_t parameters = getStackTop(L);

	bool precise = true;
	if (parameters >= 3) {
		precise = getBoolean(L, 3);
	}

	uint16_t count = 1;
	if (parameters >= 2) {
		count = getNumber<uint16_t>(L, 2);
	}

	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		double weight = itemType->weight * std::max<int32_t>(1, count);
		if (precise) {
			std::ostringstream ws;
			ws << std::fixed << std::setprecision(2) << weight;
			weight = atof(ws.str().c_str());
		}
		pushNumber(L, weight);
	} else {
		pushNil(L);
	}
	return 1;
}

//
LuaEnvironment::LuaEnvironment() :
	LuaScriptInterface("Main Interface")
{
	m_lastEventTimerId = 1000;
}

LuaEnvironment::~LuaEnvironment()
{
	closeState();
}

bool LuaEnvironment::reInitState()
{
	// TODO: get children, reload children
	closeState();
	return initState();
}

bool LuaEnvironment::initState()
{
	m_luaState = luaL_newstate();
	if (!m_luaState) {
		return false;
	}

	luaL_openlibs(m_luaState);
	registerFunctions();

	if (loadFile("data/global.lua") == -1) {
		std::cout << "Warning: [LuaEnvironment::initState] Can not load data/global.lua." << std::endl;
	}

	m_runningEventId = EVENT_ID_USER;
	return true;
}

bool LuaEnvironment::closeState()
{
	if (!m_luaState) {
		return false;
	}

	for (auto timerEntry : m_timerEvents) {
		const LuaTimerEventDesc& timerEventDesc = timerEntry.second;
		for (auto parameter : timerEventDesc.parameters) {
			luaL_unref(m_luaState, LUA_REGISTRYINDEX, parameter);
		}
		luaL_unref(m_luaState, LUA_REGISTRYINDEX, timerEventDesc.function);
	}

	m_timerEvents.clear();
	m_cacheFiles.clear();

	lua_close(m_luaState);
	m_luaState = nullptr;
	return true;
}

void LuaEnvironment::executeTimerEvent(uint32_t eventIndex)
{
	auto it = m_timerEvents.find(eventIndex);
	if (it == m_timerEvents.end()) {
		return;
	}

	const LuaTimerEventDesc& timerEventDesc = it->second;

	//push function
	lua_rawgeti(m_luaState, LUA_REGISTRYINDEX, timerEventDesc.function);

	//push parameters
	for (auto parameter : boost::adaptors::reverse(timerEventDesc.parameters)) {
		lua_rawgeti(m_luaState, LUA_REGISTRYINDEX, parameter);
	}

	//call the function
	if (reserveScriptEnv()) {
		ScriptEnvironment* env = getScriptEnv();
		env->setTimerEvent();
		env->setScriptId(timerEventDesc.scriptId, this);
		callFunction(timerEventDesc.parameters.size());
	} else {
		std::cout << "[Error - LuaScriptInterface::executeTimerEvent] Call stack overflow" << std::endl;
	}

	//free resources
	luaL_unref(m_luaState, LUA_REGISTRYINDEX, timerEventDesc.function);
	for (auto parameter : timerEventDesc.parameters) {
		luaL_unref(m_luaState, LUA_REGISTRYINDEX, parameter);
	}

	m_timerEvents.erase(it);
}
