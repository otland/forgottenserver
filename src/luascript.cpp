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

#include <boost/range/adaptor/reversed.hpp>

#include "luascript.h"
#include "chat.h"
#include "player.h"
#include "item.h"
#include "game.h"
#include "house.h"
#include "housetile.h"
#include "protocolstatus.h"
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
#include "monster.h"
#include "scheduler.h"
#include "raids.h"

extern Chat g_chat;
extern Game g_game;
extern Monsters g_monsters;
extern ConfigManager g_config;
extern Vocations g_vocations;
extern Spells* g_spells;

enum {
	EVENT_ID_LOADING = 1,
	EVENT_ID_USER = 1000,
};

ScriptEnvironment::ThingMap ScriptEnvironment::m_globalMap;

ScriptEnvironment::DBResultMap ScriptEnvironment::m_tempResults;
uint32_t ScriptEnvironment::m_lastResultId = 0;

ScriptEnvironment::TempItemListMap ScriptEnvironment::m_tempItems;

LuaEnvironment g_luaEnvironment;

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
	m_tempResults.clear();
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
	if (!item) {
		return;
	}

	uint16_t uniqueId = item->getUniqueId();
	if (uniqueId != 0) {
		auto it = m_globalMap.find(uniqueId);
		if (it == m_globalMap.end()) {
			m_globalMap[uniqueId] = thing;
		} else {
			std::cout << "Duplicate uniqueId " << uniqueId << std::endl;
		}
	}
}

void ScriptEnvironment::removeUniqueThing(Thing* thing)
{
	Item* item = thing->getItem();
	if (!item) {
		return;
	}

	uint16_t uniqueId = item->getUniqueId();
	if (uniqueId != 0) {
		auto it = m_globalMap.find(uniqueId);
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

	m_localMap[++m_lastUID] = thing;
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
		return g_game.getCreatureByID(uid);
	}

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

uint32_t ScriptEnvironment::addResult(DBResult_ptr res)
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

	m_tempResults.erase(it);
	return true;
}

DBResult_ptr ScriptEnvironment::getResultByID(uint32_t id)
{
	auto it = m_tempResults.find(id);
	if (it == m_tempResults.end()) {
		return nullptr;
	}
	return it->second;
}

std::string LuaScriptInterface::getErrorDesc(ErrorCode_t code)
{
	switch (code) {
		case LUA_ERROR_PLAYER_NOT_FOUND: return "Player not found";
		case LUA_ERROR_CREATURE_NOT_FOUND: return "Creature not found";
		case LUA_ERROR_ITEM_NOT_FOUND: return "Item not found";
		case LUA_ERROR_THING_NOT_FOUND: return "Thing not found";
		case LUA_ERROR_TILE_NOT_FOUND: return "Tile not found";
		case LUA_ERROR_HOUSE_NOT_FOUND: return "House not found";
		case LUA_ERROR_COMBAT_NOT_FOUND: return "Combat not found";
		case LUA_ERROR_CONDITION_NOT_FOUND: return "Condition not found";
		case LUA_ERROR_AREA_NOT_FOUND: return "Area not found";
		case LUA_ERROR_CONTAINER_NOT_FOUND: return "Container not found";
		case LUA_ERROR_VARIANT_NOT_FOUND: return "Variant not found";
		case LUA_ERROR_VARIANT_UNKNOWN: return "Unknown variant type";
		case LUA_ERROR_SPELL_NOT_FOUND: return "Spell not found";
		default: return "Bad error code";
	}
}

ScriptEnvironment LuaScriptInterface::m_scriptEnv[16];
int32_t LuaScriptInterface::m_scriptEnvIndex = -1;

LuaScriptInterface::LuaScriptInterface(const std::string& interfaceName)
{
	if (interfaceName != "Main Interface" && !g_luaEnvironment.getLuaState()) {
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
	g_luaEnvironment.clearCombatObjects(this);
	g_luaEnvironment.clearConditionObjects(this);
	g_luaEnvironment.clearAreaObjects(this);

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
	lua_pushvalue(m_luaState, -1);
	lua_rawseti(m_luaState, -3, m_runningEventId);
	lua_pop(m_luaState, 2);

	//reset global value of this event
	lua_pushnil(m_luaState);
	lua_setglobal(m_luaState, eventName.c_str());

	m_cacheFiles[m_runningEventId] = m_loadingFile + ":" + eventName;
	return m_runningEventId++;
}

int32_t LuaScriptInterface::getMetaEvent(const std::string& globalName, const std::string& eventName)
{
	//get our events table
	lua_rawgeti(m_luaState, LUA_REGISTRYINDEX, m_eventTableRef);
	if (!isTable(m_luaState, -1)) {
		lua_pop(m_luaState, 1);
		return -1;
	}

	//get current event function pointer
	lua_getglobal(m_luaState, globalName.c_str());
	lua_getfield(m_luaState, -1, eventName.c_str());
	if (!isFunction(m_luaState, -1)) {
		lua_pop(m_luaState, 3);
		return -1;
	}

	//save in our events table
	lua_pushvalue(m_luaState, -1);
	lua_rawseti(m_luaState, -4, m_runningEventId);
	lua_pop(m_luaState, 1);

	//reset global value of this event
	lua_pushnil(m_luaState);
	lua_setfield(m_luaState, -2, eventName.c_str());
	lua_pop(m_luaState, 2);

	m_cacheFiles[m_runningEventId] = m_loadingFile + ":" + globalName + "@" + eventName;
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

	lua_rawgeti(m_luaState, -1, functionId);
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
	if (!g_luaEnvironment.getLuaState() || !m_luaState) {
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

bool LuaScriptInterface::callFunction(int32_t params)
{
	bool result = false;
	int32_t size = lua_gettop(m_luaState);
	if (protectedCall(m_luaState, params, 1) != 0) {
		LuaScriptInterface::reportError(nullptr, LuaScriptInterface::getString(m_luaState, -1));
	} else {
		result = LuaScriptInterface::getBoolean(m_luaState, -1);
	}

	lua_pop(m_luaState, 1);
	if ((lua_gettop(m_luaState) + params + 1) != size) {
		LuaScriptInterface::reportError(nullptr, "Stack size changed!");
	}

	resetScriptEnv();
	return result;
}

void LuaScriptInterface::callVoidFunction(int32_t params)
{
	int32_t size = lua_gettop(m_luaState);
	if (protectedCall(m_luaState, params, 0) != 0) {
		LuaScriptInterface::reportError(nullptr, LuaScriptInterface::getString(m_luaState, -1));
		lua_pop(m_luaState, 1);
	}

	if ((lua_gettop(m_luaState) + params + 1) != size) {
		LuaScriptInterface::reportError(nullptr, "Stack size changed!");
	}

	resetScriptEnv();
}

void LuaScriptInterface::pushVariant(lua_State* L, const LuaVariant& var)
{
	lua_createtable(L, 0, 2);
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
			pushPosition(L, var.pos, var.pos.stackpos);
			lua_setfield(L, -2, "pos");
			break;
		}
		default:
			break;
	}
}

void LuaScriptInterface::pushThing(lua_State* L, Thing* thing, uint32_t uid)
{
	lua_createtable(L, 0, 4);

	if (thing && thing->getItem()) {
		const Item* item = thing->getItem();
		setField(L, "uid", uid);
		setField(L, "itemid", item->getID());

		const ItemType& it = Item::items[item->getID()];
		if (it.hasSubType()) {
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

void LuaScriptInterface::pushString(lua_State* L, const std::string& value)
{
	lua_pushlstring(L, value.c_str(), value.length());
}

void LuaScriptInterface::pushCallback(lua_State* L, int32_t callback)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, callback);
}

std::string LuaScriptInterface::popString(lua_State* L)
{
	if (lua_gettop(L) == 0) {
		return std::string();
	}

	std::string str(getString(L, -1));
	lua_pop(L, 1);
	return str;
}

int32_t LuaScriptInterface::popCallback(lua_State* L)
{
	return luaL_ref(L, LUA_REGISTRYINDEX);
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
	size_t len;
	const char* c_str = lua_tolstring(L, arg, &len);
	if (!c_str || len == 0) {
		return std::string();
	}
	return std::string(c_str, len);
}

Position LuaScriptInterface::getPosition(lua_State* L, int32_t arg, int32_t& stackpos)
{
	Position position;
	position.x = getField<uint16_t>(L, arg, "x");
	position.y = getField<uint16_t>(L, arg, "y");
	position.z = getField<uint8_t>(L, arg, "z");

	lua_getfield(L, arg, "stackpos");
	if (lua_isnil(L, -1) == 1) {
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

Outfit_t LuaScriptInterface::getOutfit(lua_State* L, int32_t arg)
{
	Outfit_t outfit;
	outfit.lookMount = getField<uint16_t>(L, arg, "lookMount");
	outfit.lookAddons = getField<uint8_t>(L, arg, "lookAddons");

	outfit.lookFeet = getField<uint8_t>(L, arg, "lookFeet");
	outfit.lookLegs = getField<uint8_t>(L, arg, "lookLegs");
	outfit.lookBody = getField<uint8_t>(L, arg, "lookBody");
	outfit.lookHead = getField<uint8_t>(L, arg, "lookHead");

	outfit.lookTypeEx = getField<uint16_t>(L, arg, "lookTypeEx");
	outfit.lookType = getField<uint16_t>(L, arg, "lookType");

	lua_pop(L, 8);
	return outfit;
}

LuaVariant LuaScriptInterface::getVariant(lua_State* L, int32_t arg)
{
	LuaVariant var;
	switch (var.type = static_cast<LuaVariantType_t>(getField<int64_t>(L, arg, "type"))) {
		case VARIANT_NUMBER: {
			var.number = getField<uint32_t>(L, arg, "number");
			lua_pop(L, 2);
			break;
		}

		case VARIANT_STRING: {
			var.text = getFieldString(L, arg, "string");
			lua_pop(L, 2);
			break;
		}

		case VARIANT_POSITION:
		case VARIANT_TARGETPOSITION: {
			lua_getfield(L, arg, "pos");
			var.pos = getPosition(L, lua_gettop(L));
			lua_pop(L, 2);
			break;
		}

		default: {
			var.type = VARIANT_NONE;
			lua_pop(L, 1);
			break;
		}
	}
	return var;
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

std::string LuaScriptInterface::getFieldString(lua_State* L, int32_t arg, const std::string& key)
{
	lua_getfield(L, arg, key.c_str());
	return getString(L, -1);
}

// Push
void LuaScriptInterface::pushBoolean(lua_State* L, bool value)
{
	lua_pushboolean(L, value ? 1 : 0);
}

void LuaScriptInterface::pushPosition(lua_State* L, const Position& position, int32_t stackpos/* = 0*/)
{
	lua_createtable(L, 0, 4);

	setField(L, "x", position.x);
	setField(L, "y", position.y);
	setField(L, "z", position.z);
	setField(L, "stackpos", stackpos);

	setMetatable(L, -1, "Position");
}

void LuaScriptInterface::pushOutfit(lua_State* L, const Outfit_t& outfit)
{
	lua_createtable(L, 0, 8);
	setField(L, "lookType", outfit.lookType);
	setField(L, "lookTypeEx", outfit.lookTypeEx);
	setField(L, "lookHead", outfit.lookHead);
	setField(L, "lookBody", outfit.lookBody);
	setField(L, "lookLegs", outfit.lookLegs);
	setField(L, "lookFeet", outfit.lookFeet);
	setField(L, "lookAddons", outfit.lookAddons);
	setField(L, "lookMount", outfit.lookMount);
}

std::string LuaScriptInterface::popFieldString(lua_State* L, const std::string& key)
{
	lua_getfield(L, -1, key.c_str());
	return popString(L);
}

#define registerEnum(value) { std::string enumName = #value; registerGlobalVariable(enumName.substr(enumName.find_last_of(':') + 1), value); }
#define registerEnumIn(tableName, value) { std::string enumName = #value; registerVariable(tableName, enumName.substr(enumName.find_last_of(':') + 1), value); }

void LuaScriptInterface::registerFunctions()
{
	//getPlayerFlagValue(cid, flag)
	lua_register(m_luaState, "getPlayerFlagValue", LuaScriptInterface::luaGetPlayerFlagValue);

	//getPlayerInstantSpellCount(cid)
	lua_register(m_luaState, "getPlayerInstantSpellCount", LuaScriptInterface::luaGetPlayerInstantSpellCount);

	//getPlayerInstantSpellInfo(cid, index)
	lua_register(m_luaState, "getPlayerInstantSpellInfo", LuaScriptInterface::luaGetPlayerInstantSpellInfo);

	//getThingfromPos(pos)
	lua_register(m_luaState, "getThingfromPos", LuaScriptInterface::luaGetThingfromPos);

	//getThingPos(uid)
	lua_register(m_luaState, "getThingPos", LuaScriptInterface::luaGetThingPos);

	//doPlayerAddItem(uid, itemid, <optional: default: 1> count/subtype)
	//doPlayerAddItem(cid, itemid, <optional: default: 1> count, <optional: default: 1> canDropOnMap, <optional: default: 1>subtype)
	//Returns uid of the created item
	lua_register(m_luaState, "doPlayerAddItem", LuaScriptInterface::luaDoPlayerAddItem);

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

	//doAddCondition(cid, condition)
	lua_register(m_luaState, "doAddCondition", LuaScriptInterface::luaDoAddCondition);

	//doRemoveCondition(cid, type[, subId])
	lua_register(m_luaState, "doRemoveCondition", LuaScriptInterface::luaDoRemoveCondition);

	//doMoveCreature(cid, direction)
	lua_register(m_luaState, "doMoveCreature", LuaScriptInterface::luaDoMoveCreature);

	//doPlayerRemoveItem(cid, itemid, count, <optional> subtype, <optional> ignoreEquipped)
	lua_register(m_luaState, "doPlayerRemoveItem", LuaScriptInterface::luaDoPlayerRemoveItem);

	//doSetCreatureLight(cid, lightLevel, lightColor, time)
	lua_register(m_luaState, "doSetCreatureLight", LuaScriptInterface::luaDoSetCreatureLight);

	//getSpectators(centerPos, rangex, rangey, multifloor, onlyPlayers)
	lua_register(m_luaState, "getSpectators", LuaScriptInterface::luaGetSpectators);

	//getCreatureCondition(cid, condition[, subId])
	lua_register(m_luaState, "getCreatureCondition", LuaScriptInterface::luaGetCreatureCondition);

	//isValidUID(uid)
	lua_register(m_luaState, "isValidUID", LuaScriptInterface::luaIsValidUID);

	//isDepot(uid)
	lua_register(m_luaState, "isDepot", LuaScriptInterface::luaIsDepot);

	//isMovable(uid)
	lua_register(m_luaState, "isMovable", LuaScriptInterface::luaIsMoveable);

	//doAddContainerItem(uid, itemid, <optional> count/subtype)
	lua_register(m_luaState, "doAddContainerItem", LuaScriptInterface::luaDoAddContainerItem);

	//getDepotId(uid)
	lua_register(m_luaState, "getDepotId", LuaScriptInterface::luaGetDepotId);

	//getHouseByPlayerGUID(playerGUID)
	lua_register(m_luaState, "getHouseByPlayerGUID", LuaScriptInterface::luaGetHouseByPlayerGUID);

	//getWorldTime()
	lua_register(m_luaState, "getWorldTime", LuaScriptInterface::luaGetWorldTime);

	//getWorldLight()
	lua_register(m_luaState, "getWorldLight", LuaScriptInterface::luaGetWorldLight);

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

	//hasProperty(uid, prop)
	lua_register(m_luaState, "hasProperty", LuaScriptInterface::luaHasProperty);

	//isInArray(array, value)
	lua_register(m_luaState, "isInArray", LuaScriptInterface::luaIsInArray);

	//addEvent(callback, delay, ...)
	lua_register(m_luaState, "addEvent", LuaScriptInterface::luaAddEvent);

	//stopEvent(eventid)
	lua_register(m_luaState, "stopEvent", LuaScriptInterface::luaStopEvent);

	//saveServer()
	lua_register(m_luaState, "saveServer", LuaScriptInterface::luaSaveServer);

	//cleanMap()
	lua_register(m_luaState, "cleanMap", LuaScriptInterface::luaCleanMap);

	//debugPrint(text)
	lua_register(m_luaState, "debugPrint", LuaScriptInterface::luaDebugPrint);

	//isInWar(cid, target)
	lua_register(m_luaState, "isInWar", LuaScriptInterface::luaIsInWar);

	//doPlayerSetOfflineTrainingSkill(cid, skill)
	lua_register(m_luaState, "doPlayerSetOfflineTrainingSkill", LuaScriptInterface::luaDoPlayerSetOfflineTrainingSkill);

	//getWaypointPosition(name)
	lua_register(m_luaState, "getWaypointPositionByName", LuaScriptInterface::luaGetWaypointPositionByName);

	//sendChannelMessage(channelId, type, message)
	lua_register(m_luaState, "sendChannelMessage", LuaScriptInterface::luaSendChannelMessage);

	//sendGuildChannelMessage(guildId, type, message)
	lua_register(m_luaState, "sendGuildChannelMessage", LuaScriptInterface::luaSendGuildChannelMessage);

#ifndef LUAJIT_VERSION
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
	//registerTable(tableName)
	//registerMethod(className, functionName, function)
	//registerMetaMethod(className, functionName, function)
	//registerGlobalMethod(functionName, function)
	//registerVariable(tableName, name, value)
	//registerGlobalVariable(name, value)
	//registerEnum(value)
	//registerEnumIn(tableName, value)

	// Enums
	registerEnum(ACCOUNT_TYPE_NORMAL)
	registerEnum(ACCOUNT_TYPE_TUTOR)
	registerEnum(ACCOUNT_TYPE_SENIORTUTOR)
	registerEnum(ACCOUNT_TYPE_GAMEMASTER)
	registerEnum(ACCOUNT_TYPE_GOD)

	registerEnum(CALLBACK_PARAM_LEVELMAGICVALUE)
	registerEnum(CALLBACK_PARAM_SKILLVALUE)
	registerEnum(CALLBACK_PARAM_TARGETTILE)
	registerEnum(CALLBACK_PARAM_TARGETCREATURE)

	registerEnum(COMBAT_FORMULA_UNDEFINED)
	registerEnum(COMBAT_FORMULA_LEVELMAGIC)
	registerEnum(COMBAT_FORMULA_SKILL)
	registerEnum(COMBAT_FORMULA_DAMAGE)

	registerEnum(NORTH)
	registerEnum(EAST)
	registerEnum(SOUTH)
	registerEnum(WEST)
	registerEnum(SOUTHWEST)
	registerEnum(SOUTHEAST)
	registerEnum(NORTHWEST)
	registerEnum(NORTHEAST)

	registerEnum(COMBAT_NONE)
	registerEnum(COMBAT_PHYSICALDAMAGE)
	registerEnum(COMBAT_ENERGYDAMAGE)
	registerEnum(COMBAT_EARTHDAMAGE)
	registerEnum(COMBAT_FIREDAMAGE)
	registerEnum(COMBAT_UNDEFINEDDAMAGE)
	registerEnum(COMBAT_LIFEDRAIN)
	registerEnum(COMBAT_MANADRAIN)
	registerEnum(COMBAT_HEALING)
	registerEnum(COMBAT_DROWNDAMAGE)
	registerEnum(COMBAT_ICEDAMAGE)
	registerEnum(COMBAT_HOLYDAMAGE)
	registerEnum(COMBAT_DEATHDAMAGE)

	registerEnum(COMBAT_PARAM_TYPE)
	registerEnum(COMBAT_PARAM_EFFECT)
	registerEnum(COMBAT_PARAM_DISTANCEEFFECT)
	registerEnum(COMBAT_PARAM_BLOCKSHIELD)
	registerEnum(COMBAT_PARAM_BLOCKARMOR)
	registerEnum(COMBAT_PARAM_TARGETCASTERORTOPMOST)
	registerEnum(COMBAT_PARAM_CREATEITEM)
	registerEnum(COMBAT_PARAM_AGGRESSIVE)
	registerEnum(COMBAT_PARAM_DISPEL)
	registerEnum(COMBAT_PARAM_USECHARGES)

	registerEnum(CONDITION_NONE)
	registerEnum(CONDITION_POISON)
	registerEnum(CONDITION_FIRE)
	registerEnum(CONDITION_ENERGY)
	registerEnum(CONDITION_BLEEDING)
	registerEnum(CONDITION_HASTE)
	registerEnum(CONDITION_PARALYZE)
	registerEnum(CONDITION_OUTFIT)
	registerEnum(CONDITION_INVISIBLE)
	registerEnum(CONDITION_LIGHT)
	registerEnum(CONDITION_MANASHIELD)
	registerEnum(CONDITION_INFIGHT)
	registerEnum(CONDITION_DRUNK)
	registerEnum(CONDITION_EXHAUST_WEAPON)
	registerEnum(CONDITION_REGENERATION)
	registerEnum(CONDITION_SOUL)
	registerEnum(CONDITION_DROWN)
	registerEnum(CONDITION_MUTED)
	registerEnum(CONDITION_CHANNELMUTEDTICKS)
	registerEnum(CONDITION_YELLTICKS)
	registerEnum(CONDITION_ATTRIBUTES)
	registerEnum(CONDITION_FREEZING)
	registerEnum(CONDITION_DAZZLED)
	registerEnum(CONDITION_CURSED)
	registerEnum(CONDITION_EXHAUST_COMBAT)
	registerEnum(CONDITION_EXHAUST_HEAL)
	registerEnum(CONDITION_PACIFIED)
	registerEnum(CONDITION_SPELLCOOLDOWN)
	registerEnum(CONDITION_SPELLGROUPCOOLDOWN)

	registerEnum(CONDITIONID_DEFAULT)
	registerEnum(CONDITIONID_COMBAT)
	registerEnum(CONDITIONID_HEAD)
	registerEnum(CONDITIONID_NECKLACE)
	registerEnum(CONDITIONID_BACKPACK)
	registerEnum(CONDITIONID_ARMOR)
	registerEnum(CONDITIONID_RIGHT)
	registerEnum(CONDITIONID_LEFT)
	registerEnum(CONDITIONID_LEGS)
	registerEnum(CONDITIONID_FEET)
	registerEnum(CONDITIONID_RING)
	registerEnum(CONDITIONID_AMMO)

	registerEnum(CONDITION_PARAM_OWNER)
	registerEnum(CONDITION_PARAM_TICKS)
	registerEnum(CONDITION_PARAM_HEALTHGAIN)
	registerEnum(CONDITION_PARAM_HEALTHTICKS)
	registerEnum(CONDITION_PARAM_MANAGAIN)
	registerEnum(CONDITION_PARAM_MANATICKS)
	registerEnum(CONDITION_PARAM_DELAYED)
	registerEnum(CONDITION_PARAM_SPEED)
	registerEnum(CONDITION_PARAM_LIGHT_LEVEL)
	registerEnum(CONDITION_PARAM_LIGHT_COLOR)
	registerEnum(CONDITION_PARAM_SOULGAIN)
	registerEnum(CONDITION_PARAM_SOULTICKS)
	registerEnum(CONDITION_PARAM_MINVALUE)
	registerEnum(CONDITION_PARAM_MAXVALUE)
	registerEnum(CONDITION_PARAM_STARTVALUE)
	registerEnum(CONDITION_PARAM_TICKINTERVAL)
	registerEnum(CONDITION_PARAM_FORCEUPDATE)
	registerEnum(CONDITION_PARAM_SKILL_MELEE)
	registerEnum(CONDITION_PARAM_SKILL_FIST)
	registerEnum(CONDITION_PARAM_SKILL_CLUB)
	registerEnum(CONDITION_PARAM_SKILL_SWORD)
	registerEnum(CONDITION_PARAM_SKILL_AXE)
	registerEnum(CONDITION_PARAM_SKILL_DISTANCE)
	registerEnum(CONDITION_PARAM_SKILL_SHIELD)
	registerEnum(CONDITION_PARAM_SKILL_FISHING)
	registerEnum(CONDITION_PARAM_STAT_MAXHITPOINTS)
	registerEnum(CONDITION_PARAM_STAT_MAXMANAPOINTS)
	registerEnum(CONDITION_PARAM_STAT_SOULPOINTS)
	registerEnum(CONDITION_PARAM_STAT_MAGICPOINTS)
	registerEnum(CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT)
	registerEnum(CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT)
	registerEnum(CONDITION_PARAM_STAT_SOULPOINTSPERCENT)
	registerEnum(CONDITION_PARAM_STAT_MAGICPOINTSPERCENT)
	registerEnum(CONDITION_PARAM_PERIODICDAMAGE)
	registerEnum(CONDITION_PARAM_SKILL_MELEEPERCENT)
	registerEnum(CONDITION_PARAM_SKILL_FISTPERCENT)
	registerEnum(CONDITION_PARAM_SKILL_CLUBPERCENT)
	registerEnum(CONDITION_PARAM_SKILL_SWORDPERCENT)
	registerEnum(CONDITION_PARAM_SKILL_AXEPERCENT)
	registerEnum(CONDITION_PARAM_SKILL_DISTANCEPERCENT)
	registerEnum(CONDITION_PARAM_SKILL_SHIELDPERCENT)
	registerEnum(CONDITION_PARAM_SKILL_FISHINGPERCENT)
	registerEnum(CONDITION_PARAM_BUFF_SPELL)
	registerEnum(CONDITION_PARAM_SUBID)
	registerEnum(CONDITION_PARAM_FIELD)

	registerEnum(CONST_ME_DRAWBLOOD)
	registerEnum(CONST_ME_LOSEENERGY)
	registerEnum(CONST_ME_POFF)
	registerEnum(CONST_ME_BLOCKHIT)
	registerEnum(CONST_ME_EXPLOSIONAREA)
	registerEnum(CONST_ME_EXPLOSIONHIT)
	registerEnum(CONST_ME_FIREAREA)
	registerEnum(CONST_ME_YELLOW_RINGS)
	registerEnum(CONST_ME_GREEN_RINGS)
	registerEnum(CONST_ME_HITAREA)
	registerEnum(CONST_ME_TELEPORT)
	registerEnum(CONST_ME_ENERGYHIT)
	registerEnum(CONST_ME_MAGIC_BLUE)
	registerEnum(CONST_ME_MAGIC_RED)
	registerEnum(CONST_ME_MAGIC_GREEN)
	registerEnum(CONST_ME_HITBYFIRE)
	registerEnum(CONST_ME_HITBYPOISON)
	registerEnum(CONST_ME_MORTAREA)
	registerEnum(CONST_ME_SOUND_GREEN)
	registerEnum(CONST_ME_SOUND_RED)
	registerEnum(CONST_ME_POISONAREA)
	registerEnum(CONST_ME_SOUND_YELLOW)
	registerEnum(CONST_ME_SOUND_PURPLE)
	registerEnum(CONST_ME_SOUND_BLUE)
	registerEnum(CONST_ME_SOUND_WHITE)
	registerEnum(CONST_ME_BUBBLES)
	registerEnum(CONST_ME_CRAPS)
	registerEnum(CONST_ME_GIFT_WRAPS)
	registerEnum(CONST_ME_FIREWORK_YELLOW)
	registerEnum(CONST_ME_FIREWORK_RED)
	registerEnum(CONST_ME_FIREWORK_BLUE)
	registerEnum(CONST_ME_STUN)
	registerEnum(CONST_ME_SLEEP)
	registerEnum(CONST_ME_WATERCREATURE)
	registerEnum(CONST_ME_GROUNDSHAKER)
	registerEnum(CONST_ME_HEARTS)
	registerEnum(CONST_ME_FIREATTACK)
	registerEnum(CONST_ME_ENERGYAREA)
	registerEnum(CONST_ME_SMALLCLOUDS)
	registerEnum(CONST_ME_HOLYDAMAGE)
	registerEnum(CONST_ME_BIGCLOUDS)
	registerEnum(CONST_ME_ICEAREA)
	registerEnum(CONST_ME_ICETORNADO)
	registerEnum(CONST_ME_ICEATTACK)
	registerEnum(CONST_ME_STONES)
	registerEnum(CONST_ME_SMALLPLANTS)
	registerEnum(CONST_ME_CARNIPHILA)
	registerEnum(CONST_ME_PURPLEENERGY)
	registerEnum(CONST_ME_YELLOWENERGY)
	registerEnum(CONST_ME_HOLYAREA)
	registerEnum(CONST_ME_BIGPLANTS)
	registerEnum(CONST_ME_CAKE)
	registerEnum(CONST_ME_GIANTICE)
	registerEnum(CONST_ME_WATERSPLASH)
	registerEnum(CONST_ME_PLANTATTACK)
	registerEnum(CONST_ME_TUTORIALARROW)
	registerEnum(CONST_ME_TUTORIALSQUARE)
	registerEnum(CONST_ME_MIRRORHORIZONTAL)
	registerEnum(CONST_ME_MIRRORVERTICAL)
	registerEnum(CONST_ME_SKULLHORIZONTAL)
	registerEnum(CONST_ME_SKULLVERTICAL)
	registerEnum(CONST_ME_ASSASSIN)
	registerEnum(CONST_ME_STEPSHORIZONTAL)
	registerEnum(CONST_ME_BLOODYSTEPS)
	registerEnum(CONST_ME_STEPSVERTICAL)
	registerEnum(CONST_ME_YALAHARIGHOST)
	registerEnum(CONST_ME_BATS)
	registerEnum(CONST_ME_SMOKE)
	registerEnum(CONST_ME_INSECTS)
	registerEnum(CONST_ME_DRAGONHEAD)
	registerEnum(CONST_ME_ORCSHAMAN)
	registerEnum(CONST_ME_ORCSHAMAN_FIRE)
	registerEnum(CONST_ME_THUNDER)
	registerEnum(CONST_ME_FERUMBRAS)
	registerEnum(CONST_ME_CONFETTI_HORIZONTAL)
	registerEnum(CONST_ME_CONFETTI_VERTICAL)
	registerEnum(CONST_ME_BLACKSMOKE)
	registerEnum(CONST_ME_NONE)

	registerEnum(CONST_ANI_SPEAR)
	registerEnum(CONST_ANI_BOLT)
	registerEnum(CONST_ANI_ARROW)
	registerEnum(CONST_ANI_FIRE)
	registerEnum(CONST_ANI_ENERGY)
	registerEnum(CONST_ANI_POISONARROW)
	registerEnum(CONST_ANI_BURSTARROW)
	registerEnum(CONST_ANI_THROWINGSTAR)
	registerEnum(CONST_ANI_THROWINGKNIFE)
	registerEnum(CONST_ANI_SMALLSTONE)
	registerEnum(CONST_ANI_DEATH)
	registerEnum(CONST_ANI_LARGEROCK)
	registerEnum(CONST_ANI_SNOWBALL)
	registerEnum(CONST_ANI_POWERBOLT)
	registerEnum(CONST_ANI_POISON)
	registerEnum(CONST_ANI_INFERNALBOLT)
	registerEnum(CONST_ANI_HUNTINGSPEAR)
	registerEnum(CONST_ANI_ENCHANTEDSPEAR)
	registerEnum(CONST_ANI_REDSTAR)
	registerEnum(CONST_ANI_GREENSTAR)
	registerEnum(CONST_ANI_ROYALSPEAR)
	registerEnum(CONST_ANI_SNIPERARROW)
	registerEnum(CONST_ANI_ONYXARROW)
	registerEnum(CONST_ANI_PIERCINGBOLT)
	registerEnum(CONST_ANI_WHIRLWINDSWORD)
	registerEnum(CONST_ANI_WHIRLWINDAXE)
	registerEnum(CONST_ANI_WHIRLWINDCLUB)
	registerEnum(CONST_ANI_ETHEREALSPEAR)
	registerEnum(CONST_ANI_ICE)
	registerEnum(CONST_ANI_EARTH)
	registerEnum(CONST_ANI_HOLY)
	registerEnum(CONST_ANI_SUDDENDEATH)
	registerEnum(CONST_ANI_FLASHARROW)
	registerEnum(CONST_ANI_FLAMMINGARROW)
	registerEnum(CONST_ANI_SHIVERARROW)
	registerEnum(CONST_ANI_ENERGYBALL)
	registerEnum(CONST_ANI_SMALLICE)
	registerEnum(CONST_ANI_SMALLHOLY)
	registerEnum(CONST_ANI_SMALLEARTH)
	registerEnum(CONST_ANI_EARTHARROW)
	registerEnum(CONST_ANI_EXPLOSION)
	registerEnum(CONST_ANI_CAKE)
	registerEnum(CONST_ANI_TARSALARROW)
	registerEnum(CONST_ANI_VORTEXBOLT)
	registerEnum(CONST_ANI_PRISMATICBOLT)
	registerEnum(CONST_ANI_CRYSTALLINEARROW)
	registerEnum(CONST_ANI_DRILLBOLT)
	registerEnum(CONST_ANI_ENVENOMEDARROW)
	registerEnum(CONST_ANI_WEAPONTYPE)
	registerEnum(CONST_ANI_NONE)

	registerEnum(CONST_PROP_BLOCKSOLID)
	registerEnum(CONST_PROP_HASHEIGHT)
	registerEnum(CONST_PROP_BLOCKPROJECTILE)
	registerEnum(CONST_PROP_BLOCKPATH)
	registerEnum(CONST_PROP_ISVERTICAL)
	registerEnum(CONST_PROP_ISHORIZONTAL)
	registerEnum(CONST_PROP_MOVEABLE)
	registerEnum(CONST_PROP_IMMOVABLEBLOCKSOLID)
	registerEnum(CONST_PROP_IMMOVABLEBLOCKPATH)
	registerEnum(CONST_PROP_IMMOVABLENOFIELDBLOCKPATH)
	registerEnum(CONST_PROP_NOFIELDBLOCKPATH)
	registerEnum(CONST_PROP_SUPPORTHANGABLE)

	registerEnum(CONST_SLOT_HEAD)
	registerEnum(CONST_SLOT_NECKLACE)
	registerEnum(CONST_SLOT_BACKPACK)
	registerEnum(CONST_SLOT_ARMOR)
	registerEnum(CONST_SLOT_RIGHT)
	registerEnum(CONST_SLOT_LEFT)
	registerEnum(CONST_SLOT_LEGS)
	registerEnum(CONST_SLOT_FEET)
	registerEnum(CONST_SLOT_RING)
	registerEnum(CONST_SLOT_AMMO)

	registerEnum(GAME_STATE_STARTUP)
	registerEnum(GAME_STATE_INIT)
	registerEnum(GAME_STATE_NORMAL)
	registerEnum(GAME_STATE_CLOSED)
	registerEnum(GAME_STATE_SHUTDOWN)
	registerEnum(GAME_STATE_CLOSING)
	registerEnum(GAME_STATE_MAINTAIN)

	registerEnum(MESSAGE_STATUS_CONSOLE_BLUE)
	registerEnum(MESSAGE_STATUS_CONSOLE_RED)
	registerEnum(MESSAGE_STATUS_DEFAULT)
	registerEnum(MESSAGE_STATUS_WARNING)
	registerEnum(MESSAGE_EVENT_ADVANCE)
	registerEnum(MESSAGE_STATUS_SMALL)
	registerEnum(MESSAGE_INFO_DESCR)
	registerEnum(MESSAGE_DAMAGE_DEALT)
	registerEnum(MESSAGE_DAMAGE_RECEIVED)
	registerEnum(MESSAGE_HEALED)
	registerEnum(MESSAGE_EXPERIENCE)
	registerEnum(MESSAGE_DAMAGE_OTHERS)
	registerEnum(MESSAGE_HEALED_OTHERS)
	registerEnum(MESSAGE_EXPERIENCE_OTHERS)
	registerEnum(MESSAGE_EVENT_DEFAULT)
	registerEnum(MESSAGE_EVENT_ORANGE)
	registerEnum(MESSAGE_STATUS_CONSOLE_ORANGE)

	registerEnum(CLIENTOS_LINUX)
	registerEnum(CLIENTOS_WINDOWS)
	registerEnum(CLIENTOS_FLASH)
	registerEnum(CLIENTOS_OTCLIENT_LINUX)
	registerEnum(CLIENTOS_OTCLIENT_WINDOWS)
	registerEnum(CLIENTOS_OTCLIENT_MAC)

	registerEnum(ITEM_ATTRIBUTE_NONE)
	registerEnum(ITEM_ATTRIBUTE_ACTIONID)
	registerEnum(ITEM_ATTRIBUTE_UNIQUEID)
	registerEnum(ITEM_ATTRIBUTE_DESCRIPTION)
	registerEnum(ITEM_ATTRIBUTE_TEXT)
	registerEnum(ITEM_ATTRIBUTE_DATE)
	registerEnum(ITEM_ATTRIBUTE_WRITER)
	registerEnum(ITEM_ATTRIBUTE_OWNER)
	registerEnum(ITEM_ATTRIBUTE_DURATION)
	registerEnum(ITEM_ATTRIBUTE_DECAYSTATE)
	registerEnum(ITEM_ATTRIBUTE_CORPSEOWNER)
	registerEnum(ITEM_ATTRIBUTE_CHARGES)
	registerEnum(ITEM_ATTRIBUTE_FLUIDTYPE)
	registerEnum(ITEM_ATTRIBUTE_DOORID)

	registerEnum(ITEM_TYPE_DEPOT)
	registerEnum(ITEM_TYPE_MAILBOX)
	registerEnum(ITEM_TYPE_TRASHHOLDER)
	registerEnum(ITEM_TYPE_CONTAINER)
	registerEnum(ITEM_TYPE_DOOR)
	registerEnum(ITEM_TYPE_MAGICFIELD)
	registerEnum(ITEM_TYPE_TELEPORT)
	registerEnum(ITEM_TYPE_BED)
	registerEnum(ITEM_TYPE_KEY)
	registerEnum(ITEM_TYPE_RUNE)

	registerEnum(ITEM_GOLD_COIN)
	registerEnum(ITEM_PLATINUM_COIN)
	registerEnum(ITEM_CRYSTAL_COIN)
	registerEnum(ITEM_PARCEL)
	registerEnum(ITEM_LABEL)
	registerEnum(ITEM_FIREFIELD_PVP_FULL)
	registerEnum(ITEM_FIREFIELD_PVP_MEDIUM)
	registerEnum(ITEM_FIREFIELD_PVP_SMALL)
	registerEnum(ITEM_FIREFIELD_PERSISTENT_FULL)
	registerEnum(ITEM_FIREFIELD_PERSISTENT_MEDIUM)
	registerEnum(ITEM_FIREFIELD_PERSISTENT_SMALL)
	registerEnum(ITEM_FIREFIELD_NOPVP)
	registerEnum(ITEM_POISONFIELD_PVP)
	registerEnum(ITEM_POISONFIELD_PERSISTENT)
	registerEnum(ITEM_POISONFIELD_NOPVP)
	registerEnum(ITEM_ENERGYFIELD_PVP)
	registerEnum(ITEM_ENERGYFIELD_PERSISTENT)
	registerEnum(ITEM_ENERGYFIELD_NOPVP)
	registerEnum(ITEM_MAGICWALL)
	registerEnum(ITEM_MAGICWALL_PERSISTENT)
	registerEnum(ITEM_MAGICWALL_SAFE)
	registerEnum(ITEM_WILDGROWTH)
	registerEnum(ITEM_WILDGROWTH_PERSISTENT)
	registerEnum(ITEM_WILDGROWTH_SAFE)

	registerEnum(PlayerFlag_CannotUseCombat)
	registerEnum(PlayerFlag_CannotAttackPlayer)
	registerEnum(PlayerFlag_CannotAttackMonster)
	registerEnum(PlayerFlag_CannotBeAttacked)
	registerEnum(PlayerFlag_CanConvinceAll)
	registerEnum(PlayerFlag_CanSummonAll)
	registerEnum(PlayerFlag_CanIllusionAll)
	registerEnum(PlayerFlag_CanSenseInvisibility)
	registerEnum(PlayerFlag_IgnoredByMonsters)
	registerEnum(PlayerFlag_NotGainInFight)
	registerEnum(PlayerFlag_HasInfiniteMana)
	registerEnum(PlayerFlag_HasInfiniteSoul)
	registerEnum(PlayerFlag_HasNoExhaustion)
	registerEnum(PlayerFlag_CannotUseSpells)
	registerEnum(PlayerFlag_CannotPickupItem)
	registerEnum(PlayerFlag_CanAlwaysLogin)
	registerEnum(PlayerFlag_CanBroadcast)
	registerEnum(PlayerFlag_CanEditHouses)
	registerEnum(PlayerFlag_CannotBeBanned)
	registerEnum(PlayerFlag_CannotBePushed)
	registerEnum(PlayerFlag_HasInfiniteCapacity)
	registerEnum(PlayerFlag_CanPushAllCreatures)
	registerEnum(PlayerFlag_CanTalkRedPrivate)
	registerEnum(PlayerFlag_CanTalkRedChannel)
	registerEnum(PlayerFlag_TalkOrangeHelpChannel)
	registerEnum(PlayerFlag_NotGainExperience)
	registerEnum(PlayerFlag_NotGainMana)
	registerEnum(PlayerFlag_NotGainHealth)
	registerEnum(PlayerFlag_NotGainSkill)
	registerEnum(PlayerFlag_SetMaxSpeed)
	registerEnum(PlayerFlag_SpecialVIP)
	registerEnum(PlayerFlag_NotGenerateLoot)
	registerEnum(PlayerFlag_CanTalkRedChannelAnonymous)
	registerEnum(PlayerFlag_IgnoreProtectionZone)
	registerEnum(PlayerFlag_IgnoreSpellCheck)
	registerEnum(PlayerFlag_IgnoreWeaponCheck)
	registerEnum(PlayerFlag_CannotBeMuted)
	registerEnum(PlayerFlag_IsAlwaysPremium)

	registerEnum(PLAYERSEX_FEMALE)
	registerEnum(PLAYERSEX_MALE)

	registerEnum(SKILL_FIST)
	registerEnum(SKILL_CLUB)
	registerEnum(SKILL_SWORD)
	registerEnum(SKILL_AXE)
	registerEnum(SKILL_DISTANCE)
	registerEnum(SKILL_SHIELD)
	registerEnum(SKILL_FISHING)
	registerEnum(SKILL_MAGLEVEL)
	registerEnum(SKILLVALUE_LEVEL)

	registerEnum(SKULL_NONE)
	registerEnum(SKULL_YELLOW)
	registerEnum(SKULL_GREEN)
	registerEnum(SKULL_WHITE)
	registerEnum(SKULL_RED)
	registerEnum(SKULL_BLACK)
	registerEnum(SKULL_ORANGE)

	registerEnum(TALKTYPE_SAY)
	registerEnum(TALKTYPE_WHISPER)
	registerEnum(TALKTYPE_YELL)
	registerEnum(TALKTYPE_PRIVATE_FROM)
	registerEnum(TALKTYPE_PRIVATE_TO)
	registerEnum(TALKTYPE_CHANNEL_Y)
	registerEnum(TALKTYPE_CHANNEL_O)
	registerEnum(TALKTYPE_PRIVATE_NP)
	registerEnum(TALKTYPE_PRIVATE_PN)
	registerEnum(TALKTYPE_BROADCAST)
	registerEnum(TALKTYPE_CHANNEL_R1)
	registerEnum(TALKTYPE_PRIVATE_RED_FROM)
	registerEnum(TALKTYPE_PRIVATE_RED_TO)
	registerEnum(TALKTYPE_MONSTER_SAY)
	registerEnum(TALKTYPE_MONSTER_YELL)
	registerEnum(TALKTYPE_CHANNEL_R2)
	registerEnum(TALKTYPE_CHANNEL_W)

	registerEnum(TEXTCOLOR_BLUE)
	registerEnum(TEXTCOLOR_LIGHTGREEN)
	registerEnum(TEXTCOLOR_LIGHTBLUE)
	registerEnum(TEXTCOLOR_MAYABLUE)
	registerEnum(TEXTCOLOR_DARKRED)
	registerEnum(TEXTCOLOR_LIGHTGREY)
	registerEnum(TEXTCOLOR_SKYBLUE)
	registerEnum(TEXTCOLOR_PURPLE)
	registerEnum(TEXTCOLOR_RED)
	registerEnum(TEXTCOLOR_ORANGE)
	registerEnum(TEXTCOLOR_YELLOW)
	registerEnum(TEXTCOLOR_WHITE_EXP)
	registerEnum(TEXTCOLOR_NONE)

	registerEnum(TILESTATE_NONE)
	registerEnum(TILESTATE_PROTECTIONZONE)
	registerEnum(TILESTATE_DEPRECATED_HOUSE)
	registerEnum(TILESTATE_NOPVPZONE)
	registerEnum(TILESTATE_NOLOGOUT)
	registerEnum(TILESTATE_PVPZONE)
	registerEnum(TILESTATE_REFRESH)
	registerEnum(TILESTATE_HOUSE)
	registerEnum(TILESTATE_FLOORCHANGE)
	registerEnum(TILESTATE_FLOORCHANGE_DOWN)
	registerEnum(TILESTATE_FLOORCHANGE_NORTH)
	registerEnum(TILESTATE_FLOORCHANGE_SOUTH)
	registerEnum(TILESTATE_FLOORCHANGE_EAST)
	registerEnum(TILESTATE_FLOORCHANGE_WEST)
	registerEnum(TILESTATE_TELEPORT)
	registerEnum(TILESTATE_MAGICFIELD)
	registerEnum(TILESTATE_MAILBOX)
	registerEnum(TILESTATE_TRASHHOLDER)
	registerEnum(TILESTATE_BED)
	registerEnum(TILESTATE_DEPOT)
	registerEnum(TILESTATE_BLOCKSOLID)
	registerEnum(TILESTATE_BLOCKPATH)
	registerEnum(TILESTATE_IMMOVABLEBLOCKSOLID)
	registerEnum(TILESTATE_IMMOVABLEBLOCKPATH)
	registerEnum(TILESTATE_IMMOVABLENOFIELDBLOCKPATH)
	registerEnum(TILESTATE_NOFIELDBLOCKPATH)
	registerEnum(TILESTATE_DYNAMIC_TILE)
	registerEnum(TILESTATE_FLOORCHANGE_SOUTH_ALT)
	registerEnum(TILESTATE_FLOORCHANGE_EAST_ALT)
	registerEnum(TILESTATE_SUPPORTS_HANGABLE)

	registerEnum(WEAPON_NONE)
	registerEnum(WEAPON_SWORD)
	registerEnum(WEAPON_CLUB)
	registerEnum(WEAPON_AXE)
	registerEnum(WEAPON_SHIELD)
	registerEnum(WEAPON_DISTANCE)
	registerEnum(WEAPON_WAND)
	registerEnum(WEAPON_AMMO)

	registerEnum(WORLD_TYPE_NO_PVP)
	registerEnum(WORLD_TYPE_PVP)
	registerEnum(WORLD_TYPE_PVP_ENFORCED)

	// Use with container:addItem, container:addItemEx and possibly other functions.
	registerEnum(FLAG_NOLIMIT)
	registerEnum(FLAG_IGNOREBLOCKITEM)
	registerEnum(FLAG_IGNOREBLOCKCREATURE)
	registerEnum(FLAG_CHILDISOWNER)
	registerEnum(FLAG_PATHFINDING)
	registerEnum(FLAG_IGNOREFIELDDAMAGE)
	registerEnum(FLAG_IGNORENOTMOVEABLE)
	registerEnum(FLAG_IGNOREAUTOSTACK)

	// _G
	registerGlobalVariable("INDEX_WHEREEVER", INDEX_WHEREEVER);
	registerGlobalBoolean("VIRTUAL_PARENT", true);

	registerGlobalMethod("isType", LuaScriptInterface::luaIsType);

	// configKeys
	registerTable("configKeys");

	registerEnumIn("configKeys", ConfigManager::ALLOW_CHANGEOUTFIT)
	registerEnumIn("configKeys", ConfigManager::CANNOT_ATTACK_SAME_LOOKFEET)
	registerEnumIn("configKeys", ConfigManager::ONE_PLAYER_ON_ACCOUNT)
	registerEnumIn("configKeys", ConfigManager::AIMBOT_HOTKEY_ENABLED)
	registerEnumIn("configKeys", ConfigManager::REMOVE_AMMO)
	registerEnumIn("configKeys", ConfigManager::REMOVE_RUNE_CHARGES)
	registerEnumIn("configKeys", ConfigManager::EXPERIENCE_FROM_PLAYERS)
	registerEnumIn("configKeys", ConfigManager::FREE_PREMIUM)
	registerEnumIn("configKeys", ConfigManager::REPLACE_KICK_ON_LOGIN)
	registerEnumIn("configKeys", ConfigManager::ALLOW_CLONES)
	registerEnumIn("configKeys", ConfigManager::BIND_ONLY_GLOBAL_ADDRESS)
	registerEnumIn("configKeys", ConfigManager::OPTIMIZE_DATABASE)
	registerEnumIn("configKeys", ConfigManager::MARKET_PREMIUM)
	registerEnumIn("configKeys", ConfigManager::EMOTE_SPELLS)
	registerEnumIn("configKeys", ConfigManager::STAMINA_SYSTEM)

	registerEnumIn("configKeys", ConfigManager::MAP_NAME)
	registerEnumIn("configKeys", ConfigManager::HOUSE_RENT_PERIOD)
	registerEnumIn("configKeys", ConfigManager::SERVER_NAME)
	registerEnumIn("configKeys", ConfigManager::OWNER_NAME)
	registerEnumIn("configKeys", ConfigManager::OWNER_EMAIL)
	registerEnumIn("configKeys", ConfigManager::URL)
	registerEnumIn("configKeys", ConfigManager::LOCATION)
	registerEnumIn("configKeys", ConfigManager::IP)
	registerEnumIn("configKeys", ConfigManager::MOTD)
	registerEnumIn("configKeys", ConfigManager::WORLD_TYPE)
	registerEnumIn("configKeys", ConfigManager::MYSQL_HOST)
	registerEnumIn("configKeys", ConfigManager::MYSQL_USER)
	registerEnumIn("configKeys", ConfigManager::MYSQL_PASS)
	registerEnumIn("configKeys", ConfigManager::MYSQL_DB)
	registerEnumIn("configKeys", ConfigManager::MYSQL_SOCK)
	registerEnumIn("configKeys", ConfigManager::DEFAULT_PRIORITY)
	registerEnumIn("configKeys", ConfigManager::MAP_AUTHOR)

	registerEnumIn("configKeys", ConfigManager::SQL_PORT)
	registerEnumIn("configKeys", ConfigManager::MAX_PLAYERS)
	registerEnumIn("configKeys", ConfigManager::PZ_LOCKED)
	registerEnumIn("configKeys", ConfigManager::DEFAULT_DESPAWNRANGE)
	registerEnumIn("configKeys", ConfigManager::DEFAULT_DESPAWNRADIUS)
	registerEnumIn("configKeys", ConfigManager::RATE_EXPERIENCE)
	registerEnumIn("configKeys", ConfigManager::RATE_SKILL)
	registerEnumIn("configKeys", ConfigManager::RATE_LOOT)
	registerEnumIn("configKeys", ConfigManager::RATE_MAGIC)
	registerEnumIn("configKeys", ConfigManager::RATE_SPAWN)
	registerEnumIn("configKeys", ConfigManager::HOUSE_PRICE)
	registerEnumIn("configKeys", ConfigManager::KILLS_TO_RED)
	registerEnumIn("configKeys", ConfigManager::KILLS_TO_BLACK)
	registerEnumIn("configKeys", ConfigManager::MAX_MESSAGEBUFFER)
	registerEnumIn("configKeys", ConfigManager::ACTIONS_DELAY_INTERVAL)
	registerEnumIn("configKeys", ConfigManager::EX_ACTIONS_DELAY_INTERVAL)
	registerEnumIn("configKeys", ConfigManager::KICK_AFTER_MINUTES)
	registerEnumIn("configKeys", ConfigManager::PROTECTION_LEVEL)
	registerEnumIn("configKeys", ConfigManager::DEATH_LOSE_PERCENT)
	registerEnumIn("configKeys", ConfigManager::STATUSQUERY_TIMEOUT)
	registerEnumIn("configKeys", ConfigManager::FRAG_TIME)
	registerEnumIn("configKeys", ConfigManager::WHITE_SKULL_TIME)
	registerEnumIn("configKeys", ConfigManager::GAME_PORT)
	registerEnumIn("configKeys", ConfigManager::LOGIN_PORT)
	registerEnumIn("configKeys", ConfigManager::STATUS_PORT)
	registerEnumIn("configKeys", ConfigManager::STAIRHOP_DELAY)
	registerEnumIn("configKeys", ConfigManager::MARKET_OFFER_DURATION)
	registerEnumIn("configKeys", ConfigManager::CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES)
	registerEnumIn("configKeys", ConfigManager::MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER)
	registerEnumIn("configKeys", ConfigManager::EXP_FROM_PLAYERS_LEVEL_RANGE)
	registerEnumIn("configKeys", ConfigManager::MAX_PACKETS_PER_SECOND)

	// Game
	registerTable("Game");

	registerMethod("Game", "getSpectators", LuaScriptInterface::luaGameGetSpectators);
	registerMethod("Game", "getPlayers", LuaScriptInterface::luaGameGetPlayers);
	registerMethod("Game", "loadMap", LuaScriptInterface::luaGameLoadMap);

	registerMethod("Game", "getExperienceStage", LuaScriptInterface::luaGameGetExperienceStage);
	registerMethod("Game", "getMonsterCount", LuaScriptInterface::luaGameGetMonsterCount);
	registerMethod("Game", "getPlayerCount", LuaScriptInterface::luaGameGetPlayerCount);
	registerMethod("Game", "getNpcCount", LuaScriptInterface::luaGameGetNpcCount);

	registerMethod("Game", "getTowns", LuaScriptInterface::luaGameGetTowns);
	registerMethod("Game", "getHouses", LuaScriptInterface::luaGameGetHouses);

	registerMethod("Game", "getGameState", LuaScriptInterface::luaGameGetGameState);
	registerMethod("Game", "setGameState", LuaScriptInterface::luaGameSetGameState);

	registerMethod("Game", "getWorldType", LuaScriptInterface::luaGameGetWorldType);
	registerMethod("Game", "setWorldType", LuaScriptInterface::luaGameSetWorldType);

	registerMethod("Game", "getReturnMessage", LuaScriptInterface::luaGameGetReturnMessage);

	registerMethod("Game", "createItem", LuaScriptInterface::luaGameCreateItem);
	registerMethod("Game", "createMonster", LuaScriptInterface::luaGameCreateMonster);
	registerMethod("Game", "createNpc", LuaScriptInterface::luaGameCreateNpc);

	registerMethod("Game", "startRaid", LuaScriptInterface::luaGameStartRaid);

	// Position
	registerClass("Position", "", LuaScriptInterface::luaPositionCreate);
	registerMetaMethod("Position", "__add", LuaScriptInterface::luaPositionAdd);
	registerMetaMethod("Position", "__sub", LuaScriptInterface::luaPositionSub);
	registerMetaMethod("Position", "__eq", LuaScriptInterface::luaPositionCompare);

	registerMethod("Position", "getTile", LuaScriptInterface::luaPositionGetTile);
	registerMethod("Position", "getDistance", LuaScriptInterface::luaPositionGetDistance);
	registerMethod("Position", "isSightClear", LuaScriptInterface::luaPositionIsSightClear);

	registerMethod("Position", "sendMagicEffect", LuaScriptInterface::luaPositionSendMagicEffect);
	registerMethod("Position", "sendDistanceEffect", LuaScriptInterface::luaPositionSendDistanceEffect);

	// Tile
	registerClass("Tile", "", LuaScriptInterface::luaTileCreate);
	registerMetaMethod("Tile", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Tile", "getPosition", LuaScriptInterface::luaTileGetPosition);
	registerMethod("Tile", "getGround", LuaScriptInterface::luaTileGetGround);
	registerMethod("Tile", "getThing", LuaScriptInterface::luaTileGetThing);
	registerMethod("Tile", "getThingCount", LuaScriptInterface::luaTileGetThingCount);
	registerMethod("Tile", "getTopVisibleThing", LuaScriptInterface::luaTileGetTopVisibleThing);

	registerMethod("Tile", "getTopTopItem", LuaScriptInterface::luaTileGetTopTopItem);
	registerMethod("Tile", "getTopDownItem", LuaScriptInterface::luaTileGetTopDownItem);
	registerMethod("Tile", "getFieldItem", LuaScriptInterface::luaTileGetFieldItem);

	registerMethod("Tile", "getItemById", LuaScriptInterface::luaTileGetItemById);
	registerMethod("Tile", "getItemByType", LuaScriptInterface::luaTileGetItemByType);
	registerMethod("Tile", "getItemByTopOrder", LuaScriptInterface::luaTileGetItemByTopOrder);
	registerMethod("Tile", "getItemCountById", LuaScriptInterface::luaTileGetItemCountById);

	registerMethod("Tile", "getBottomCreature", LuaScriptInterface::luaTileGetBottomCreature);
	registerMethod("Tile", "getTopCreature", LuaScriptInterface::luaTileGetTopCreature);
	registerMethod("Tile", "getBottomVisibleCreature", LuaScriptInterface::luaTileGetBottomVisibleCreature);
	registerMethod("Tile", "getTopVisibleCreature", LuaScriptInterface::luaTileGetTopVisibleCreature);

	registerMethod("Tile", "getItems", LuaScriptInterface::luaTileGetItems);
	registerMethod("Tile", "getItemCount", LuaScriptInterface::luaTileGetItemCount);
	registerMethod("Tile", "getDownItemCount", LuaScriptInterface::luaTileGetDownItemCount);
	registerMethod("Tile", "getTopItemCount", LuaScriptInterface::luaTileGetTopItemCount);

	registerMethod("Tile", "getCreatures", LuaScriptInterface::luaTileGetCreatures);
	registerMethod("Tile", "getCreatureCount", LuaScriptInterface::luaTileGetCreatureCount);

	registerMethod("Tile", "hasProperty", LuaScriptInterface::luaTileHasProperty);
	registerMethod("Tile", "hasFlag", LuaScriptInterface::luaTileHasFlag);

	registerMethod("Tile", "queryAdd", LuaScriptInterface::luaTileQueryAdd);

	registerMethod("Tile", "getHouse", LuaScriptInterface::luaTileGetHouse);

	// NetworkMessage
	registerClass("NetworkMessage", "", LuaScriptInterface::luaNetworkMessageCreate);
	registerMetaMethod("NetworkMessage", "__eq", LuaScriptInterface::luaUserdataCompare);
	registerMetaMethod("NetworkMessage", "__gc", LuaScriptInterface::luaNetworkMessageDelete);
	registerMethod("NetworkMessage", "delete", LuaScriptInterface::luaNetworkMessageDelete);

	registerMethod("NetworkMessage", "getByte", LuaScriptInterface::luaNetworkMessageGetByte);
	registerMethod("NetworkMessage", "getU16", LuaScriptInterface::luaNetworkMessageGetU16);
	registerMethod("NetworkMessage", "getU32", LuaScriptInterface::luaNetworkMessageGetU32);
	registerMethod("NetworkMessage", "getU64", LuaScriptInterface::luaNetworkMessageGetU64);
	registerMethod("NetworkMessage", "getString", LuaScriptInterface::luaNetworkMessageGetString);
	registerMethod("NetworkMessage", "getPosition", LuaScriptInterface::luaNetworkMessageGetPosition);

	registerMethod("NetworkMessage", "addByte", LuaScriptInterface::luaNetworkMessageAddByte);
	registerMethod("NetworkMessage", "addU16", LuaScriptInterface::luaNetworkMessageAddU16);
	registerMethod("NetworkMessage", "addU32", LuaScriptInterface::luaNetworkMessageAddU32);
	registerMethod("NetworkMessage", "addU64", LuaScriptInterface::luaNetworkMessageAddU64);
	registerMethod("NetworkMessage", "addString", LuaScriptInterface::luaNetworkMessageAddString);
	registerMethod("NetworkMessage", "addPosition", LuaScriptInterface::luaNetworkMessageAddPosition);
	registerMethod("NetworkMessage", "addDouble", LuaScriptInterface::luaNetworkMessageAddDouble);
	registerMethod("NetworkMessage", "addItem", LuaScriptInterface::luaNetworkMessageAddItem);
	registerMethod("NetworkMessage", "addItemId", LuaScriptInterface::luaNetworkMessageAddItemId);

	registerMethod("NetworkMessage", "reset", LuaScriptInterface::luaNetworkMessageReset);
	registerMethod("NetworkMessage", "skipBytes", LuaScriptInterface::luaNetworkMessageSkipBytes);
	registerMethod("NetworkMessage", "sendToPlayer", LuaScriptInterface::luaNetworkMessageSendToPlayer);

	// ModalWindow
	registerClass("ModalWindow", "", LuaScriptInterface::luaModalWindowCreate);
	registerMetaMethod("ModalWindow", "__eq", LuaScriptInterface::luaUserdataCompare);
	registerMetaMethod("ModalWindow", "__gc", LuaScriptInterface::luaModalWindowDelete);

	registerMethod("ModalWindow", "getId", LuaScriptInterface::luaModalWindowGetId);
	registerMethod("ModalWindow", "getTitle", LuaScriptInterface::luaModalWindowGetTitle);
	registerMethod("ModalWindow", "getMessage", LuaScriptInterface::luaModalWindowGetMessage);

	registerMethod("ModalWindow", "setTitle", LuaScriptInterface::luaModalWindowSetTitle);
	registerMethod("ModalWindow", "setMessage", LuaScriptInterface::luaModalWindowSetMessage);

	registerMethod("ModalWindow", "getButtonCount", LuaScriptInterface::luaModalWindowGetButtonCount);
	registerMethod("ModalWindow", "getChoiceCount", LuaScriptInterface::luaModalWindowGetChoiceCount);

	registerMethod("ModalWindow", "addButton", LuaScriptInterface::luaModalWindowAddButton);
	registerMethod("ModalWindow", "addChoice", LuaScriptInterface::luaModalWindowAddChoice);

	registerMethod("ModalWindow", "getDefaultEnterButton", LuaScriptInterface::luaModalWindowGetDefaultEnterButton);
	registerMethod("ModalWindow", "setDefaultEnterButton", LuaScriptInterface::luaModalWindowSetDefaultEnterButton);

	registerMethod("ModalWindow", "getDefaultEscapeButton", LuaScriptInterface::luaModalWindowGetDefaultEscapeButton);
	registerMethod("ModalWindow", "setDefaultEscapeButton", LuaScriptInterface::luaModalWindowSetDefaultEscapeButton);

	registerMethod("ModalWindow", "hasPriority", LuaScriptInterface::luaModalWindowHasPriority);
	registerMethod("ModalWindow", "setPriority", LuaScriptInterface::luaModalWindowSetPriority);

	registerMethod("ModalWindow", "sendToPlayer", LuaScriptInterface::luaModalWindowSendToPlayer);

	// Item
	registerClass("Item", "", LuaScriptInterface::luaItemCreate);
	registerMetaMethod("Item", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Item", "isCreature", LuaScriptInterface::luaItemIsCreature);
	registerMethod("Item", "isItem", LuaScriptInterface::luaItemIsItem);
	registerMethod("Item", "isContainer", LuaScriptInterface::luaItemIsContainer);

	registerMethod("Item", "getParent", LuaScriptInterface::luaItemGetParent);
	registerMethod("Item", "getTopParent", LuaScriptInterface::luaItemGetTopParent);

	registerMethod("Item", "getId", LuaScriptInterface::luaItemGetId);
	registerMethod("Item", "getType", LuaScriptInterface::luaItemGetType);

	registerMethod("Item", "clone", LuaScriptInterface::luaItemClone);
	registerMethod("Item", "split", LuaScriptInterface::luaItemSplit);
	registerMethod("Item", "remove", LuaScriptInterface::luaItemRemove);

	registerMethod("Item", "getUniqueId", LuaScriptInterface::luaItemGetUniqueId);
	registerMethod("Item", "getActionId", LuaScriptInterface::luaItemGetActionId);
	registerMethod("Item", "setActionId", LuaScriptInterface::luaItemSetActionId);

	registerMethod("Item", "getCount", LuaScriptInterface::luaItemGetCount);
	registerMethod("Item", "getCharges", LuaScriptInterface::luaItemGetCharges);
	registerMethod("Item", "getFluidType", LuaScriptInterface::luaItemGetFluidType);

	registerMethod("Item", "getSubType", LuaScriptInterface::luaItemGetSubType);

	registerMethod("Item", "getName", LuaScriptInterface::luaItemGetName);
	registerMethod("Item", "getPluralName", LuaScriptInterface::luaItemGetPluralName);
	registerMethod("Item", "getArticle", LuaScriptInterface::luaItemGetArticle);

	registerMethod("Item", "getPosition", LuaScriptInterface::luaItemGetPosition);
	registerMethod("Item", "getTile", LuaScriptInterface::luaItemGetTile);

	registerMethod("Item", "getAttribute", LuaScriptInterface::luaItemGetAttribute);
	registerMethod("Item", "setAttribute", LuaScriptInterface::luaItemSetAttribute);
	registerMethod("Item", "removeAttribute", LuaScriptInterface::luaItemRemoveAttribute);

	registerMethod("Item", "moveTo", LuaScriptInterface::luaItemMoveTo);
	registerMethod("Item", "transform", LuaScriptInterface::luaItemTransform);
	registerMethod("Item", "decay", LuaScriptInterface::luaItemDecay);

	registerMethod("Item", "getDescription", LuaScriptInterface::luaItemGetDescription);

	// Container
	registerClass("Container", "Item", LuaScriptInterface::luaContainerCreate);
	registerMetaMethod("Container", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Container", "isContainer", LuaScriptInterface::luaContainerIsContainer);

	registerMethod("Container", "getSize", LuaScriptInterface::luaContainerGetSize);
	registerMethod("Container", "getCapacity", LuaScriptInterface::luaContainerGetCapacity);
	registerMethod("Container", "getEmptySlots", LuaScriptInterface::luaContainerGetEmptySlots);

	registerMethod("Container", "getItemHoldingCount", LuaScriptInterface::luaContainerGetItemHoldingCount);
	registerMethod("Container", "getItemCountById", LuaScriptInterface::luaContainerGetItemCountById);

	registerMethod("Container", "getItem", LuaScriptInterface::luaContainerGetItem);
	registerMethod("Container", "hasItem", LuaScriptInterface::luaContainerHasItem);
	registerMethod("Container", "addItem", LuaScriptInterface::luaContainerAddItem);
	registerMethod("Container", "addItemEx", LuaScriptInterface::luaContainerAddItemEx);

	// Creature
	registerClass("Creature", "", LuaScriptInterface::luaCreatureCreate);
	registerMetaMethod("Creature", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Creature", "registerEvent", LuaScriptInterface::luaCreatureRegisterEvent);
	registerMethod("Creature", "unregisterEvent", LuaScriptInterface::luaCreatureUnregisterEvent);

	registerMethod("Creature", "isRemoved", LuaScriptInterface::luaCreatureIsRemoved);
	registerMethod("Creature", "isCreature", LuaScriptInterface::luaCreatureIsCreature);
	registerMethod("Creature", "isPlayer", LuaScriptInterface::luaCreatureIsPlayer);
	registerMethod("Creature", "isMonster", LuaScriptInterface::luaCreatureIsMonster);
	registerMethod("Creature", "isNpc", LuaScriptInterface::luaCreatureIsNpc);
	registerMethod("Creature", "isItem", LuaScriptInterface::luaCreatureIsItem);
	registerMethod("Creature", "isInGhostMode", LuaScriptInterface::luaCreatureIsInGhostMode);
	registerMethod("Creature", "isHealthHidden", LuaScriptInterface::luaCreatureIsHealthHidden);

	registerMethod("Creature", "canSee", LuaScriptInterface::luaCreatureCanSee);
	registerMethod("Creature", "canSeeCreature", LuaScriptInterface::luaCreatureCanSeeCreature);

	registerMethod("Creature", "getParent", LuaScriptInterface::luaCreatureGetParent);

	registerMethod("Creature", "getId", LuaScriptInterface::luaCreatureGetId);
	registerMethod("Creature", "getName", LuaScriptInterface::luaCreatureGetName);

	registerMethod("Creature", "getTarget", LuaScriptInterface::luaCreatureGetTarget);
	registerMethod("Creature", "setTarget", LuaScriptInterface::luaCreatureSetTarget);

	registerMethod("Creature", "getFollowCreature", LuaScriptInterface::luaCreatureGetFollowCreature);
	registerMethod("Creature", "setFollowCreature", LuaScriptInterface::luaCreatureSetFollowCreature);

	registerMethod("Creature", "getMaster", LuaScriptInterface::luaCreatureGetMaster);
	registerMethod("Creature", "setMaster", LuaScriptInterface::luaCreatureSetMaster);

	registerMethod("Creature", "getLight", LuaScriptInterface::luaCreatureGetLight);
	registerMethod("Creature", "setLight", LuaScriptInterface::luaCreatureSetLight);

	registerMethod("Creature", "getSpeed", LuaScriptInterface::luaCreatureGetSpeed);
	registerMethod("Creature", "getBaseSpeed", LuaScriptInterface::luaCreatureGetBaseSpeed);

	registerMethod("Creature", "setDropLoot", LuaScriptInterface::luaCreatureSetDropLoot);

	registerMethod("Creature", "getPosition", LuaScriptInterface::luaCreatureGetPosition);
	registerMethod("Creature", "getTile", LuaScriptInterface::luaCreatureGetTile);
	registerMethod("Creature", "getDirection", LuaScriptInterface::luaCreatureGetDirection);
	registerMethod("Creature", "setDirection", LuaScriptInterface::luaCreatureSetDirection);

	registerMethod("Creature", "getHealth", LuaScriptInterface::luaCreatureGetHealth);
	registerMethod("Creature", "addHealth", LuaScriptInterface::luaCreatureAddHealth);
	registerMethod("Creature", "getMaxHealth", LuaScriptInterface::luaCreatureGetMaxHealth);
	registerMethod("Creature", "setMaxHealth", LuaScriptInterface::luaCreatureSetMaxHealth);
	registerMethod("Creature", "setHiddenHealth", LuaScriptInterface::luaCreatureSetHiddenHealth);

	registerMethod("Creature", "getMana", LuaScriptInterface::luaCreatureGetMana);
	registerMethod("Creature", "addMana", LuaScriptInterface::luaCreatureAddMana);
	registerMethod("Creature", "getMaxMana", LuaScriptInterface::luaCreatureGetMaxMana);

	registerMethod("Creature", "getOutfit", LuaScriptInterface::luaCreatureGetOutfit);
	registerMethod("Creature", "setOutfit", LuaScriptInterface::luaCreatureSetOutfit);

	registerMethod("Creature", "getCondition", LuaScriptInterface::luaCreatureGetCondition);
	registerMethod("Creature", "addCondition", LuaScriptInterface::luaCreatureAddCondition);
	registerMethod("Creature", "removeCondition", LuaScriptInterface::luaCreatureRemoveCondition);

	registerMethod("Creature", "remove", LuaScriptInterface::luaCreatureRemove);
	registerMethod("Creature", "teleportTo", LuaScriptInterface::luaCreatureTeleportTo);
	registerMethod("Creature", "say", LuaScriptInterface::luaCreatureSay);

	registerMethod("Creature", "getDamageMap", LuaScriptInterface::luaCreatureGetDamageMap);

	registerMethod("Creature", "getSummons", LuaScriptInterface::luaCreatureGetSummons);

	registerMethod("Creature", "getDescription", LuaScriptInterface::luaCreatureGetDescription);

	registerMethod("Creature", "getPathTo", LuaScriptInterface::luaCreatureGetPathTo);

	// Player
	registerClass("Player", "Creature", LuaScriptInterface::luaPlayerCreate);
	registerMetaMethod("Player", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Player", "isPlayer", LuaScriptInterface::luaPlayerIsPlayer);

	registerMethod("Player", "getGuid", LuaScriptInterface::luaPlayerGetGuid);
	registerMethod("Player", "getIp", LuaScriptInterface::luaPlayerGetIp);
	registerMethod("Player", "getAccountId", LuaScriptInterface::luaPlayerGetAccountId);
	registerMethod("Player", "getLastLoginSaved", LuaScriptInterface::luaPlayerGetLastLoginSaved);

	registerMethod("Player", "getAccountType", LuaScriptInterface::luaPlayerGetAccountType);
	registerMethod("Player", "setAccountType", LuaScriptInterface::luaPlayerSetAccountType);

	registerMethod("Player", "getCapacity", LuaScriptInterface::luaPlayerGetCapacity);
	registerMethod("Player", "setCapacity", LuaScriptInterface::luaPlayerSetCapacity);

	registerMethod("Player", "getFreeCapacity", LuaScriptInterface::luaPlayerGetFreeCapacity);

	registerMethod("Player", "getDepotChest", LuaScriptInterface::luaPlayerGetDepotChest);
	registerMethod("Player", "getInbox", LuaScriptInterface::luaPlayerGetInbox);

	registerMethod("Player", "getSkull", LuaScriptInterface::luaPlayerGetSkull);
	registerMethod("Player", "setSkull", LuaScriptInterface::luaPlayerSetSkull);
	registerMethod("Player", "getSkullTime", LuaScriptInterface::luaPlayerGetSkullTime);
	registerMethod("Player", "setSkullTime", LuaScriptInterface::luaPlayerSetSkullTime);
	registerMethod("Player", "getDeathPenalty", LuaScriptInterface::luaPlayerGetDeathPenalty);

	registerMethod("Player", "getExperience", LuaScriptInterface::luaPlayerGetExperience);
	registerMethod("Player", "addExperience", LuaScriptInterface::luaPlayerAddExperience);
	registerMethod("Player", "removeExperience", LuaScriptInterface::luaPlayerRemoveExperience);
	registerMethod("Player", "getLevel", LuaScriptInterface::luaPlayerGetLevel);

	registerMethod("Player", "getMagicLevel", LuaScriptInterface::luaPlayerGetMagicLevel);
	registerMethod("Player", "getBaseMagicLevel", LuaScriptInterface::luaPlayerGetBaseMagicLevel);
	registerMethod("Player", "setMaxMana", LuaScriptInterface::luaPlayerSetMaxMana);
	registerMethod("Player", "getManaSpent", LuaScriptInterface::luaPlayerGetManaSpent);
	registerMethod("Player", "addManaSpent", LuaScriptInterface::luaPlayerAddManaSpent);

	registerMethod("Player", "getSkillLevel", LuaScriptInterface::luaPlayerGetSkillLevel);
	registerMethod("Player", "getEffectiveSkillLevel", LuaScriptInterface::luaPlayerGetEffectiveSkillLevel);
	registerMethod("Player", "getSkillPercent", LuaScriptInterface::luaPlayerGetSkillPercent);
	registerMethod("Player", "getSkillTries", LuaScriptInterface::luaPlayerGetSkillTries);
	registerMethod("Player", "addSkillTries", LuaScriptInterface::luaPlayerAddSkillTries);

	registerMethod("Player", "getItemCount", LuaScriptInterface::luaPlayerGetItemCount);
	registerMethod("Player", "getItemById", LuaScriptInterface::luaPlayerGetItemById);

	registerMethod("Player", "getVocation", LuaScriptInterface::luaPlayerGetVocation);
	registerMethod("Player", "setVocation", LuaScriptInterface::luaPlayerSetVocation);

	registerMethod("Player", "getSex", LuaScriptInterface::luaPlayerGetSex);
	registerMethod("Player", "setSex", LuaScriptInterface::luaPlayerSetSex);

	registerMethod("Player", "getTown", LuaScriptInterface::luaPlayerGetTown);
	registerMethod("Player", "setTown", LuaScriptInterface::luaPlayerSetTown);

	registerMethod("Player", "getGuild", LuaScriptInterface::luaPlayerGetGuild);
	registerMethod("Player", "setGuild", LuaScriptInterface::luaPlayerSetGuild);

	registerMethod("Player", "getGuildLevel", LuaScriptInterface::luaPlayerGetGuildLevel);
	registerMethod("Player", "setGuildLevel", LuaScriptInterface::luaPlayerSetGuildLevel);

	registerMethod("Player", "getGuildNick", LuaScriptInterface::luaPlayerGetGuildNick);
	registerMethod("Player", "setGuildNick", LuaScriptInterface::luaPlayerSetGuildNick);

	registerMethod("Player", "getGroup", LuaScriptInterface::luaPlayerGetGroup);
	registerMethod("Player", "setGroup", LuaScriptInterface::luaPlayerSetGroup);

	registerMethod("Player", "getStamina", LuaScriptInterface::luaPlayerGetStamina);
	registerMethod("Player", "setStamina", LuaScriptInterface::luaPlayerSetStamina);

	registerMethod("Player", "getSoul", LuaScriptInterface::luaPlayerGetSoul);
	registerMethod("Player", "addSoul", LuaScriptInterface::luaPlayerAddSoul);
	registerMethod("Player", "getMaxSoul", LuaScriptInterface::luaPlayerGetMaxSoul);

	registerMethod("Player", "getBankBalance", LuaScriptInterface::luaPlayerGetBankBalance);
	registerMethod("Player", "setBankBalance", LuaScriptInterface::luaPlayerSetBankBalance);

	registerMethod("Player", "getStorageValue", LuaScriptInterface::luaPlayerGetStorageValue);
	registerMethod("Player", "setStorageValue", LuaScriptInterface::luaPlayerSetStorageValue);

	registerMethod("Player", "addItem", LuaScriptInterface::luaPlayerAddItem);
	registerMethod("Player", "addItemEx", LuaScriptInterface::luaPlayerAddItemEx);
	registerMethod("Player", "removeItem", LuaScriptInterface::luaPlayerRemoveItem);

	registerMethod("Player", "getMoney", LuaScriptInterface::luaPlayerGetMoney);
	registerMethod("Player", "addMoney", LuaScriptInterface::luaPlayerAddMoney);
	registerMethod("Player", "removeMoney", LuaScriptInterface::luaPlayerRemoveMoney);

	registerMethod("Player", "showTextDialog", LuaScriptInterface::luaPlayerShowTextDialog);

	registerMethod("Player", "sendTextMessage", LuaScriptInterface::luaPlayerSendTextMessage);
	registerMethod("Player", "sendChannelMessage", LuaScriptInterface::luaPlayerSendChannelMessage);
	registerMethod("Player", "sendPrivateMessage", LuaScriptInterface::luaPlayerSendPrivateMessage);
	registerMethod("Player", "channelSay", LuaScriptInterface::luaPlayerChannelSay);
	registerMethod("Player", "openChannel", LuaScriptInterface::luaPlayerOpenChannel);

	registerMethod("Player", "getSlotItem", LuaScriptInterface::luaPlayerGetSlotItem);

	registerMethod("Player", "getParty", LuaScriptInterface::luaPlayerGetParty);

	registerMethod("Player", "addOutfit", LuaScriptInterface::luaPlayerAddOutfit);
	registerMethod("Player", "addOutfitAddon", LuaScriptInterface::luaPlayerAddOutfitAddon);
	registerMethod("Player", "removeOutfit", LuaScriptInterface::luaPlayerRemoveOutfit);
	registerMethod("Player", "removeOutfitAddon", LuaScriptInterface::luaPlayerRemoveOutfitAddon);
	registerMethod("Player", "hasOutfit", LuaScriptInterface::luaPlayerHasOutfit);
	registerMethod("Player", "sendOutfitWindow", LuaScriptInterface::luaPlayerSendOutfitWindow);

	registerMethod("Player", "addMount", LuaScriptInterface::luaPlayerAddMount);
	registerMethod("Player", "removeMount", LuaScriptInterface::luaPlayerRemoveMount);
	registerMethod("Player", "hasMount", LuaScriptInterface::luaPlayerHasMount);

	registerMethod("Player", "getPremiumDays", LuaScriptInterface::luaPlayerGetPremiumDays);
	registerMethod("Player", "addPremiumDays", LuaScriptInterface::luaPlayerAddPremiumDays);
	registerMethod("Player", "removePremiumDays", LuaScriptInterface::luaPlayerRemovePremiumDays);

	registerMethod("Player", "hasBlessing", LuaScriptInterface::luaPlayerHasBlessing);
	registerMethod("Player", "addBlessing", LuaScriptInterface::luaPlayerAddBlessing);
	registerMethod("Player", "removeBlessing", LuaScriptInterface::luaPlayerRemoveBlessing);

	registerMethod("Player", "canLearnSpell", LuaScriptInterface::luaPlayerCanLearnSpell);
	registerMethod("Player", "learnSpell", LuaScriptInterface::luaPlayerLearnSpell);
	registerMethod("Player", "forgetSpell", LuaScriptInterface::luaPlayerForgetSpell);
	registerMethod("Player", "hasLearnedSpell", LuaScriptInterface::luaPlayerHasLearnedSpell);

	registerMethod("Player", "sendTutorial", LuaScriptInterface::luaPlayerSendTutorial);
	registerMethod("Player", "addMapMark", LuaScriptInterface::luaPlayerAddMapMark);

	registerMethod("Player", "save", LuaScriptInterface::luaPlayerSave);
	registerMethod("Player", "popupFYI", LuaScriptInterface::luaPlayerPopupFYI);

	registerMethod("Player", "isPzLocked", LuaScriptInterface::luaPlayerIsPzLocked);

	registerMethod("Player", "getClient", LuaScriptInterface::luaPlayerGetClient);
	registerMethod("Player", "getHouse", LuaScriptInterface::luaPlayerGetHouse);

	registerMethod("Player", "setGhostMode", LuaScriptInterface::luaPlayerSetGhostMode);

	registerMethod("Player", "getContainerId", LuaScriptInterface::luaPlayerGetContainerId);
	registerMethod("Player", "getContainerById", LuaScriptInterface::luaPlayerGetContainerById);
	registerMethod("Player", "getContainerIndex", LuaScriptInterface::luaPlayerGetContainerIndex);

	// Monster
	registerClass("Monster", "Creature", LuaScriptInterface::luaMonsterCreate);
	registerMetaMethod("Monster", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Monster", "isMonster", LuaScriptInterface::luaMonsterIsMonster);

	registerMethod("Monster", "getType", LuaScriptInterface::luaMonsterGetType);

	registerMethod("Monster", "getSpawnPosition", LuaScriptInterface::luaMonsterGetSpawnPosition);
	registerMethod("Monster", "despawn", LuaScriptInterface::luaMonsterDespawn);

	registerMethod("Monster", "isIdle", LuaScriptInterface::luaMonsterIsIdle);
	registerMethod("Monster", "setIdle", LuaScriptInterface::luaMonsterSetIdle);

	registerMethod("Monster", "isTarget", LuaScriptInterface::luaMonsterIsTarget);
	registerMethod("Monster", "isOpponent", LuaScriptInterface::luaMonsterIsOpponent);
	registerMethod("Monster", "isFriend", LuaScriptInterface::luaMonsterIsFriend);

	registerMethod("Monster", "addFriend", LuaScriptInterface::luaMonsterAddFriend);
	registerMethod("Monster", "removeFriend", LuaScriptInterface::luaMonsterRemoveFriend);
	registerMethod("Monster", "getFriendList", LuaScriptInterface::luaMonsterGetFriendList);
	registerMethod("Monster", "getFriendCount", LuaScriptInterface::luaMonsterGetFriendCount);

	registerMethod("Monster", "addTarget", LuaScriptInterface::luaMonsterAddTarget);
	registerMethod("Monster", "removeTarget", LuaScriptInterface::luaMonsterRemoveTarget);
	registerMethod("Monster", "getTargetList", LuaScriptInterface::luaMonsterGetTargetList);
	registerMethod("Monster", "getTargetCount", LuaScriptInterface::luaMonsterGetTargetCount);

	registerMethod("Monster", "selectTarget", LuaScriptInterface::luaMonsterSelectTarget);
	registerMethod("Monster", "searchTarget", LuaScriptInterface::luaMonsterSearchTarget);

	// Npc
	registerClass("Npc", "Creature", LuaScriptInterface::luaNpcCreate);
	registerMetaMethod("Npc", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Npc", "isNpc", LuaScriptInterface::luaNpcIsNpc);

	registerMethod("Npc", "setMasterPos", LuaScriptInterface::luaNpcSetMasterPos);

	registerMethod("Npc", "getSpeechBubble", LuaScriptInterface::luaNpcGetSpeechBubble);
	registerMethod("Npc", "setSpeechBubble", LuaScriptInterface::luaNpcSetSpeechBubble);

	// Guild
	registerClass("Guild", "", LuaScriptInterface::luaGuildCreate);
	registerMetaMethod("Guild", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Guild", "getId", LuaScriptInterface::luaGuildGetId);
	registerMethod("Guild", "getName", LuaScriptInterface::luaGuildGetName);
	registerMethod("Guild", "getMembersOnline", LuaScriptInterface::luaGuildGetMembersOnline);

	registerMethod("Guild", "addMember", LuaScriptInterface::luaGuildAddMember);
	registerMethod("Guild", "removeMember", LuaScriptInterface::luaGuildRemoveMember);

	registerMethod("Guild", "addRank", LuaScriptInterface::luaGuildAddRank);
	registerMethod("Guild", "getRankById", LuaScriptInterface::luaGuildGetRankById);
	registerMethod("Guild", "getRankByLevel", LuaScriptInterface::luaGuildGetRankByLevel);

	registerMethod("Guild", "getMotd", LuaScriptInterface::luaGuildGetMotd);
	registerMethod("Guild", "setMotd", LuaScriptInterface::luaGuildSetMotd);

	// Group
	registerClass("Group", "", LuaScriptInterface::luaGroupCreate);
	registerMetaMethod("Group", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Group", "getId", LuaScriptInterface::luaGroupGetId);
	registerMethod("Group", "getName", LuaScriptInterface::luaGroupGetName);
	registerMethod("Group", "getFlags", LuaScriptInterface::luaGroupGetFlags);
	registerMethod("Group", "getAccess", LuaScriptInterface::luaGroupGetAccess);
	registerMethod("Group", "getMaxDepotItems", LuaScriptInterface::luaGroupGetMaxDepotItems);
	registerMethod("Group", "getMaxVipEntries", LuaScriptInterface::luaGroupGetMaxVipEntries);

	// Vocation
	registerClass("Vocation", "", LuaScriptInterface::luaVocationCreate);
	registerMetaMethod("Vocation", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Vocation", "getId", LuaScriptInterface::luaVocationGetId);
	registerMethod("Vocation", "getClientId", LuaScriptInterface::luaVocationGetClientId);
	registerMethod("Vocation", "getName", LuaScriptInterface::luaVocationGetName);
	registerMethod("Vocation", "getDescription", LuaScriptInterface::luaVocationGetDescription);

	registerMethod("Vocation", "getRequiredSkillTries", LuaScriptInterface::luaVocationGetRequiredSkillTries);
	registerMethod("Vocation", "getRequiredManaSpent", LuaScriptInterface::luaVocationGetRequiredManaSpent);

	registerMethod("Vocation", "getCapacityGain", LuaScriptInterface::luaVocationGetCapacityGain);

	registerMethod("Vocation", "getHealthGain", LuaScriptInterface::luaVocationGetHealthGain);
	registerMethod("Vocation", "getHealthGainTicks", LuaScriptInterface::luaVocationGetHealthGainTicks);
	registerMethod("Vocation", "getHealthGainAmount", LuaScriptInterface::luaVocationGetHealthGainAmount);

	registerMethod("Vocation", "getManaGain", LuaScriptInterface::luaVocationGetManaGain);
	registerMethod("Vocation", "getManaGainTicks", LuaScriptInterface::luaVocationGetManaGainTicks);
	registerMethod("Vocation", "getManaGainAmount", LuaScriptInterface::luaVocationGetManaGainAmount);

	registerMethod("Vocation", "getMaxSoul", LuaScriptInterface::luaVocationGetMaxSoul);
	registerMethod("Vocation", "getSoulGainTicks", LuaScriptInterface::luaVocationGetSoulGainTicks);

	registerMethod("Vocation", "getAttackSpeed", LuaScriptInterface::luaVocationGetAttackSpeed);
	registerMethod("Vocation", "getBaseSpeed", LuaScriptInterface::luaVocationGetBaseSpeed);

	registerMethod("Vocation", "getDemotion", LuaScriptInterface::luaVocationGetDemotion);
	registerMethod("Vocation", "getPromotion", LuaScriptInterface::luaVocationGetPromotion);

	// Town
	registerClass("Town", "", LuaScriptInterface::luaTownCreate);
	registerMetaMethod("Town", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Town", "getId", LuaScriptInterface::luaTownGetId);
	registerMethod("Town", "getName", LuaScriptInterface::luaTownGetName);
	registerMethod("Town", "getTemplePosition", LuaScriptInterface::luaTownGetTemplePosition);

	// House
	registerClass("House", "", LuaScriptInterface::luaHouseCreate);
	registerMetaMethod("House", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("House", "getId", LuaScriptInterface::luaHouseGetId);
	registerMethod("House", "getName", LuaScriptInterface::luaHouseGetName);
	registerMethod("House", "getTown", LuaScriptInterface::luaHouseGetTown);
	registerMethod("House", "getExitPosition", LuaScriptInterface::luaHouseGetExitPosition);
	registerMethod("House", "getRent", LuaScriptInterface::luaHouseGetRent);

	registerMethod("House", "getOwnerGuid", LuaScriptInterface::luaHouseGetOwnerGuid);
	registerMethod("House", "setOwnerGuid", LuaScriptInterface::luaHouseSetOwnerGuid);

	registerMethod("House", "getBeds", LuaScriptInterface::luaHouseGetBeds);
	registerMethod("House", "getBedCount", LuaScriptInterface::luaHouseGetBedCount);

	registerMethod("House", "getDoors", LuaScriptInterface::luaHouseGetDoors);
	registerMethod("House", "getDoorCount", LuaScriptInterface::luaHouseGetDoorCount);

	registerMethod("House", "getTiles", LuaScriptInterface::luaHouseGetTiles);
	registerMethod("House", "getTileCount", LuaScriptInterface::luaHouseGetTileCount);

	registerMethod("House", "getAccessList", LuaScriptInterface::luaHouseGetAccessList);
	registerMethod("House", "setAccessList", LuaScriptInterface::luaHouseSetAccessList);

	// ItemType
	registerClass("ItemType", "", LuaScriptInterface::luaItemTypeCreate);
	registerMetaMethod("ItemType", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("ItemType", "isCorpse", LuaScriptInterface::luaItemTypeIsCorpse);
	registerMethod("ItemType", "isDoor", LuaScriptInterface::luaItemTypeIsDoor);
	registerMethod("ItemType", "isContainer", LuaScriptInterface::luaItemTypeIsContainer);
	registerMethod("ItemType", "isFluidContainer", LuaScriptInterface::luaItemTypeIsFluidContainer);
	registerMethod("ItemType", "isMovable", LuaScriptInterface::luaItemTypeIsMovable);
	registerMethod("ItemType", "isRune", LuaScriptInterface::luaItemTypeIsRune);
	registerMethod("ItemType", "isStackable", LuaScriptInterface::luaItemTypeIsStackable);
	registerMethod("ItemType", "isReadable", LuaScriptInterface::luaItemTypeIsReadable);
	registerMethod("ItemType", "isWritable", LuaScriptInterface::luaItemTypeIsWritable);

	registerMethod("ItemType", "getType", LuaScriptInterface::luaItemTypeGetType);
	registerMethod("ItemType", "getId", LuaScriptInterface::luaItemTypeGetId);
	registerMethod("ItemType", "getName", LuaScriptInterface::luaItemTypeGetName);
	registerMethod("ItemType", "getPluralName", LuaScriptInterface::luaItemTypeGetPluralName);
	registerMethod("ItemType", "getArticle", LuaScriptInterface::luaItemTypeGetArticle);
	registerMethod("ItemType", "getDescription", LuaScriptInterface::luaItemTypeGetDescription);
	registerMethod("ItemType", "getSlotPosition", LuaScriptInterface::luaItemTypeGetSlotPosition);

	registerMethod("ItemType", "getFluidSource", LuaScriptInterface::luaItemTypeGetFluidSource);
	registerMethod("ItemType", "getCapacity", LuaScriptInterface::luaItemTypeGetCapacity);
	registerMethod("ItemType", "getWeight", LuaScriptInterface::luaItemTypeGetWeight);

	registerMethod("ItemType", "getAttack", LuaScriptInterface::luaItemTypeGetAttack);
	registerMethod("ItemType", "getDefense", LuaScriptInterface::luaItemTypeGetDefense);
	registerMethod("ItemType", "getExtraDefense", LuaScriptInterface::luaItemTypeGetExtraDefense);
	registerMethod("ItemType", "getArmor", LuaScriptInterface::luaItemTypeGetArmor);
	registerMethod("ItemType", "getWeaponType", LuaScriptInterface::luaItemTypeGetWeaponType);

	registerMethod("ItemType", "getElementType", LuaScriptInterface::luaItemTypeGetElementType);
	registerMethod("ItemType", "getElementDamage", LuaScriptInterface::luaItemTypeGetElementDamage);

	registerMethod("ItemType", "getTransformEquipId", LuaScriptInterface::luaItemTypeGetTransformEquipId);
	registerMethod("ItemType", "getTransformDeEquipId", LuaScriptInterface::luaItemTypeGetTransformDeEquipId);
	registerMethod("ItemType", "getDecayId", LuaScriptInterface::luaItemTypeGetDecayId);

	registerMethod("ItemType", "hasSubType", LuaScriptInterface::luaItemTypeHasSubType);

	// Combat
	registerClass("Combat", "", LuaScriptInterface::luaCombatCreate);
	registerMetaMethod("Combat", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Combat", "setParameter", LuaScriptInterface::luaCombatSetParameter);
	registerMethod("Combat", "setFormula", LuaScriptInterface::luaCombatSetFormula);

	registerMethod("Combat", "setArea", LuaScriptInterface::luaCombatSetArea);
	registerMethod("Combat", "setCondition", LuaScriptInterface::luaCombatSetCondition);
	registerMethod("Combat", "setCallback", LuaScriptInterface::luaCombatSetCallback);

	registerMethod("Combat", "execute", LuaScriptInterface::luaCombatExecute);

	// Condition
	registerClass("Condition", "", LuaScriptInterface::luaConditionCreate);
	registerMetaMethod("Condition", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Condition", "getId", LuaScriptInterface::luaConditionGetId);
	registerMethod("Condition", "getSubId", LuaScriptInterface::luaConditionGetSubId);
	registerMethod("Condition", "getType", LuaScriptInterface::luaConditionGetType);
	registerMethod("Condition", "getIcons", LuaScriptInterface::luaConditionGetIcons);
	registerMethod("Condition", "getEndTime", LuaScriptInterface::luaConditionGetEndTime);

	registerMethod("Condition", "clone", LuaScriptInterface::luaConditionClone);

	registerMethod("Condition", "getTicks", LuaScriptInterface::luaConditionGetTicks);
	registerMethod("Condition", "setTicks", LuaScriptInterface::luaConditionSetTicks);

	registerMethod("Condition", "setParameter", LuaScriptInterface::luaConditionSetParameter);
	registerMethod("Condition", "setFormula", LuaScriptInterface::luaConditionSetFormula);

	registerMethod("Condition", "addDamage", LuaScriptInterface::luaConditionAddDamage);
	registerMethod("Condition", "addOutfit", LuaScriptInterface::luaConditionAddOutfit);

	// MonsterType
	registerClass("MonsterType", "", LuaScriptInterface::luaMonsterTypeCreate);
	registerMetaMethod("MonsterType", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("MonsterType", "isAttackable", LuaScriptInterface::luaMonsterTypeIsAttackable);
	registerMethod("MonsterType", "isConvinceable", LuaScriptInterface::luaMonsterTypeIsConvinceable);
	registerMethod("MonsterType", "isSummonable", LuaScriptInterface::luaMonsterTypeIsSummonable);
	registerMethod("MonsterType", "isIllusionable", LuaScriptInterface::luaMonsterTypeIsIllusionable);
	registerMethod("MonsterType", "isHostile", LuaScriptInterface::luaMonsterTypeIsHostile);
	registerMethod("MonsterType", "isPushable", LuaScriptInterface::luaMonsterTypeIsPushable);
	registerMethod("MonsterType", "isHealthShown", LuaScriptInterface::luaMonsterTypeIsHealthShown);

	registerMethod("MonsterType", "canPushItems", LuaScriptInterface::luaMonsterTypeCanPushItems);
	registerMethod("MonsterType", "canPushCreatures", LuaScriptInterface::luaMonsterTypeCanPushCreatures);

	registerMethod("MonsterType", "getName", LuaScriptInterface::luaMonsterTypeGetName);
	registerMethod("MonsterType", "getNameDescription", LuaScriptInterface::luaMonsterTypeGetNameDescription);

	registerMethod("MonsterType", "getHealth", LuaScriptInterface::luaMonsterTypeGetHealth);
	registerMethod("MonsterType", "getMaxHealth", LuaScriptInterface::luaMonsterTypeGetMaxHealth);
	registerMethod("MonsterType", "getRunHealth", LuaScriptInterface::luaMonsterTypeGetRunHealth);
	registerMethod("MonsterType", "getExperience", LuaScriptInterface::luaMonsterTypeGetExperience);

	registerMethod("MonsterType", "getCombatImmunities", LuaScriptInterface::luaMonsterTypeGetCombatImmunities);
	registerMethod("MonsterType", "getConditionImmunities", LuaScriptInterface::luaMonsterTypeGetConditionImmunities);

	registerMethod("MonsterType", "getAttackList", LuaScriptInterface::luaMonsterTypeGetAttackList);
	registerMethod("MonsterType", "getDefenseList", LuaScriptInterface::luaMonsterTypeGetDefenseList);
	registerMethod("MonsterType", "getElementList", LuaScriptInterface::luaMonsterTypeGetElementList);

	registerMethod("MonsterType", "getVoices", LuaScriptInterface::luaMonsterTypeGetVoices);
	registerMethod("MonsterType", "getLoot", LuaScriptInterface::luaMonsterTypeGetLoot);
	registerMethod("MonsterType", "getCreatureEvents", LuaScriptInterface::luaMonsterTypeGetCreatureEvents);

	registerMethod("MonsterType", "getSummonList", LuaScriptInterface::luaMonsterTypeGetSummonList);
	registerMethod("MonsterType", "getMaxSummons", LuaScriptInterface::luaMonsterTypeGetMaxSummons);

	registerMethod("MonsterType", "getArmor", LuaScriptInterface::luaMonsterTypeGetArmor);
	registerMethod("MonsterType", "getDefense", LuaScriptInterface::luaMonsterTypeGetDefense);
	registerMethod("MonsterType", "getOutfit", LuaScriptInterface::luaMonsterTypeGetOutfit);
	registerMethod("MonsterType", "getRace", LuaScriptInterface::luaMonsterTypeGetRace);
	registerMethod("MonsterType", "getCorpseId", LuaScriptInterface::luaMonsterTypeGetCorpseId);
	registerMethod("MonsterType", "getManaCost", LuaScriptInterface::luaMonsterTypeGetManaCost);
	registerMethod("MonsterType", "getBaseSpeed", LuaScriptInterface::luaMonsterTypeGetBaseSpeed);
	registerMethod("MonsterType", "getLight", LuaScriptInterface::luaMonsterTypeGetLight);

	registerMethod("MonsterType", "getStaticAttackChance", LuaScriptInterface::luaMonsterTypeGetStaticAttackChance);
	registerMethod("MonsterType", "getTargetDistance", LuaScriptInterface::luaMonsterTypeGetTargetDistance);
	registerMethod("MonsterType", "getYellChance", LuaScriptInterface::luaMonsterTypeGetYellChance);
	registerMethod("MonsterType", "getYellSpeedTicks", LuaScriptInterface::luaMonsterTypeGetYellSpeedTicks);
	registerMethod("MonsterType", "getChangeTargetChance", LuaScriptInterface::luaMonsterTypeGetChangeTargetChance);
	registerMethod("MonsterType", "getChangeTargetSpeed", LuaScriptInterface::luaMonsterTypeGetChangeTargetSpeed);

	// Party
	registerClass("Party", "", nullptr);
	registerMetaMethod("Party", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod("Party", "disband", LuaScriptInterface::luaPartyDisband);

	registerMethod("Party", "getLeader", LuaScriptInterface::luaPartyGetLeader);
	registerMethod("Party", "setLeader", LuaScriptInterface::luaPartySetLeader);

	registerMethod("Party", "getMembers", LuaScriptInterface::luaPartyGetMembers);
	registerMethod("Party", "getMemberCount", LuaScriptInterface::luaPartyGetMemberCount);

	registerMethod("Party", "getInvitees", LuaScriptInterface::luaPartyGetInvitees);
	registerMethod("Party", "getInviteeCount", LuaScriptInterface::luaPartyGetInviteeCount);

	registerMethod("Party", "addInvite", LuaScriptInterface::luaPartyAddInvite);
	registerMethod("Party", "removeInvite", LuaScriptInterface::luaPartyRemoveInvite);

	registerMethod("Party", "addMember", LuaScriptInterface::luaPartyAddMember);
	registerMethod("Party", "removeMember", LuaScriptInterface::luaPartyRemoveMember);

	registerMethod("Party", "isSharedExperienceActive", LuaScriptInterface::luaPartyIsSharedExperienceActive);
	registerMethod("Party", "isSharedExperienceEnabled", LuaScriptInterface::luaPartyIsSharedExperienceEnabled);
	registerMethod("Party", "shareExperience", LuaScriptInterface::luaPartyShareExperience);
	registerMethod("Party", "setSharedExperience", LuaScriptInterface::luaPartySetSharedExperience);
}

#undef registerEnum
#undef registerEnumIn

void LuaScriptInterface::registerClass(const std::string& className, const std::string& baseClass, lua_CFunction newFunction/* = nullptr*/)
{
	// className = {}
	lua_newtable(m_luaState);
	lua_pushvalue(m_luaState, -1);
	lua_setglobal(m_luaState, className.c_str());
	int32_t methods = lua_gettop(m_luaState);

	// methodsTable = {}
	lua_newtable(m_luaState);
	int32_t methodsTable = lua_gettop(m_luaState);

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
		parents = getNumber<uint32_t>(m_luaState, -1) + 1;
		lua_pop(m_luaState, 1);
		lua_setfield(m_luaState, methodsTable, "__index");
	}

	// setmetatable(className, methodsTable)
	lua_setmetatable(m_luaState, methods);

	// className.metatable = {}
	luaL_newmetatable(m_luaState, className.c_str());
	int32_t metatable = lua_gettop(m_luaState);

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

void LuaScriptInterface::registerTable(const std::string& tableName)
{
	// _G[tableName] = {}
	lua_newtable(m_luaState);
	lua_setglobal(m_luaState, tableName.c_str());
}

void LuaScriptInterface::registerMethod(const std::string& globalName, const std::string& methodName, lua_CFunction func)
{
	// globalName.methodName = func
	lua_getglobal(m_luaState, globalName.c_str());
	lua_pushcfunction(m_luaState, func);
	lua_setfield(m_luaState, -2, methodName.c_str());

	// pop globalName
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

void LuaScriptInterface::registerVariable(const std::string& tableName, const std::string& name, lua_Number value)
{
	// tableName.name = value
	lua_getglobal(m_luaState, tableName.c_str());
	setField(m_luaState, name.c_str(), value);

	// pop tableName
	lua_pop(m_luaState, 1);
}

void LuaScriptInterface::registerGlobalVariable(const std::string& name, lua_Number value)
{
	// _G[name] = value
	lua_pushnumber(m_luaState, value);
	lua_setglobal(m_luaState, name.c_str());
}

void LuaScriptInterface::registerGlobalBoolean(const std::string& name, bool value)
{
	// _G[name] = value
	lua_pushboolean(m_luaState, value ? 1 : 0);
	lua_setglobal(m_luaState, name.c_str());
}

int32_t LuaScriptInterface::luaGetPlayerFlagValue(lua_State* L)
{
	//getPlayerFlagValue(cid, flag)
	Player* player = getPlayer(L, 1);
	if (player) {
		uint64_t flag = getNumber<uint64_t>(L, 2);
		pushBoolean(L, player->hasFlag(static_cast<PlayerFlags>(flag)));
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetPlayerInstantSpellCount(lua_State* L)
{
	//getPlayerInstantSpellCount(cid)
	Player* player = getPlayer(L, 1);
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
	Player* player = getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t index = getNumber<uint32_t>(L, 2);
	InstantSpell* spell = g_spells->getInstantSpellByIndex(player, index);
	if (!spell) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_SPELL_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	lua_createtable(L, 0, 6);
	setField(L, "name", spell->getName());
	setField(L, "words", spell->getWords());
	setField(L, "level", spell->getLevel());
	setField(L, "mlevel", spell->getMagicLevel());
	setField(L, "mana", spell->getManaCost(player));
	setField(L, "manapercent", spell->getManaPercent());
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerRemoveItem(lua_State* L)
{
	//doPlayerRemoveItem(cid, itemid, count, <optional> subtype, <optional> ignoreEquipped)
	Player* player = getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint16_t itemId = getNumber<uint16_t>(L, 2);
	uint32_t count = getNumber<uint32_t>(L, 3);
	int32_t subType = getNumber<int32_t>(L, 4, -1);
	bool ignoreEquipped = getBoolean(L, 5, false);
	pushBoolean(L, player->removeItemOfType(itemId, count, subType, ignoreEquipped));
	return 1;
}

int32_t LuaScriptInterface::luaDoPlayerAddItem(lua_State* L)
{
	//doPlayerAddItem(cid, itemid, <optional: default: 1> count/subtype, <optional: default: 1> canDropOnMap)
	//doPlayerAddItem(cid, itemid, <optional: default: 1> count, <optional: default: 1> canDropOnMap, <optional: default: 1>subtype)
	Player* player = getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t itemId = getNumber<uint32_t>(L, 2);
	int32_t count = getNumber<int32_t>(L, 3, 1);
	bool canDropOnMap = getBoolean(L, 4, true);
	uint16_t subType = getNumber<uint16_t>(L, 5, 1);

	const ItemType& it = Item::items[itemId];
	int32_t itemCount;

	auto parameters = lua_gettop(L);
	if (parameters > 4) {
		//subtype already supplied, count then is the amount
		itemCount = std::max<int32_t>(1, count);
	} else if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = (int32_t)std::ceil(static_cast<float>(count) / 100);
		} else {
			itemCount = 1;
		}
		subType = count;
	} else {
		itemCount = std::max<int32_t>(1, count);
	}

	while (itemCount > 0) {
		uint16_t stackCount = subType;
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
				uint32_t uid = getScriptEnv()->addThing(newItem);
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
	const Position& pos = getPosition(L, 1);

	Tile* tile = g_game.getTile(pos.x, pos.y, pos.z);
	if (!tile) {
		std::ostringstream ss;
		ss << pos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	uint32_t uid = getNumber<uint32_t>(L, 2);
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

	lua_pushnumber(L, g_game.internalAddItem(tile, item));
	return 1;
}

int32_t LuaScriptInterface::luaDoRelocate(lua_State* L)
{
	//doRelocate(pos, posTo)
	//Moves all moveable objects from pos to posTo
	const Position& fromPos = getPosition(L, 1);
	Tile* fromTile = g_game.getTile(fromPos.x, fromPos.y, fromPos.z);
	if (!fromTile) {
		std::ostringstream ss;
		ss << fromPos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	const Position& toPos = getPosition(L, 2);
	Tile* toTile = g_game.getTile(toPos.x, toPos.y, toPos.z);
	if (!toTile) {
		std::ostringstream ss;
		ss << toPos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	if (fromTile != toTile) {
		for (int32_t i = fromTile->getThingCount(); --i >= 0;) {
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

int32_t LuaScriptInterface::luaGetThingfromPos(lua_State* L)
{
	//Consider using getTileItemById/getTileItemByType/getTileThingByPos/getTopCreature instead.

	//getThingfromPos(pos)
	//Note:
	//	stackpos = 255. Get the top thing(item moveable or creature)
	//	stackpos = 254. Get MagicFieldtItem
	//	stackpos = 253. Get the top creature (moveable creature)

	int32_t stackpos;
	const Position& pos = getPosition(L, 1, stackpos);

	Tile* tile = g_game.getTile(pos);
	if (!tile) {
		pushThing(L, nullptr, 0);
		return 1;
	}

	Thing* thing;
	switch (stackpos) {
		case 255: {
			thing = tile->getTopCreature();
			if (!thing) {
				Item* item = tile->getTopDownItem();
				if (item && item->isMoveable()) {
					thing = item;
				}
			}
			break;
		}

		case 254: {
			thing = tile->getFieldItem();
			break;
		}

		case 253: {
			thing = tile->getTopCreature();
			break;
		}

		default: {
			thing = tile->__getThing(stackpos);
			break;
		}
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
	const Position& pos = getPosition(L, 3);
	Tile* tile = g_game.getTile(pos.x, pos.y, pos.z);
	if (!tile) {
		std::ostringstream ss;
		ss << pos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	ScriptEnvironment* env = getScriptEnv();

	int32_t itemCount = 1;
	int32_t subType = 1;

	uint32_t itemId = getNumber<uint32_t>(L, 1);
	uint32_t count = getNumber<uint32_t>(L, 2, 1);

	const ItemType& it = Item::items[itemId];
	if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = (int32_t)std::ceil(static_cast<float>(count) / 100);
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
	uint32_t itemId = getNumber<uint32_t>(L, 1);
	uint32_t count = getNumber<uint32_t>(L, 2, 1);

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

	ScriptEnvironment* env = getScriptEnv();
	env->addTempItem(env, newItem);

	uint32_t uid = env->addThing(newItem);
	lua_pushnumber(L, uid);
	return 1;
}

int32_t LuaScriptInterface::luaDoCreateTeleport(lua_State* L)
{
	//doCreateTeleport(itemid, topos, createpos)
	const Position& createPos = getPosition(L, 3);
	Tile* tile = g_game.getTile(createPos);
	if (!tile) {
		std::ostringstream ss;
		ss << createPos << ' ' << getErrorDesc(LUA_ERROR_TILE_NOT_FOUND);
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	uint32_t itemId = getNumber<uint32_t>(L, 1);
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

	const Position& toPos = getPosition(L, 2);
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

int32_t LuaScriptInterface::luaDebugPrint(lua_State* L)
{
	//debugPrint(text)
	reportErrorFunc(getString(L, -1));
	return 0;
}

int32_t LuaScriptInterface::luaGetHouseByPlayerGUID(lua_State* L)
{
	//getHouseByPlayerGUID(playerGUID)
	uint32_t guid = getNumber<uint32_t>(L, -1);

	House* house = Houses::getInstance().getHouseByPlayerId(guid);
	if (house) {
		lua_pushnumber(L, house->getId());
	} else {
		lua_pushnil(L);
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

int32_t LuaScriptInterface::luaGetWorldUpTime(lua_State* L)
{
	//getWorldUpTime()
	uint64_t uptime = (OTSYS_TIME() - ProtocolStatus::start) / 1000;
	lua_pushnumber(L, uptime);
	return 1;
}

int32_t LuaScriptInterface::luaBroadcastMessage(lua_State* L)
{
	//broadcastMessage(message, type)
	const std::string& message = getString(L, 1);
	MessageClasses type = static_cast<MessageClasses>(getNumber<uint32_t>(L, 2, MESSAGE_STATUS_WARNING));
	g_game.broadcastMessage(message, type);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetThingPos(lua_State* L)
{
	//getThingPos(uid)
	uint32_t uid = getNumber<uint32_t>(L, -1);
	Thing* thing = getScriptEnv()->getThingByUID(uid);
	if (!thing) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t stackpos = 0;

	const Position& pos = thing->getPosition();
	if (Tile* tile = thing->getTile()) {
		stackpos = tile->__getIndexOfThing(thing);
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

	lua_pushnumber(L, g_luaEnvironment.createCombatObject(env->getScriptInterface()));
	return 1;
}

bool LuaScriptInterface::getArea(lua_State* L, std::list<uint32_t>& list, uint32_t& rows)
{
	lua_pushnil(L);
	for (rows = 0; lua_next(L, -2) != 0; ++rows) {
		if (!isTable(L, -1)) {
			return false;
		}

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (!isNumber(L, -1)) {
				return false;
			}
			list.push_back(getNumber<uint32_t>(L, -1));
			lua_pop(L, 1);
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

	uint32_t areaId = g_luaEnvironment.createAreaObject(env->getScriptInterface());
	AreaCombat* area = g_luaEnvironment.getAreaObject(areaId);

	int32_t parameters = lua_gettop(L);
	if (parameters >= 2) {
		uint32_t rowsExtArea;
		std::list<uint32_t> listExtArea;
		if (!getArea(L, listExtArea, rowsExtArea)) {
			reportErrorFunc("Invalid extended area table.");
			pushBoolean(L, false);
			return 1;
		}
		area->setupExtArea(listExtArea, rowsExtArea);
	}

	uint32_t rowsArea = 0;
	std::list<uint32_t> listArea;
	if (!getArea(L, listArea, rowsArea)) {
		reportErrorFunc("Invalid area table.");
		pushBoolean(L, false);
		return 1;
	}

	area->setupArea(listArea, rowsArea);
	lua_pushnumber(L, areaId);
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

	ConditionType_t type = static_cast<ConditionType_t>(getNumber<uint32_t>(L, -1));

	uint32_t id;
	if (g_luaEnvironment.createConditionObject(env->getScriptInterface(), type, CONDITIONID_COMBAT, id)) {
		lua_pushnumber(L, id);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSetCombatArea(lua_State* L)
{
	//setCombatArea(combat, area)
	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	uint32_t combatId = getNumber<uint32_t>(L, 1);

	Combat* combat = g_luaEnvironment.getCombatObject(combatId);
	if (!combat) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t areaId = getNumber<uint32_t>(L, 2);

	const AreaCombat* area = g_luaEnvironment.getAreaObject(areaId);
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
	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	uint32_t combatId = getNumber<uint32_t>(L, 1);
	Combat* combat = g_luaEnvironment.getCombatObject(combatId);
	if (!combat) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t conditionId = getNumber<uint32_t>(L, 2);
	const Condition* condition = g_luaEnvironment.getConditionObject(conditionId);
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
	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	uint32_t combatId = getNumber<uint32_t>(L, 1);
	Combat* combat = g_luaEnvironment.getCombatObject(combatId);
	if (combat) {
		CombatParam_t key = (CombatParam_t)getNumber<uint32_t>(L, 2);
		uint32_t value = getNumber<uint32_t>(L, 3);
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
	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	uint32_t conditionId = getNumber<uint32_t>(L, 1);
	Condition* condition = g_luaEnvironment.getConditionObject(conditionId);
	if (!condition) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	ConditionParam_t key = static_cast<ConditionParam_t>(getNumber<uint32_t>(L, 2));

	int32_t value;
	if (isBoolean(L, 3)) {
		value = getBoolean(L, 3) ? 1 : 0;
	} else {
		value = getNumber<int32_t>(L, 3);
	}

	condition->setParam(key, value);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaAddDamageCondition(lua_State* L)
{
	//addDamageCondition(condition, rounds, time, value)
	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	uint32_t conditionId = getNumber<uint32_t>(L, 1);
	ConditionDamage* condition = dynamic_cast<ConditionDamage*>(g_luaEnvironment.getConditionObject(conditionId));
	if (condition) {
		int32_t rounds = getNumber<int32_t>(L, 2);
		int32_t time = getNumber<int32_t>(L, 3);
		int32_t value = getNumber<int32_t>(L, 4);
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
	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	uint32_t conditionId = getNumber<uint32_t>(L, 1);
	ConditionOutfit* condition = dynamic_cast<ConditionOutfit*>(g_luaEnvironment.getConditionObject(conditionId));
	if (condition) {
		Outfit_t outfit;
		outfit.lookTypeEx = getNumber<uint32_t>(L, 2);
		outfit.lookType = getNumber<uint32_t>(L, 3);
		outfit.lookHead = getNumber<uint32_t>(L, 4);
		outfit.lookBody = getNumber<uint32_t>(L, 5);
		outfit.lookLegs = getNumber<uint32_t>(L, 6);
		outfit.lookFeet = getNumber<uint32_t>(L, 7);
		outfit.lookAddons = getNumber<uint32_t>(L, 8, outfit.lookAddons);
		outfit.lookMount = getNumber<uint32_t>(L, 9, outfit.lookMount);
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
	ScriptEnvironment* env = getScriptEnv();
	if (env->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		pushBoolean(L, false);
		return 1;
	}

	uint32_t combatId = getNumber<uint32_t>(L, 1);

	Combat* combat = g_luaEnvironment.getCombatObject(combatId);
	if (!combat) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	CallBackParam_t key = (CallBackParam_t)getNumber<uint32_t>(L, 2);
	combat->setCallback(key);

	CallBack* callback = combat->getCallback(key);
	if (!callback) {
		std::ostringstream ss;
		ss << key << " is not a valid callback key.";
		reportErrorFunc(ss.str());
		pushBoolean(L, false);
		return 1;
	}

	std::string function = getString(L, 3);
	if (!callback->loadCallBack(env->getScriptInterface(), function)) {
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

	uint32_t combatId = getNumber<uint32_t>(L, 1);

	Combat* combat = g_luaEnvironment.getCombatObject(combatId);
	if (combat) {
		combat->setPlayerCombatValues(
			static_cast<formulaType_t>(getNumber<uint32_t>(L, 2)),
			getNumber<float>(L, 3),
			getNumber<float>(L, 4),
			getNumber<float>(L, 5),
			getNumber<float>(L, 6)
		);
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

	uint32_t conditionId = getNumber<uint32_t>(L, 1);

	ConditionSpeed* condition = dynamic_cast<ConditionSpeed*>(g_luaEnvironment.getConditionObject(conditionId));
	if (condition) {
		condition->setFormulaVars(
			getNumber<double>(L, 2),
			getNumber<double>(L, 3),
			getNumber<double>(L, 4),
			getNumber<double>(L, 5)
		);
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
	Creature* creature = getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t combatId = getNumber<uint32_t>(L, 2);
	const Combat* combat = g_luaEnvironment.getCombatObject(combatId);
	if (!combat) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	const LuaVariant& var = getVariant(L, 3);
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
				g_game.addMagicEffect(var.pos, CONST_ME_POFF);
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

		case VARIANT_NONE: {
			reportErrorFunc(getErrorDesc(LUA_ERROR_VARIANT_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
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
	Creature* creature = getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t areaId = getNumber<uint32_t>(L, 4);
	const AreaCombat* area = g_luaEnvironment.getAreaObject(areaId);
	if (area || areaId == 0) {
		CombatType_t combatType = static_cast<CombatType_t>(getNumber<uint32_t>(L, 2));

		CombatParams params;
		params.combatType = combatType;
		params.impactEffect = getNumber<uint8_t>(L, 7);

		CombatDamage damage;
		damage.primary.type = combatType;
		damage.primary.value = normal_random(getNumber<int32_t>(L, 6), getNumber<int32_t>(L, 5));

		Combat::doCombatHealth(creature, getPosition(L, 3), area, damage, params);
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
	Creature* creature = getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Creature* target = getCreature(L, 2);
	if (!target) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	CombatType_t combatType = static_cast<CombatType_t>(getNumber<uint32_t>(L, 3));

	CombatParams params;
	params.combatType = combatType;
	params.impactEffect = getNumber<uint8_t>(L, 6);

	CombatDamage damage;
	damage.primary.type = combatType;
	damage.primary.value = normal_random(getNumber<int32_t>(L, 4), getNumber<int32_t>(L, 5));

	Combat::doCombatHealth(creature, target, damage, params);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoAreaCombatMana(lua_State* L)
{
	//doAreaCombatMana(cid, pos, area, min, max, effect)
	Creature* creature = getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t areaId = getNumber<uint32_t>(L, 3);
	const AreaCombat* area = g_luaEnvironment.getAreaObject(areaId);
	if (area || areaId == 0) {
		CombatParams params;
		params.impactEffect = getNumber<uint8_t>(L, 6);

		CombatDamage damage;
		damage.primary.type = COMBAT_MANADRAIN;
		damage.primary.value = normal_random(getNumber<int32_t>(L, 4), getNumber<int32_t>(L, 5));

		PositionEx pos = getPosition(L, 2);
		Combat::doCombatMana(creature, pos, area, damage, params);
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
	Creature* creature = getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Creature* target = getCreature(L, 2);
	if (!target) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	CombatParams params;
	params.impactEffect = getNumber<uint8_t>(L, 5);

	CombatDamage damage;
	damage.primary.type = COMBAT_MANADRAIN;
	damage.primary.value = normal_random(getNumber<int32_t>(L, 3), getNumber<int32_t>(L, 4));

	Combat::doCombatMana(creature, target, damage, params);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoAreaCombatCondition(lua_State* L)
{
	//doAreaCombatCondition(cid, pos, area, condition, effect)
	Creature* creature = getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t conditionId = getNumber<uint32_t>(L, 4);
	const Condition* condition = g_luaEnvironment.getConditionObject(conditionId);
	if (!condition) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t areaId = getNumber<uint32_t>(L, 3);
	const AreaCombat* area = g_luaEnvironment.getAreaObject(areaId);
	if (area || areaId == 0) {
		CombatParams params;
		params.impactEffect = getNumber<uint8_t>(L, 5);
		params.conditionList.push_front(condition);
		Combat::doCombatCondition(creature, getPosition(L, 2), area, params);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_AREA_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDoTargetCombatCondition(lua_State* L)
{
	//doTargetCombatCondition(cid, target, condition, effect)
	Creature* creature = getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Creature* target = getCreature(L, 2);
	if (!target) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t conditionId = getNumber<uint32_t>(L, 3);
	const Condition* condition = g_luaEnvironment.getConditionObject(conditionId);
	if (!condition) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONDITION_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	CombatParams params;
	params.impactEffect = getNumber<uint8_t>(L, 4);
	params.conditionList.push_front(condition);
	Combat::doCombatCondition(creature, target, params);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoAreaCombatDispel(lua_State* L)
{
	//doAreaCombatDispel(cid, pos, area, type, effect)
	Creature* creature = getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t areaId = getNumber<uint32_t>(L, 3);
	const AreaCombat* area = g_luaEnvironment.getAreaObject(areaId);
	if (area || areaId == 0) {
		CombatParams params;
		params.impactEffect = getNumber<uint8_t>(L, 5);
		params.dispelType = static_cast<ConditionType_t>(getNumber<uint32_t>(L, 4));
		Combat::doCombatDispel(creature, getPosition(L, 2), area, params);

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
	Creature* creature = getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Creature* target = getCreature(L, 2);
	if (!target) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	CombatParams params;
	params.dispelType = (ConditionType_t)getNumber<uint32_t>(L, 3);
	params.impactEffect = getNumber<uint8_t>(L, 4);
	Combat::doCombatDispel(creature, target, params);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoChallengeCreature(lua_State* L)
{
	//doChallengeCreature(cid, target)
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Creature* target = getCreature(L, 2);
	if (!target) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	target->challengeCreature(creature);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaDoAddCondition(lua_State* L)
{
	//doAddCondition(cid, condition)
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t conditionId = getNumber<uint32_t>(L, 2);
	Condition* condition = g_luaEnvironment.getConditionObject(conditionId);
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
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	ConditionType_t conditionType = (ConditionType_t)getNumber<uint32_t>(L, 2);
	uint32_t subId = getNumber<uint32_t>(L, 3, 0);

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
	var.number = getNumber<uint32_t>(L, -1);
	LuaScriptInterface::pushVariant(L, var);
	return 1;
}

int32_t LuaScriptInterface::luaStringToVariant(lua_State* L)
{
	//stringToVariant(string)
	LuaVariant var;
	var.type = VARIANT_STRING;
	var.text = getString(L, -1);
	LuaScriptInterface::pushVariant(L, var);
	return 1;
}

int32_t LuaScriptInterface::luaPositionToVariant(lua_State* L)
{
	//positionToVariant(pos)
	LuaVariant var;
	var.type = VARIANT_POSITION;
	var.pos = getPosition(L, 1);
	LuaScriptInterface::pushVariant(L, var);
	return 1;
}

int32_t LuaScriptInterface::luaTargetPositionToVariant(lua_State* L)
{
	//targetPositionToVariant(pos)
	LuaVariant var;
	var.type = VARIANT_TARGETPOSITION;
	var.pos = getPosition(L, -1);
	LuaScriptInterface::pushVariant(L, var);
	return 1;
}

int32_t LuaScriptInterface::luaVariantToNumber(lua_State* L)
{
	//variantToNumber(var)
	const LuaVariant& var = getVariant(L, 1);
	if (var.type == VARIANT_NUMBER) {
		lua_pushnumber(L, var.number);
	} else {
		lua_pushnumber(L, 0);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVariantToString(lua_State* L)
{
	//variantToString(var)
	const LuaVariant& var = getVariant(L, 1);
	if (var.type == VARIANT_STRING) {
		pushString(L, var.text);
	} else {
		pushString(L, std::string());
	}
	return 1;
}

int32_t LuaScriptInterface::luaVariantToPosition(lua_State* L)
{
	//luaVariantToPosition(var)
	const LuaVariant& var = getVariant(L, 1);

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
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	int32_t delta = getNumber<int32_t>(L, 2);
	g_game.changeSpeed(creature, delta);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaSetCreatureOutfit(lua_State* L)
{
	//doSetCreatureOutfit(cid, outfit, time)
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Outfit_t outfit = getOutfit(L, 2);
	int32_t time = getNumber<int32_t>(L, 3);
	pushBoolean(L, Spell::CreateIllusion(creature, outfit, time) == RET_NOERROR);
	return 1;
}

int32_t LuaScriptInterface::luaSetMonsterOutfit(lua_State* L)
{
	//doSetMonsterOutfit(cid, name, time)
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	std::string name = getString(L, 2);
	int32_t time = getNumber<int32_t>(L, 3);
	pushBoolean(L, Spell::CreateIllusion(creature, name, time) == RET_NOERROR);
	return 1;
}

int32_t LuaScriptInterface::luaSetItemOutfit(lua_State* L)
{
	//doSetItemOutfit(cid, item, time)
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t item = getNumber<uint32_t>(L, 2);
	int32_t time = getNumber<int32_t>(L, 3);
	pushBoolean(L, Spell::CreateIllusion(creature, item, time) == RET_NOERROR);
	return 1;
}

int32_t LuaScriptInterface::luaDoMoveCreature(lua_State* L)
{
	//doMoveCreature(cid, direction)
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t direction = getNumber<uint32_t>(L, 2);
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

	ReturnValue ret = g_game.internalMoveCreature(creature, (Direction)direction, FLAG_NOLIMIT);
	lua_pushnumber(L, ret);
	return 1;
}

int32_t LuaScriptInterface::luaIsValidUID(lua_State* L)
{
	//isValidUID(uid)
	pushBoolean(L, getScriptEnv()->getThingByUID(getNumber<uint32_t>(L, -1)) != nullptr);
	return 1;
}

int32_t LuaScriptInterface::luaIsDepot(lua_State* L)
{
	//isDepot(uid)
	Container* container = getScriptEnv()->getContainerByUID(getNumber<uint32_t>(L, -1));
	pushBoolean(L, container && container->getDepotLocker());
	return 1;
}

int32_t LuaScriptInterface::luaIsMoveable(lua_State* L)
{
	//isMoveable(uid)
	//isMovable(uid)
	Thing* thing = getScriptEnv()->getThingByUID(getNumber<uint32_t>(L, -1));
	pushBoolean(L, thing && thing->isPushable());
	return 1;
}

int32_t LuaScriptInterface::luaDoAddContainerItem(lua_State* L)
{
	//doAddContainerItem(uid, itemid, <optional> count/subtype)
	uint32_t uid = getNumber<uint32_t>(L, 1);

	ScriptEnvironment* env = getScriptEnv();
	Container* container = env->getContainerByUID(uid);
	if (!container) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CONTAINER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint16_t itemId = getNumber<uint16_t>(L, 2);
	const ItemType& it = Item::items[itemId];

	int32_t itemCount = 1;
	int32_t subType = 1;
	uint32_t count = getNumber<uint32_t>(L, 3, 1);

	if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = (int32_t)std::ceil(static_cast<float>(count) / 100);
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
			} else {
				//stackable item stacked with existing object, newItem will be released
				pushBoolean(L, false);
			}
			return 1;
		}
	}

	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaGetDepotId(lua_State* L)
{
	//getDepotId(uid)
	uint32_t uid = getNumber<uint32_t>(L, -1);

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

int32_t LuaScriptInterface::luaIsInArray(lua_State* L)
{
	//isInArray(array, value)
	if (!isTable(L, 1)) {
		pushBoolean(L, false);
		return 1;
	}

	lua_pushnil(L);
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
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint8_t level = getNumber<uint8_t>(L, 2);
	uint8_t color = getNumber<uint8_t>(L, 3);
	uint32_t time = getNumber<uint32_t>(L, 4);
	Condition* condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_LIGHT, time, level | (color << 8));
	creature->addCondition(condition);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaHasProperty(lua_State* L)
{
	//hasProperty(uid, prop)
	uint32_t uid = getNumber<uint32_t>(L, 1);

	Item* item = getScriptEnv()->getItemByUID(uid);
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t prop = getNumber<uint32_t>(L, 2);

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

int32_t LuaScriptInterface::luaGetSpectators(lua_State* L)
{
	//getSpectators(centerPos, rangex, rangey, multifloor, onlyPlayers = false)
	bool onlyPlayers = getBoolean(L, 5, false);
	bool multifloor = getBoolean(L, 4);
	uint32_t rangey = getNumber<uint32_t>(L, 3);
	uint32_t rangex = getNumber<uint32_t>(L, 2);
	const Position& centerPos = getPosition(L, 1);

	SpectatorVec list;
	g_game.getSpectators(list, centerPos, multifloor, onlyPlayers, rangex, rangex, rangey, rangey);
	if (list.empty()) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, list.size(), 0);

	int32_t index = 0;
	for (Creature* spectator : list) {
		lua_pushnumber(L, spectator->getID());
		lua_rawseti(L, -2, ++index);
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

	int32_t parameters = lua_gettop(L);
	if (!isFunction(globalState, -parameters)) { //-parameters means the first parameter from left to right
		reportErrorFunc("callback parameter should be a function.");
		pushBoolean(L, false);
		return 1;
	}

	LuaTimerEventDesc eventDesc;
	for (int32_t i = 0; i < parameters - 2; ++i) { //-2 because addEvent needs at least two parameters
		eventDesc.parameters.push_back(luaL_ref(globalState, LUA_REGISTRYINDEX));
	}

	uint32_t delay = std::max<uint32_t>(100, getNumber<uint32_t>(globalState, 2));
	lua_pop(globalState, 1);
	eventDesc.function = luaL_ref(globalState, LUA_REGISTRYINDEX);
	eventDesc.scriptId = getScriptEnv()->getScriptId();

	auto& lastTimerEventId = g_luaEnvironment.m_lastEventTimerId;
	eventDesc.eventId = g_scheduler.addEvent(createSchedulerTask(
		delay, std::bind(&LuaEnvironment::executeTimerEvent, &g_luaEnvironment, lastTimerEventId)
	));

	g_luaEnvironment.m_timerEvents.insert(std::make_pair(lastTimerEventId, std::move(eventDesc)));

	lua_pushnumber(L, lastTimerEventId++);
	return 1;
}

int32_t LuaScriptInterface::luaStopEvent(lua_State* L)
{
	//stopEvent(eventid)
	lua_State* globalState = g_luaEnvironment.getLuaState();
	if (!globalState) {
		reportErrorFunc("No valid script interface!");
		pushBoolean(L, false);
		return 1;
	}

	uint32_t eventId = getNumber<uint32_t>(L, 1);

	auto& timerEvents = g_luaEnvironment.m_timerEvents;
	auto it = timerEvents.find(eventId);
	if (it == timerEvents.end()) {
		pushBoolean(L, false);
		return 1;
	}

	LuaTimerEventDesc timerEventDesc = std::move(it->second);
	timerEvents.erase(it);

	g_scheduler.stopEvent(timerEventDesc.eventId);
	luaL_unref(globalState, LUA_REGISTRYINDEX, timerEventDesc.function);

	for (auto parameter : timerEventDesc.parameters) {
		luaL_unref(globalState, LUA_REGISTRYINDEX, parameter);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGetCreatureCondition(lua_State* L)
{
	Creature* creature = getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t condition = getNumber<uint32_t>(L, 2);
	uint32_t subId = getNumber<uint32_t>(L, 3, 0);
	pushBoolean(L, creature->hasCondition((ConditionType_t)condition, subId));
	return 1;
}

int32_t LuaScriptInterface::luaSaveServer(lua_State* L)
{
	g_game.saveGameState();
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaCleanMap(lua_State* L)
{
	lua_pushnumber(L, g_game.getMap()->clean());
	return 1;
}

int32_t LuaScriptInterface::luaIsInWar(lua_State* L)
{
	//isInWar(cid, target)
	Player* player = getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Player* targetPlayer = getPlayer(L, 2);
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
	Player* player = getPlayer(L, 1);
	if (player) {
		uint32_t skillid = getNumber<uint32_t>(L, 2);
		player->setOfflineTrainingSkill(skillid);
		pushBoolean(L, true);
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGetWaypointPositionByName(lua_State* L)
{
	//getWaypointPositionByName(name)
	auto& waypoints = g_game.getMap()->waypoints;
	auto it = waypoints.find(getString(L, -1));
	if (it != waypoints.end()) {
		pushPosition(L, it->second);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaSendChannelMessage(lua_State* L)
{
	//sendChannelMessage(channelId, type, message)
	uint32_t channelId = getNumber<uint32_t>(L, 1);
	ChatChannel* channel = g_chat.getChannelById(channelId);
	if (!channel) {
		pushBoolean(L, false);
		return 1;
	}

	SpeakClasses type = static_cast<SpeakClasses>(getNumber<uint32_t>(L, 2));
	std::string message = getString(L, 3);
	channel->sendToAll(message, type);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaSendGuildChannelMessage(lua_State* L)
{
	//sendGuildChannelMessage(guildId, type, message)
	uint32_t guildId = getNumber<uint32_t>(L, 1);
	ChatChannel* channel = g_chat.getGuildChannelById(guildId);
	if (!channel) {
		pushBoolean(L, false);
		return 1;
	}

	SpeakClasses type = static_cast<SpeakClasses>(getNumber<uint32_t>(L, 2));
	std::string message = getString(L, 3);
	channel->sendToAll(message, type);
	pushBoolean(L, true);
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

#ifndef LUAJIT_VERSION
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
	lua_pushnumber(L, ~getNumber<int32_t>(L, -1));
	return 1;
}

int32_t LuaScriptInterface::luaBitUNot(lua_State* L)
{
	lua_pushnumber(L, ~getNumber<uint32_t>(L, -1));
	return 1;
}

#define MULTIOP(type, name, op) \
int32_t LuaScriptInterface::luaBit##name(lua_State* L) \
{ \
	int32_t n = lua_gettop(L); \
	type w = getNumber<type>(L, -1); \
	for (int32_t i = 1; i < n; ++i) \
		w op getNumber<type>(L, i); \
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
	type n2 = getNumber<type>(L, 1), n1 = getNumber<type>(L, 2); \
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
	pushString(L, g_config.getString(static_cast<ConfigManager::string_config_t>(getNumber<uint32_t>(L, -1))));
	return 1;
}

int32_t LuaScriptInterface::luaConfigManagerGetNumber(lua_State* L)
{
	lua_pushnumber(L, g_config.getNumber(static_cast<ConfigManager::integer_config_t>(getNumber<uint32_t>(L, -1))));
	return 1;
}

int32_t LuaScriptInterface::luaConfigManagerGetBoolean(lua_State* L)
{
	pushBoolean(L, g_config.getBoolean(static_cast<ConfigManager::boolean_config_t>(getNumber<uint32_t>(L, -1))));
	return 1;
}

const luaL_Reg LuaScriptInterface::luaDatabaseTable[] = {
	{"query", LuaScriptInterface::luaDatabaseExecute},
	{"storeQuery", LuaScriptInterface::luaDatabaseStoreQuery},
	{"escapeString", LuaScriptInterface::luaDatabaseEscapeString},
	{"escapeBlob", LuaScriptInterface::luaDatabaseEscapeBlob},
	{"lastInsertId", LuaScriptInterface::luaDatabaseLastInsertId},
	{"tableExists", LuaScriptInterface::luaDatabaseTableExists},
	{nullptr, nullptr}
};

int32_t LuaScriptInterface::luaDatabaseExecute(lua_State* L)
{
	pushBoolean(L, Database::getInstance()->executeQuery(getString(L, -1)));
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseStoreQuery(lua_State* L)
{
	if (DBResult_ptr res = Database::getInstance()->storeQuery(getString(L, -1))) {
		lua_pushnumber(L, ScriptEnvironment::addResult(res));
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseEscapeString(lua_State* L)
{
	pushString(L, Database::getInstance()->escapeString(getString(L, -1)));
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseEscapeBlob(lua_State* L)
{
	uint32_t length = getNumber<uint32_t>(L, 2);
	pushString(L, Database::getInstance()->escapeBlob(getString(L, 1).c_str(), length));
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseLastInsertId(lua_State* L)
{
	lua_pushnumber(L, Database::getInstance()->getLastInsertId());
	return 1;
}

int32_t LuaScriptInterface::luaDatabaseTableExists(lua_State* L)
{
	pushBoolean(L, DatabaseManager::tableExists(getString(L, -1)));
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
	DBResult_ptr res = ScriptEnvironment::getResultByID(getNumber<uint32_t>(L, 1));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	const std::string& s = getString(L, 2);
	lua_pushnumber(L, res->getDataInt(s));
	return 1;
}

int32_t LuaScriptInterface::luaResultGetDataLong(lua_State* L)
{
	DBResult_ptr res = ScriptEnvironment::getResultByID(getNumber<uint32_t>(L, 1));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	const std::string& s = getString(L, 2);
	lua_pushnumber(L, res->getNumber<int64_t>(s));
	return 1;
}

int32_t LuaScriptInterface::luaResultGetDataString(lua_State* L)
{
	DBResult_ptr res = ScriptEnvironment::getResultByID(getNumber<uint32_t>(L, 1));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	const std::string& s = getString(L, 2);
	pushString(L, res->getDataString(s));
	return 1;
}

int32_t LuaScriptInterface::luaResultGetDataStream(lua_State* L)
{
	DBResult_ptr res = ScriptEnvironment::getResultByID(getNumber<uint32_t>(L, 1));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	unsigned long length;
	const std::string& s = getString(L, 2);
	lua_pushstring(L, res->getDataStream(s, length));
	lua_pushnumber(L, length);
	return 2;
}

int32_t LuaScriptInterface::luaResultNext(lua_State* L)
{
	DBResult_ptr res = ScriptEnvironment::getResultByID(getNumber<uint32_t>(L, -1));
	if (!res) {
		pushBoolean(L, false);
		return 1;
	}

	pushBoolean(L, res->next());
	return 1;
}

int32_t LuaScriptInterface::luaResultFree(lua_State* L)
{
	pushBoolean(L, ScriptEnvironment::removeResult(getNumber<uint32_t>(L, -1)));
	return 1;
}

// Userdata
int32_t LuaScriptInterface::luaUserdataCompare(lua_State* L)
{
	// userdataA == userdataB
	pushBoolean(L, getUserdata<void>(L, 1) == getUserdata<void>(L, 2));
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
		lua_getfield(L, -3, "__index");
		lua_replace(L, -4);
	}

	lua_rawgeti(L, -4, 'h');
	size_t hashA = getNumber<size_t>(L, 1);

	pushBoolean(L, hashA == hashB);
	return 1;
}

// Game
int32_t LuaScriptInterface::luaGameGetSpectators(lua_State* L)
{
	// Game.getSpectators(position[, multifloor = false[, onlyPlayer = false[, minRangeX = 0[, maxRangeX = 0[, minRangeY = 0[, maxRangeY = 0]]]]]])
	const Position& position = getPosition(L, 1);
	bool multifloor = getBoolean(L, 2, false);
	bool onlyPlayers = getBoolean(L, 3, false);
	int32_t minRangeX = getNumber<int32_t>(L, 4, 0);
	int32_t maxRangeX = getNumber<int32_t>(L, 5, 0);
	int32_t minRangeY = getNumber<int32_t>(L, 6, 0);
	int32_t maxRangeY = getNumber<int32_t>(L, 7, 0);

	SpectatorVec spectators;
	g_game.getSpectators(spectators, position, multifloor, onlyPlayers, minRangeX, maxRangeX, minRangeY, maxRangeY);

	lua_createtable(L, spectators.size(), 0);

	int32_t index = 0;
	for (Creature* creature : spectators) {
		pushUserdata<Creature>(L, creature);
		setCreatureMetatable(L, -1, creature);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGameGetPlayers(lua_State* L)
{
	// Game.getPlayers()
	lua_createtable(L, g_game.getPlayersOnline(), 0);

	int32_t index = 0;
	for (const auto& playerEntry : g_game.getPlayers()) {
		pushUserdata<Player>(L, playerEntry.second);
		setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGameLoadMap(lua_State* L)
{
	// Game.loadMap(path)
	const std::string& path = getString(L, 1);
	g_dispatcher.addTask(createTask(std::bind(&Game::loadMap, &g_game, path)));
	return 1;
}

int32_t LuaScriptInterface::luaGameGetExperienceStage(lua_State* L)
{
	// Game.getExperienceStage(level)
	uint32_t level = getNumber<uint32_t>(L, 1);
	lua_pushnumber(L, g_game.getExperienceStage(level));
	return 1;
}

int32_t LuaScriptInterface::luaGameGetMonsterCount(lua_State* L)
{
	// Game.getMonsterCount()
	lua_pushnumber(L, g_game.getMonstersOnline());
	return 1;
}

int32_t LuaScriptInterface::luaGameGetPlayerCount(lua_State* L)
{
	// Game.getPlayerCount()
	lua_pushnumber(L, g_game.getPlayersOnline());
	return 1;
}

int32_t LuaScriptInterface::luaGameGetNpcCount(lua_State* L)
{
	// Game.getNpcCount()
	lua_pushnumber(L, g_game.getNpcsOnline());
	return 1;
}

int32_t LuaScriptInterface::luaGameGetTowns(lua_State* L)
{
	// Game.getTowns()
	const auto& towns = Towns::getInstance().getTowns();
	lua_createtable(L, towns.size(), 0);

	int32_t index = 0;
	for (auto townEntry : towns) {
		pushUserdata<Town>(L, townEntry.second);
		setMetatable(L, -1, "Town");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGameGetHouses(lua_State* L)
{
	// Game.getHouses()
	const auto& houses = Houses::getInstance().getHouses();
	lua_createtable(L, houses.size(), 0);

	int32_t index = 0;
	for (auto houseEntry : houses) {
		pushUserdata<House>(L, houseEntry.second);
		setMetatable(L, -1, "House");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGameGetGameState(lua_State* L)
{
	// Game.getGameState()
	lua_pushnumber(L, g_game.getGameState());
	return 1;
}

int32_t LuaScriptInterface::luaGameSetGameState(lua_State* L)
{
	// Game.setGameState(state)
	GameState_t state = static_cast<GameState_t>(getNumber<int64_t>(L, 1));
	g_game.setGameState(state);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGameGetWorldType(lua_State* L)
{
	// Game.getWorldType()
	lua_pushnumber(L, g_game.getWorldType());
	return 1;
}

int32_t LuaScriptInterface::luaGameSetWorldType(lua_State* L)
{
	// Game.setWorldType(type)
	WorldType_t type = static_cast<WorldType_t>(getNumber<int64_t>(L, 1));
	g_game.setWorldType(type);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaGameGetReturnMessage(lua_State* L)
{
	// Game.getReturnMessage(value)
	ReturnValue value = static_cast<ReturnValue>(getNumber<int64_t>(L, 1));
	pushString(L, getReturnMessage(value));
	return 1;
}

int32_t LuaScriptInterface::luaGameCreateItem(lua_State* L)
{
	// Game.createItem(itemId, count[, position])
	uint16_t count = getNumber<uint16_t>(L, 2);
	uint16_t id = getNumber<uint16_t>(L, 1);

	const ItemType& it = Item::items[id];
	if (it.stackable) {
		count = std::min<uint16_t>(count, 100);
	}

	Item* item = Item::CreateItem(id, count);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	if (lua_gettop(L) >= 3) {
		const Position& position = getPosition(L, 3);
		Tile* tile = g_game.getTile(position.x, position.y, position.z);
		if (!tile) {
			delete item;
			lua_pushnil(L);
			return 1;
		}

		g_game.internalAddItem(tile, item, INDEX_WHEREEVER, FLAG_NOLIMIT);
	} else {
		ScriptEnvironment* env = getScriptEnv();
		env->addTempItem(env, item);
		item->setParent(VirtualCylinder::virtualCylinder);
	}

	pushUserdata<Item>(L, item);
	setItemMetatable(L, -1, item);
	return 1;
}

int32_t LuaScriptInterface::luaGameCreateMonster(lua_State* L)
{
	// Game.createMonster(monsterName, position[, extended = false[, force = false]])
	Monster* monster = Monster::createMonster(getString(L, 1));
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	const Position& position = getPosition(L, 2);
	bool extended = getBoolean(L, 3, false);
	bool force = getBoolean(L, 4, false);
	if (g_game.placeCreature(monster, position, extended, force)) {
		pushUserdata<Monster>(L, monster);
		setMetatable(L, -1, "Monster");
	} else {
		delete monster;
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGameCreateNpc(lua_State* L)
{
	// Game.createNpc(npcName, position[, extended = false[, force = false]])
	Npc* npc = Npc::createNpc(getString(L, 1));
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	const Position& position = getPosition(L, 2);
	bool extended = getBoolean(L, 3, false);
	bool force = getBoolean(L, 4, false);
	if (g_game.placeCreature(npc, position, extended, force)) {
		pushUserdata<Npc>(L, npc);
		setMetatable(L, -1, "Npc");
	} else {
		delete npc;
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGameStartRaid(lua_State* L)
{
	// Game.startRaid(raidName)
	const std::string& raidName = getString(L, 1);

	Raid* raid = Raids::getInstance()->getRaidByName(raidName);
	if (raid) {
		raid->startRaid();
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Position
int32_t LuaScriptInterface::luaPositionCreate(lua_State* L)
{
	// Position([x = 0[, y = 0[, z = 0[, stackpos = 0]]]])
	// Position([position])
	// Position.new([x = 0[, y = 0[, z = 0[, stackpos = 0]]]])
	// Position.new([position])
	if (lua_gettop(L) <= 1) {
		pushPosition(L, Position());
		return 1;
	}

	int32_t stackpos;
	if (isTable(L, 2)) {
		const Position& position = getPosition(L, 2, stackpos);
		pushPosition(L, position, stackpos);
	} else {
		uint16_t x = getNumber<uint16_t>(L, 2, 0);
		uint16_t y = getNumber<uint16_t>(L, 3, 0);
		uint8_t z = getNumber<uint8_t>(L, 4, 0);
		stackpos = getNumber<int32_t>(L, 5, 0);

		pushPosition(L, Position(x, y, z), stackpos);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPositionAdd(lua_State* L)
{
	// positionValue = position + positionEx
	int32_t stackpos;
	const Position& position = getPosition(L, 1, stackpos);

	Position positionEx;
	if (stackpos == 0) {
		positionEx = getPosition(L, 2, stackpos);
	} else {
		positionEx = getPosition(L, 2);
	}

	pushPosition(L, position + positionEx, stackpos);
	return 1;
}

int32_t LuaScriptInterface::luaPositionSub(lua_State* L)
{
	// positionValue = position - positionEx
	int32_t stackpos;
	const Position& position = getPosition(L, 1, stackpos);

	Position positionEx;
	if (stackpos == 0) {
		positionEx = getPosition(L, 2, stackpos);
	} else {
		positionEx = getPosition(L, 2);
	}

	pushPosition(L, position - positionEx, stackpos);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPositionGetDistance(lua_State* L)
{
	// position:getDistance(positionEx)
	const Position& positionEx = getPosition(L, 2);
	const Position& position = getPosition(L, 1);
	lua_pushnumber(L, std::max<int32_t>(
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
	bool sameFloor = getBoolean(L, 3, true);
	const Position& positionEx = getPosition(L, 2);
	const Position& position = getPosition(L, 1);
	pushBoolean(L, g_game.isSightClear(position, positionEx, sameFloor));
	return 1;
}

int32_t LuaScriptInterface::luaPositionSendMagicEffect(lua_State* L)
{
	// position:sendMagicEffect(magicEffect[, player = nullptr])
	SpectatorVec list;
	if (lua_gettop(L) >= 3) {
		Player* player = getPlayer(L, 3);
		if (player) {
			list.insert(player);
		}
	}

	MagicEffectClasses magicEffect = static_cast<MagicEffectClasses>(getNumber<int64_t>(L, 2));
	const Position& position = getPosition(L, 1);
	if (!list.empty()) {
		Game::addMagicEffect(list, position, magicEffect);
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
	if (lua_gettop(L) >= 4) {
		Player* player = getPlayer(L, 4);
		if (player) {
			list.insert(player);
		}
	}

	ShootType_t distanceEffect = static_cast<ShootType_t>(getNumber<int64_t>(L, 3));
	const Position& positionEx = getPosition(L, 2);
	const Position& position = getPosition(L, 1);
	if (!list.empty()) {
		Game::addDistanceEffect(list, position, positionEx, distanceEffect);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetPosition(lua_State* L)
{
	// tile:getPosition()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		pushPosition(L, tile->getPosition());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
				lua_pushnil(L);
			}
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetThingCount(lua_State* L)
{
	// tile:getThingCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		lua_pushnumber(L, tile->getThingCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetTopVisibleThing(lua_State* L)
{
	// tile:getTopVisibleThing(creature)
	Creature* creature = getCreature(L, 2);
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		Thing* thing = tile->getTopVisibleThing(creature);
		if (thing) {
			if (Creature* visibleCreature = thing->getCreature()) {
				pushUserdata<Creature>(L, visibleCreature);
				setCreatureMetatable(L, -1, visibleCreature);
			} else if (Item* visibleItem = thing->getItem()) {
				pushUserdata<Item>(L, visibleItem);
				setItemMetatable(L, -1, visibleItem);
			} else {
				lua_pushnil(L);
			}
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
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
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
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
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
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
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItemById(lua_State* L)
{
	// tile:getItemById(itemId[, subType = -1])
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}
	int32_t subType = getNumber<int32_t>(L, 3, -1);

	Item* item = g_game.findItemOfType(tile, itemId, false, subType);
	if (item) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItemByType(lua_State* L)
{
	// tile:getItemByType(itemType)
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	bool found;

	ItemTypes_t itemType = static_cast<ItemTypes_t>(getNumber<int64_t>(L, 2));
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

	lua_pushnil(L);
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItemByTopOrder(lua_State* L)
{
	// tile:getItemByTopOrder(topOrder)
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	int32_t topOrder = getNumber<int32_t>(L, 2);

	Item* item = tile->getItemByTopOrder(topOrder);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	pushUserdata<Item>(L, item);
	setItemMetatable(L, -1, item);
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItemCountById(lua_State* L)
{
	// tile:getItemCountById(itemId[, subType = -1])
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	int32_t subType = getNumber<int32_t>(L, 3, -1);

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	lua_pushnumber(L, tile->__getItemTypeCount(itemId, subType));
	return 1;
}

int32_t LuaScriptInterface::luaTileGetBottomCreature(lua_State* L)
{
	// tile:getBottomCreature()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	const Creature* creature = tile->getBottomCreature();
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	pushUserdata<const Creature>(L, creature);
	setCreatureMetatable(L, -1, creature);
	return 1;
}

int32_t LuaScriptInterface::luaTileGetTopCreature(lua_State* L)
{
	// tile:getTopCreature()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = tile->getTopCreature();
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	pushUserdata<Creature>(L, creature);
	setCreatureMetatable(L, -1, creature);
	return 1;
}

int32_t LuaScriptInterface::luaTileGetBottomVisibleCreature(lua_State* L)
{
	// tile:getBottomVisibleCreature(creature)
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = getCreature(L, 2);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const Creature* visibleCreature = tile->getBottomVisibleCreature(creature);
	if (visibleCreature) {
		pushUserdata<const Creature>(L, visibleCreature);
		setCreatureMetatable(L, -1, visibleCreature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetTopVisibleCreature(lua_State* L)
{
	// tile:getTopVisibleCreature(creature)
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = getCreature(L, 2);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Creature* visibleCreature = tile->getTopVisibleCreature(creature);
	if (visibleCreature) {
		pushUserdata<Creature>(L, visibleCreature);
		setCreatureMetatable(L, -1, visibleCreature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItems(lua_State* L)
{
	// tile:getItems()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	TileItemVector* itemVector = tile->getItemList();
	if (!itemVector) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, itemVector->size(), 0);

	int32_t index = 0;
	for (Item* item : *itemVector) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetItemCount(lua_State* L)
{
	// tile:getItemCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, tile->getItemCount());
	return 1;
}

int32_t LuaScriptInterface::luaTileGetDownItemCount(lua_State* L)
{
	// tile:getDownItemCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		lua_pushnumber(L, tile->getDownItemCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetTopItemCount(lua_State* L)
{
	// tile:getTopItemCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, tile->getTopItemCount());
	return 1;
}

int32_t LuaScriptInterface::luaTileGetCreatures(lua_State* L)
{
	// tile:getCreatures()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	CreatureVector* creatureVector = tile->getCreatures();
	if (!creatureVector) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, creatureVector->size(), 0);

	int32_t index = 0;
	for (Creature* creature : *creatureVector) {
		pushUserdata<Creature>(L, creature);
		setCreatureMetatable(L, -1, creature);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetCreatureCount(lua_State* L)
{
	// tile:getCreatureCount()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, tile->getCreatureCount());
	return 1;
}

int32_t LuaScriptInterface::luaTileHasProperty(lua_State* L)
{
	// tile:hasProperty(property[, item])
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Item* item;
	if (lua_gettop(L) >= 3) {
		item = getUserdata<Item>(L, 3);
	} else {
		item = nullptr;
	}

	ITEMPROPERTY property = static_cast<ITEMPROPERTY>(getNumber<int64_t>(L, 2));
	if (item) {
		pushBoolean(L, tile->hasProperty(item, property));
	} else {
		pushBoolean(L, tile->hasProperty(property));
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileHasFlag(lua_State* L)
{
	// tile:hasFlag(flag)
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		tileflags_t flag = static_cast<tileflags_t>(getNumber<int64_t>(L, 2));
		pushBoolean(L, tile->hasFlag(flag));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileQueryAdd(lua_State* L)
{
	// tile:queryAdd(thing[, flags])
	Tile* tile = getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Thing* thing = getThing(L, 2);
	if (thing) {
		uint32_t flags = getNumber<uint32_t>(L, 3, 0);
		lua_pushnumber(L, tile->__queryAdd(0, thing, 1, flags));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTileGetHouse(lua_State* L)
{
	// tile:getHouse()
	Tile* tile = getUserdata<Tile>(L, 1);
	if (tile) {
		if (HouseTile* houseTile = dynamic_cast<HouseTile*>(tile)) {
			pushUserdata<House>(L, houseTile->getHouse());
			setMetatable(L, -1, "House");
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
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
		lua_pushnumber(L, message->GetByte());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageGetU16(lua_State* L)
{
	// networkMessage:getU16()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		lua_pushnumber(L, message->get<uint16_t>());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageGetU32(lua_State* L)
{
	// networkMessage:getU32()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		lua_pushnumber(L, message->get<uint32_t>());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageGetU64(lua_State* L)
{
	// networkMessage:getU64()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		lua_pushnumber(L, message->get<uint64_t>());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageGetPosition(lua_State* L)
{
	// networkMessage:getPosition()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		pushPosition(L, message->GetPosition());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddU16(lua_State* L)
{
	// networkMessage:addU16(number)
	uint16_t number = getNumber<uint16_t>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->add<uint16_t>(number);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddU32(lua_State* L)
{
	// networkMessage:addU32(number)
	uint32_t number = getNumber<uint32_t>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->add<uint32_t>(number);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddU64(lua_State* L)
{
	// networkMessage:addU64(number)
	uint64_t number = getNumber<uint64_t>(L, 2);
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->add<uint64_t>(number);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddItem(lua_State* L)
{
	// networkMessage:addItem(item)
	Item* item = getUserdata<Item>(L, 2);
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->AddItem(item);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageAddItemId(lua_State* L)
{
	// networkMessage:addItemId(itemId)
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (!message) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	message->AddItemId(itemId);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaNetworkMessageReset(lua_State* L)
{
	// networkMessage:reset()
	NetworkMessage* message = getUserdata<NetworkMessage>(L, 1);
	if (message) {
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnumber(L, window->id);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSetTitle(lua_State* L)
{
	// modalWindow:setTitle(text)
	const std::string& text = getString(L, 2);
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->title = text;
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSetMessage(lua_State* L)
{
	// modalWindow:setMessage(text)
	const std::string& text = getString(L, 2);
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->message = text;
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetButtonCount(lua_State* L)
{
	// modalWindow:getButtonCount()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		lua_pushnumber(L, window->buttons.size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetChoiceCount(lua_State* L)
{
	// modalWindow:getChoiceCount()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		lua_pushnumber(L, window->choices.size());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetDefaultEnterButton(lua_State* L)
{
	// modalWindow:getDefaultEnterButton()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		lua_pushnumber(L, window->defaultEnterButton);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSetDefaultEnterButton(lua_State* L)
{
	// modalWindow:setDefaultEnterButton(buttonId)
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->defaultEnterButton = getNumber<uint8_t>(L, 2);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowGetDefaultEscapeButton(lua_State* L)
{
	// modalWindow:getDefaultEscapeButton()
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		lua_pushnumber(L, window->defaultEscapeButton);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSetDefaultEscapeButton(lua_State* L)
{
	// modalWindow:setDefaultEscapeButton(buttonId)
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->defaultEscapeButton = getNumber<uint8_t>(L, 2);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSetPriority(lua_State* L)
{
	// modalWindow:setPriority(priority)
	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->priority = getBoolean(L, 2);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaModalWindowSendToPlayer(lua_State* L)
{
	// modalWindow:sendToPlayer(player)
	Player* player = getPlayer(L, 2);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	ModalWindow* window = getUserdata<ModalWindow>(L, 1);
	if (window) {
		if (!player->hasModalWindowOpen(window->id)) {
			player->sendModalWindow(*window);
		}
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Item
int32_t LuaScriptInterface::luaItemCreate(lua_State* L)
{
	// Item(uid)
	// Item.new(uid)
	uint32_t id = getNumber<uint32_t>(L, 2);

	Item* item = getScriptEnv()->getItemByUID(id);
	if (item) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
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
	pushBoolean(L, getUserdata<const Item>(L, 1) != nullptr);
	return 1;
}

int32_t LuaScriptInterface::luaItemIsContainer(lua_State* L)
{
	// item:isContainer()
	pushBoolean(L, false);
	return 1;
}

int32_t LuaScriptInterface::luaItemGetParent(lua_State* L)
{
	// item:getParent()
	Item* item = getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Cylinder* parent = item->getParent();
	if (!parent) {
		lua_pushnil(L);
		return 1;
	}

	if (Creature* creature = parent->getCreature()) {
		pushUserdata<Creature>(L, creature);
		setCreatureMetatable(L, -1, creature);
	} else if (Item* item = parent->getItem()) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else if (Tile* tile = parent->getTile()) {
		pushUserdata<Tile>(L, tile);
		setMetatable(L, -1, "Tile");
	} else if (parent == VirtualCylinder::virtualCylinder) {
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetTopParent(lua_State* L)
{
	// item:getTopParent()
	Item* item = getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Cylinder* topParent = item->getTopParent();
	if (!topParent) {
		lua_pushnil(L);
		return 1;
	}

	if (Creature* creature = topParent->getCreature()) {
		pushUserdata<Creature>(L, creature);
		setCreatureMetatable(L, -1, creature);
	} else if (Item* item = topParent->getItem()) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else if (Tile* tile = topParent->getTile()) {
		pushUserdata<Tile>(L, tile);
		setMetatable(L, -1, "Tile");
	} else if (topParent == VirtualCylinder::virtualCylinder) {
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetId(lua_State* L)
{
	// item:getId()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetType(lua_State* L)
{
	// item:getType()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		const ItemType& it = Item::items[item->getID()];
		pushUserdata<const ItemType>(L, &it);
		setMetatable(L, -1, "ItemType");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemClone(lua_State* L)
{
	// item:clone()
	Item* item = getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Item* clone = item->clone();
	if (!clone) {
		lua_pushnil(L);
		return 1;
	}

	ScriptEnvironment* env = getScriptEnv();
	env->addTempItem(env, clone);
	clone->setParent(VirtualCylinder::virtualCylinder);

	pushUserdata<Item>(L, clone);
	setItemMetatable(L, -1, clone);
	return 1;
}

int32_t LuaScriptInterface::luaItemSplit(lua_State* L)
{
	// item:split([count = 1])
	Item** itemPtr = getRawUserdata<Item>(L, 1);
	if (!itemPtr) {
		lua_pushnil(L);
		return 1;
	}

	Item* item = *itemPtr;
	if (!item || !item->isStackable()) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t count = std::min<uint16_t>(getNumber<uint16_t>(L, 2, 1), item->getItemCount());
	uint16_t diff = item->getItemCount() - count;

	Item* splitItem = item->clone();
	if (!splitItem) {
		lua_pushnil(L);
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
	return 1;
}

int32_t LuaScriptInterface::luaItemRemove(lua_State* L)
{
	// item:remove([count = -1])
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		int32_t count = getNumber<int32_t>(L, 2, -1);
		pushBoolean(L, g_game.internalRemoveItem(item, count) == RET_NOERROR);
	} else {
		lua_pushnil(L);
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
		lua_pushnumber(L, uniqueId);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetActionId(lua_State* L)
{
	// item:getActionId()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getActionId());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetCount(lua_State* L)
{
	// item:getCount()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getItemCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetCharges(lua_State* L)
{
	// item:getCharges()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getCharges());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetFluidType(lua_State* L)
{
	// item:getFluidType()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getFluidType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetSubType(lua_State* L)
{
	// item:getSubType()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getSubType());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetPosition(lua_State* L)
{
	// item:getPosition()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushPosition(L, item->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetTile(lua_State* L)
{
	// item:getTile()
	Item* item = getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Tile* tile = item->getTile();
	if (tile) {
		pushUserdata<Tile>(L, tile);
		setMetatable(L, -1, "Tile");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetAttribute(lua_State* L)
{
	// item:getAttribute(key)
	Item* item = getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	itemAttrTypes attribute;
	if (isNumber(L, 2)) {
		attribute = static_cast<itemAttrTypes>(getNumber<int64_t>(L, 2));
	} else if (isString(L, 2)) {
		attribute = stringToItemAttribute(getString(L, 2));
	} else {
		attribute = ITEM_ATTRIBUTE_NONE;
	}

	if (attribute & 0x7F0013) { // All integer attributes
		lua_pushnumber(L, item->getIntAttr(attribute));
	} else if (attribute & 0x2C) { // All string attributes
		pushString(L, item->getStrAttr(attribute));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemSetAttribute(lua_State* L)
{
	// item:setAttribute(key, value)
	Item* item = getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	itemAttrTypes attribute;
	if (isNumber(L, 2)) {
		attribute = static_cast<itemAttrTypes>(getNumber<int64_t>(L, 2));
	} else if (isString(L, 2)) {
		attribute = stringToItemAttribute(getString(L, 2));
	} else {
		attribute = ITEM_ATTRIBUTE_NONE;
	}

	if (attribute & 0x7F0013) { // All integer attributes
		item->setIntAttr(attribute, getNumber<int32_t>(L, 3));
		pushBoolean(L, true);
	} else if (attribute & 0x2C) { // All string attributes
		item->setStrAttr(attribute, getString(L, 3));
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemRemoveAttribute(lua_State* L)
{
	// item:removeAttribute(key)
	Item* item = getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	itemAttrTypes attribute;
	if (isNumber(L, 2)) {
		attribute = static_cast<itemAttrTypes>(getNumber<int64_t>(L, 2));
	} else if (isString(L, 2)) {
		attribute = stringToItemAttribute(getString(L, 2));
	} else {
		attribute = ITEM_ATTRIBUTE_NONE;
	}

	bool ret = attribute != ITEM_ATTRIBUTE_UNIQUEID;
	if (ret) {
		item->removeAttribute(attribute);
	} else {
		reportErrorFunc("Attempt to erase protected key \"uid\"");
	}
	pushBoolean(L, ret);
	return 1;
}

int32_t LuaScriptInterface::luaItemMoveTo(lua_State* L)
{
	// item:moveTo(position)
	Item** itemPtr = getRawUserdata<Item>(L, 1);
	if (!itemPtr) {
		lua_pushnil(L);
		return 1;
	}

	Item* item = *itemPtr;
	if (!item || item->isRemoved()) {
		lua_pushnil(L);
		return 1;
	}

	const Position& position = getPosition(L, 2);
	Tile* tile = g_game.getTile(position);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (item->getParent() == tile) {
		pushBoolean(L, true);
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
	return 1;
}

int32_t LuaScriptInterface::luaItemTransform(lua_State* L)
{
	// item:transform(itemId[, count/subType = -1])
	Item** itemPtr = getRawUserdata<Item>(L, 1);
	if (!itemPtr) {
		lua_pushnil(L);
		return 1;
	}

	Item*& item = *itemPtr;
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t subType = getNumber<int32_t>(L, 3, -1);
	if (item->getID() == itemId && (subType == -1 || subType == item->getSubType())) {
		pushBoolean(L, true);
		return 1;
	}

	const ItemType& it = Item::items[itemId];
	if (it.stackable) {
		subType = std::min<int32_t>(subType, 100);
	}

	ScriptEnvironment* env = getScriptEnv();
	uint32_t uid = env->addThing(item);

	Item* newItem = g_game.transformItem(item, itemId, subType);
	if (item->isRemoved()) {
		env->removeItemByUID(uid);
	}

	if (newItem && newItem != item) {
		env->insertThing(uid, newItem);
	}

	item = newItem;
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaItemDecay(lua_State* L)
{
	// item:decay()
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		g_game.startDecay(item);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemGetDescription(lua_State* L)
{
	// item:getDescription(distance)
	int32_t distance = getNumber<int32_t>(L, 2);
	Item* item = getUserdata<Item>(L, 1);
	if (item) {
		pushString(L, item->getDescription(distance));
	} else {
		lua_pushnil(L);
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
	int32_t parameters = lua_gettop(L);

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
			lua_pushnil(L);
			return 1;
		}

		if (!item->getContainer()) {
			delete item;
			lua_pushnil(L);
			return 1;
		}

		if (parameters >= 4) {
			Tile* tile = g_game.getTile(position.x, position.y, position.z);
			if (!tile) {
				delete item;
				lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerIsContainer(lua_State* L)
{
	// container:isContainer()
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetSize(lua_State* L)
{
	// container:getSize()
	Container* container = getUserdata<Container>(L, 1);
	if (container) {
		lua_pushnumber(L, container->size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetCapacity(lua_State* L)
{
	// container:getCapacity()
	Container* container = getUserdata<Container>(L, 1);
	if (container) {
		lua_pushnumber(L, container->capacity());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetEmptySlots(lua_State* L)
{
	// container:getEmptySlots([recursive = false])
	Container* container = getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t slots = container->capacity() - container->size();
	bool recursive = getBoolean(L, 2, false);
	if (recursive) {
		for (ContainerIterator cit = container->begin(); cit != container->end(); ++cit) {
			if (Container* tmpContainer = (*cit)->getContainer()) {
				slots += tmpContainer->capacity() - tmpContainer->size();
			}
		}
	}
	lua_pushnumber(L, slots);
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetItemHoldingCount(lua_State* L)
{
	// container:getItemHoldingCount()
	Container* container = getUserdata<Container>(L, 1);
	if (container) {
		lua_pushnumber(L, container->getItemHoldingCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetItem(lua_State* L)
{
	// container:getItem(index)
	Container* container = getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t index = getNumber<uint32_t>(L, 2);
	Item* item = container->getItemByIndex(index);
	if (item) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerAddItem(lua_State* L)
{
	// container:addItem(itemId[, count/subType = 1[, index = INDEX_WHEREEVER[, flags = 0]]])
	Container* container = getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	uint32_t subType = getNumber<uint32_t>(L, 3, 1);

	Item* item = Item::CreateItem(itemId, std::min<uint32_t>(subType, 100));
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	int32_t index = getNumber<int32_t>(L, 4, INDEX_WHEREEVER);
	uint32_t flags = getNumber<uint32_t>(L, 5, 0);

	ReturnValue ret = g_game.internalAddItem(container, item, index, flags);
	if (ret == RET_NOERROR) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else {
		delete item;
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaContainerAddItemEx(lua_State* L)
{
	// container:addItemEx(item[, index = INDEX_WHEREEVER[, flags = 0]])
	Item* item = getUserdata<Item>(L, 2);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Container* container = getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	if (item->getParent() != VirtualCylinder::virtualCylinder) {
		reportErrorFunc("Item already has a parent");
		lua_pushnil(L);
		return 1;
	}

	int32_t index = getNumber<int32_t>(L, 3, INDEX_WHEREEVER);
	uint32_t flags = getNumber<uint32_t>(L, 4, 0);
	ReturnValue ret = g_game.internalAddItem(container, item, index, flags);
	if (ret == RET_NOERROR) {
		getScriptEnv()->removeTempItem(item);
	}
	lua_pushnumber(L, ret);
	return 1;
}

int32_t LuaScriptInterface::luaContainerGetItemCountById(lua_State* L)
{
	// container:getItemCountById(itemId[, subType = -1])
	Container* container = getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t subType = getNumber<int32_t>(L, 3, -1);
	lua_pushnumber(L, container->__getItemTypeCount(itemId, subType));
	return 1;
}

// Creature
int32_t LuaScriptInterface::luaCreatureCreate(lua_State* L)
{
	// Creature(id or name or userdata)
	// Creature.new(id or name or userdata)
	Creature* creature;
	if (isNumber(L, 2)) {
		creature = g_game.getCreatureByID(getNumber<uint32_t>(L, 2));
	} else if (isString(L, 2)) {
		creature = g_game.getCreatureByName(getString(L, 2));
	} else if (isUserdata(L, 2)) {
		creature = getUserdata<Creature>(L, 2);
	} else {
		creature = nullptr;
	}

	if (creature) {
		pushUserdata<Creature>(L, creature);
		setCreatureMetatable(L, -1, creature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureRegisterEvent(lua_State* L)
{
	// creature:registerEvent(name)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		const std::string& name = getString(L, 2);
		pushBoolean(L, creature->registerCreatureEvent(name));
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureIsRemoved(lua_State* L)
{
	// creature:isRemoved()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushBoolean(L, creature->isRemoved());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureIsCreature(lua_State* L)
{
	// creature:isCreature()
	pushBoolean(L, getUserdata<const Creature>(L, 1) != nullptr);
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

int32_t LuaScriptInterface::luaCreatureIsInGhostMode(lua_State* L)
{
	// creature:isInGhostMode()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushBoolean(L, creature->isInGhostMode());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureIsHealthHidden(lua_State* L)
{
	// creature:isHealthHidden()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushBoolean(L, creature->isHealthHidden());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureCanSee(lua_State* L)
{
	// creature:canSee(position)
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		const Position& position = getPosition(L, 2);
		pushBoolean(L, creature->canSee(position));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureCanSeeCreature(lua_State* L)
{
	// creature:canSeeCreature(creature)
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		const Creature* otherCreature = getCreature(L, 2);
		pushBoolean(L, creature->canSeeCreature(otherCreature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetParent(lua_State* L)
{
	// creature:getParent()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Cylinder* parent = creature->getParent();
	if (!parent) {
		lua_pushnil(L);
		return 1;
	}

	if (Creature* creature = parent->getCreature()) {
		pushUserdata<Creature>(L, creature);
		setCreatureMetatable(L, -1, creature);
	} else if (Item* item = parent->getItem()) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else if (Tile* tile = parent->getTile()) {
		pushUserdata<Tile>(L, tile);
		setMetatable(L, -1, "Tile");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetId(lua_State* L)
{
	// creature:getId()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getID());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetTarget(lua_State* L)
{
	// creature:getTarget()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Creature* target = creature->getAttackedCreature();
	if (target) {
		pushUserdata<Creature>(L, target);
		setCreatureMetatable(L, -1, target);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetTarget(lua_State* L)
{
	// creature:setTarget(target)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		Creature* target = getCreature(L, 2);
		pushBoolean(L, creature->setAttackedCreature(target));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetFollowCreature(lua_State* L)
{
	// creature:getFollowCreature()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Creature* followCreature = creature->getFollowCreature();
	if (followCreature) {
		pushUserdata<Creature>(L, followCreature);
		setCreatureMetatable(L, -1, followCreature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetFollowCreature(lua_State* L)
{
	// creature:setFollowCreature(followedCreature)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		Creature* followCreature = getCreature(L, 2);
		pushBoolean(L, creature->setFollowCreature(followCreature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetMaster(lua_State* L)
{
	// creature:getMaster()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Creature* master = creature->getMaster();
	if (!master) {
		lua_pushnil(L);
		return 1;
	}

	pushUserdata<Creature>(L, master);
	setCreatureMetatable(L, -1, master);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetMaster(lua_State* L)
{
	// creature:setMaster(master)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Creature* master = getCreature(L, 2);
	if (master) {
		pushBoolean(L, creature->convinceCreature(master));
	} else {
		master = creature->getMaster();
		if (master) {
			master->removeSummon(creature);
			creature->useThing2();
			creature->setDropLoot(true);
		}
		pushBoolean(L, true);
	}
	g_game.updateCreatureType(creature);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetLight(lua_State* L)
{
	// creature:getLight()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	LightInfo light;
	creature->getCreatureLight(light);
	lua_pushnumber(L, light.level);
	lua_pushnumber(L, light.color);
	return 2;
}

int32_t LuaScriptInterface::luaCreatureSetLight(lua_State* L)
{
	// creature:setLight(color, level)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	LightInfo light;
	light.color = getNumber<uint32_t>(L, 2);;
	light.level = getNumber<uint32_t>(L, 3);
	creature->setCreatureLight(light);
	g_game.changeLight(creature);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetSpeed(lua_State* L)
{
	// creature:getSpeed()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetBaseSpeed(lua_State* L)
{
	// creature:getBaseSpeed()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getBaseSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetDropLoot(lua_State* L)
{
	// creature:setDropLoot(doDrop)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		creature->setDropLoot(getBoolean(L, 2));
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetPosition(lua_State* L)
{
	// creature:getPosition()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		pushPosition(L, creature->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetTile(lua_State* L)
{
	// creature:getTile()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Tile* tile = creature->getTile();
	if (tile) {
		pushUserdata<Tile>(L, tile);
		setMetatable(L, -1, "Tile");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetDirection(lua_State* L)
{
	// creature:getDirection()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getDirection());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetDirection(lua_State* L)
{
	// creature:setDirection(direction)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		pushBoolean(L, g_game.internalCreatureTurn(creature, static_cast<Direction>(getNumber<int64_t>(L, 2))));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetHealth(lua_State* L)
{
	// creature:getHealth()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getHealth());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureAddHealth(lua_State* L)
{
	// creature:addHealth(healthChange)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	int32_t healthChange = getNumber<int32_t>(L, 2);
	if (healthChange >= 0) {
		g_game.combatChangeHealth(COMBAT_HEALING, nullptr, creature, healthChange);
	} else {
		g_game.combatChangeHealth(COMBAT_UNDEFINEDDAMAGE, nullptr, creature, healthChange);
	}
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetMaxHealth(lua_State* L)
{
	// creature:getMaxHealth()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getMaxHealth());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetMaxHealth(lua_State* L)
{
	// creature:setMaxHealth(maxHealth)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	creature->healthMax = getNumber<uint32_t>(L, 2);
	creature->health = std::min<int32_t>(creature->health, creature->healthMax);
	g_game.addCreatureHealth(creature);

	Player* player = creature->getPlayer();
	if (player) {
		player->sendStats();
	}
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetHiddenHealth(lua_State* L)
{
	// creature:setHiddenHealth(hide)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		creature->setHiddenHealth(getBoolean(L, 2));
		g_game.addCreatureHealth(creature);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetMana(lua_State* L)
{
	// creature:getMana()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getMana());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureAddMana(lua_State* L)
{
	// creature:addMana(manaChange[, animationOnLoss = false])
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	int32_t manaChange = getNumber<int32_t>(L, 2);
	bool animationOnLoss = getBoolean(L, 3, false);
	if (!animationOnLoss && manaChange < 0) {
		creature->changeMana(manaChange);
	} else {
		g_game.combatChangeMana(nullptr, creature, manaChange);
	}
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetMaxMana(lua_State* L)
{
	// creature:getMaxMana()
	const Creature* creature = getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getMaxMana());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSetOutfit(lua_State* L)
{
	// creature:setOutfit(outfit)
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		creature->defaultOutfit = getOutfit(L, 2);
		g_game.internalCreatureChangeOutfit(creature, creature->defaultOutfit);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetCondition(lua_State* L)
{
	// creature:getCondition(conditionType[, conditionId = CONDITIONID_COMBAT[, subId = 0]])
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	ConditionType_t conditionType = static_cast<ConditionType_t>(getNumber<int64_t>(L, 2));
	ConditionId_t conditionId = static_cast<ConditionId_t>(getNumber<int64_t>(L, 3, CONDITIONID_COMBAT));
	uint32_t subId = getNumber<uint32_t>(L, 4, 0);

	Condition* condition = creature->getCondition(conditionType, conditionId, subId);
	if (condition) {
		pushUserdata<Condition>(L, condition);
		setMetatable(L, -1, "Condition");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureAddCondition(lua_State* L)
{
	// creature:addCondition(condition[, force = false])
	Condition* condition;
	if (isNumber(L, 2)) {
		condition = g_luaEnvironment.getConditionObject(getNumber<uint32_t>(L, 2));
	} else {
		condition = getUserdata<Condition>(L, 2);
	}

	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature && condition) {
		bool force = getBoolean(L, 3, false);
		pushBoolean(L, creature->addCondition(condition->clone(), force));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureRemoveCondition(lua_State* L)
{
	// creature:removeCondition(conditionType[, conditionId = CONDITIONID_COMBAT[, subId = 0[, force = false]]])
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	ConditionType_t conditionType = static_cast<ConditionType_t>(getNumber<int64_t>(L, 2));
	ConditionId_t conditionId = static_cast<ConditionId_t>(getNumber<int64_t>(L, 3, CONDITIONID_COMBAT));
	uint32_t subId = getNumber<uint32_t>(L, 4, 0);
	Condition* condition = creature->getCondition(conditionType, conditionId, subId);
	if (condition) {
		bool force = getBoolean(L, 5, false);
		creature->removeCondition(condition, force);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureRemove(lua_State* L)
{
	// creature:remove()
	Creature** creaturePtr = getRawUserdata<Creature>(L, 1);
	if (!creaturePtr) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = *creaturePtr;
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Player* player = creature->getPlayer();
	if (player) {
		player->kickPlayer(true);
	} else {
		g_game.removeCreature(creature);
	}

	*creaturePtr = nullptr;
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaCreatureTeleportTo(lua_State* L)
{
	// creature:teleportTo(position[, pushMovement = false])
	bool pushMovement = getBoolean(L, 3, false);

	const Position& position = getPosition(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const Position oldPosition = creature->getPosition();
	if (g_game.internalTeleport(creature, position, pushMovement) != RET_NOERROR) {
		pushBoolean(L, false);
		return 1;
	}

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
	return 1;
}

int32_t LuaScriptInterface::luaCreatureSay(lua_State* L)
{
	// creature:say(text, type[, ghost = false[, target = nullptr[, position]]])
	int32_t parameters = lua_gettop(L);

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

	bool ghost = getBoolean(L, 4, false);

	SpeakClasses type = static_cast<SpeakClasses>(getNumber<int64_t>(L, 3));
	const std::string& text = getString(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	SpectatorVec list;
	if (target) {
		list.insert(target);
	}

	if (position.x != 0) {
		pushBoolean(L, g_game.internalCreatureSay(creature, type, text, ghost, &list, &position));
	} else {
		pushBoolean(L, g_game.internalCreatureSay(creature, type, text, ghost, &list));
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetDamageMap(lua_State* L)
{
	// creature:getDamageMap()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, creature->damageMap.size(), 0);
	for (auto damageEntry : creature->damageMap) {
		lua_createtable(L, 0, 2);
		setField(L, "total", damageEntry.second.total);
		setField(L, "ticks", damageEntry.second.ticks);
		lua_rawseti(L, -2, damageEntry.first);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetSummons(lua_State* L)
{
	// creature:getSummons()
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, creature->getSummonCount(), 0);

	int32_t index = 0;
	for (Creature* summon : creature->getSummons()) {
		pushUserdata<Creature>(L, summon);
		setCreatureMetatable(L, -1, summon);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetDescription(lua_State* L)
{
	// creature:getDescription(distance)
	int32_t distance = getNumber<int32_t>(L, 2);
	Creature* creature = getUserdata<Creature>(L, 1);
	if (creature) {
		pushString(L, creature->getDescription(distance));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCreatureGetPathTo(lua_State* L)
{
	// creature:getPathTo(pos[, minTargetDist = 0[, maxTargetDist = 1[, fullPathSearch = true[, clearSight = true[, maxSearchDist = 0]]]]])
	Creature* creature = getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const Position& position = getPosition(L, 2);

	FindPathParams fpp;
	fpp.minTargetDist = getNumber<int32_t>(L, 3, 0);
	fpp.maxTargetDist = getNumber<int32_t>(L, 4, 1);
	fpp.fullPathSearch = getBoolean(L, 5, fpp.fullPathSearch);
	fpp.clearSight = getBoolean(L, 6, fpp.clearSight);
	fpp.maxSearchDist = getNumber<int32_t>(L, 7, fpp.maxSearchDist);

	std::list<Direction> dirList;
	if (creature->getPathTo(position, dirList, fpp)) {
		lua_createtable(L, dirList.size(), 0);

		int32_t index = 0;
		for (Direction dir : dirList) {
			lua_pushnumber(L, dir);
			lua_rawseti(L, -2, ++index);
		}
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

// Player
int32_t LuaScriptInterface::luaPlayerCreate(lua_State* L)
{
	// Player(id or name or userdata)
	// Player.new(id or name or userdata)
	Player* player;
	if (isNumber(L, 2)) {
		player = g_game.getPlayerByID(getNumber<uint32_t>(L, 2));
	} else if (isString(L, 2)) {
		ReturnValue ret = g_game.getPlayerByNameWildcard(getString(L, 2), player);
		if (ret != RET_NOERROR) {
			lua_pushnil(L);
			lua_pushnumber(L, ret);
			return 2;
		}
	} else if (isUserdata(L, 2)) {
		player = getUserdata<Player>(L, 2);
	} else {
		player = nullptr;
	}

	if (player) {
		pushUserdata<Player>(L, player);
		setMetatable(L, -1, "Player");
	} else {
		lua_pushnil(L);
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
		lua_pushnumber(L, player->getGUID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetIp(lua_State* L)
{
	// player:getIp()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getIP());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetAccountId(lua_State* L)
{
	// player:getAccountId()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getAccount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetLastLoginSaved(lua_State* L)
{
	// player:getLastLoginSaved()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getLastLoginSaved());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetAccountType(lua_State* L)
{
	// player:getAccountType()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getAccountType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetAccountType(lua_State* L)
{
	// player:setAccountType(accountType)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->accountType = static_cast<AccountType_t>(getNumber<uint32_t>(L, 2));;
		IOLoginData::setAccountType(player->getAccount(), player->accountType);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetCapacity(lua_State* L)
{
	// player:getCapacity()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getCapacity());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetCapacity(lua_State* L)
{
	// player:setCapacity(capacity)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->capacity = std::max(0.0, std::min<double>(10000.0, getNumber<double>(L, 2)));
		player->sendStats();
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetFreeCapacity(lua_State* L)
{
	// player:getFreeCapacity()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getFreeCapacity());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetDepotChest(lua_State* L)
{
	// player:getDepotChest(depotId[, autoCreate = false])
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t depotId = getNumber<uint32_t>(L, 2);
	bool autoCreate = getBoolean(L, 3, false);
	DepotChest* depotChest = player->getDepotChest(depotId, autoCreate);
	if (depotChest) {
		pushUserdata<Item>(L, depotChest);
		setItemMetatable(L, -1, depotChest);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetInbox(lua_State* L)
{
	// player:getInbox()
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Inbox* inbox = player->getInbox();
	if (inbox) {
		pushUserdata<Item>(L, inbox);
		setItemMetatable(L, -1, inbox);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkull(lua_State* L)
{
	// player:getSkull()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getSkull());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetSkull(lua_State* L)
{
	// player:setSkull(skull)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setSkull(static_cast<Skulls_t>(getNumber<int64_t>(L, 2)));
		g_game.updatePlayerSkull(player);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkullTime(lua_State* L)
{
	// player:getSkullTime()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getSkullTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetSkullTime(lua_State* L)
{
	// player:setSkullTime(skullTime)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setSkullTicks(getNumber<int64_t>(L, 2));
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetDeathPenalty(lua_State* L)
{
	// player:getDeathPenalty()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, static_cast<uint32_t>(player->getLostPercent() * 100));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetExperience(lua_State* L)
{
	// player:getExperience()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getExperience());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddExperience(lua_State* L)
{
	// player:addExperience(experience[, sendText = false])
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		int64_t experience = getNumber<int64_t>(L, 2);
		bool sendText = getBoolean(L, 3, false);
		player->addExperience(experience, sendText);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveExperience(lua_State* L)
{
	// player:removeExperience(experience[, sendText = false])
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		int64_t experience = getNumber<int64_t>(L, 2);
		bool sendText = getBoolean(L, 3, false);
		player->removeExperience(experience, sendText);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetLevel(lua_State* L)
{
	// player:getLevel()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetMagicLevel(lua_State* L)
{
	// player:getMagicLevel()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getMagicLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetBaseMagicLevel(lua_State* L)
{
	// player:getBaseMagicLevel()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getBaseMagicLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetMaxMana(lua_State* L)
{
	// player:setMaxMana(maxMana)
	Player* player = getPlayer(L, 1);
	if (player) {
		player->manaMax = getNumber<int32_t>(L, 2);
		player->mana = std::min<int32_t>(player->mana, player->manaMax);
		player->sendStats();
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetManaSpent(lua_State* L)
{
	// player:getManaSpent()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getSpentMana());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddManaSpent(lua_State* L)
{
	// player:addManaSpent(amount)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->addManaSpent(getNumber<uint64_t>(L, 2));
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkillLevel(lua_State* L)
{
	// player:getSkillLevel(skillType)
	skills_t skillType = static_cast<skills_t>(getNumber<int64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		lua_pushnumber(L, player->skills[skillType][SKILLVALUE_LEVEL]);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetEffectiveSkillLevel(lua_State* L)
{
	// player:getEffectiveSkillLevel(skillType)
	skills_t skillType = static_cast<skills_t>(getNumber<int64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		lua_pushnumber(L, player->getSkill(skillType, SKILLVALUE_LEVEL));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkillPercent(lua_State* L)
{
	// player:getSkillPercent(skillType)
	skills_t skillType = static_cast<skills_t>(getNumber<int64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		lua_pushnumber(L, player->skills[skillType][SKILLVALUE_PERCENT]);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSkillTries(lua_State* L)
{
	// player:getSkillTries(skillType)
	skills_t skillType = static_cast<skills_t>(getNumber<int64_t>(L, 2));
	Player* player = getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		lua_pushnumber(L, player->skills[skillType][SKILLVALUE_TRIES]);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddSkillTries(lua_State* L)
{
	// player:addSkillTries(skillType, tries)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		skills_t skillType = static_cast<skills_t>(getNumber<int64_t>(L, 2));
		uint32_t tries = getNumber<uint32_t>(L, 3);
		player->addSkillAdvance(skillType, tries);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetItemCount(lua_State* L)
{
	// player:getItemCount(itemId[, subType = -1])
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t subType = getNumber<int32_t>(L, 3, -1);
	lua_pushnumber(L, player->__getItemTypeCount(itemId, subType));
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetItemById(lua_State* L)
{
	// player:getItemById(itemId, deepSearch[, subType = -1])
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}
	bool deepSearch = getBoolean(L, 3);
	int32_t subType = getNumber<int32_t>(L, 4, -1);

	Item* item = g_game.findItemOfType(player, itemId, deepSearch, subType);
	if (item) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetVocation(lua_State* L)
{
	// player:setVocation(vocation)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Vocation* vocation = getUserdata<Vocation>(L, 2);
	if (!vocation) {
		pushBoolean(L, false);
		return 1;
	}

	player->setVocation(vocation->getId());

	uint32_t promotedVocation = g_vocations.getPromotedVocation(player->getVocationId());
	if (promotedVocation == 0 && player->getVocationId() != promotedVocation) {
		player->addStorageValue(STORAGEVALUE_PROMOTION, 1);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSex(lua_State* L)
{
	// player:getSex()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getSex());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetSex(lua_State* L)
{
	// player:setSex(newSex)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		PlayerSex_t newSex = static_cast<PlayerSex_t>(getNumber<int64_t>(L, 2));
		player->setSex(newSex);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetGuild(lua_State* L)
{
	// player:getGuild()
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Guild* guild = player->getGuild();
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	pushUserdata<Guild>(L, guild);
	setMetatable(L, -1, "Guild");
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetGuildLevel(lua_State* L)
{
	// player:getGuildLevel()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getGuildLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetGuildLevel(lua_State* L)
{
	// player:setGuildLevel(level)
	uint8_t level = getNumber<uint8_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setGuildLevel(level);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetGuildNick(lua_State* L)
{
	// player:setGuildNick(nick)
	const std::string& nick = getString(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setGuildNick(nick);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetStamina(lua_State* L)
{
	// player:getStamina()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getStaminaMinutes());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetStamina(lua_State* L)
{
	// player:setStamina(stamina)
	uint16_t stamina = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->staminaMinutes = std::min<uint16_t>(2520, stamina);
		player->sendStats();
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSoul(lua_State* L)
{
	// player:getSoul()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getPlayerInfo(PLAYERINFO_SOUL));
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetMaxSoul(lua_State* L)
{
	// player:getMaxSoul()
	Player* player = getUserdata<Player>(L, 1);
	if (player && player->vocation) {
		lua_pushnumber(L, player->vocation->getSoulMax());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetBankBalance(lua_State* L)
{
	// player:getBankBalance()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getBankBalance());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetBankBalance(lua_State* L)
{
	// player:setBankBalance(bankBalance)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->setBankBalance(getNumber<uint64_t>(L, 2));
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetStorageValue(lua_State* L)
{
	// player:getStorageValue(key)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t key = getNumber<uint32_t>(L, 2);
	int32_t value;
	if (player->getStorageValue(key, value)) {
		lua_pushnumber(L, value);
	} else {
		lua_pushnumber(L, -1);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddItem(lua_State* L)
{
	// player:addItem(itemId[, count = 1[, canDropOnMap = true[, subType = 1[, slot = CONST_SLOT_WHEREEVER]]]])
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		pushBoolean(L, false);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t count = getNumber<int32_t>(L, 3, 1);
	int32_t subType = getNumber<int32_t>(L, 5, 1);

	const ItemType& it = Item::items[itemId];

	int32_t itemCount = 1;
	int32_t parameters = lua_gettop(L);
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
		lua_pushnil(L);
		return 1;
	}

	bool canDropOnMap = getBoolean(L, 4, true);
	slots_t slot = static_cast<slots_t>(getNumber<int64_t>(L, 6, CONST_SLOT_WHEREEVER));
	for (int32_t i = 1; i <= itemCount; ++i) {
		int32_t stackCount = subType;
		if (it.stackable) {
			stackCount = std::min<int32_t>(stackCount, 100);
			subType -= stackCount;
		}

		Item* item = Item::CreateItem(itemId, stackCount);
		if (!item) {
			if (!hasTable) {
				lua_pushnil(L);
			}
			return 1;
		}

		ReturnValue ret = g_game.internalPlayerAddItem(player, item, canDropOnMap, slot);
		if (ret != RET_NOERROR) {
			delete item;
			if (!hasTable) {
				lua_pushnil(L);
			}
			return 1;
		}

		if (hasTable) {
			lua_pushnumber(L, i);
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
	// player:addItemEx(item[, canDropOnMap = false[, slot = CONST_SLOT_WHEREEVER]])
	Item* item = getUserdata<Item>(L, 2);
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (item->getParent() != VirtualCylinder::virtualCylinder) {
		reportErrorFunc("Item already has a parent");
		pushBoolean(L, false);
		return 1;
	}

	bool canDropOnMap = getBoolean(L, 3, false);
	if (canDropOnMap) {
		slots_t slot = static_cast<slots_t>(getNumber<int64_t>(L, 4, CONST_SLOT_WHEREEVER));
		lua_pushnumber(L, g_game.internalPlayerAddItem(player, item, true, slot));
	} else {
		lua_pushnumber(L, g_game.internalAddItem(player, item));
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveItem(lua_State* L)
{
	// player:removeItem(itemId, count[, subType = -1[, ignoreEquipped = false]])
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	uint32_t count = getNumber<uint32_t>(L, 3);
	int32_t subType = getNumber<int32_t>(L, 4, -1);
	bool ignoreEquipped = getBoolean(L, 5, false);
	pushBoolean(L, player->removeItemOfType(itemId, count, subType, ignoreEquipped));
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetMoney(lua_State* L)
{
	// player:getMoney()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getMoney());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveMoney(lua_State* L)
{
	// player:removeMoney(money)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		uint64_t money = getNumber<uint64_t>(L, 2);
		pushBoolean(L, g_game.removeMoney(player, money));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerShowTextDialog(lua_State* L)
{
	// player:showTextDialog(itemId[, text[, canWrite[, length]]])
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	int32_t length = getNumber<int32_t>(L, 5, -1);
	bool canWrite = getBoolean(L, 4, false);
	std::string text;

	int32_t parameters = lua_gettop(L);
	if (parameters >= 3) {
		text = getString(L, 3);
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	Item* item = Item::CreateItem(itemId);
	if (!item) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		pushBoolean(L, false);
	}

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
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSendTextMessage(lua_State* L)
{
	// player:sendTextMessage(type, text[, position, primaryValue = 0, primaryColor = TEXTCOLOR_NONE[, secondaryValue = 0, secondaryColor = TEXTCOLOR_NONE]])
	int32_t parameters = lua_gettop(L);

	TextMessage message;
	if (parameters >= 8) {
		message.secondary.color = static_cast<TextColor_t>(getNumber<int64_t>(L, 8));
		message.secondary.value = getNumber<int32_t>(L, 7);
	}

	if (parameters >= 6) {
		message.primary.color = static_cast<TextColor_t>(getNumber<int64_t>(L, 6));
		message.primary.value = getNumber<int32_t>(L, 5);
		message.position = getPosition(L, 4);
	}

	message.text = getString(L, 3);
	message.type = static_cast<MessageClasses>(getNumber<int64_t>(L, 2));

	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->sendTextMessage(message);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSendChannelMessage(lua_State* L)
{
	// player:sendChannelMessage(author, text, type, channelId)
	uint16_t channelId = getNumber<uint16_t>(L, 5);
	SpeakClasses type = static_cast<SpeakClasses>(getNumber<int64_t>(L, 4));
	const std::string& text = getString(L, 3);
	const std::string& author = getString(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->sendChannelMessage(author, text, type, channelId);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSendPrivateMessage(lua_State* L)
{
	// player:sendPrivateMessage(speaker, text[, type])
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const Player* speaker = getUserdata<const Player>(L, 2);
	const std::string& text = getString(L, 3);
	SpeakClasses type = static_cast<SpeakClasses>(getNumber<int64_t>(L, 4, TALKTYPE_PRIVATE_FROM));
	player->sendPrivateMessage(speaker, type, text);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaPlayerChannelSay(lua_State* L)
{
	// player:channelSay(speaker, type, text, channelId)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Creature* speaker = getCreature(L, 2);
	SpeakClasses type = static_cast<SpeakClasses>(getNumber<int64_t>(L, 3));
	const std::string& text = getString(L, 4);
	uint16_t channelId = getNumber<uint16_t>(L, 5);
	player->sendToChannel(speaker, type, text, channelId);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaPlayerOpenChannel(lua_State* L)
{
	// player:openChannel(channelId)
	uint16_t channelId = getNumber<uint16_t>(L, 2);
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		g_game.playerOpenChannel(player->getID(), channelId);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetSlotItem(lua_State* L)
{
	// player:getSlotItem(slot)
	const Player* player = getUserdata<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t slot = getNumber<uint32_t>(L, 2);
	Thing* thing = player->__getThing(slot);
	if (!thing) {
		lua_pushnil(L);
		return 1;
	}

	Item* item = thing->getItem();
	if (item) {
		pushUserdata<Item>(L, item);
		setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetParty(lua_State* L)
{
	// player:getParty()
	const Player* player = getUserdata<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Party* party = player->getParty();
	if (party) {
		pushUserdata<Party>(L, party);
		setMetatable(L, -1, "Party");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddOutfit(lua_State* L)
{
	// player:addOutfit(lookType)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->addOutfit(getNumber<uint16_t>(L, 2), 0);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddOutfitAddon(lua_State* L)
{
	// player:addOutfitAddon(lookType, addon)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		uint16_t lookType = getNumber<uint16_t>(L, 2);
		uint8_t addon = getNumber<uint8_t>(L, 3);
		player->addOutfit(lookType, addon);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveOutfit(lua_State* L)
{
	// player:removeOutfit(lookType)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		uint16_t lookType = getNumber<uint16_t>(L, 2);
		pushBoolean(L, player->removeOutfit(lookType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveOutfitAddon(lua_State* L)
{
	// player:removeOutfitAddon(lookType, addon)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		uint16_t lookType = getNumber<uint16_t>(L, 2);
		uint8_t addon = getNumber<uint8_t>(L, 3);
		pushBoolean(L, player->removeOutfitAddon(lookType, addon));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerHasOutfit(lua_State* L)
{
	// player:hasOutfit(lookType[, addon = 0])
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		uint16_t lookType = getNumber<uint16_t>(L, 2);
		uint8_t addon = getNumber<uint8_t>(L, 3, 0);
		pushBoolean(L, player->canWear(lookType, addon));
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddMount(lua_State* L)
{
	// player:addMount(mountId)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		uint8_t mountId = getNumber<uint8_t>(L, 2);
		pushBoolean(L, player->tameMount(mountId));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveMount(lua_State* L)
{
	// player:removeMount(mountId)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		uint8_t mountId = getNumber<uint8_t>(L, 2);
		pushBoolean(L, player->untameMount(mountId));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerHasMount(lua_State* L)
{
	// player:hasMount(mountId)
	const Player* player = getUserdata<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t mountId = getNumber<uint8_t>(L, 2);
	Mount* mount = Mounts::getInstance()->getMountByID(mountId);
	if (mount) {
		pushBoolean(L, player->hasMount(mount));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetPremiumDays(lua_State* L)
{
	// player:getPremiumDays()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->premiumDays);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddPremiumDays(lua_State* L)
{
	// player:addPremiumDays(days)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (player->premiumDays != std::numeric_limits<uint16_t>::max()) {
		uint16_t days = getNumber<uint16_t>(L, 2);
		int32_t addDays = std::min<int32_t>(0xFFFE - player->premiumDays, days);
		if (addDays > 0) {
			player->setPremiumDays(player->premiumDays + addDays);
			IOLoginData::addPremiumDays(player->getAccount(), addDays);
		}
	}
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemovePremiumDays(lua_State* L)
{
	// player:removePremiumDays(days)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (player->premiumDays != std::numeric_limits<uint16_t>::max()) {
		uint16_t days = getNumber<uint16_t>(L, 2);
		int32_t removeDays = std::min<int32_t>(player->premiumDays, days);
		if (removeDays > 0) {
			player->setPremiumDays(player->premiumDays - removeDays);
			IOLoginData::removePremiumDays(player->getAccount(), removeDays);
		}
	}
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaPlayerHasBlessing(lua_State* L)
{
	// player:hasBlessing(blessing)
	uint8_t blessing = getNumber<uint8_t>(L, 2) - 1;
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, player->hasBlessing(blessing));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddBlessing(lua_State* L)
{
	// player:addBlessing(blessing)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t blessing = getNumber<uint8_t>(L, 2) - 1;
	if (player->hasBlessing(blessing)) {
		pushBoolean(L, false);
		return 1;
	}

	player->addBlessing(1 << blessing);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaPlayerRemoveBlessing(lua_State* L)
{
	// player:removeBlessing(blessing)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t blessing = getNumber<uint8_t>(L, 2) - 1;
	if (!player->hasBlessing(blessing)) {
		pushBoolean(L, false);
		return 1;
	}

	player->removeBlessing(1 << blessing);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaPlayerCanLearnSpell(lua_State* L)
{
	// player:canLearnSpell(spellName)
	const Player* player = getUserdata<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const std::string& spellName = getString(L, 2);
	InstantSpell* spell = g_spells->getInstantSpellByName(spellName);
	if (!spell) {
		reportErrorFunc("Spell \"" + spellName + "\" not found");
		pushBoolean(L, false);
		return 1;
	}

	if (player->hasFlag(PlayerFlag_IgnoreSpellCheck)) {
		pushBoolean(L, true);
		return 1;
	}

	const auto& vocMap = spell->getVocMap();
	if (vocMap.count(player->getVocationId()) == 0) {
		pushBoolean(L, false);
	} else if (player->getLevel() < spell->getLevel()) {
		pushBoolean(L, false);
	} else if (player->getMagicLevel() < spell->getMagicLevel()) {
		pushBoolean(L, false);
	} else {
		pushBoolean(L, true);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerLearnSpell(lua_State* L)
{
	// player:learnSpell(spellName)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		const std::string& spellName = getString(L, 2);
		player->learnInstantSpell(spellName);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerForgetSpell(lua_State* L)
{
	// player:forgetSpell(spellName)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		const std::string& spellName = getString(L, 2);
		player->forgetInstantSpell(spellName);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerHasLearnedSpell(lua_State* L)
{
	// player:hasLearnedSpell(spellName)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		const std::string& spellName = getString(L, 2);
		pushBoolean(L, player->hasLearnedInstantSpell(spellName));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSendTutorial(lua_State* L)
{
	// player:sendTutorial(tutorialId)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		uint8_t tutorialId = getNumber<uint8_t>(L, 2);
		player->sendTutorial(tutorialId);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerAddMapMark(lua_State* L)
{
	// player:addMapMark(position, type, description)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		const Position& position = getPosition(L, 2);
		uint8_t type = getNumber<uint8_t>(L, 3);
		const std::string& description = getString(L, 4);
		player->sendAddMarker(position, type, description);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSave(lua_State* L)
{
	// player:save()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		player->loginPosition = player->getPosition();
		pushBoolean(L, IOLoginData::savePlayer(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerPopupFYI(lua_State* L)
{
	// player:popupFYI(message)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		const std::string& message = getString(L, 2);
		player->sendFYIBox(message);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerIsPzLocked(lua_State* L)
{
	// player:isPzLocked()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		pushBoolean(L, player->isPzLocked());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetClient(lua_State* L)
{
	// player:getClient()
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_createtable(L, 0, 2);
		setField(L, "version", player->getProtocolVersion());
		setField(L, "os", player->getOperatingSystem());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetHouse(lua_State* L)
{
	// player:getHouse()
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	House* house = Houses::getInstance().getHouseByPlayerId(player->getGUID());
	if (house) {
		pushUserdata<House>(L, house);
		setMetatable(L, -1, "House");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerSetGhostMode(lua_State* L)
{
	// player:setGhostMode(enabled)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	bool enabled = getBoolean(L, 2);
	if (player->isInGhostMode() == enabled) {
		pushBoolean(L, true);
		return 1;
	}

	player->switchGhostMode();

	Tile* tile = player->getTile();
	const Position& position = player->getPosition();

	SpectatorVec list;
	g_game.getSpectators(list, position, true, true);
	for (Creature* spectator : list) {
		Player* tmpPlayer = spectator->getPlayer();
		if (tmpPlayer != player && !tmpPlayer->isAccessPlayer()) {
			if (enabled) {
				tmpPlayer->sendRemoveTileThing(position, tile->getStackposOfCreature(tmpPlayer, player));
			} else {
				tmpPlayer->sendCreatureAppear(player, position, true);
			}
		} else {
			tmpPlayer->sendCreatureChangeVisible(player, !enabled);
		}
	}

	if (player->isInGhostMode()) {
		for (const auto& it : g_game.getPlayers()) {
			if (!it.second->isAccessPlayer()) {
				it.second->notifyStatusChange(player, VIPSTATUS_OFFLINE);
			}
		}
		IOLoginData::updateOnlineStatus(player->getGUID(), false);
	} else {
		for (const auto& it : g_game.getPlayers()) {
			if (!it.second->isAccessPlayer()) {
				it.second->notifyStatusChange(player, VIPSTATUS_ONLINE);
			}
		}
		IOLoginData::updateOnlineStatus(player->getGUID(), true);
	}
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetContainerId(lua_State* L)
{
	// player:getContainerId(container)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Container* container = getUserdata<Container>(L, 2);
	if (container) {
		lua_pushnumber(L, player->getContainerID(container));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetContainerById(lua_State* L)
{
	// player:getContainerById(id)
	Player* player = getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Container* container = player->getContainerByID(getNumber<uint8_t>(L, 2));
	if (container) {
		pushUserdata<Container>(L, container);
		setMetatable(L, -1, "Container");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPlayerGetContainerIndex(lua_State* L)
{
	// player:getContainerIndex(id)
	Player* player = getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getContainerIndex(getNumber<uint8_t>(L, 2)));
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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

int32_t LuaScriptInterface::luaMonsterGetType(lua_State* L)
{
	// monster:getType()
	const Monster* monster = getUserdata<const Monster>(L, 1);
	if (monster) {
		pushUserdata<MonsterType>(L, monster->mType);
		setMetatable(L, -1, "MonsterType");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterGetSpawnPosition(lua_State* L)
{
	// monster:getSpawnPosition()
	const Monster* monster = getUserdata<const Monster>(L, 1);
	if (monster) {
		pushPosition(L, monster->getMasterPos());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterDespawn(lua_State* L)
{
	// monster:despawn()
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		pushBoolean(L, monster->despawn());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterIsIdle(lua_State* L)
{
	// monster:isIdle()
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		pushBoolean(L, monster->getIdleStatus());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterSetIdle(lua_State* L)
{
	// monster:setIdle(idle)
	Monster* monster = getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	monster->setIdle(getBoolean(L, 2));
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaMonsterIsTarget(lua_State* L)
{
	// monster:isTarget(creature)
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		const Creature* creature = getCreature(L, 2);
		pushBoolean(L, monster->isTarget(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterIsOpponent(lua_State* L)
{
	// monster:isOpponent(creature)
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		const Creature* creature = getCreature(L, 2);
		pushBoolean(L, monster->isOpponent(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterIsFriend(lua_State* L)
{
	// monster:isFriend(creature)
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		const Creature* creature = getCreature(L, 2);
		pushBoolean(L, monster->isFriend(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterAddFriend(lua_State* L)
{
	// monster:addFriend(creature)
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		Creature* creature = getCreature(L, 2);
		monster->addFriend(creature);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterRemoveFriend(lua_State* L)
{
	// monster:removeFriend(creature)
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		Creature* creature = getCreature(L, 2);
		monster->removeFriend(creature);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterGetFriendList(lua_State* L)
{
	// monster:getFriendList()
	Monster* monster = getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	const auto& friendList = monster->getFriendList();
	lua_createtable(L, friendList.size(), 0);

	int32_t index = 0;
	for (Creature* creature : friendList) {
		pushUserdata<Creature>(L, creature);
		setCreatureMetatable(L, -1, creature);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterGetFriendCount(lua_State* L)
{
	// monster:getFriendCount()
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		lua_pushnumber(L, monster->getFriendList().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterAddTarget(lua_State* L)
{
	// monster:addTarget(creature[, pushFront = false])
	Monster* monster = getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = getCreature(L, 2);
	bool pushFront = getBoolean(L, 3, false);
	monster->addTarget(creature, pushFront);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaMonsterRemoveTarget(lua_State* L)
{
	// monster:removeTarget(creature)
	Monster* monster = getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	monster->removeTarget(getCreature(L, 2));
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaMonsterGetTargetList(lua_State* L)
{
	// monster:getTargetList()
	Monster* monster = getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	const auto& targetList = monster->getTargetList();
	lua_createtable(L, targetList.size(), 0);

	int32_t index = 0;
	for (Creature* creature : targetList) {
		pushUserdata<Creature>(L, creature);
		setCreatureMetatable(L, -1, creature);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterGetTargetCount(lua_State* L)
{
	// monster:getTargetCount()
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		lua_pushnumber(L, monster->getTargetList().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterSelectTarget(lua_State* L)
{
	// monster:selectTarget(creature)
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		Creature* creature = getCreature(L, 2);
		pushBoolean(L, monster->selectTarget(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterSearchTarget(lua_State* L)
{
	// monster:searchTarget([searchType = TARGETSEARCH_DEFAULT])
	Monster* monster = getUserdata<Monster>(L, 1);
	if (monster) {
		TargetSearchType_t searchType = static_cast<TargetSearchType_t>(getNumber<int64_t>(L, 2, TARGETSEARCH_DEFAULT));
		pushBoolean(L, monster->searchTarget(searchType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Npc
int32_t LuaScriptInterface::luaNpcCreate(lua_State* L)
{
	// Npc([id/name])
	// Npc.new([id/name])
	Npc* npc = nullptr;
	if (lua_gettop(L) >= 2) {
		if (isNumber(L, 2)) {
			npc = g_game.getNpcByID(getNumber<uint32_t>(L, 2));
		} else if (isString(L, 2)) {
			npc = g_game.getNpcByName(getString(L, 2));
		}
	} else {
		npc = getScriptEnv()->getNpc();
	}

	if (npc) {
		pushUserdata<Npc>(L, npc);
		setMetatable(L, -1, "Npc");
	} else {
		lua_pushnil(L);
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

int32_t LuaScriptInterface::luaNpcSetMasterPos(lua_State* L)
{
	// npc:setMasterPos(pos[, radius])
	Npc* npc = getUserdata<Npc>(L, 1);
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	const Position& pos = getPosition(L, 2);
	int32_t radius = getNumber<int32_t>(L, 3, 1);
	npc->setMasterPos(pos, radius);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaNpcGetSpeechBubble(lua_State* L)
{
	// npc:getSpeechBubble()
	Npc* npc = getUserdata<Npc>(L, 1);
	if (npc) {
		lua_pushnumber(L, npc->getSpeechBubble());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaNpcSetSpeechBubble(lua_State* L)
{
	// npc:setSpeechBubble(speechBubble)
	Npc* npc = getUserdata<Npc>(L, 1);
	if (npc) {
		npc->setSpeechBubble(getNumber<uint8_t>(L, 2));
	}
	return 0;
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetId(lua_State* L)
{
	// guild:getId()
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		lua_pushnumber(L, guild->getId());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetMembersOnline(lua_State* L)
{
	// guild:getMembersOnline()
	const Guild* guild = getUserdata<const Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	const auto& members = guild->getMembersOnline();
	lua_createtable(L, members.size(), 0);

	int32_t index = 0;
	for (Player* player : members) {
		pushUserdata<Player>(L, player);
		setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildAddMember(lua_State* L)
{
	// guild:addMember(player)
	Guild* guild = getUserdata<Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	Player* player = getPlayer(L, 2);
	if (player) {
		guild->addMember(player);
		pushBoolean(L, true);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildRemoveMember(lua_State* L)
{
	// guild:removeMember(player)
	Guild* guild = getUserdata<Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	Player* player = getPlayer(L, 2);
	if (player) {
		guild->removeMember(player);
		pushBoolean(L, true);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildAddRank(lua_State* L)
{
	// guild:addRank(id, name, level)
	Guild* guild = getUserdata<Guild>(L, 1);
	if (guild) {
		uint32_t id = getNumber<uint32_t>(L, 2);
		const std::string& name = getString(L, 3);
		uint8_t level = getNumber<uint8_t>(L, 4);
		guild->addRank(id, name, level);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetRankById(lua_State* L)
{
	// guild:getRankById(id)
	Guild* guild = getUserdata<Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t id = getNumber<uint32_t>(L, 2);
	GuildRank* rank = guild->getRankById(id);
	if (rank) {
		lua_createtable(L, 0, 3);
		setField(L, "id", rank->id);
		setField(L, "name", rank->name);
		setField(L, "level", rank->level);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGuildGetRankByLevel(lua_State* L)
{
	// guild:getRankByLevel(level)
	const Guild* guild = getUserdata<const Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t level = getNumber<uint8_t>(L, 2);
	const GuildRank* rank = guild->getRankByLevel(level);
	if (rank) {
		lua_createtable(L, 0, 3);
		setField(L, "id", rank->id);
		setField(L, "name", rank->name);
		setField(L, "level", rank->level);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetId(lua_State* L)
{
	// group:getId()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->id);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetFlags(lua_State* L)
{
	// group:getFlags()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->flags);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetMaxDepotItems(lua_State* L)
{
	// group:getMaxDepotItems()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->maxDepotItems);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaGroupGetMaxVipEntries(lua_State* L)
{
	// group:getMaxVipEntries()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->maxVipEntries);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetId(lua_State* L)
{
	// vocation:getId()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetClientId(lua_State* L)
{
	// vocation:getClientId()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getClientId());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetRequiredSkillTries(lua_State* L)
{
	// vocation:getRequiredSkillTries(skillType, skillLevel)
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		skills_t skillType = static_cast<skills_t>(getNumber<int64_t>(L, 2));
		int32_t skillLevel = getNumber<int32_t>(L, 3);
		lua_pushnumber(L, vocation->getReqSkillTries(skillType, skillLevel));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetRequiredManaSpent(lua_State* L)
{
	// vocation:getRequiredManaSpent(magicLevel)
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		uint32_t magicLevel = getNumber<uint32_t>(L, 2);
		lua_pushnumber(L, vocation->getReqMana(magicLevel));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetCapacityGain(lua_State* L)
{
	// vocation:getCapacityGain()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getCapGain());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetHealthGain(lua_State* L)
{
	// vocation:getHealthGain()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getHPGain());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetHealthGainTicks(lua_State* L)
{
	// vocation:getHealthGainTicks()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getHealthGainTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetHealthGainAmount(lua_State* L)
{
	// vocation:getHealthGainAmount()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getHealthGainAmount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetManaGain(lua_State* L)
{
	// vocation:getManaGain()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getManaGain());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetManaGainTicks(lua_State* L)
{
	// vocation:getManaGainTicks()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getManaGainTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetManaGainAmount(lua_State* L)
{
	// vocation:getManaGainAmount()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getManaGainAmount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetMaxSoul(lua_State* L)
{
	// vocation:getMaxSoul()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getSoulMax());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetSoulGainTicks(lua_State* L)
{
	// vocation:getSoulGainTicks()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getSoulGainTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetAttackSpeed(lua_State* L)
{
	// vocation:getAttackSpeed()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getAttackSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetBaseSpeed(lua_State* L)
{
	// vocation:getBaseSpeed()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getBaseSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetDemotion(lua_State* L)
{
	// vocation:getDemotion()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (!vocation) {
		lua_pushnil(L);
		return 1;
	}

	Vocation* demotedVocation = g_vocations.getVocation(vocation->getFromVocation());
	if (demotedVocation && demotedVocation != vocation) {
		pushUserdata<Vocation>(L, demotedVocation);
		setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaVocationGetPromotion(lua_State* L)
{
	// vocation:getPromotion()
	Vocation* vocation = getUserdata<Vocation>(L, 1);
	if (!vocation) {
		lua_pushnil(L);
		return 1;
	}

	Vocation* promotedVocation = g_vocations.getVocation(g_vocations.getPromotedVocation(vocation->getId()));
	if (promotedVocation && promotedVocation != vocation) {
		pushUserdata<Vocation>(L, promotedVocation);
		setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTownGetId(lua_State* L)
{
	// town:getId()
	Town* town = getUserdata<Town>(L, 1);
	if (town) {
		lua_pushnumber(L, town->getID());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaTownGetTemplePosition(lua_State* L)
{
	// town:getTemplePosition()
	Town* town = getUserdata<Town>(L, 1);
	if (town) {
		pushPosition(L, town->getTemplePosition());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetId(lua_State* L)
{
	// house:getId()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getId());
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetTown(lua_State* L)
{
	// house:getTown()
	House* house = getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	Town* town = Towns::getInstance().getTown(house->getTownId());
	if (town) {
		pushUserdata<Town>(L, town);
		setMetatable(L, -1, "Town");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetExitPosition(lua_State* L)
{
	// house:getExitPosition()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		pushPosition(L, house->getEntryPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetRent(lua_State* L)
{
	// house:getRent()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getRent());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetOwnerGuid(lua_State* L)
{
	// house:getOwnerGuid()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getOwner());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseSetOwnerGuid(lua_State* L)
{
	// house:setOwnerGuid(guid[, updateDatabase = true])
	House* house = getUserdata<House>(L, 1);
	if (house) {
		uint32_t guid = getNumber<uint32_t>(L, 2);
		bool updateDatabase = getBoolean(L, 3, true);
		house->setOwner(guid, updateDatabase);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetBeds(lua_State* L)
{
	// house:getBeds()
	House* house = getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& beds = house->getBeds();
	lua_createtable(L, beds.size(), 0);

	int32_t index = 0;
	for (BedItem* bedItem : beds) {
		pushUserdata<Item>(L, bedItem);
		setItemMetatable(L, -1, bedItem);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetBedCount(lua_State* L)
{
	// house:getBedCount()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getBedCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetDoors(lua_State* L)
{
	// house:getDoors()
	House* house = getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& doors = house->getDoors();
	lua_createtable(L, doors.size(), 0);

	int32_t index = 0;
	for (Door* door : doors) {
		pushUserdata<Item>(L, door);
		setItemMetatable(L, -1, door);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetDoorCount(lua_State* L)
{
	// house:getDoorCount()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getDoors().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetTiles(lua_State* L)
{
	// house:getTiles()
	House* house = getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& tiles = house->getTiles();
	lua_createtable(L, tiles.size(), 0);

	int32_t index = 0;
	for (Tile* tile : tiles) {
		pushUserdata<Tile>(L, tile);
		setMetatable(L, -1, "Tile");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetTileCount(lua_State* L)
{
	// house:getTileCount()
	House* house = getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getTiles().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseGetAccessList(lua_State* L)
{
	// house:getAccessList(listId)
	House* house = getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	std::string list;
	uint32_t listId = getNumber<uint32_t>(L, 2);
	if (house->getAccessList(listId, list)) {
		pushString(L, list);
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t LuaScriptInterface::luaHouseSetAccessList(lua_State* L)
{
	// house:setAccessList(listId, list)
	House* house = getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t listId = getNumber<uint32_t>(L, 2);
	const std::string& list = getString(L, 3);
	house->setAccessList(listId, list);
	pushBoolean(L, true);
	return 1;
}

// ItemType
int32_t LuaScriptInterface::luaItemTypeCreate(lua_State* L)
{
	// ItemType(id or name)
	// ItemType.new(id or name)
	uint32_t id;
	if (isNumber(L, 2)) {
		id = getNumber<uint32_t>(L, 2);
	} else {
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeIsReadable(lua_State* L)
{
	// itemType:isReadable()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->canReadText);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeIsWritable(lua_State* L)
{
	// itemType:isWritable()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->canWriteText);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetType(lua_State* L)
{
	// itemType:getType()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->type);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetId(lua_State* L)
{
	// itemType:getId()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->id);
	} else {
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
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
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetSlotPosition(lua_State *L)
{
	// itemType:getSlotPosition()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->slotPosition);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetFluidSource(lua_State* L)
{
	// itemType:getFluidSource()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->fluidSource);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetCapacity(lua_State* L)
{
	// itemType:getCapacity()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->maxItems);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetWeight(lua_State* L)
{
	// itemType:getWeight([count = 1[, precise = true]])
	uint16_t count = getNumber<uint16_t>(L, 2, 1);
	bool precise = getBoolean(L, 3, true);

	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	double weight = itemType->weight * std::max<int32_t>(1, count);
	if (precise) {
		std::ostringstream ws;
		ws << std::fixed << std::setprecision(2) << weight;
		weight = std::stof(ws.str());
	}
	lua_pushnumber(L, weight);
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetAttack(lua_State* L)
{
	// itemType:getAttack()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->attack);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetDefense(lua_State* L)
{
	// itemType:getDefense()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->defense);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetExtraDefense(lua_State* L)
{
	// itemType:getExtraDefense()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->extraDefense);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetArmor(lua_State* L)
{
	// itemType:getArmor()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->armor);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetWeaponType(lua_State* L)
{
	// itemType:getWeaponType()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->weaponType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetElementType(lua_State* L)
{
	// itemType:getElementType()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	Abilities* abilities = itemType->abilities;
	if (abilities) {
		lua_pushnumber(L, abilities->elementType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetElementDamage(lua_State* L)
{
	// itemType:getElementDamage()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	Abilities* abilities = itemType->abilities;
	if (abilities) {
		lua_pushnumber(L, abilities->elementDamage);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetTransformEquipId(lua_State* L)
{
	// itemType:getTransformEquipId()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->transformEquipTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetTransformDeEquipId(lua_State* L)
{
	// itemType:getTransformDeEquipId()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->transformDeEquipTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeGetDecayId(lua_State* L)
{
	// itemType:getDecayId()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->decayTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaItemTypeHasSubType(lua_State* L)
{
	// itemType:hasSubType()
	const ItemType* itemType = getUserdata<const ItemType>(L, 1);
	if (itemType) {
		pushBoolean(L, itemType->hasSubType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Combat
int32_t LuaScriptInterface::luaCombatCreate(lua_State* L)
{
	// Combat()
	// Combat.new()
	uint32_t id = g_luaEnvironment.createCombatObject(getScriptEnv()->getScriptInterface());
	pushUserdata<Combat>(L, g_luaEnvironment.getCombatObject(id));
	setMetatable(L, -1, "Combat");
	return 1;
}

int32_t LuaScriptInterface::luaCombatSetParameter(lua_State* L)
{
	// combat:setParameter(key, value)
	uint32_t value = getNumber<uint32_t>(L, 3);
	CombatParam_t key = static_cast<CombatParam_t>(getNumber<int64_t>(L, 2));
	Combat* combat = getUserdata<Combat>(L, 1);
	if (combat) {
		pushBoolean(L, combat->setParam(key, value));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCombatSetFormula(lua_State* L)
{
	// combat:setFormula(type, mina, minb, maxa, maxb)
	Combat* combat = getUserdata<Combat>(L, 1);
	if (!combat) {
		lua_pushnil(L);
		return 1;
	}

	formulaType_t type = static_cast<formulaType_t>(getNumber<int64_t>(L, 2));
	double mina = getNumber<double>(L, 3);
	double minb = getNumber<double>(L, 4);
	double maxa = getNumber<double>(L, 5);
	double maxb = getNumber<double>(L, 6);
	combat->setPlayerCombatValues(type, mina, minb, maxa, maxb);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaCombatSetArea(lua_State* L)
{
	// combat:setArea(area)
	if (getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc("This function can only be used while loading the script.");
		lua_pushnil(L);
		return 1;
	}

	const AreaCombat* area = g_luaEnvironment.getAreaObject(getNumber<uint32_t>(L, 2));
	if (!area) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_AREA_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	Combat* combat = getUserdata<Combat>(L, 1);
	if (combat) {
		combat->setArea(new AreaCombat(*area));
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCombatSetCondition(lua_State* L)
{
	// combat:setCondition(condition)
	Condition* condition = getUserdata<Condition>(L, 2);
	Combat* combat = getUserdata<Combat>(L, 1);
	if (combat && condition) {
		combat->setCondition(condition->clone());
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaCombatSetCallback(lua_State* L)
{
	// combat:setCallBack(key, function)
	Combat* combat = getUserdata<Combat>(L, 1);
	if (!combat) {
		lua_pushnil(L);
		return 1;
	}

	CallBackParam_t key = static_cast<CallBackParam_t>(getNumber<int64_t>(L, 2));
	if (!combat->setCallback(key)) {
		lua_pushnil(L);
		return 1;
	}

	CallBack* callback = combat->getCallback(key);
	if (!callback) {
		lua_pushnil(L);
		return 1;
	}

	const std::string& function = getString(L, 3);
	pushBoolean(L, callback->loadCallBack(getScriptEnv()->getScriptInterface(), function));
	return 1;
}

int32_t LuaScriptInterface::luaCombatExecute(lua_State* L)
{
	// combat:execute(creature, variant)
	Combat* combat = getUserdata<Combat>(L, 1);
	if (!combat) {
		pushBoolean(L, false);
		return 1;
	}

	Creature* creature = getCreature(L, 2);

	const LuaVariant& variant = getVariant(L, 3);
	switch (variant.type) {
		case VARIANT_NUMBER: {
			Creature* target = g_game.getCreatureByID(variant.number);
			if (!target) {
				pushBoolean(L, false);
				return 1;
			}

			if (combat->hasArea()) {
				combat->doCombat(creature, target->getPosition());
			} else {
				combat->doCombat(creature, target);
			}
			break;
		}

		case VARIANT_POSITION: {
			combat->doCombat(creature, variant.pos);
			break;
		}

		case VARIANT_TARGETPOSITION: {
			if (combat->hasArea()) {
				combat->doCombat(creature, variant.pos);
			} else {
				combat->postCombatEffects(creature, variant.pos);
				g_game.addMagicEffect(variant.pos, CONST_ME_POFF);
			}
			break;
		}

		case VARIANT_STRING: {
			Player* target = g_game.getPlayerByName(variant.text);
			if (!target) {
				pushBoolean(L, false);
				return 1;
			}

			combat->doCombat(creature, target);
			break;
		}

		case VARIANT_NONE: {
			reportErrorFunc(getErrorDesc(LUA_ERROR_VARIANT_NOT_FOUND));
			pushBoolean(L, false);
			return 1;
		}

		default: {
			break;
		}
	}

	pushBoolean(L, true);
	return 1;
}

// Condition
int32_t LuaScriptInterface::luaConditionCreate(lua_State* L)
{
	// Condition(conditionType[, conditionId = CONDITIONID_COMBAT])
	// Condition.new(conditionType[, conditionId = CONDITIONID_COMBAT])
	ConditionType_t conditionType = static_cast<ConditionType_t>(getNumber<int64_t>(L, 2));
	ConditionId_t conditionId = static_cast<ConditionId_t>(getNumber<int64_t>(L, 3, CONDITIONID_COMBAT));
	uint32_t id;
	if (g_luaEnvironment.createConditionObject(getScriptEnv()->getScriptInterface(), conditionType, conditionId, id)) {
		pushUserdata<Condition>(L, g_luaEnvironment.getConditionObject(id));
		setMetatable(L, -1, "Condition");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionGetId(lua_State* L)
{
	// condition:getId()
	Condition* condition = getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionGetSubId(lua_State* L)
{
	// condition:getSubId()
	Condition* condition = getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getSubId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionGetType(lua_State* L)
{
	// condition:getType()
	Condition* condition = getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionGetIcons(lua_State* L)
{
	// condition:getIcons()
	Condition* condition = getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getIcons());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionGetEndTime(lua_State* L)
{
	// condition:getEndTime()
	Condition* condition = getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getEndTime());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionClone(lua_State* L)
{
	// condition:clone()
	Condition* condition = getUserdata<Condition>(L, 1);
	if (condition) {
		pushUserdata<Condition>(L, condition->clone());
		setMetatable(L, -1, "Condition");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionGetTicks(lua_State* L)
{
	// condition:getTicks()
	Condition* condition = getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionSetTicks(lua_State* L)
{
	// condition:setTicks(ticks)
	int32_t ticks = getNumber<int32_t>(L, 2);
	Condition* condition = getUserdata<Condition>(L, 1);
	if (condition) {
		condition->setTicks(ticks);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionSetParameter(lua_State* L)
{
	// condition:setParameter(key, value)
	Condition* condition = getUserdata<Condition>(L, 1);
	if (!condition) {
		lua_pushnil(L);
		return 1;
	}

	ConditionParam_t key = static_cast<ConditionParam_t>(getNumber<int64_t>(L, 2));
	int32_t value;
	if (isBoolean(L, 3)) {
		value = getBoolean(L, 3) ? 1 : 0;
	} else {
		value = getNumber<int32_t>(L, 3);
	}
	condition->setParam(key, value);
	pushBoolean(L, true);
	return 1;
}

int32_t LuaScriptInterface::luaConditionSetFormula(lua_State* L)
{
	// condition:setFormula(mina, minb, maxa, maxb)
	double maxb = getNumber<double>(L, 5);
	double maxa = getNumber<double>(L, 4);
	double minb = getNumber<double>(L, 3);
	double mina = getNumber<double>(L, 2);
	ConditionSpeed* condition = dynamic_cast<ConditionSpeed*>(getUserdata<Condition>(L, 1));
	if (condition) {
		condition->setFormulaVars(mina, minb, maxa, maxb);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionAddDamage(lua_State* L)
{
	// condition:addDamage(rounds, time, value)
	int32_t value = getNumber<int32_t>(L, 4);
	int32_t time = getNumber<int32_t>(L, 3);
	int32_t rounds = getNumber<int32_t>(L, 2);
	ConditionDamage* condition = dynamic_cast<ConditionDamage*>(getUserdata<Condition>(L, 1));
	if (condition) {
		pushBoolean(L, condition->addDamage(rounds, time, value));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaConditionAddOutfit(lua_State* L)
{
	// condition:addOutfit(outfit)
	// condition:addOutfit(lookTypeEx, lookType, lookHead, lookBody, lookLegs, lookFeet[, lookAddons[, lookMount]])
	Outfit_t outfit;
	if (isTable(L, 2)) {
		outfit = getOutfit(L, 2);
	} else {
		outfit.lookMount = getNumber<uint16_t>(L, 9, outfit.lookMount);
		outfit.lookAddons = getNumber<uint8_t>(L, 8, outfit.lookAddons);
		outfit.lookFeet = getNumber<uint8_t>(L, 7);
		outfit.lookLegs = getNumber<uint8_t>(L, 6);
		outfit.lookBody = getNumber<uint8_t>(L, 5);
		outfit.lookHead = getNumber<uint8_t>(L, 4);
		outfit.lookType = getNumber<uint16_t>(L, 3);
		outfit.lookTypeEx = getNumber<uint16_t>(L, 2);
	}
	ConditionOutfit* condition = dynamic_cast<ConditionOutfit*>(getUserdata<Condition>(L, 1));
	if (condition) {
		condition->addOutfit(outfit);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// MonsterType
int32_t LuaScriptInterface::luaMonsterTypeCreate(lua_State* L)
{
	// MonsterType(id or name)
	// MonsterType.new(id or name)
	MonsterType* monsterType;
	if (isNumber(L, 2)) {
		monsterType = g_monsters.getMonsterType(getNumber<uint32_t>(L, 2));
	} else {
		monsterType = g_monsters.getMonsterType(getString(L, 2));
	}

	if (monsterType) {
		pushUserdata<MonsterType>(L, monsterType);
		setMetatable(L, -1, "MonsterType");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeIsAttackable(lua_State* L)
{
	// monsterType:isAttackable()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushBoolean(L, monsterType->isAttackable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeIsConvinceable(lua_State* L)
{
	// monsterType:isConvinceable()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushBoolean(L, monsterType->isConvinceable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeIsSummonable(lua_State* L)
{
	// monsterType:isSummonable()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushBoolean(L, monsterType->isSummonable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeIsIllusionable(lua_State* L)
{
	// monsterType:isIllusionable()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushBoolean(L, monsterType->isIllusionable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeIsHostile(lua_State* L)
{
	// monsterType:isHostile()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushBoolean(L, monsterType->isHostile);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeIsPushable(lua_State* L)
{
	// monsterType:isPushable()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushBoolean(L, monsterType->pushable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeIsHealthShown(lua_State* L)
{
	// monsterType:isHealthShown()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushBoolean(L, !monsterType->hiddenHealth);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeCanPushItems(lua_State* L)
{
	// monsterType:canPushItems()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushBoolean(L, monsterType->canPushItems);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeCanPushCreatures(lua_State* L)
{
	// monsterType:canPushCreatures()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushBoolean(L, monsterType->canPushCreatures);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetName(lua_State* L)
{
	// monsterType:getName()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushString(L, monsterType->name);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetNameDescription(lua_State* L)
{
	// monsterType:getNameDescription()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushString(L, monsterType->nameDescription);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetHealth(lua_State* L)
{
	// monsterType:getHealth()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->health);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetMaxHealth(lua_State* L)
{
	// monsterType:getMaxHealth()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->healthMax);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetRunHealth(lua_State* L)
{
	// monsterType:getRunHealth()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->runAwayHealth);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetExperience(lua_State* L)
{
	// monsterType:getExperience()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->experience);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetCombatImmunities(lua_State* L)
{
	// monsterType:getCombatImmunities()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->damageImmunities);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetConditionImmunities(lua_State* L)
{
	// monsterType:getConditionImmunities()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->conditionImmunities);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetAttackList(lua_State* L)
{
	// monsterType:getAttackList()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, monsterType->spellAttackList.size(), 0);

	int32_t index = 0;
	for (const auto& spellBlock : monsterType->spellAttackList) {
		lua_createtable(L, 0, 8);

		setField(L, "chance", spellBlock.chance);
		setField(L, "isCombatSpell", spellBlock.combatSpell ? 1 : 0);
		setField(L, "isMelee", spellBlock.isMelee ? 1 : 0);
		setField(L, "minCombatValue", spellBlock.minCombatValue);
		setField(L, "maxCombatValue", spellBlock.maxCombatValue);
		setField(L, "range", spellBlock.range);
		setField(L, "speed", spellBlock.speed);
		pushUserdata<CombatSpell>(L, static_cast<CombatSpell*>(spellBlock.spell));
		lua_setfield(L, -2, "spell");

		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetDefenseList(lua_State* L)
{
	// monsterType:getDefenseList()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, monsterType->spellDefenseList.size(), 0);

	int32_t index = 0;
	for (const auto& spellBlock : monsterType->spellDefenseList) {
		lua_createtable(L, 0, 8);

		setField(L, "chance", spellBlock.chance);
		setField(L, "isCombatSpell", spellBlock.combatSpell ? 1 : 0);
		setField(L, "isMelee", spellBlock.isMelee ? 1 : 0);
		setField(L, "minCombatValue", spellBlock.minCombatValue);
		setField(L, "maxCombatValue", spellBlock.maxCombatValue);
		setField(L, "range", spellBlock.range);
		setField(L, "speed", spellBlock.speed);
		pushUserdata<CombatSpell>(L, static_cast<CombatSpell*>(spellBlock.spell));
		lua_setfield(L, -2, "spell");

		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetElementList(lua_State* L)
{
	// monsterType:getElementList()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, monsterType->elementMap.size(), 0);
	for (const auto& elementEntry : monsterType->elementMap) {
		lua_pushnumber(L, elementEntry.second);
		lua_rawseti(L, -2, elementEntry.first);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetVoices(lua_State* L)
{
	// monsterType:getVoices()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	int32_t index = 0;
	lua_createtable(L, monsterType->voiceVector.size(), 0);
	for (const auto& voiceBlock : monsterType->voiceVector) {
		lua_createtable(L, 0, 2);
		setField(L, "text", voiceBlock.text);
		setField(L, "yellText", voiceBlock.yellText);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetLoot(lua_State* L)
{
	// monsterType:getLoot()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	static const std::function<void(const std::list<LootBlock>&)> parseLoot = [&](const std::list<LootBlock>& lootList) {
		lua_createtable(L, lootList.size(), 0);

		int32_t index = 0;
		for (const auto& lootBlock : lootList) {
			lua_createtable(L, 0, 7);

			setField(L, "itemId", lootBlock.id);
			setField(L, "chance", lootBlock.chance);
			setField(L, "subType", lootBlock.subType);
			setField(L, "maxCount", lootBlock.countmax);
			setField(L, "actionId", lootBlock.actionId);
			setField(L, "text", lootBlock.text);

			parseLoot(lootBlock.childLoot);
			lua_setfield(L, -2, "childLoot");

			lua_rawseti(L, -2, ++index);
		}
	};
	parseLoot(monsterType->lootItems);
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetCreatureEvents(lua_State* L)
{
	// monsterType:getCreatureEvents()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	int32_t index = 0;
	lua_createtable(L, monsterType->scriptList.size(), 0);
	for (const std::string& creatureEvent : monsterType->scriptList) {
		pushString(L, creatureEvent);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetSummonList(lua_State* L)
{
	// monsterType:getSummonList()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	int32_t index = 0;
	lua_createtable(L, monsterType->summonList.size(), 0);
	for (const auto& summonBlock : monsterType->summonList) {
		lua_createtable(L, 0, 3);
		setField(L, "name", summonBlock.name);
		setField(L, "speed", summonBlock.speed);
		setField(L, "chance", summonBlock.chance);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetMaxSummons(lua_State* L)
{
	// monsterType:getMaxSummons()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->maxSummons);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetArmor(lua_State* L)
{
	// monsterType:getArmor()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->armor);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetDefense(lua_State* L)
{
	// monsterType:getDefense()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->defense);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetOutfit(lua_State* L)
{
	// monsterType:getOutfit()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		pushOutfit(L, monsterType->outfit);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetRace(lua_State* L)
{
	// monsterType:getRace()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->race);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetCorpseId(lua_State* L)
{
	// monsterType:getCorpseId()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->lookcorpse);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetManaCost(lua_State* L)
{
	// monsterType:getManaCost()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->manaCost);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetBaseSpeed(lua_State* L)
{
	// monsterType:getBaseSpeed()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->baseSpeed);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetLight(lua_State* L)
{
	// monsterType:getLight()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, monsterType->lightLevel);
	lua_pushnumber(L, monsterType->lightColor);
	return 2;
}

int32_t LuaScriptInterface::luaMonsterTypeGetStaticAttackChance(lua_State* L)
{
	// monsterType:getStaticAttackChance()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->staticAttackChance);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetTargetDistance(lua_State* L)
{
	// monsterType:getTargetDistance()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->targetDistance);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetYellChance(lua_State* L)
{
	// monsterType:getYellChance()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->yellChance);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetYellSpeedTicks(lua_State* L)
{
	// monsterType:getYellSpeedTicks()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->yellSpeedTicks);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetChangeTargetChance(lua_State* L)
{
	// monsterType:getChangeTargetChance()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->changeTargetChance);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeGetChangeTargetSpeed(lua_State* L)
{
	// monsterType:getChangeTargetSpeed()
	MonsterType* monsterType = getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		lua_pushnumber(L, monsterType->changeTargetSpeed);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Party
int32_t LuaScriptInterface::luaPartyDisband(lua_State* L)
{
	// party:disband()
	Party** partyPtr = getRawUserdata<Party>(L, 1);
	if (partyPtr && *partyPtr) {
		Party*& party = *partyPtr;
		party->disband();
		party = nullptr;
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyGetLeader(lua_State* L)
{
	// party:getLeader()
	Party* party = getUserdata<Party>(L, 1);
	if (!party) {
		lua_pushnil(L);
		return 1;
	}

	Player* leader = party->getLeader();
	if (leader) {
		pushUserdata<Player>(L, leader);
		setMetatable(L, -1, "Player");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartySetLeader(lua_State* L)
{
	// party:setLeader(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->passPartyLeadership(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyGetMembers(lua_State* L)
{
	// party:getMembers()
	Party* party = getUserdata<Party>(L, 1);
	if (!party) {
		lua_pushnil(L);
		return 1;
	}

	int32_t index = 0;
	lua_createtable(L, party->getMemberCount(), 0);
	for (Player* player : party->getMembers()) {
		pushUserdata<Player>(L, player);
		setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyGetMemberCount(lua_State* L)
{
	// party:getMemberCount()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		lua_pushnumber(L, party->getMemberCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyGetInvitees(lua_State* L)
{
	// party:getInvitees()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		lua_createtable(L, party->getInvitationCount(), 0);

		int32_t index = 0;
		for (Player* player : party->getInvitees()) {
			pushUserdata<Player>(L, player);
			setMetatable(L, -1, "Player");
			lua_rawseti(L, -2, ++index);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyGetInviteeCount(lua_State* L)
{
	// party:getInviteeCount()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		lua_pushnumber(L, party->getInvitationCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyAddInvite(lua_State* L)
{
	// party:addInvite(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->invitePlayer(*player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyRemoveInvite(lua_State* L)
{
	// party:removeInvite(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->removeInvite(*player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyAddMember(lua_State* L)
{
	// party:addMember(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->joinParty(*player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyRemoveMember(lua_State* L)
{
	// party:removeMember(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->leaveParty(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyIsSharedExperienceActive(lua_State* L)
{
	// party:isSharedExperienceActive()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		pushBoolean(L, party->isSharedExperienceActive());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyIsSharedExperienceEnabled(lua_State* L)
{
	// party:isSharedExperienceEnabled()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		pushBoolean(L, party->isSharedExperienceEnabled());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartyShareExperience(lua_State* L)
{
	// party:shareExperience(experience)
	uint64_t experience = getNumber<uint64_t>(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		party->shareExperience(experience);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaPartySetSharedExperience(lua_State* L)
{
	// party:setSharedExperience(active)
	bool active = getBoolean(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		pushBoolean(L, party->setSharedExperience(party->getLeader(), active));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

//
LuaEnvironment::LuaEnvironment() :
	LuaScriptInterface("Main Interface"), m_testInterface(nullptr),
	m_lastEventTimerId(0), m_lastCombatId(0), m_lastConditionId(0), m_lastAreaId(0)
{
	//
}

LuaEnvironment::~LuaEnvironment()
{
	delete m_testInterface;
	closeState();
}

bool LuaEnvironment::initState()
{
	m_luaState = luaL_newstate();
	if (!m_luaState) {
		return false;
	}

	luaL_openlibs(m_luaState);
	registerFunctions();

	m_runningEventId = EVENT_ID_USER;
	return true;
}

bool LuaEnvironment::reInitState()
{
	// TODO: get children, reload children
	closeState();
	return initState();
}

bool LuaEnvironment::closeState()
{
	if (!m_luaState) {
		return false;
	}

	for (const auto& combatEntry : m_combatIdMap) {
		clearCombatObjects(combatEntry.first);
	}

	for (const auto& conditionEntry : m_conditionIdMap) {
		clearConditionObjects(conditionEntry.first);
	}

	for (const auto& areaEntry : m_areaIdMap) {
		clearAreaObjects(areaEntry.first);
	}

	for (auto& timerEntry : m_timerEvents) {
		LuaTimerEventDesc timerEventDesc = std::move(timerEntry.second);
		for (int32_t parameter : timerEventDesc.parameters) {
			luaL_unref(m_luaState, LUA_REGISTRYINDEX, parameter);
		}
		luaL_unref(m_luaState, LUA_REGISTRYINDEX, timerEventDesc.function);
	}

	m_combatIdMap.clear();
	m_conditionIdMap.clear();
	m_areaIdMap.clear();
	m_timerEvents.clear();
	m_cacheFiles.clear();

	lua_close(m_luaState);
	m_luaState = nullptr;
	return true;
}

LuaScriptInterface* LuaEnvironment::getTestInterface()
{
	if (!m_testInterface) {
		m_testInterface = new LuaScriptInterface("Test Interface");
		m_testInterface->initState();
	}
	return m_testInterface;
}

Combat* LuaEnvironment::getCombatObject(uint32_t id) const
{
	auto it = m_combatMap.find(id);
	if (it == m_combatMap.end()) {
		return nullptr;
	}
	return it->second;
}

uint32_t LuaEnvironment::createCombatObject(LuaScriptInterface* interface)
{
	m_combatMap[++m_lastCombatId] = new Combat;
	m_combatIdMap[interface].push_back(m_lastCombatId);
	return m_lastCombatId;
}

void LuaEnvironment::clearCombatObjects(LuaScriptInterface* interface)
{
	auto it = m_combatIdMap.find(interface);
	if (it == m_combatIdMap.end()) {
		return;
	}

	for (uint32_t id : it->second) {
		auto itt = m_combatMap.find(id);
		if (itt != m_combatMap.end()) {
			delete itt->second;
			m_combatMap.erase(itt);
		}
	}
	it->second.clear();
}

Condition* LuaEnvironment::getConditionObject(uint32_t id) const
{
	auto it = m_conditionMap.find(id);
	if (it == m_conditionMap.end()) {
		return nullptr;
	}
	return it->second;
}

bool LuaEnvironment::createConditionObject(LuaScriptInterface* interface, ConditionType_t conditionType, ConditionId_t conditionId, uint32_t& id)
{
	Condition* condition = Condition::createCondition(conditionId, conditionType, 0, 0);
	if (!condition) {
		return false;
	}
	id = ++m_lastConditionId;
	m_conditionMap[m_lastConditionId] = condition;
	m_conditionIdMap[interface].push_back(m_lastConditionId);
	return true;
}

void LuaEnvironment::clearConditionObjects(LuaScriptInterface* interface)
{
	auto it = m_conditionIdMap.find(interface);
	if (it == m_conditionIdMap.end()) {
		return;
	}

	for (uint32_t id : it->second) {
		auto itt = m_conditionMap.find(id);
		if (itt != m_conditionMap.end()) {
			delete itt->second;
			m_conditionMap.erase(itt);
		}
	}
	it->second.clear();
}

AreaCombat* LuaEnvironment::getAreaObject(uint32_t id) const
{
	auto it = m_areaMap.find(id);
	if (it == m_areaMap.end()) {
		return nullptr;
	}
	return it->second;
}

uint32_t LuaEnvironment::createAreaObject(LuaScriptInterface* interface)
{
	m_areaMap[++m_lastAreaId] = new AreaCombat;
	m_areaIdMap[interface].push_back(m_lastAreaId);
	return m_lastAreaId;
}

void LuaEnvironment::clearAreaObjects(LuaScriptInterface* interface)
{
	auto it = m_areaIdMap.find(interface);
	if (it == m_areaIdMap.end()) {
		return;
	}

	for (uint32_t id : it->second) {
		auto itt = m_areaMap.find(id);
		if (itt != m_areaMap.end()) {
			delete itt->second;
			m_areaMap.erase(itt);
		}
	}
	it->second.clear();
}

void LuaEnvironment::executeTimerEvent(uint32_t eventIndex)
{
	auto it = m_timerEvents.find(eventIndex);
	if (it == m_timerEvents.end()) {
		return;
	}
	
	LuaTimerEventDesc timerEventDesc = std::move(it->second);
	m_timerEvents.erase(it);
	
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
}
