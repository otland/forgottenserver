// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "luascript.h"

#include "chat.h"
#include "combat.h"
#include "configmanager.h"
#include "databasemanager.h"
#include "databasetasks.h"
#include "game.h"
#include "luaapi.h"
#include "luaenv.h"
#include "luaerror.h"
#include "luamodules/luamodules.h"
#include "matrixarea.h"
#include "player.h"
#include "protocolstatus.h"
#include "scheduler.h"
#include "script.h"

#include <boost/range/adaptor/reversed.hpp>

using namespace tfs;

extern Chat* g_chat;
extern ConfigManager g_config;
extern Game g_game;
extern Scripts* g_scripts;

LuaEnvironment g_luaEnvironment;

LuaScriptInterface::LuaScriptInterface(std::string interfaceName) : interfaceName(std::move(interfaceName))
{
	if (!g_luaEnvironment.getLuaState()) {
		g_luaEnvironment.initState();
	}
}

LuaScriptInterface::~LuaScriptInterface() { closeState(); }

bool LuaScriptInterface::reInitState()
{
	g_luaEnvironment.clearCombatObjects(this);
	g_luaEnvironment.clearAreaObjects(this);

	closeState();
	return initState();
}

int32_t LuaScriptInterface::loadFile(const std::string& file, Npc* npc /* = nullptr*/)
{
	// loads file as a chunk at stack top
	int ret = luaL_loadfile(luaState, file.c_str());
	if (ret != 0) {
		lastLuaError = lua::popString(luaState);
		return -1;
	}

	// check that it is loaded as a function
	if (!lua_isfunction(luaState, -1)) {
		lua_pop(luaState, 1);
		return -1;
	}

	loadingFile = file;

	if (!lua::reserveScriptEnv()) {
		lua_pop(luaState, 1);
		return -1;
	}

	lua::ScriptEnvironment* env = lua::getScriptEnv();
	env->setScriptId(EVENT_ID_LOADING, this);
	env->setNpc(npc);

	// execute it
	ret = lua::protectedCall(luaState, 0, 0);
	if (ret != 0) {
		lua::reportError(nullptr, lua::popString(luaState));
		lua::resetScriptEnv();
		return -1;
	}

	lua::resetScriptEnv();
	return 0;
}

int32_t LuaScriptInterface::getEvent(std::string_view eventName)
{
	// get our events table
	lua_rawgeti(luaState, LUA_REGISTRYINDEX, eventTableRef);
	if (!lua_istable(luaState, -1)) {
		lua_pop(luaState, 1);
		return -1;
	}

	// get current event function pointer
	lua_getglobal(luaState, eventName.data());
	if (!lua_isfunction(luaState, -1)) {
		lua_pop(luaState, 2);
		return -1;
	}

	// save in our events table
	lua_pushvalue(luaState, -1);
	lua_rawseti(luaState, -3, runningEventId);
	lua_pop(luaState, 2);

	// reset global value of this event
	lua_pushnil(luaState);
	lua_setglobal(luaState, eventName.data());

	cacheFiles[runningEventId] = fmt::format("{:s}:{:s}", loadingFile, eventName);
	return runningEventId++;
}

int32_t LuaScriptInterface::getEvent()
{
	// check if function is on the stack
	if (!lua_isfunction(luaState, -1)) {
		return -1;
	}

	// get our events table
	lua_rawgeti(luaState, LUA_REGISTRYINDEX, eventTableRef);
	if (!lua_istable(luaState, -1)) {
		lua_pop(luaState, 1);
		return -1;
	}

	// save in our events table
	lua_pushvalue(luaState, -2);
	lua_rawseti(luaState, -2, runningEventId);
	lua_pop(luaState, 2);

	cacheFiles[runningEventId] = loadingFile + ":callback";
	return runningEventId++;
}

int32_t LuaScriptInterface::getMetaEvent(const std::string& globalName, const std::string& eventName)
{
	// get our events table
	lua_rawgeti(luaState, LUA_REGISTRYINDEX, eventTableRef);
	if (!lua_istable(luaState, -1)) {
		lua_pop(luaState, 1);
		return -1;
	}

	// get current event function pointer
	lua_getglobal(luaState, globalName.c_str());
	lua_getfield(luaState, -1, eventName.c_str());
	if (!lua_isfunction(luaState, -1)) {
		lua_pop(luaState, 3);
		return -1;
	}

	// save in our events table
	lua_pushvalue(luaState, -1);
	lua_rawseti(luaState, -4, runningEventId);
	lua_pop(luaState, 1);

	// reset global value of this event
	lua_pushnil(luaState);
	lua_setfield(luaState, -2, eventName.c_str());
	lua_pop(luaState, 2);

	cacheFiles[runningEventId] = loadingFile + ":" + globalName + "@" + eventName;
	return runningEventId++;
}

const std::string& LuaScriptInterface::getFileById(int32_t scriptId)
{
	if (scriptId == EVENT_ID_LOADING) {
		return loadingFile;
	}

	auto it = cacheFiles.find(scriptId);
	if (it == cacheFiles.end()) {
		static const std::string& unk = "(Unknown scriptfile)";
		return unk;
	}
	return it->second;
}

bool LuaScriptInterface::pushFunction(int32_t functionId)
{
	lua_rawgeti(luaState, LUA_REGISTRYINDEX, eventTableRef);
	if (!lua_istable(luaState, -1)) {
		return false;
	}

	lua_rawgeti(luaState, -1, functionId);
	lua_replace(luaState, -2);
	return lua_isfunction(luaState, -1);
}

bool LuaScriptInterface::initState()
{
	luaState = g_luaEnvironment.getLuaState();
	if (!luaState) {
		return false;
	}

	lua_newtable(luaState);
	eventTableRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
	runningEventId = EVENT_ID_USER;
	return true;
}

bool LuaScriptInterface::closeState()
{
	if (!g_luaEnvironment.getLuaState() || !luaState) {
		return false;
	}

	cacheFiles.clear();
	if (eventTableRef != -1) {
		luaL_unref(luaState, LUA_REGISTRYINDEX, eventTableRef);
		eventTableRef = -1;
	}

	luaState = nullptr;
	return true;
}

bool LuaScriptInterface::callFunction(int params)
{
	bool result = false;
	int size = lua_gettop(luaState);
	if (lua::protectedCall(luaState, params, 1) != 0) {
		lua::reportError(nullptr, lua::getString(luaState, -1));
	} else {
		result = lua::getBoolean(luaState, -1);
	}

	lua_pop(luaState, 1);
	if ((lua_gettop(luaState) + params + 1) != size) {
		lua::reportError(nullptr, "Stack size changed!");
	}

	lua::resetScriptEnv();
	return result;
}

void LuaScriptInterface::callVoidFunction(int params)
{
	int size = lua_gettop(luaState);
	if (lua::protectedCall(luaState, params, 0) != 0) {
		lua::reportError(nullptr, lua::popString(luaState));
	}

	if ((lua_gettop(luaState) + params + 1) != size) {
		lua::reportError(nullptr, "Stack size changed!");
	}

	lua::resetScriptEnv();
}

void LuaScriptInterface::registerClass(std::string_view className, std::string_view baseClass,
                                       lua_CFunction newFunction /* = nullptr*/)
{
	// className = {}
	lua_newtable(luaState);
	lua_pushvalue(luaState, -1);
	lua_setglobal(luaState, className.data());
	int methods = lua_gettop(luaState);

	// methodsTable = {}
	lua_newtable(luaState);
	int methodsTable = lua_gettop(luaState);

	if (newFunction) {
		// className.__call = newFunction
		lua_pushcfunction(luaState, newFunction);
		lua_setfield(luaState, methodsTable, "__call");
	}

	uint32_t parents = 0;
	if (!baseClass.empty()) {
		lua_getglobal(luaState, baseClass.data());
		lua_rawgeti(luaState, -1, 'p');
		parents = lua::getNumber<uint32_t>(luaState, -1) + 1;
		lua_pop(luaState, 1);
		lua_setfield(luaState, methodsTable, "__index");
	}

	// setmetatable(className, methodsTable)
	lua_setmetatable(luaState, methods);

	// className.metatable = {}
	luaL_newmetatable(luaState, className.data());
	int metatable = lua_gettop(luaState);

	// className.metatable.__metatable = className
	lua_pushvalue(luaState, methods);
	lua_setfield(luaState, metatable, "__metatable");

	// className.metatable.__index = className
	lua_pushvalue(luaState, methods);
	lua_setfield(luaState, metatable, "__index");

	// className.metatable['h'] = hash
	lua_pushnumber(luaState, std::hash<std::string_view>()(className));
	lua_rawseti(luaState, metatable, 'h');

	// className.metatable['p'] = parents
	lua_pushnumber(luaState, parents);
	lua_rawseti(luaState, metatable, 'p');

	// className.metatable['t'] = type
	if (className == "Item") {
		lua_pushnumber(luaState, lua::LuaData_Item);
	} else if (className == "Container") {
		lua_pushnumber(luaState, lua::LuaData_Container);
	} else if (className == "Teleport") {
		lua_pushnumber(luaState, lua::LuaData_Teleport);
	} else if (className == "Podium") {
		lua_pushnumber(luaState, lua::LuaData_Podium);
	} else if (className == "Player") {
		lua_pushnumber(luaState, lua::LuaData_Player);
	} else if (className == "Monster") {
		lua_pushnumber(luaState, lua::LuaData_Monster);
	} else if (className == "Npc") {
		lua_pushnumber(luaState, lua::LuaData_Npc);
	} else if (className == "Tile") {
		lua_pushnumber(luaState, lua::LuaData_Tile);
	} else {
		lua_pushnumber(luaState, lua::LuaData_Unknown);
	}
	lua_rawseti(luaState, metatable, 't');

	// pop className, className.metatable
	lua_pop(luaState, 2);
}

void LuaScriptInterface::registerTable(std::string_view tableName)
{
	// _G[tableName] = {}
	lua_newtable(luaState);
	lua_setglobal(luaState, tableName.data());
}

void LuaScriptInterface::registerMethod(std::string_view globalName, std::string_view methodName, lua_CFunction func)
{
	// globalName.methodName = func
	lua_getglobal(luaState, globalName.data());
	lua_pushcfunction(luaState, func);
	lua_setfield(luaState, -2, methodName.data());

	// pop globalName
	lua_pop(luaState, 1);
}

void LuaScriptInterface::registerMetaMethod(std::string_view className, std::string_view methodName, lua_CFunction func)
{
	// className.metatable.methodName = func
	luaL_getmetatable(luaState, className.data());
	lua_pushcfunction(luaState, func);
	lua_setfield(luaState, -2, methodName.data());

	// pop className.metatable
	lua_pop(luaState, 1);
}

void LuaScriptInterface::registerGlobalMethod(std::string_view functionName, lua_CFunction func)
{
	// _G[functionName] = func
	lua_pushcfunction(luaState, func);
	lua_setglobal(luaState, functionName.data());
}

void LuaScriptInterface::registerVariable(std::string_view tableName, std::string_view name, lua_Number value)
{
	// tableName.name = value
	lua_getglobal(luaState, tableName.data());
	lua::setField(luaState, name.data(), value);

	// pop tableName
	lua_pop(luaState, 1);
}

void LuaScriptInterface::registerGlobalVariable(std::string_view name, lua_Number value)
{
	// _G[name] = value
	lua_pushnumber(luaState, value);
	lua_setglobal(luaState, name.data());
}

void LuaScriptInterface::registerGlobalBoolean(std::string_view name, bool value)
{
	// _G[name] = value
	lua::pushBoolean(luaState, value);
	lua_setglobal(luaState, name.data());
}

namespace {

int luaDebugPrint(lua_State* L)
{
	// debugPrint(text)
	reportErrorFunc(L, lua::getString(L, -1));
	return 0;
}

int luaGetWorldTime(lua_State* L)
{
	// getWorldTime()
	int16_t time = g_game.getWorldTime();
	lua_pushnumber(L, time);
	return 1;
}

int luaGetWorldLight(lua_State* L)
{
	// getWorldLight()
	LightInfo lightInfo = g_game.getWorldLightInfo();
	lua_pushnumber(L, lightInfo.level);
	lua_pushnumber(L, lightInfo.color);
	return 2;
}

int luaSetWorldLight(lua_State* L)
{
	// setWorldLight(level, color)
	if (g_config.getBoolean(ConfigManager::DEFAULT_WORLD_LIGHT)) {
		lua::pushBoolean(L, false);
		return 1;
	}

	LightInfo lightInfo;
	lightInfo.level = lua::getNumber<uint8_t>(L, 1);
	lightInfo.color = lua::getNumber<uint8_t>(L, 2);
	g_game.setWorldLightInfo(lightInfo);
	lua::pushBoolean(L, true);
	return 1;
}

int luaGetWorldUpTime(lua_State* L)
{
	// getWorldUpTime()
	uint64_t uptime = (OTSYS_TIME() - ProtocolStatus::start) / 1000;
	lua_pushnumber(L, uptime);
	return 1;
}

int luaGetSubTypeName(lua_State* L)
{
	// getSubTypeName(subType)
	int32_t subType = lua::getNumber<int32_t>(L, 1);
	if (subType > 0) {
		lua::pushString(L, Item::items[subType].name);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

static bool getArea(lua_State* L, std::vector<uint32_t>& vec, uint32_t& rows)
{
	lua_pushnil(L);
	for (rows = 0; lua_next(L, -2) != 0; ++rows) {
		if (!lua_istable(L, -1)) {
			return false;
		}

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (!lua::isNumber(L, -1)) {
				return false;
			}
			vec.push_back(lua::getNumber<uint32_t>(L, -1));
			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}

	lua_pop(L, 1);
	return (rows != 0);
}

int luaCreateCombatArea(lua_State* L)
{
	// createCombatArea({area}, <optional> {extArea})
	lua::ScriptEnvironment* env = lua::getScriptEnv();
	if (env->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc(L, "This function can only be used while loading the script.");
		lua::pushBoolean(L, false);
		return 1;
	}

	uint32_t areaId = g_luaEnvironment.createAreaObject(env->getScriptInterface());
	AreaCombat* area = g_luaEnvironment.getAreaObject(areaId);

	int parameters = lua_gettop(L);
	if (parameters >= 2) {
		uint32_t rowsExtArea;
		std::vector<uint32_t> vecExtArea;
		if (!lua_istable(L, 2) || !getArea(L, vecExtArea, rowsExtArea)) {
			reportErrorFunc(L, "Invalid extended area table.");
			lua::pushBoolean(L, false);
			return 1;
		}
		area->setupExtArea(vecExtArea, rowsExtArea);
	}

	uint32_t rowsArea = 0;
	std::vector<uint32_t> vecArea;
	if (!lua_istable(L, 1) || !getArea(L, vecArea, rowsArea)) {
		reportErrorFunc(L, "Invalid area table.");
		lua::pushBoolean(L, false);
		return 1;
	}

	area->setupArea(vecArea, rowsArea);
	lua_pushnumber(L, areaId);
	return 1;
}

int luaDoAreaCombat(lua_State* L)
{
	// doAreaCombat(cid, type, pos, area, min, max, effect[, origin = ORIGIN_SPELL[, blockArmor = false[, blockShield =
	// false[, ignoreResistances = false]]]])
	Creature* creature = lua::getCreature(L, 1);
	if (!creature && (!lua::isNumber(L, 1) || lua::getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(L, lua::getErrorDesc(lua::LUA_ERROR_CREATURE_NOT_FOUND));
		lua::pushBoolean(L, false);
		return 1;
	}

	uint32_t areaId = lua::getNumber<uint32_t>(L, 4);
	const AreaCombat* area = g_luaEnvironment.getAreaObject(areaId);
	if (area || areaId == 0) {
		CombatType_t combatType = lua::getNumber<CombatType_t>(L, 2);

		CombatParams params;
		params.combatType = combatType;
		params.impactEffect = lua::getNumber<uint8_t>(L, 7);

		params.blockedByArmor = lua::getBoolean(L, 9, false);
		params.blockedByShield = lua::getBoolean(L, 10, false);
		params.ignoreResistances = lua::getBoolean(L, 11, false);

		CombatDamage damage;
		damage.origin = lua::getNumber<CombatOrigin>(L, 8, ORIGIN_SPELL);
		damage.primary.type = combatType;
		damage.primary.value = normal_random(lua::getNumber<int32_t>(L, 5), lua::getNumber<int32_t>(L, 6));

		Combat::doAreaCombat(creature, lua::getPosition(L, 3), area, damage, params);
		lua::pushBoolean(L, true);
	} else {
		reportErrorFunc(L, lua::getErrorDesc(lua::LUA_ERROR_AREA_NOT_FOUND));
		lua::pushBoolean(L, false);
	}
	return 1;
}

int luaDoTargetCombat(lua_State* L)
{
	// doTargetCombat(cid, target, type, min, max, effect[, origin = ORIGIN_SPELL[, blockArmor = false[, blockShield =
	// false[, ignoreResistances = false]]]])
	Creature* creature = lua::getCreature(L, 1);
	if (!creature && (!lua::isNumber(L, 1) || lua::getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(L, lua::getErrorDesc(lua::LUA_ERROR_CREATURE_NOT_FOUND));
		lua::pushBoolean(L, false);
		return 1;
	}

	Creature* target = lua::getCreature(L, 2);
	if (!target) {
		reportErrorFunc(L, lua::getErrorDesc(lua::LUA_ERROR_CREATURE_NOT_FOUND));
		lua::pushBoolean(L, false);
		return 1;
	}

	CombatType_t combatType = lua::getNumber<CombatType_t>(L, 3);

	CombatParams params;
	params.combatType = combatType;
	params.impactEffect = lua::getNumber<uint8_t>(L, 6);
	params.blockedByArmor = lua::getBoolean(L, 8, false);
	params.blockedByShield = lua::getBoolean(L, 9, false);
	params.ignoreResistances = lua::getBoolean(L, 10, false);

	CombatDamage damage;
	damage.origin = lua::getNumber<CombatOrigin>(L, 7, ORIGIN_SPELL);
	damage.primary.type = combatType;
	damage.primary.value = normal_random(lua::getNumber<int32_t>(L, 4), lua::getNumber<int32_t>(L, 5));

	Combat::doTargetCombat(creature, target, damage, params);
	lua::pushBoolean(L, true);
	return 1;
}

int luaDoChallengeCreature(lua_State* L)
{
	// doChallengeCreature(cid, target[, force = false])
	Creature* creature = lua::getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(L, lua::getErrorDesc(lua::LUA_ERROR_CREATURE_NOT_FOUND));
		lua::pushBoolean(L, false);
		return 1;
	}

	Creature* target = lua::getCreature(L, 2);
	if (!target) {
		reportErrorFunc(L, lua::getErrorDesc(lua::LUA_ERROR_CREATURE_NOT_FOUND));
		lua::pushBoolean(L, false);
		return 1;
	}

	target->challengeCreature(creature, lua::getBoolean(L, 3, false));
	lua::pushBoolean(L, true);
	return 1;
}

int luaIsValidUID(lua_State* L)
{
	// isValidUID(uid)
	lua::pushBoolean(L, lua::getScriptEnv()->getThingByUID(lua::getNumber<uint32_t>(L, -1)) != nullptr);
	return 1;
}

int luaIsDepot(lua_State* L)
{
	// isDepot(uid)
	Container* container = lua::getScriptEnv()->getContainerByUID(lua::getNumber<uint32_t>(L, -1));
	lua::pushBoolean(L, container && container->getDepotLocker());
	return 1;
}

int luaIsMoveable(lua_State* L)
{
	// isMoveable(uid)
	// isMovable(uid)
	Thing* thing = lua::getScriptEnv()->getThingByUID(lua::getNumber<uint32_t>(L, -1));
	lua::pushBoolean(L, thing && thing->isPushable());
	return 1;
}

int luaGetDepotId(lua_State* L)
{
	// getDepotId(uid)
	uint32_t uid = lua::getNumber<uint32_t>(L, -1);

	Container* container = lua::getScriptEnv()->getContainerByUID(uid);
	if (!container) {
		reportErrorFunc(L, lua::getErrorDesc(lua::LUA_ERROR_CONTAINER_NOT_FOUND));
		lua::pushBoolean(L, false);
		return 1;
	}

	DepotLocker* depotLocker = container->getDepotLocker();
	if (!depotLocker) {
		reportErrorFunc(L, "Depot not found");
		lua::pushBoolean(L, false);
		return 1;
	}

	lua_pushnumber(L, depotLocker->getDepotId());
	return 1;
}

int luaAddEvent(lua_State* L)
{
	// addEvent(callback, delay, ...)
	int parameters = lua_gettop(L);
	if (parameters < 2) {
		reportErrorFunc(L, fmt::format("Not enough parameters: {:d}.", parameters));
		lua::pushBoolean(L, false);
		return 1;
	}

	if (!lua_isfunction(L, 1)) {
		reportErrorFunc(L, "callback parameter should be a function.");
		lua::pushBoolean(L, false);
		return 1;
	}

	if (!lua::isNumber(L, 2)) {
		reportErrorFunc(L, "delay parameter should be a number.");
		lua::pushBoolean(L, false);
		return 1;
	}

	if (g_config.getBoolean(ConfigManager::WARN_UNSAFE_SCRIPTS) ||
	    g_config.getBoolean(ConfigManager::CONVERT_UNSAFE_SCRIPTS)) {
		std::vector<std::pair<int32_t, lua::LuaDataType>> indexes;
		for (int i = 3; i <= parameters; ++i) {
			if (lua_getmetatable(L, i) == 0) {
				continue;
			}
			lua_rawgeti(L, -1, 't');

			auto type = lua::getNumber<lua::LuaDataType>(L, -1);
			if (type != lua::LuaData_Unknown && type != lua::LuaData_Tile) {
				indexes.push_back({i, type});
			}
			lua_pop(L, 2);
		}

		if (!indexes.empty()) {
			if (g_config.getBoolean(ConfigManager::WARN_UNSAFE_SCRIPTS)) {
				bool plural = indexes.size() > 1;

				std::string warningString = "Argument";
				if (plural) {
					warningString += 's';
				}

				for (const auto& entry : indexes) {
					if (entry == indexes.front()) {
						warningString += ' ';
					} else if (entry == indexes.back()) {
						warningString += " and ";
					} else {
						warningString += ", ";
					}
					warningString += '#';
					warningString += std::to_string(entry.first);
				}

				if (plural) {
					warningString += " are unsafe";
				} else {
					warningString += " is unsafe";
				}

				reportErrorFunc(L, warningString);
			}

			if (g_config.getBoolean(ConfigManager::CONVERT_UNSAFE_SCRIPTS)) {
				for (const auto& entry : indexes) {
					switch (entry.second) {
						case lua::LuaData_Item:
						case lua::LuaData_Container:
						case lua::LuaData_Teleport:
						case lua::LuaData_Podium: {
							lua_getglobal(L, "Item");
							lua_getfield(L, -1, "getUniqueId");
							break;
						}
						case lua::LuaData_Player:
						case lua::LuaData_Monster:
						case lua::LuaData_Npc: {
							lua_getglobal(L, "Creature");
							lua_getfield(L, -1, "getId");
							break;
						}
						default:
							break;
					}
					lua_replace(L, -2);
					lua_pushvalue(L, entry.first);
					lua_call(L, 1, 1);
					lua_replace(L, entry.first);
				}
			}
		}
	}

	LuaTimerEventDesc eventDesc;
	eventDesc.parameters.reserve(parameters -
	                             2); // safe to use -2 since we garanteed that there is at least two parameters
	for (int i = 0; i < parameters - 2; ++i) {
		eventDesc.parameters.push_back(luaL_ref(L, LUA_REGISTRYINDEX));
	}

	uint32_t delay = std::max<uint32_t>(100, lua::getNumber<uint32_t>(L, 2));
	lua_pop(L, 1);

	eventDesc.function = luaL_ref(L, LUA_REGISTRYINDEX);
	eventDesc.scriptId = lua::getScriptEnv()->getScriptId();

	auto& lastTimerEventId = g_luaEnvironment.lastEventTimerId;
	eventDesc.eventId = g_scheduler.addEvent(
	    createSchedulerTask(delay, [=]() { g_luaEnvironment.executeTimerEvent(lastTimerEventId); }));

	g_luaEnvironment.timerEvents.emplace(lastTimerEventId, std::move(eventDesc));
	lua_pushnumber(L, lastTimerEventId++);
	return 1;
}

int luaStopEvent(lua_State* L)
{
	// stopEvent(eventid)
	uint32_t eventId = lua::getNumber<uint32_t>(L, 1);

	auto& timerEvents = g_luaEnvironment.timerEvents;
	auto it = timerEvents.find(eventId);
	if (it == timerEvents.end()) {
		lua::pushBoolean(L, false);
		return 1;
	}

	LuaTimerEventDesc timerEventDesc = std::move(it->second);
	timerEvents.erase(it);

	g_scheduler.stopEvent(timerEventDesc.eventId);
	luaL_unref(L, LUA_REGISTRYINDEX, timerEventDesc.function);

	for (auto parameter : timerEventDesc.parameters) {
		luaL_unref(L, LUA_REGISTRYINDEX, parameter);
	}

	lua::pushBoolean(L, true);
	return 1;
}

int luaSaveServer(lua_State* L)
{
	g_game.saveGameState();
	lua::pushBoolean(L, true);
	return 1;
}

int luaCleanMap(lua_State* L)
{
	lua_pushnumber(L, g_game.map.clean());
	return 1;
}

int luaIsInWar(lua_State* L)
{
	// isInWar(cid, target)
	Player* player = lua::getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(L, lua::getErrorDesc(lua::LUA_ERROR_PLAYER_NOT_FOUND));
		lua::pushBoolean(L, false);
		return 1;
	}

	Player* targetPlayer = lua::getPlayer(L, 2);
	if (!targetPlayer) {
		reportErrorFunc(L, lua::getErrorDesc(lua::LUA_ERROR_PLAYER_NOT_FOUND));
		lua::pushBoolean(L, false);
		return 1;
	}

	lua::pushBoolean(L, player->isInWar(targetPlayer));
	return 1;
}

int luaGetWaypointPositionByName(lua_State* L)
{
	// getWaypointPositionByName(name)
	auto& waypoints = g_game.map.waypoints;

	auto it = waypoints.find(lua::getString(L, -1));
	if (it != waypoints.end()) {
		lua::pushPosition(L, it->second);
	} else {
		lua::pushBoolean(L, false);
	}
	return 1;
}

int luaSendChannelMessage(lua_State* L)
{
	// sendChannelMessage(channelId, type, message)
	uint32_t channelId = lua::getNumber<uint32_t>(L, 1);
	ChatChannel* channel = g_chat->getChannelById(channelId);
	if (!channel) {
		lua::pushBoolean(L, false);
		return 1;
	}

	SpeakClasses type = lua::getNumber<SpeakClasses>(L, 2);
	std::string message = lua::getString(L, 3);
	channel->sendToAll(message, type);
	lua::pushBoolean(L, true);
	return 1;
}

int luaSendGuildChannelMessage(lua_State* L)
{
	// sendGuildChannelMessage(guildId, type, message)
	uint32_t guildId = lua::getNumber<uint32_t>(L, 1);
	ChatChannel* channel = g_chat->getGuildChannelById(guildId);
	if (!channel) {
		lua::pushBoolean(L, false);
		return 1;
	}

	SpeakClasses type = lua::getNumber<SpeakClasses>(L, 2);
	std::string message = lua::getString(L, 3);
	channel->sendToAll(message, type);
	lua::pushBoolean(L, true);
	return 1;
}

int luaIsScriptsInterface(lua_State* L)
{
	// isScriptsInterface()
	if (lua::getScriptEnv()->getScriptInterface() == &g_scripts->getScriptInterface()) {
		lua::pushBoolean(L, true);
	} else {
		reportErrorFunc(L, "EventCallback: can only be called inside (data/scripts/)");
		lua::pushBoolean(L, false);
	}
	return 1;
}

#ifndef LUAJIT_VERSION

int luaBitNot(lua_State* L)
{
	lua_pushnumber(L, ~lua::getNumber<uint32_t>(L, -1));
	return 1;
}

#define MULTIOP(name, op) \
	int luaBit##name(lua_State* L) \
	{ \
		int n = lua_gettop(L); \
		uint32_t w = tfs::lua::getNumber<uint32_t>(L, -1); \
		for (int i = 1; i < n; ++i) w op tfs::lua::getNumber<uint32_t>(L, i); \
		lua_pushnumber(L, w); \
		return 1; \
	}

MULTIOP(And, &=)
MULTIOP(Or, |=)
MULTIOP(Xor, ^=)

#define SHIFTOP(name, op) \
	int luaBit##name(lua_State* L) \
	{ \
		uint32_t n1 = tfs::lua::getNumber<uint32_t>(L, 1), n2 = tfs::lua::getNumber<uint32_t>(L, 2); \
		lua_pushnumber(L, (n1 op n2)); \
		return 1; \
	}

SHIFTOP(LeftShift, <<)
SHIFTOP(RightShift, >>)

#endif

int luaConfigManagerGetString(lua_State* L)
{
	lua::pushString(L, g_config.getString(lua::getNumber<ConfigManager::string_config_t>(L, -1)));
	return 1;
}

int luaConfigManagerGetNumber(lua_State* L)
{
	lua_pushnumber(L, g_config.getNumber(lua::getNumber<ConfigManager::integer_config_t>(L, -1)));
	return 1;
}

int luaConfigManagerGetBoolean(lua_State* L)
{
	lua::pushBoolean(L, g_config.getBoolean(lua::getNumber<ConfigManager::boolean_config_t>(L, -1)));
	return 1;
}

int luaDatabaseExecute(lua_State* L)
{
	lua::pushBoolean(L, Database::getInstance().executeQuery(lua::getString(L, -1)));
	return 1;
}

int luaDatabaseAsyncExecute(lua_State* L)
{
	std::function<void(DBResult_ptr, bool)> callback;
	if (lua_gettop(L) > 1) {
		int32_t ref = luaL_ref(L, LUA_REGISTRYINDEX);
		auto scriptId = lua::getScriptEnv()->getScriptId();
		callback = [ref, scriptId](DBResult_ptr, bool success) {
			lua_State* luaState = g_luaEnvironment.getLuaState();
			if (!luaState) {
				return;
			}

			if (!lua::reserveScriptEnv()) {
				luaL_unref(luaState, LUA_REGISTRYINDEX, ref);
				return;
			}

			lua_rawgeti(luaState, LUA_REGISTRYINDEX, ref);
			lua::pushBoolean(luaState, success);
			auto env = lua::getScriptEnv();
			env->setScriptId(scriptId, &getGlobalLuaEnvironment());
			g_luaEnvironment.callFunction(1);

			luaL_unref(luaState, LUA_REGISTRYINDEX, ref);
		};
	}
	g_databaseTasks.addTask(lua::getString(L, -1), callback);
	return 0;
}

int luaDatabaseStoreQuery(lua_State* L)
{
	if (DBResult_ptr res = Database::getInstance().storeQuery(lua::getString(L, -1))) {
		lua_pushnumber(L, lua::ScriptEnvironment::addResult(res));
	} else {
		lua::pushBoolean(L, false);
	}
	return 1;
}

int luaDatabaseAsyncStoreQuery(lua_State* L)
{
	std::function<void(DBResult_ptr, bool)> callback;
	if (lua_gettop(L) > 1) {
		int32_t ref = luaL_ref(L, LUA_REGISTRYINDEX);
		auto scriptId = lua::getScriptEnv()->getScriptId();
		callback = [ref, scriptId](DBResult_ptr result, bool) {
			lua_State* luaState = g_luaEnvironment.getLuaState();
			if (!luaState) {
				return;
			}

			if (!lua::reserveScriptEnv()) {
				luaL_unref(luaState, LUA_REGISTRYINDEX, ref);
				return;
			}

			lua_rawgeti(luaState, LUA_REGISTRYINDEX, ref);
			if (result) {
				lua_pushnumber(luaState, lua::ScriptEnvironment::addResult(result));
			} else {
				lua::pushBoolean(luaState, false);
			}
			auto env = lua::getScriptEnv();
			env->setScriptId(scriptId, &getGlobalLuaEnvironment());
			g_luaEnvironment.callFunction(1);

			luaL_unref(luaState, LUA_REGISTRYINDEX, ref);
		};
	}
	g_databaseTasks.addTask(lua::getString(L, -1), callback, true);
	return 0;
}

int luaDatabaseEscapeString(lua_State* L)
{
	lua::pushString(L, Database::getInstance().escapeString(lua::getString(L, -1)));
	return 1;
}

int luaDatabaseEscapeBlob(lua_State* L)
{
	uint32_t length = lua::getNumber<uint32_t>(L, 2);
	lua::pushString(L, Database::getInstance().escapeBlob(lua::getString(L, 1).c_str(), length));
	return 1;
}

int luaDatabaseLastInsertId(lua_State* L)
{
	lua_pushnumber(L, Database::getInstance().getLastInsertId());
	return 1;
}

int luaDatabaseTableExists(lua_State* L)
{
	lua::pushBoolean(L, DatabaseManager::tableExists(lua::getString(L, -1)));
	return 1;
}

int luaResultGetNumber(lua_State* L)
{
	DBResult_ptr res = lua::ScriptEnvironment::getResultByID(lua::getNumber<uint32_t>(L, 1));
	if (!res) {
		lua::pushBoolean(L, false);
		return 1;
	}

	const std::string& s = lua::getString(L, 2);
	lua_pushnumber(L, res->getNumber<int64_t>(s));
	return 1;
}

int luaResultGetString(lua_State* L)
{
	DBResult_ptr res = lua::ScriptEnvironment::getResultByID(lua::getNumber<uint32_t>(L, 1));
	if (!res) {
		lua::pushBoolean(L, false);
		return 1;
	}

	const std::string& s = lua::getString(L, 2);
	lua::pushString(L, res->getString(s));
	return 1;
}

int luaResultGetStream(lua_State* L)
{
	DBResult_ptr res = lua::ScriptEnvironment::getResultByID(lua::getNumber<uint32_t>(L, 1));
	if (!res) {
		lua::pushBoolean(L, false);
		return 1;
	}

	auto stream = res->getString(lua::getString(L, 2));
	lua_pushlstring(L, stream.data(), stream.size());
	lua_pushnumber(L, stream.size());
	return 2;
}

int luaResultNext(lua_State* L)
{
	DBResult_ptr res = lua::ScriptEnvironment::getResultByID(lua::getNumber<uint32_t>(L, -1));
	if (!res) {
		lua::pushBoolean(L, false);
		return 1;
	}

	lua::pushBoolean(L, res->next());
	return 1;
}

int luaResultFree(lua_State* L)
{
	lua::pushBoolean(L, lua::ScriptEnvironment::removeResult(lua::getNumber<uint32_t>(L, -1)));
	return 1;
}

} // namespace

#ifndef LUAJIT_VERSION
const luaL_Reg LuaScriptInterface::luaBitReg[] = { //{"tobit", lua::luaBitToBit},
    {"bnot", luaBitNot},
    {"band", luaBitAnd},
    {"bor", luaBitOr},
    {"bxor", luaBitXor},
    {"lshift", luaBitLeftShift},
    {"rshift", luaBitRightShift},
    //{"arshift", luaBitArithmeticalRightShift},
    //{"rol", luaBitRotateLeft},
    //{"ror", luaBitRotateRight},
    //{"bswap", luaBitSwapEndian},
    //{"tohex", luaBitToHex},
    {nullptr, nullptr}};
#endif

const luaL_Reg LuaScriptInterface::luaConfigManagerTable[] = {{"getString", luaConfigManagerGetString},
                                                              {"getNumber", luaConfigManagerGetNumber},
                                                              {"getBoolean", luaConfigManagerGetBoolean},
                                                              {nullptr, nullptr}};

const luaL_Reg LuaScriptInterface::luaDatabaseTable[] = {{"query", luaDatabaseExecute},
                                                         {"asyncQuery", luaDatabaseAsyncExecute},
                                                         {"storeQuery", luaDatabaseStoreQuery},
                                                         {"asyncStoreQuery", luaDatabaseAsyncStoreQuery},
                                                         {"escapeString", luaDatabaseEscapeString},
                                                         {"escapeBlob", luaDatabaseEscapeBlob},
                                                         {"lastInsertId", luaDatabaseLastInsertId},
                                                         {"tableExists", luaDatabaseTableExists},
                                                         {nullptr, nullptr}};

const luaL_Reg LuaScriptInterface::luaResultTable[] = {
    {"getNumber", luaResultGetNumber}, {"getString", luaResultGetString}, {"getStream", luaResultGetStream},
    {"next", luaResultNext},           {"free", luaResultFree},           {nullptr, nullptr}};

// Metatables

void LuaScriptInterface::registerFunctions()
{
	// isValidUID(uid)
	lua_register(luaState, "isValidUID", luaIsValidUID);

	// isDepot(uid)
	lua_register(luaState, "isDepot", luaIsDepot);

	// isMovable(uid)
	lua_register(luaState, "isMovable", luaIsMoveable);

	// getDepotId(uid)
	lua_register(luaState, "getDepotId", luaGetDepotId);

	// getWorldTime()
	lua_register(luaState, "getWorldTime", luaGetWorldTime);

	// getWorldLight()
	lua_register(luaState, "getWorldLight", luaGetWorldLight);

	// setWorldLight(level, color)
	lua_register(luaState, "setWorldLight", luaSetWorldLight);

	// getWorldUpTime()
	lua_register(luaState, "getWorldUpTime", luaGetWorldUpTime);

	// getSubTypeName(subType)
	lua_register(luaState, "getSubTypeName", luaGetSubTypeName);

	// createCombatArea({area}, <optional> {extArea})
	lua_register(luaState, "createCombatArea", luaCreateCombatArea);

	// doAreaCombat(cid, type, pos, area, min, max, effect[, origin = ORIGIN_SPELL[, blockArmor = false[, blockShield =
	// false[, ignoreResistances = false]]]])
	lua_register(luaState, "doAreaCombat", luaDoAreaCombat);

	// doTargetCombat(cid, target, type, min, max, effect[, origin = ORIGIN_SPELL[, blockArmor = false[, blockShield =
	// false[, ignoreResistances = false]]]])
	lua_register(luaState, "doTargetCombat", luaDoTargetCombat);

	// doChallengeCreature(cid, target[, force = false])
	lua_register(luaState, "doChallengeCreature", luaDoChallengeCreature);

	// addEvent(callback, delay, ...)
	lua_register(luaState, "addEvent", luaAddEvent);

	// stopEvent(eventid)
	lua_register(luaState, "stopEvent", luaStopEvent);

	// saveServer()
	lua_register(luaState, "saveServer", luaSaveServer);

	// cleanMap()
	lua_register(luaState, "cleanMap", luaCleanMap);

	// debugPrint(text)
	lua_register(luaState, "debugPrint", luaDebugPrint);

	// isInWar(cid, target)
	lua_register(luaState, "isInWar", luaIsInWar);

	// getWaypointPosition(name)
	lua_register(luaState, "getWaypointPositionByName", luaGetWaypointPositionByName);

	// sendChannelMessage(channelId, type, message)
	lua_register(luaState, "sendChannelMessage", luaSendChannelMessage);

	// sendGuildChannelMessage(guildId, type, message)
	lua_register(luaState, "sendGuildChannelMessage", luaSendGuildChannelMessage);

	// isScriptsInterface()
	lua_register(luaState, "isScriptsInterface", luaIsScriptsInterface);

#ifndef LUAJIT_VERSION
	// bit operations for Lua, based on bitlib project release 24
	// bit.bnot, bit.band, bit.bor, bit.bxor, bit.lshift, bit.rshift
	luaL_register(luaState, "bit", luaBitReg);
	lua_pop(luaState, 1);
#endif

	// configManager table
	luaL_register(luaState, "configManager", luaConfigManagerTable);
	lua_pop(luaState, 1);

	// db table
	luaL_register(luaState, "db", luaDatabaseTable);
	lua_pop(luaState, 1);

	// result table
	luaL_register(luaState, "result", luaResultTable);
	lua_pop(luaState, 1);

	/* New functions */
	// registerClass(className, baseClass, newFunction)
	// registerTable(tableName)
	// registerMethod(className, functionName, function)
	// registerMetaMethod(className, functionName, function)
	// registerGlobalMethod(functionName, function)
	// registerVariable(tableName, name, value)
	// registerGlobalVariable(name, value)
	// registerEnum(value)
	// registerEnumIn(tableName, value)

	lua::importModules(*this);
}

//
LuaEnvironment::LuaEnvironment() : LuaScriptInterface("Main Interface") {}

LuaEnvironment::~LuaEnvironment()
{
	delete testInterface;
	closeState();
}

bool LuaEnvironment::initState()
{
	luaState = luaL_newstate();
	if (!luaState) {
		return false;
	}

	luaL_openlibs(luaState);
	registerFunctions();

	runningEventId = EVENT_ID_USER;
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
	if (!luaState) {
		return false;
	}

	for (const auto& combatEntry : combatIdMap) {
		clearCombatObjects(combatEntry.first);
	}

	for (const auto& areaEntry : areaIdMap) {
		clearAreaObjects(areaEntry.first);
	}

	for (auto& timerEntry : timerEvents) {
		LuaTimerEventDesc timerEventDesc = std::move(timerEntry.second);
		for (int32_t parameter : timerEventDesc.parameters) {
			luaL_unref(luaState, LUA_REGISTRYINDEX, parameter);
		}
		luaL_unref(luaState, LUA_REGISTRYINDEX, timerEventDesc.function);
	}

	combatIdMap.clear();
	areaIdMap.clear();
	timerEvents.clear();
	cacheFiles.clear();

	lua_close(luaState);
	luaState = nullptr;
	return true;
}

LuaScriptInterface* LuaEnvironment::getTestInterface()
{
	if (!testInterface) {
		testInterface = new LuaScriptInterface("Test Interface");
		testInterface->initState();
	}
	return testInterface;
}

Combat_ptr LuaEnvironment::getCombatObject(uint32_t id) const
{
	auto it = combatMap.find(id);
	if (it == combatMap.end()) {
		return nullptr;
	}
	return it->second;
}

Combat_ptr LuaEnvironment::createCombatObject(LuaScriptInterface* interface)
{
	Combat_ptr combat = std::make_shared<Combat>();
	combatMap[++lastCombatId] = combat;
	combatIdMap[interface].push_back(lastCombatId);
	return combat;
}

void LuaEnvironment::clearCombatObjects(LuaScriptInterface* interface)
{
	auto it = combatIdMap.find(interface);
	if (it == combatIdMap.end()) {
		return;
	}

	for (uint32_t id : it->second) {
		auto itt = combatMap.find(id);
		if (itt != combatMap.end()) {
			combatMap.erase(itt);
		}
	}
	it->second.clear();
}

AreaCombat* LuaEnvironment::getAreaObject(uint32_t id) const
{
	auto it = areaMap.find(id);
	if (it == areaMap.end()) {
		return nullptr;
	}
	return it->second;
}

uint32_t LuaEnvironment::createAreaObject(LuaScriptInterface* interface)
{
	areaMap[++lastAreaId] = new AreaCombat;
	areaIdMap[interface].push_back(lastAreaId);
	return lastAreaId;
}

void LuaEnvironment::clearAreaObjects(LuaScriptInterface* interface)
{
	auto it = areaIdMap.find(interface);
	if (it == areaIdMap.end()) {
		return;
	}

	for (uint32_t id : it->second) {
		auto itt = areaMap.find(id);
		if (itt != areaMap.end()) {
			delete itt->second;
			areaMap.erase(itt);
		}
	}
	it->second.clear();
}

void LuaEnvironment::executeTimerEvent(uint32_t eventIndex)
{
	auto it = timerEvents.find(eventIndex);
	if (it == timerEvents.end()) {
		return;
	}

	LuaTimerEventDesc timerEventDesc = std::move(it->second);
	timerEvents.erase(it);

	// push function
	lua_rawgeti(luaState, LUA_REGISTRYINDEX, timerEventDesc.function);

	// push parameters
	for (auto parameter : boost::adaptors::reverse(timerEventDesc.parameters)) {
		lua_rawgeti(luaState, LUA_REGISTRYINDEX, parameter);
	}

	// call the function
	if (lua::reserveScriptEnv()) {
		lua::ScriptEnvironment* env = lua::getScriptEnv();
		env->setTimerEvent();
		env->setScriptId(timerEventDesc.scriptId, this);
		callFunction(timerEventDesc.parameters.size());
	} else {
		std::cout << "[Error - lua::executeTimerEvent] Call stack overflow" << std::endl;
	}

	// free resources
	luaL_unref(luaState, LUA_REGISTRYINDEX, timerEventDesc.function);
	for (auto parameter : timerEventDesc.parameters) {
		luaL_unref(luaState, LUA_REGISTRYINDEX, parameter);
	}
}

LuaEnvironment& getGlobalLuaEnvironment()
{
	static LuaEnvironment g_luaEnvironment;
	if (!g_luaEnvironment.getLuaState()) {
		g_luaEnvironment.initState();
	}
	return g_luaEnvironment;
}
