// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "luascript.h"

#include "bed.h"
#include "chat.h"
#include "configmanager.h"
#include "databasemanager.h"
#include "databasetasks.h"
#include "depotchest.h"
#include "events.h"
#include "game.h"
#include "globalevent.h"
#include "housetile.h"
#include "inbox.h"
#include "iologindata.h"
#include "iomapserialize.h"
#include "iomarket.h"
#include "luavariant.h"
#include "matrixarea.h"
#include "monster.h"
#include "movement.h"
#include "npc.h"
#include "outfit.h"
#include "party.h"
#include "player.h"
#include "podium.h"
#include "protocolstatus.h"
#include "scheduler.h"
#include "script.h"
#include "spells.h"
#include "storeinbox.h"
#include "teleport.h"
#include "weapons.h"

#include <ranges>

extern Chat* g_chat;
extern Game g_game;
extern GlobalEvents* g_globalEvents;
extern Monsters g_monsters;
extern Vocations g_vocations;
extern Spells* g_spells;
extern Actions* g_actions;
extern TalkActions* g_talkActions;
extern CreatureEvents* g_creatureEvents;
extern MoveEvents* g_moveEvents;
extern GlobalEvents* g_globalEvents;
extern Scripts* g_scripts;
extern Weapons* g_weapons;

LuaEnvironment g_luaEnvironment;

namespace {

constexpr int32_t EVENT_ID_LOADING = 1;

enum LuaDataType
{
	LuaData_Unknown,

	LuaData_Item,
	LuaData_Container,
	LuaData_Teleport,
	LuaData_Podium,
	LuaData_Player,
	LuaData_Monster,
	LuaData_Npc,
	LuaData_Tile,
};

// temporary item list
std::multimap<ScriptEnvironment*, Item*> tempItems = {};

// result map
uint32_t lastResultId = 0;
std::map<uint32_t, DBResult_ptr> tempResults = {};

bool isNumber(lua_State* L, int32_t arg) { return lua_type(L, arg) == LUA_TNUMBER; }

void setField(lua_State* L, const char* index, lua_Number value)
{
	lua_pushnumber(L, value);
	lua_setfield(L, -2, index);
}

void setField(lua_State* L, const char* index, std::string_view value)
{
	tfs::lua::pushString(L, value);
	lua_setfield(L, -2, index);
}

void registerClass(lua_State* L, std::string_view className, std::string_view baseClass,
                   lua_CFunction newFunction = nullptr)
{
	// className = {}
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setglobal(L, className.data());
	int methods = lua_gettop(L);

	// methodsTable = {}
	lua_newtable(L);
	int methodsTable = lua_gettop(L);

	if (newFunction) {
		// className.__call = newFunction
		lua_pushcfunction(L, newFunction);
		lua_setfield(L, methodsTable, "__call");
	}

	uint32_t parents = 0;
	if (!baseClass.empty()) {
		lua_getglobal(L, baseClass.data());
		lua_rawgeti(L, -1, 'p');
		parents = tfs::lua::getNumber<uint32_t>(L, -1) + 1;
		lua_pop(L, 1);
		lua_setfield(L, methodsTable, "__index");
	}

	// setmetatable(className, methodsTable)
	lua_setmetatable(L, methods);

	// className.metatable = {}
	luaL_newmetatable(L, className.data());
	int metatable = lua_gettop(L);

	// className.metatable.__metatable = className
	lua_pushvalue(L, methods);
	lua_setfield(L, metatable, "__metatable");

	// className.metatable.__index = className
	lua_pushvalue(L, methods);
	lua_setfield(L, metatable, "__index");

	// className.metatable['h'] = hash
	lua_pushnumber(L, std::hash<std::string_view>()(className));
	lua_rawseti(L, metatable, 'h');

	// className.metatable['p'] = parents
	lua_pushnumber(L, parents);
	lua_rawseti(L, metatable, 'p');

	// className.metatable['t'] = type
	if (className == "Item") {
		lua_pushnumber(L, LuaData_Item);
	} else if (className == "Container") {
		lua_pushnumber(L, LuaData_Container);
	} else if (className == "Teleport") {
		lua_pushnumber(L, LuaData_Teleport);
	} else if (className == "Podium") {
		lua_pushnumber(L, LuaData_Podium);
	} else if (className == "Player") {
		lua_pushnumber(L, LuaData_Player);
	} else if (className == "Monster") {
		lua_pushnumber(L, LuaData_Monster);
	} else if (className == "Npc") {
		lua_pushnumber(L, LuaData_Npc);
	} else if (className == "Tile") {
		lua_pushnumber(L, LuaData_Tile);
	} else {
		lua_pushnumber(L, LuaData_Unknown);
	}
	lua_rawseti(L, metatable, 't');

	// pop className, className.metatable
	lua_pop(L, 2);
}

void registerTable(lua_State* L, std::string_view tableName)
{
	// _G[tableName] = {}
	lua_newtable(L);
	lua_setglobal(L, tableName.data());
}

void registerMetaMethod(lua_State* L, std::string_view className, std::string_view methodName, lua_CFunction func)
{
	// className.metatable.methodName = func
	luaL_getmetatable(L, className.data());
	lua_pushcfunction(L, func);
	lua_setfield(L, -2, methodName.data());

	// pop className.metatable
	lua_pop(L, 1);
}

void registerGlobalMethod(lua_State* L, std::string_view functionName, lua_CFunction func)
{
	// _G[functionName] = func
	lua_pushcfunction(L, func);
	lua_setglobal(L, functionName.data());
}

void registerVariable(lua_State* L, std::string_view tableName, std::string_view name, lua_Number value)
{
	// tableName.name = value
	lua_getglobal(L, tableName.data());
	setField(L, name.data(), value);

	// pop tableName
	lua_pop(L, 1);
}

void registerGlobalVariable(lua_State* L, std::string_view name, lua_Number value)
{
	// _G[name] = value
	lua_pushnumber(L, value);
	lua_setglobal(L, name.data());
}

void registerGlobalBoolean(lua_State* L, std::string_view name, bool value)
{
	// _G[name] = value
	tfs::lua::pushBoolean(L, value);
	lua_setglobal(L, name.data());
}

std::string getStackTrace(lua_State* L, std::string_view error_desc)
{
	luaL_traceback(L, L, error_desc.data(), 1);
	return tfs::lua::popString(L);
}

int luaErrorHandler(lua_State* L)
{
	std::string errorMessage = tfs::lua::popString(L);
	tfs::lua::pushString(L, getStackTrace(L, errorMessage));
	return 1;
}

bool getArea(lua_State* L, std::vector<uint32_t>& vec, uint32_t& rows)
{
	lua_pushnil(L);
	for (rows = 0; lua_next(L, -2) != 0; ++rows) {
		if (!lua_istable(L, -1)) {
			return false;
		}

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (!isNumber(L, -1)) {
				return false;
			}
			vec.push_back(tfs::lua::getNumber<uint32_t>(L, -1));
			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}

	lua_pop(L, 1);
	return (rows != 0);
}

template <class T>
std::shared_ptr<T>& getSharedPtr(lua_State* L, int32_t arg)
{
	return *static_cast<std::shared_ptr<T>*>(lua_touserdata(L, arg));
}
template <class T>
void pushSharedPtr(lua_State* L, T value)
{
	new (lua_newuserdata(L, sizeof(T))) T(std::move(value));
}

} // namespace

ScriptEnvironment::ScriptEnvironment() { resetEnv(); }

ScriptEnvironment::~ScriptEnvironment() { resetEnv(); }

void ScriptEnvironment::resetEnv()
{
	scriptId = 0;
	callbackId = 0;
	timerEvent = false;
	interface = nullptr;
	localMap.clear();
	tempResults.clear();

	auto pair = tempItems.equal_range(this);
	auto it = pair.first;
	while (it != pair.second) {
		Item* item = it->second;
		if (item && item->getParent() == VirtualCylinder::virtualCylinder) {
			g_game.ReleaseItem(item);
		}
		it = tempItems.erase(it);
	}
}

bool ScriptEnvironment::setCallbackId(int32_t callbackId, LuaScriptInterface* scriptInterface)
{
	if (this->callbackId != 0) {
		// nested callbacks are not allowed
		if (interface) {
			reportErrorFunc(interface->getLuaState(), "Nested callbacks!");
		}
		return false;
	}

	this->callbackId = callbackId;
	interface = scriptInterface;
	return true;
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
	if (item && item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		return item->getUniqueId();
	}

	for (const auto& it : localMap) {
		if (it.second == item) {
			return it.first;
		}
	}

	localMap[++lastUID] = item;
	return lastUID;
}

void ScriptEnvironment::insertItem(uint32_t uid, Item* item)
{
	auto result = localMap.emplace(uid, item);
	if (!result.second) {
		std::cout << "\nLua Script Error: Thing uid already taken.";
	}
}

Thing* ScriptEnvironment::getThingByUID(uint32_t uid)
{
	if (uid >= CREATURE_ID_MIN) {
		return g_game.getCreatureByID(uid);
	}

	if (uid <= std::numeric_limits<uint16_t>::max()) {
		Item* item = g_game.getUniqueItem(uid);
		if (item && !item->isRemoved()) {
			return item;
		}
		return nullptr;
	}

	auto it = localMap.find(uid);
	if (it != localMap.end()) {
		Item* item = it->second;
		if (!item->isRemoved()) {
			return item;
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
	if (uid <= std::numeric_limits<uint16_t>::max()) {
		g_game.removeUniqueItem(uid);
		return;
	}

	auto it = localMap.find(uid);
	if (it != localMap.end()) {
		localMap.erase(it);
	}
}

static void addTempItem(Item* item) { tempItems.emplace(tfs::lua::getScriptEnv(), item); }

void tfs::lua::removeTempItem(Item* item)
{
	std::erase_if(tempItems, [item](const auto& pair) { return pair.second == item; });
}

static uint32_t addResult(DBResult_ptr res)
{
	tempResults[++lastResultId] = std::move(res);
	return lastResultId;
}

static bool removeResult(uint32_t id)
{
	auto it = tempResults.find(id);
	if (it == tempResults.end()) {
		return false;
	}

	tempResults.erase(it);
	return true;
}

static DBResult_ptr getResultByID(uint32_t id)
{
	auto it = tempResults.find(id);
	if (it == tempResults.end()) {
		return nullptr;
	}
	return it->second;
}

std::string tfs::lua::getErrorDesc(ErrorCode_t code)
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

static std::array<ScriptEnvironment, 16> scriptEnv = {};
static int32_t scriptEnvIndex = -1;

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

/// Same as lua_pcall, but adds stack trace to error strings in called function.
int tfs::lua::protectedCall(lua_State* L, int nargs, int nresults)
{
	int error_index = lua_gettop(L) - nargs;
	lua_pushcfunction(L, luaErrorHandler);
	lua_insert(L, error_index);

	int ret = lua_pcall(L, nargs, nresults, error_index);
	lua_remove(L, error_index);
	return ret;
}

int32_t LuaScriptInterface::loadFile(const std::string& file, Npc* npc /* = nullptr*/)
{
	// loads file as a chunk at stack top
	int ret = luaL_loadfile(L, file.data());
	if (ret != 0) {
		lastLuaError = tfs::lua::popString(L);
		return -1;
	}

	// check that it is loaded as a function
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 1);
		return -1;
	}

	loadingFile = file;

	if (!tfs::lua::reserveScriptEnv()) {
		lua_pop(L, 1);
		return -1;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(EVENT_ID_LOADING, this);
	env->setNpc(npc);

	// execute it
	ret = tfs::lua::protectedCall(L, 0, 0);
	if (ret != 0) {
		reportErrorFunc(nullptr, tfs::lua::popString(L));
		tfs::lua::resetScriptEnv();
		return -1;
	}

	tfs::lua::resetScriptEnv();
	return 0;
}

int32_t LuaScriptInterface::getEvent(std::string_view eventName)
{
	// get our events table
	lua_rawgeti(L, LUA_REGISTRYINDEX, eventTableRef);
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return -1;
	}

	// get current event function pointer
	lua_getglobal(L, eventName.data());
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 2);
		return -1;
	}

	// save in our events table
	lua_pushvalue(L, -1);
	lua_rawseti(L, -3, runningEventId);
	lua_pop(L, 2);

	// reset global value of this event
	lua_pushnil(L);
	lua_setglobal(L, eventName.data());

	cacheFiles[runningEventId] = fmt::format("{:s}:{:s}", loadingFile, eventName);
	return runningEventId++;
}

int32_t LuaScriptInterface::getEvent()
{
	// check if function is on the stack
	if (!lua_isfunction(L, -1)) {
		return -1;
	}

	// get our events table
	lua_rawgeti(L, LUA_REGISTRYINDEX, eventTableRef);
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return -1;
	}

	// save in our events table
	lua_pushvalue(L, -2);
	lua_rawseti(L, -2, runningEventId);
	lua_pop(L, 2);

	cacheFiles[runningEventId] = loadingFile + ":callback";
	return runningEventId++;
}

int32_t LuaScriptInterface::getMetaEvent(const std::string& globalName, const std::string& eventName)
{
	// get our events table
	lua_rawgeti(L, LUA_REGISTRYINDEX, eventTableRef);
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return -1;
	}

	// get current event function pointer
	lua_getglobal(L, globalName.data());
	lua_getfield(L, -1, eventName.data());
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 3);
		return -1;
	}

	// save in our events table
	lua_pushvalue(L, -1);
	lua_rawseti(L, -4, runningEventId);
	lua_pop(L, 1);

	// reset global value of this event
	lua_pushnil(L);
	lua_setfield(L, -2, eventName.data());
	lua_pop(L, 2);

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

void tfs::lua::reportError(std::string_view function, std::string_view error_desc, lua_State* L /*= nullptr*/,
                           bool stack_trace /*= false*/)
{
	auto [scriptId, scriptInterface, callbackId, timerEvent] = getScriptEnv()->getEventInfo();

	std::cout << "\nLua Script Error: ";

	if (scriptInterface) {
		std::cout << '[' << scriptInterface->getInterfaceName() << "]\n";

		if (timerEvent) {
			std::cout << "in a timer event called from:\n";
		}

		if (callbackId) {
			std::cout << "in callback: " << scriptInterface->getFileById(callbackId) << '\n';
		}

		std::cout << scriptInterface->getFileById(scriptId) << '\n';
	}

	if (!function.empty()) {
		std::cout << function << "(). ";
	}

	if (L && stack_trace) {
		std::cout << getStackTrace(L, error_desc) << '\n';
	} else {
		std::cout << error_desc << '\n';
	}
}

bool LuaScriptInterface::pushFunction(int32_t functionId)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, eventTableRef);
	if (!lua_istable(L, -1)) {
		return false;
	}

	lua_rawgeti(L, -1, functionId);
	lua_replace(L, -2);
	return lua_isfunction(L, -1);
}

bool LuaScriptInterface::initState()
{
	if (L = g_luaEnvironment.getLuaState(); !L) {
		return false;
	}

	lua_newtable(L);
	eventTableRef = luaL_ref(L, LUA_REGISTRYINDEX);
	runningEventId = EVENT_ID_USER;
	return true;
}

bool LuaScriptInterface::closeState()
{
	if (!g_luaEnvironment.getLuaState() || !L) {
		return false;
	}

	cacheFiles.clear();
	if (eventTableRef != -1) {
		luaL_unref(L, LUA_REGISTRYINDEX, eventTableRef);
		eventTableRef = -1;
	}

	L = nullptr;
	return true;
}

bool LuaScriptInterface::callFunction(int params)
{
	bool result = false;
	int size = lua_gettop(L);
	if (tfs::lua::protectedCall(L, params, 1) != 0) {
		reportErrorFunc(nullptr, tfs::lua::getString(L, -1));
	} else {
		result = tfs::lua::getBoolean(L, -1);
	}

	lua_pop(L, 1);
	if ((lua_gettop(L) + params + 1) != size) {
		reportErrorFunc(nullptr, "Stack size changed!");
	}

	tfs::lua::resetScriptEnv();
	return result;
}

void LuaScriptInterface::callVoidFunction(int params)
{
	int size = lua_gettop(L);
	if (tfs::lua::protectedCall(L, params, 0) != 0) {
		reportErrorFunc(nullptr, tfs::lua::popString(L));
	}

	if ((lua_gettop(L) + params + 1) != size) {
		reportErrorFunc(nullptr, "Stack size changed!");
	}

	tfs::lua::resetScriptEnv();
}

void tfs::lua::pushVariant(lua_State* L, const LuaVariant& var)
{
	lua_createtable(L, 0, 2);
	setField(L, "type", var.type());
	switch (var.type()) {
		case VARIANT_NUMBER:
			setField(L, "number", var.getNumber());
			break;
		case VARIANT_STRING:
			setField(L, "string", var.getString());
			break;
		case VARIANT_TARGETPOSITION:
			pushPosition(L, var.getTargetPosition());
			lua_setfield(L, -2, "pos");
			break;
		case VARIANT_POSITION: {
			pushPosition(L, var.getPosition());
			lua_setfield(L, -2, "pos");
			break;
		}
		default:
			break;
	}
	setMetatable(L, -1, "Variant");
}

void tfs::lua::pushThing(lua_State* L, Thing* thing)
{
	if (!thing) {
		lua_createtable(L, 0, 4);
		setField(L, "uid", 0);
		setField(L, "itemid", 0);
		setField(L, "actionid", 0);
		setField(L, "type", 0);
		return;
	}

	if (Item* item = thing->getItem()) {
		pushUserdata(L, item);
		setItemMetatable(L, -1, item);
	} else if (Creature* creature = thing->getCreature()) {
		pushUserdata(L, creature);
		setCreatureMetatable(L, -1, creature);
	} else {
		lua_pushnil(L);
	}
}

void tfs::lua::pushCylinder(lua_State* L, Cylinder* cylinder)
{
	if (Creature* creature = cylinder->getCreature()) {
		pushUserdata(L, creature);
		setCreatureMetatable(L, -1, creature);
	} else if (Item* parentItem = cylinder->getItem()) {
		pushUserdata(L, parentItem);
		setItemMetatable(L, -1, parentItem);
	} else if (Tile* tile = cylinder->getTile()) {
		pushUserdata(L, tile);
		setMetatable(L, -1, "Tile");
	} else if (cylinder == VirtualCylinder::virtualCylinder) {
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
}

void tfs::lua::pushString(lua_State* L, std::string_view value) { lua_pushlstring(L, value.data(), value.size()); }
void tfs::lua::pushCallback(lua_State* L, int32_t callback) { lua_rawgeti(L, LUA_REGISTRYINDEX, callback); }

std::string tfs::lua::popString(lua_State* L)
{
	if (lua_gettop(L) == 0) {
		return std::string();
	}

	std::string str = getString(L, -1);
	lua_pop(L, 1);
	return str;
}

int32_t tfs::lua::popCallback(lua_State* L) { return luaL_ref(L, LUA_REGISTRYINDEX); }

// Metatables
void tfs::lua::setMetatable(lua_State* L, int32_t index, std::string_view name)
{
	luaL_getmetatable(L, name.data());
	lua_setmetatable(L, index - 1);
}

static void setWeakMetatable(lua_State* L, int32_t index, const std::string& name)
{
	static std::set<std::string> weakObjectTypes;
	const std::string& weakName = name + "_weak";

	auto result = weakObjectTypes.emplace(name);
	if (result.second) {
		luaL_getmetatable(L, name.data());
		int childMetatable = lua_gettop(L);

		luaL_newmetatable(L, weakName.data());
		int metatable = lua_gettop(L);

		static const std::vector<std::string> methodKeys = {"__index", "__metatable", "__eq"};
		for (const std::string& metaKey : methodKeys) {
			lua_getfield(L, childMetatable, metaKey.data());
			lua_setfield(L, metatable, metaKey.data());
		}

		static const std::vector<int> methodIndexes = {'h', 'p', 't'};
		for (int metaIndex : methodIndexes) {
			lua_rawgeti(L, childMetatable, metaIndex);
			lua_rawseti(L, metatable, metaIndex);
		}

		lua_pushnil(L);
		lua_setfield(L, metatable, "__gc");

		lua_remove(L, childMetatable);
	} else {
		luaL_getmetatable(L, weakName.data());
	}
	lua_setmetatable(L, index - 1);
}

void tfs::lua::setItemMetatable(lua_State* L, int32_t index, const Item* item)
{
	if (item->getContainer()) {
		luaL_getmetatable(L, "Container");
	} else if (item->getTeleport()) {
		luaL_getmetatable(L, "Teleport");
	} else if (item->getPodium()) {
		luaL_getmetatable(L, "Podium");
	} else {
		luaL_getmetatable(L, "Item");
	}
	lua_setmetatable(L, index - 1);
}

void tfs::lua::setCreatureMetatable(lua_State* L, int32_t index, const Creature* creature)
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
std::string tfs::lua::getString(lua_State* L, int32_t arg)
{
	size_t len;
	const char* data = lua_tolstring(L, arg, &len);
	if (!data || len == 0) {
		return {};
	}
	return {data, len};
}

Position tfs::lua::getPosition(lua_State* L, int32_t arg, int32_t& stackpos)
{
	Position position{
	    getField<uint16_t>(L, arg, "x"),
	    getField<uint16_t>(L, arg, "y"),
	    getField<uint8_t>(L, arg, "z"),
	};

	lua_getfield(L, arg, "stackpos");
	if (lua_isnil(L, -1) == 1) {
		stackpos = 0;
	} else {
		stackpos = getNumber<int32_t>(L, -1);
	}

	lua_pop(L, 4);
	return position;
}

Position tfs::lua::getPosition(lua_State* L, int32_t arg)
{
	Position position{
	    getField<uint16_t>(L, arg, "x"),
	    getField<uint16_t>(L, arg, "y"),
	    getField<uint8_t>(L, arg, "z"),
	};

	lua_pop(L, 3);
	return position;
}

static Outfit_t getOutfit(lua_State* L, int32_t arg)
{
	Outfit_t outfit{
	    .lookType = tfs::lua::getField<uint16_t>(L, arg, "lookType"),
	    .lookTypeEx = tfs::lua::getField<uint16_t>(L, arg, "lookTypeEx"),

	    .lookHead = tfs::lua::getField<uint8_t>(L, arg, "lookHead"),
	    .lookBody = tfs::lua::getField<uint8_t>(L, arg, "lookBody"),
	    .lookLegs = tfs::lua::getField<uint8_t>(L, arg, "lookLegs"),
	    .lookFeet = tfs::lua::getField<uint8_t>(L, arg, "lookFeet"),
	    .lookAddons = tfs::lua::getField<uint8_t>(L, arg, "lookAddons"),

	    .lookMount = tfs::lua::getField<uint16_t>(L, arg, "lookMount"),
	    .lookMountHead = tfs::lua::getField<uint8_t>(L, arg, "lookMountHead"),
	    .lookMountBody = tfs::lua::getField<uint8_t>(L, arg, "lookMountBody"),
	    .lookMountLegs = tfs::lua::getField<uint8_t>(L, arg, "lookMountLegs"),
	    .lookMountFeet = tfs::lua::getField<uint8_t>(L, arg, "lookMountFeet"),
	};

	lua_pop(L, 12);
	return outfit;
}

static Outfit getOutfitClass(lua_State* L, int32_t arg)
{
	Outfit outfit{
	    .name = tfs::lua::getFieldString(L, arg, "name"),
	    .lookType = tfs::lua::getField<uint16_t>(L, arg, "lookType"),
	    .premium = tfs::lua::getField<uint8_t>(L, arg, "premium") == 1,
	    .unlocked = tfs::lua::getField<uint8_t>(L, arg, "unlocked") == 1,
	};

	lua_pop(L, 4);
	return outfit;
}

static LuaVariant getVariant(lua_State* L, int32_t arg)
{
	LuaVariant var;
	switch (tfs::lua::getField<LuaVariantType_t>(L, arg, "type")) {
		case VARIANT_NUMBER: {
			var.setNumber(tfs::lua::getField<uint32_t>(L, arg, "number"));
			lua_pop(L, 2);
			break;
		}

		case VARIANT_STRING: {
			var.setString(tfs::lua::getFieldString(L, arg, "string"));
			lua_pop(L, 2);
			break;
		}

		case VARIANT_POSITION:
			lua_getfield(L, arg, "pos");
			var.setPosition(tfs::lua::getPosition(L, lua_gettop(L)));
			lua_pop(L, 2);
			break;

		case VARIANT_TARGETPOSITION: {
			lua_getfield(L, arg, "pos");
			var.setTargetPosition(tfs::lua::getPosition(L, lua_gettop(L)));
			lua_pop(L, 2);
			break;
		}

		default: {
			var = {};
			lua_pop(L, 1);
			break;
		}
	}
	return var;
}

Thing* tfs::lua::getThing(lua_State* L, int32_t arg)
{
	Thing* thing;
	if (lua_getmetatable(L, arg) != 0) {
		lua_rawgeti(L, -1, 't');
		switch (getNumber<uint32_t>(L, -1)) {
			case LuaData_Item:
				thing = getUserdata<Item>(L, arg);
				break;
			case LuaData_Container:
				thing = getUserdata<Container>(L, arg);
				break;
			case LuaData_Teleport:
				thing = getUserdata<Teleport>(L, arg);
				break;
			case LuaData_Podium:
				thing = getUserdata<Podium>(L, arg);
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

Creature* tfs::lua::getCreature(lua_State* L, int32_t arg)
{
	if (lua_isuserdata(L, arg)) {
		return getUserdata<Creature>(L, arg);
	}
	return g_game.getCreatureByID(getNumber<uint32_t>(L, arg));
}

Player* tfs::lua::getPlayer(lua_State* L, int32_t arg)
{
	if (lua_isuserdata(L, arg)) {
		return getUserdata<Player>(L, arg);
	}
	return g_game.getPlayerByID(getNumber<uint32_t>(L, arg));
}

std::string tfs::lua::getFieldString(lua_State* L, int32_t arg, const std::string_view key)
{
	lua_getfield(L, arg, key.data());
	return getString(L, -1);
}

static LuaDataType getUserdataType(lua_State* L, int32_t arg)
{
	if (lua_getmetatable(L, arg) == 0) {
		return LuaData_Unknown;
	}
	lua_rawgeti(L, -1, 't');

	LuaDataType type = tfs::lua::getNumber<LuaDataType>(L, -1);
	lua_pop(L, 2);

	return type;
}

void tfs::lua::pushBoolean(lua_State* L, bool value) { lua_pushboolean(L, value ? 1 : 0); }

void tfs::lua::pushSpell(lua_State* L, const Spell& spell)
{
	lua_createtable(L, 0, 5);
	setField(L, "name", spell.getName());
	setField(L, "level", spell.getLevel());
	setField(L, "mlevel", spell.getMagicLevel());
	setField(L, "mana", spell.getMana());
	setField(L, "manapercent", spell.getManaPercent());
	setMetatable(L, -1, "Spell");
}

void tfs::lua::pushPosition(lua_State* L, const Position& position, int32_t stackpos /* = 0*/)
{
	lua_createtable(L, 0, 4);
	setField(L, "x", position.x);
	setField(L, "y", position.y);
	setField(L, "z", position.z);
	setField(L, "stackpos", stackpos);
	setMetatable(L, -1, "Position");
}

void tfs::lua::pushOutfit(lua_State* L, const Outfit_t& outfit)
{
	lua_createtable(L, 0, 12);
	setField(L, "lookType", outfit.lookType);
	setField(L, "lookTypeEx", outfit.lookTypeEx);
	setField(L, "lookHead", outfit.lookHead);
	setField(L, "lookBody", outfit.lookBody);
	setField(L, "lookLegs", outfit.lookLegs);
	setField(L, "lookFeet", outfit.lookFeet);
	setField(L, "lookAddons", outfit.lookAddons);
	setField(L, "lookMount", outfit.lookMount);
	setField(L, "lookMountHead", outfit.lookMountHead);
	setField(L, "lookMountBody", outfit.lookMountBody);
	setField(L, "lookMountLegs", outfit.lookMountLegs);
	setField(L, "lookMountFeet", outfit.lookMountFeet);
}

void tfs::lua::pushOutfit(lua_State* L, const Outfit* outfit)
{
	lua_createtable(L, 0, 4);
	setField(L, "lookType", outfit->lookType);
	setField(L, "name", outfit->name);
	setField(L, "premium", outfit->premium);
	setField(L, "unlocked", outfit->unlocked);
	setMetatable(L, -1, "Outfit");
}

static void pushLoot(lua_State* L, const std::vector<LootBlock>& lootList)
{
	lua_createtable(L, lootList.size(), 0);

	int index = 0;
	for (const auto& lootBlock : lootList) {
		lua_createtable(L, 0, 7);

		setField(L, "itemId", lootBlock.id);
		setField(L, "chance", lootBlock.chance);
		setField(L, "subType", lootBlock.subType);
		setField(L, "maxCount", lootBlock.countmax);
		setField(L, "actionId", lootBlock.actionId);
		setField(L, "text", lootBlock.text);

		pushLoot(L, lootBlock.childLoot);
		lua_setfield(L, -2, "childLoot");

		lua_rawseti(L, -2, ++index);
	}
}

static void pushTown(lua_State* L, const Town& town)
{
	lua_createtable(L, 0, 3);
	setField(L, "id", town.id);
	setField(L, "name", town.name);
	tfs::lua::pushPosition(L, town.templePosition);
	lua_setfield(L, -2, "templePosition");
	lua_getglobal(L, "Town");
	lua_setmetatable(L, -2);
}

#define registerEnum(L, value) \
	{ \
		std::string enumName = #value; \
		registerGlobalVariable(L, enumName.substr(enumName.find_last_of(':') + 1), value); \
	}
#define registerEnumIn(L, tableName, value) \
	{ \
		std::string enumName = #value; \
		registerVariable(L, tableName, enumName.substr(enumName.find_last_of(':') + 1), value); \
	}

void LuaScriptInterface::registerFunctions()
{
	using namespace tfs::lua;

	// doPlayerAddItem(uid, itemid, <optional: default: 1> count/subtype)
	// doPlayerAddItem(cid, itemid, <optional: default: 1> count, <optional: default: 1> canDropOnMap, <optional:
	// default: 1>subtype) Returns uid of the created item
	lua_register(L, "doPlayerAddItem", LuaScriptInterface::luaDoPlayerAddItem);

	// isValidUID(uid)
	lua_register(L, "isValidUID", LuaScriptInterface::luaIsValidUID);

	// isDepot(uid)
	lua_register(L, "isDepot", LuaScriptInterface::luaIsDepot);

	// isMovable(uid)
	lua_register(L, "isMovable", LuaScriptInterface::luaIsMoveable);

	// doAddContainerItem(uid, itemid, <optional> count/subtype)
	// lua_register(L, "doAddContainerItem", LuaScriptInterface::luaDoAddContainerItem);

	// getDepotId(uid)
	lua_register(L, "getDepotId", LuaScriptInterface::luaGetDepotId);

	// getWorldUpTime()
	lua_register(L, "getWorldUpTime", LuaScriptInterface::luaGetWorldUpTime);

	// getSubTypeName(subType)
	lua_register(L, "getSubTypeName", LuaScriptInterface::luaGetSubTypeName);

	// createCombatArea({area}, <optional> {extArea})
	lua_register(L, "createCombatArea", LuaScriptInterface::luaCreateCombatArea);

	// doAreaCombat(cid, type, pos, area, min, max, effect[, origin = ORIGIN_SPELL[, blockArmor = false[, blockShield =
	// false[, ignoreResistances = false]]]])
	lua_register(L, "doAreaCombat", LuaScriptInterface::luaDoAreaCombat);

	// doTargetCombat(cid, target, type, min, max, effect[, origin = ORIGIN_SPELL[, blockArmor = false[, blockShield =
	// false[, ignoreResistances = false]]]])
	lua_register(L, "doTargetCombat", LuaScriptInterface::luaDoTargetCombat);

	// doChallengeCreature(cid, target[, force = false])
	lua_register(L, "doChallengeCreature", LuaScriptInterface::luaDoChallengeCreature);

	// addEvent(callback, delay, ...)
	lua_register(L, "addEvent", LuaScriptInterface::luaAddEvent);

	// stopEvent(eventid)
	lua_register(L, "stopEvent", LuaScriptInterface::luaStopEvent);

	// saveServer()
	lua_register(L, "saveServer", LuaScriptInterface::luaSaveServer);

	// cleanMap()
	lua_register(L, "cleanMap", LuaScriptInterface::luaCleanMap);

	// debugPrint(text)
	lua_register(L, "debugPrint", LuaScriptInterface::luaDebugPrint);

	// isInWar(cid, target)
	lua_register(L, "isInWar", LuaScriptInterface::luaIsInWar);

	// getWaypointPosition(name)
	lua_register(L, "getWaypointPositionByName", LuaScriptInterface::luaGetWaypointPositionByName);

	// sendChannelMessage(channelId, type, message)
	lua_register(L, "sendChannelMessage", LuaScriptInterface::luaSendChannelMessage);

	// sendGuildChannelMessage(guildId, type, message)
	lua_register(L, "sendGuildChannelMessage", LuaScriptInterface::luaSendGuildChannelMessage);

	// isScriptsInterface()
	lua_register(L, "isScriptsInterface", LuaScriptInterface::luaIsScriptsInterface);

#ifndef LUAJIT_VERSION
	// bit operations for Lua, based on bitlib project release 24
	// bit.bnot, bit.band, bit.bor, bit.bxor, bit.lshift, bit.rshift
	luaL_register(L, "bit", LuaScriptInterface::luaBitReg);
	lua_pop(L, 1);
#endif

	// configManager table
	luaL_register(L, "configManager", LuaScriptInterface::luaConfigManagerTable);
	lua_pop(L, 1);

	// db table
	luaL_register(L, "db", LuaScriptInterface::luaDatabaseTable);
	lua_pop(L, 1);

	// result table
	luaL_register(L, "result", LuaScriptInterface::luaResultTable);
	lua_pop(L, 1);

	/* New functions */
	// registerClass(L, className, baseClass, newFunction)
	// registerTable(L, tableName)
	// registerMethod(L, className, functionName, function)
	// registerMetaMethod(L, className, functionName, function)
	// registerGlobalMethod(L, functionName, function)
	// registerVariable(L, tableName, name, value)
	// registerGlobalVariable(L, name, value)
	// registerEnum(L, value)
	// registerEnumIn(L, tableName, value)

	// Enums
	registerEnum(L, ACCOUNT_TYPE_NORMAL);
	registerEnum(L, ACCOUNT_TYPE_TUTOR);
	registerEnum(L, ACCOUNT_TYPE_SENIORTUTOR);
	registerEnum(L, ACCOUNT_TYPE_GAMEMASTER);
	registerEnum(L, ACCOUNT_TYPE_COMMUNITYMANAGER);
	registerEnum(L, ACCOUNT_TYPE_GOD);

	registerEnum(L, AMMO_NONE);
	registerEnum(L, AMMO_BOLT);
	registerEnum(L, AMMO_ARROW);
	registerEnum(L, AMMO_SPEAR);
	registerEnum(L, AMMO_THROWINGSTAR);
	registerEnum(L, AMMO_THROWINGKNIFE);
	registerEnum(L, AMMO_STONE);
	registerEnum(L, AMMO_SNOWBALL);

	registerEnum(L, CALLBACK_PARAM_LEVELMAGICVALUE);
	registerEnum(L, CALLBACK_PARAM_SKILLVALUE);
	registerEnum(L, CALLBACK_PARAM_TARGETTILE);
	registerEnum(L, CALLBACK_PARAM_TARGETCREATURE);

	registerEnum(L, COMBAT_FORMULA_UNDEFINED);
	registerEnum(L, COMBAT_FORMULA_LEVELMAGIC);
	registerEnum(L, COMBAT_FORMULA_SKILL);
	registerEnum(L, COMBAT_FORMULA_DAMAGE);

	registerEnum(L, DIRECTION_NORTH);
	registerEnum(L, DIRECTION_EAST);
	registerEnum(L, DIRECTION_SOUTH);
	registerEnum(L, DIRECTION_WEST);
	registerEnum(L, DIRECTION_SOUTHWEST);
	registerEnum(L, DIRECTION_SOUTHEAST);
	registerEnum(L, DIRECTION_NORTHWEST);
	registerEnum(L, DIRECTION_NORTHEAST);

	registerEnum(L, COMBAT_NONE);
	registerEnum(L, COMBAT_PHYSICALDAMAGE);
	registerEnum(L, COMBAT_ENERGYDAMAGE);
	registerEnum(L, COMBAT_EARTHDAMAGE);
	registerEnum(L, COMBAT_FIREDAMAGE);
	registerEnum(L, COMBAT_UNDEFINEDDAMAGE);
	registerEnum(L, COMBAT_LIFEDRAIN);
	registerEnum(L, COMBAT_MANADRAIN);
	registerEnum(L, COMBAT_HEALING);
	registerEnum(L, COMBAT_DROWNDAMAGE);
	registerEnum(L, COMBAT_ICEDAMAGE);
	registerEnum(L, COMBAT_HOLYDAMAGE);
	registerEnum(L, COMBAT_DEATHDAMAGE);

	registerEnum(L, COMBAT_PARAM_TYPE);
	registerEnum(L, COMBAT_PARAM_EFFECT);
	registerEnum(L, COMBAT_PARAM_DISTANCEEFFECT);
	registerEnum(L, COMBAT_PARAM_BLOCKSHIELD);
	registerEnum(L, COMBAT_PARAM_BLOCKARMOR);
	registerEnum(L, COMBAT_PARAM_TARGETCASTERORTOPMOST);
	registerEnum(L, COMBAT_PARAM_CREATEITEM);
	registerEnum(L, COMBAT_PARAM_AGGRESSIVE);
	registerEnum(L, COMBAT_PARAM_DISPEL);
	registerEnum(L, COMBAT_PARAM_USECHARGES);

	registerEnum(L, CONDITION_NONE);
	registerEnum(L, CONDITION_POISON);
	registerEnum(L, CONDITION_FIRE);
	registerEnum(L, CONDITION_ENERGY);
	registerEnum(L, CONDITION_BLEEDING);
	registerEnum(L, CONDITION_HASTE);
	registerEnum(L, CONDITION_PARALYZE);
	registerEnum(L, CONDITION_OUTFIT);
	registerEnum(L, CONDITION_INVISIBLE);
	registerEnum(L, CONDITION_LIGHT);
	registerEnum(L, CONDITION_MANASHIELD);
	registerEnum(L, CONDITION_MANASHIELD_BREAKABLE);
	registerEnum(L, CONDITION_INFIGHT);
	registerEnum(L, CONDITION_DRUNK);
	registerEnum(L, CONDITION_EXHAUST_WEAPON);
	registerEnum(L, CONDITION_REGENERATION);
	registerEnum(L, CONDITION_SOUL);
	registerEnum(L, CONDITION_DROWN);
	registerEnum(L, CONDITION_MUTED);
	registerEnum(L, CONDITION_CHANNELMUTEDTICKS);
	registerEnum(L, CONDITION_YELLTICKS);
	registerEnum(L, CONDITION_ATTRIBUTES);
	registerEnum(L, CONDITION_FREEZING);
	registerEnum(L, CONDITION_DAZZLED);
	registerEnum(L, CONDITION_CURSED);
	registerEnum(L, CONDITION_EXHAUST_COMBAT);
	registerEnum(L, CONDITION_EXHAUST_HEAL);
	registerEnum(L, CONDITION_PACIFIED);
	registerEnum(L, CONDITION_SPELLCOOLDOWN);
	registerEnum(L, CONDITION_SPELLGROUPCOOLDOWN);
	registerEnum(L, CONDITION_ROOT);

	registerEnum(L, CONDITIONID_DEFAULT);
	registerEnum(L, CONDITIONID_COMBAT);
	registerEnum(L, CONDITIONID_HEAD);
	registerEnum(L, CONDITIONID_NECKLACE);
	registerEnum(L, CONDITIONID_BACKPACK);
	registerEnum(L, CONDITIONID_ARMOR);
	registerEnum(L, CONDITIONID_RIGHT);
	registerEnum(L, CONDITIONID_LEFT);
	registerEnum(L, CONDITIONID_LEGS);
	registerEnum(L, CONDITIONID_FEET);
	registerEnum(L, CONDITIONID_RING);
	registerEnum(L, CONDITIONID_AMMO);

	registerEnum(L, CONDITION_PARAM_OWNER);
	registerEnum(L, CONDITION_PARAM_TICKS);
	registerEnum(L, CONDITION_PARAM_DRUNKENNESS);
	registerEnum(L, CONDITION_PARAM_HEALTHGAIN);
	registerEnum(L, CONDITION_PARAM_HEALTHTICKS);
	registerEnum(L, CONDITION_PARAM_MANAGAIN);
	registerEnum(L, CONDITION_PARAM_MANATICKS);
	registerEnum(L, CONDITION_PARAM_DELAYED);
	registerEnum(L, CONDITION_PARAM_SPEED);
	registerEnum(L, CONDITION_PARAM_LIGHT_LEVEL);
	registerEnum(L, CONDITION_PARAM_LIGHT_COLOR);
	registerEnum(L, CONDITION_PARAM_SOULGAIN);
	registerEnum(L, CONDITION_PARAM_SOULTICKS);
	registerEnum(L, CONDITION_PARAM_MINVALUE);
	registerEnum(L, CONDITION_PARAM_MAXVALUE);
	registerEnum(L, CONDITION_PARAM_STARTVALUE);
	registerEnum(L, CONDITION_PARAM_TICKINTERVAL);
	registerEnum(L, CONDITION_PARAM_FORCEUPDATE);
	registerEnum(L, CONDITION_PARAM_SKILL_MELEE);
	registerEnum(L, CONDITION_PARAM_SKILL_FIST);
	registerEnum(L, CONDITION_PARAM_SKILL_CLUB);
	registerEnum(L, CONDITION_PARAM_SKILL_SWORD);
	registerEnum(L, CONDITION_PARAM_SKILL_AXE);
	registerEnum(L, CONDITION_PARAM_SKILL_DISTANCE);
	registerEnum(L, CONDITION_PARAM_SKILL_SHIELD);
	registerEnum(L, CONDITION_PARAM_SKILL_FISHING);
	registerEnum(L, CONDITION_PARAM_STAT_MAXHITPOINTS);
	registerEnum(L, CONDITION_PARAM_STAT_MAXMANAPOINTS);
	registerEnum(L, CONDITION_PARAM_STAT_MAGICPOINTS);
	registerEnum(L, CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT);
	registerEnum(L, CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT);
	registerEnum(L, CONDITION_PARAM_STAT_MAGICPOINTSPERCENT);
	registerEnum(L, CONDITION_PARAM_PERIODICDAMAGE);
	registerEnum(L, CONDITION_PARAM_SKILL_MELEEPERCENT);
	registerEnum(L, CONDITION_PARAM_SKILL_FISTPERCENT);
	registerEnum(L, CONDITION_PARAM_SKILL_CLUBPERCENT);
	registerEnum(L, CONDITION_PARAM_SKILL_SWORDPERCENT);
	registerEnum(L, CONDITION_PARAM_SKILL_AXEPERCENT);
	registerEnum(L, CONDITION_PARAM_SKILL_DISTANCEPERCENT);
	registerEnum(L, CONDITION_PARAM_SKILL_SHIELDPERCENT);
	registerEnum(L, CONDITION_PARAM_SKILL_FISHINGPERCENT);
	registerEnum(L, CONDITION_PARAM_BUFF_SPELL);
	registerEnum(L, CONDITION_PARAM_SUBID);
	registerEnum(L, CONDITION_PARAM_FIELD);
	registerEnum(L, CONDITION_PARAM_DISABLE_DEFENSE);
	registerEnum(L, CONDITION_PARAM_MANASHIELD_BREAKABLE);
	registerEnum(L, CONDITION_PARAM_SPECIALSKILL_CRITICALHITCHANCE);
	registerEnum(L, CONDITION_PARAM_SPECIALSKILL_CRITICALHITAMOUNT);
	registerEnum(L, CONDITION_PARAM_SPECIALSKILL_LIFELEECHCHANCE);
	registerEnum(L, CONDITION_PARAM_SPECIALSKILL_LIFELEECHAMOUNT);
	registerEnum(L, CONDITION_PARAM_SPECIALSKILL_MANALEECHCHANCE);
	registerEnum(L, CONDITION_PARAM_SPECIALSKILL_MANALEECHAMOUNT);
	registerEnum(L, CONDITION_PARAM_AGGRESSIVE);

	registerEnum(L, CONST_ME_NONE);
	registerEnum(L, CONST_ME_DRAWBLOOD);
	registerEnum(L, CONST_ME_LOSEENERGY);
	registerEnum(L, CONST_ME_POFF);
	registerEnum(L, CONST_ME_BLOCKHIT);
	registerEnum(L, CONST_ME_EXPLOSIONAREA);
	registerEnum(L, CONST_ME_EXPLOSIONHIT);
	registerEnum(L, CONST_ME_FIREAREA);
	registerEnum(L, CONST_ME_YELLOW_RINGS);
	registerEnum(L, CONST_ME_GREEN_RINGS);
	registerEnum(L, CONST_ME_HITAREA);
	registerEnum(L, CONST_ME_TELEPORT);
	registerEnum(L, CONST_ME_ENERGYHIT);
	registerEnum(L, CONST_ME_MAGIC_BLUE);
	registerEnum(L, CONST_ME_MAGIC_RED);
	registerEnum(L, CONST_ME_MAGIC_GREEN);
	registerEnum(L, CONST_ME_HITBYFIRE);
	registerEnum(L, CONST_ME_HITBYPOISON);
	registerEnum(L, CONST_ME_MORTAREA);
	registerEnum(L, CONST_ME_SOUND_GREEN);
	registerEnum(L, CONST_ME_SOUND_RED);
	registerEnum(L, CONST_ME_POISONAREA);
	registerEnum(L, CONST_ME_SOUND_YELLOW);
	registerEnum(L, CONST_ME_SOUND_PURPLE);
	registerEnum(L, CONST_ME_SOUND_BLUE);
	registerEnum(L, CONST_ME_SOUND_WHITE);
	registerEnum(L, CONST_ME_BUBBLES);
	registerEnum(L, CONST_ME_CRAPS);
	registerEnum(L, CONST_ME_GIFT_WRAPS);
	registerEnum(L, CONST_ME_FIREWORK_YELLOW);
	registerEnum(L, CONST_ME_FIREWORK_RED);
	registerEnum(L, CONST_ME_FIREWORK_BLUE);
	registerEnum(L, CONST_ME_STUN);
	registerEnum(L, CONST_ME_SLEEP);
	registerEnum(L, CONST_ME_WATERCREATURE);
	registerEnum(L, CONST_ME_GROUNDSHAKER);
	registerEnum(L, CONST_ME_HEARTS);
	registerEnum(L, CONST_ME_FIREATTACK);
	registerEnum(L, CONST_ME_ENERGYAREA);
	registerEnum(L, CONST_ME_SMALLCLOUDS);
	registerEnum(L, CONST_ME_HOLYDAMAGE);
	registerEnum(L, CONST_ME_BIGCLOUDS);
	registerEnum(L, CONST_ME_ICEAREA);
	registerEnum(L, CONST_ME_ICETORNADO);
	registerEnum(L, CONST_ME_ICEATTACK);
	registerEnum(L, CONST_ME_STONES);
	registerEnum(L, CONST_ME_SMALLPLANTS);
	registerEnum(L, CONST_ME_CARNIPHILA);
	registerEnum(L, CONST_ME_PURPLEENERGY);
	registerEnum(L, CONST_ME_YELLOWENERGY);
	registerEnum(L, CONST_ME_HOLYAREA);
	registerEnum(L, CONST_ME_BIGPLANTS);
	registerEnum(L, CONST_ME_CAKE);
	registerEnum(L, CONST_ME_GIANTICE);
	registerEnum(L, CONST_ME_WATERSPLASH);
	registerEnum(L, CONST_ME_PLANTATTACK);
	registerEnum(L, CONST_ME_TUTORIALARROW);
	registerEnum(L, CONST_ME_TUTORIALSQUARE);
	registerEnum(L, CONST_ME_MIRRORHORIZONTAL);
	registerEnum(L, CONST_ME_MIRRORVERTICAL);
	registerEnum(L, CONST_ME_SKULLHORIZONTAL);
	registerEnum(L, CONST_ME_SKULLVERTICAL);
	registerEnum(L, CONST_ME_ASSASSIN);
	registerEnum(L, CONST_ME_STEPSHORIZONTAL);
	registerEnum(L, CONST_ME_BLOODYSTEPS);
	registerEnum(L, CONST_ME_STEPSVERTICAL);
	registerEnum(L, CONST_ME_YALAHARIGHOST);
	registerEnum(L, CONST_ME_BATS);
	registerEnum(L, CONST_ME_SMOKE);
	registerEnum(L, CONST_ME_INSECTS);
	registerEnum(L, CONST_ME_DRAGONHEAD);
	registerEnum(L, CONST_ME_ORCSHAMAN);
	registerEnum(L, CONST_ME_ORCSHAMAN_FIRE);
	registerEnum(L, CONST_ME_THUNDER);
	registerEnum(L, CONST_ME_FERUMBRAS);
	registerEnum(L, CONST_ME_CONFETTI_HORIZONTAL);
	registerEnum(L, CONST_ME_CONFETTI_VERTICAL);
	registerEnum(L, CONST_ME_BLACKSMOKE);
	registerEnum(L, CONST_ME_REDSMOKE);
	registerEnum(L, CONST_ME_YELLOWSMOKE);
	registerEnum(L, CONST_ME_GREENSMOKE);
	registerEnum(L, CONST_ME_PURPLESMOKE);
	registerEnum(L, CONST_ME_EARLY_THUNDER);
	registerEnum(L, CONST_ME_RAGIAZ_BONECAPSULE);
	registerEnum(L, CONST_ME_CRITICAL_DAMAGE);
	registerEnum(L, CONST_ME_PLUNGING_FISH);
	registerEnum(L, CONST_ME_BLUECHAIN);
	registerEnum(L, CONST_ME_ORANGECHAIN);
	registerEnum(L, CONST_ME_GREENCHAIN);
	registerEnum(L, CONST_ME_PURPLECHAIN);
	registerEnum(L, CONST_ME_GREYCHAIN);
	registerEnum(L, CONST_ME_YELLOWCHAIN);
	registerEnum(L, CONST_ME_YELLOWSPARKLES);
	registerEnum(L, CONST_ME_FAEEXPLOSION);
	registerEnum(L, CONST_ME_FAECOMING);
	registerEnum(L, CONST_ME_FAEGOING);
	registerEnum(L, CONST_ME_BIGCLOUDSSINGLESPACE);
	registerEnum(L, CONST_ME_STONESSINGLESPACE);
	registerEnum(L, CONST_ME_BLUEGHOST);
	registerEnum(L, CONST_ME_POINTOFINTEREST);
	registerEnum(L, CONST_ME_MAPEFFECT);
	registerEnum(L, CONST_ME_PINKSPARK);
	registerEnum(L, CONST_ME_FIREWORK_GREEN);
	registerEnum(L, CONST_ME_FIREWORK_ORANGE);
	registerEnum(L, CONST_ME_FIREWORK_PURPLE);
	registerEnum(L, CONST_ME_FIREWORK_TURQUOISE);
	registerEnum(L, CONST_ME_THECUBE);
	registerEnum(L, CONST_ME_DRAWINK);
	registerEnum(L, CONST_ME_PRISMATICSPARKLES);
	registerEnum(L, CONST_ME_THAIAN);
	registerEnum(L, CONST_ME_THAIANGHOST);
	registerEnum(L, CONST_ME_GHOSTSMOKE);
	registerEnum(L, CONST_ME_FLOATINGBLOCK);
	registerEnum(L, CONST_ME_BLOCK);
	registerEnum(L, CONST_ME_ROOTING);
	registerEnum(L, CONST_ME_GHOSTLYSCRATCH);
	registerEnum(L, CONST_ME_GHOSTLYBITE);
	registerEnum(L, CONST_ME_BIGSCRATCHING);
	registerEnum(L, CONST_ME_SLASH);
	registerEnum(L, CONST_ME_BITE);
	registerEnum(L, CONST_ME_CHIVALRIOUSCHALLENGE);
	registerEnum(L, CONST_ME_DIVINEDAZZLE);
	registerEnum(L, CONST_ME_ELECTRICALSPARK);
	registerEnum(L, CONST_ME_PURPLETELEPORT);
	registerEnum(L, CONST_ME_REDTELEPORT);
	registerEnum(L, CONST_ME_ORANGETELEPORT);
	registerEnum(L, CONST_ME_GREYTELEPORT);
	registerEnum(L, CONST_ME_LIGHTBLUETELEPORT);
	registerEnum(L, CONST_ME_FATAL);
	registerEnum(L, CONST_ME_DODGE);
	registerEnum(L, CONST_ME_HOURGLASS);
	registerEnum(L, CONST_ME_FIREWORKSSTAR);
	registerEnum(L, CONST_ME_FIREWORKSCIRCLE);
	registerEnum(L, CONST_ME_FERUMBRAS_1);
	registerEnum(L, CONST_ME_GAZHARAGOTH);
	registerEnum(L, CONST_ME_MAD_MAGE);
	registerEnum(L, CONST_ME_HORESTIS);
	registerEnum(L, CONST_ME_DEVOVORGA);
	registerEnum(L, CONST_ME_FERUMBRAS_2);
	registerEnum(L, CONST_ME_FOAM);

	registerEnum(L, CONST_ANI_NONE);
	registerEnum(L, CONST_ANI_SPEAR);
	registerEnum(L, CONST_ANI_BOLT);
	registerEnum(L, CONST_ANI_ARROW);
	registerEnum(L, CONST_ANI_FIRE);
	registerEnum(L, CONST_ANI_ENERGY);
	registerEnum(L, CONST_ANI_POISONARROW);
	registerEnum(L, CONST_ANI_BURSTARROW);
	registerEnum(L, CONST_ANI_THROWINGSTAR);
	registerEnum(L, CONST_ANI_THROWINGKNIFE);
	registerEnum(L, CONST_ANI_SMALLSTONE);
	registerEnum(L, CONST_ANI_DEATH);
	registerEnum(L, CONST_ANI_LARGEROCK);
	registerEnum(L, CONST_ANI_SNOWBALL);
	registerEnum(L, CONST_ANI_POWERBOLT);
	registerEnum(L, CONST_ANI_POISON);
	registerEnum(L, CONST_ANI_INFERNALBOLT);
	registerEnum(L, CONST_ANI_HUNTINGSPEAR);
	registerEnum(L, CONST_ANI_ENCHANTEDSPEAR);
	registerEnum(L, CONST_ANI_REDSTAR);
	registerEnum(L, CONST_ANI_GREENSTAR);
	registerEnum(L, CONST_ANI_ROYALSPEAR);
	registerEnum(L, CONST_ANI_SNIPERARROW);
	registerEnum(L, CONST_ANI_ONYXARROW);
	registerEnum(L, CONST_ANI_PIERCINGBOLT);
	registerEnum(L, CONST_ANI_WHIRLWINDSWORD);
	registerEnum(L, CONST_ANI_WHIRLWINDAXE);
	registerEnum(L, CONST_ANI_WHIRLWINDCLUB);
	registerEnum(L, CONST_ANI_ETHEREALSPEAR);
	registerEnum(L, CONST_ANI_ICE);
	registerEnum(L, CONST_ANI_EARTH);
	registerEnum(L, CONST_ANI_HOLY);
	registerEnum(L, CONST_ANI_SUDDENDEATH);
	registerEnum(L, CONST_ANI_FLASHARROW);
	registerEnum(L, CONST_ANI_FLAMMINGARROW);
	registerEnum(L, CONST_ANI_SHIVERARROW);
	registerEnum(L, CONST_ANI_ENERGYBALL);
	registerEnum(L, CONST_ANI_SMALLICE);
	registerEnum(L, CONST_ANI_SMALLHOLY);
	registerEnum(L, CONST_ANI_SMALLEARTH);
	registerEnum(L, CONST_ANI_EARTHARROW);
	registerEnum(L, CONST_ANI_EXPLOSION);
	registerEnum(L, CONST_ANI_CAKE);
	registerEnum(L, CONST_ANI_TARSALARROW);
	registerEnum(L, CONST_ANI_VORTEXBOLT);
	registerEnum(L, CONST_ANI_PRISMATICBOLT);
	registerEnum(L, CONST_ANI_CRYSTALLINEARROW);
	registerEnum(L, CONST_ANI_DRILLBOLT);
	registerEnum(L, CONST_ANI_ENVENOMEDARROW);
	registerEnum(L, CONST_ANI_GLOOTHSPEAR);
	registerEnum(L, CONST_ANI_SIMPLEARROW);
	registerEnum(L, CONST_ANI_LEAFSTAR);
	registerEnum(L, CONST_ANI_DIAMONDARROW);
	registerEnum(L, CONST_ANI_SPECTRALBOLT);
	registerEnum(L, CONST_ANI_ROYALSTAR);
	registerEnum(L, CONST_ANI_WEAPONTYPE);

	registerEnum(L, CONST_PROP_BLOCKSOLID);
	registerEnum(L, CONST_PROP_HASHEIGHT);
	registerEnum(L, CONST_PROP_BLOCKPROJECTILE);
	registerEnum(L, CONST_PROP_BLOCKPATH);
	registerEnum(L, CONST_PROP_ISVERTICAL);
	registerEnum(L, CONST_PROP_ISHORIZONTAL);
	registerEnum(L, CONST_PROP_MOVEABLE);
	registerEnum(L, CONST_PROP_IMMOVABLEBLOCKSOLID);
	registerEnum(L, CONST_PROP_IMMOVABLEBLOCKPATH);
	registerEnum(L, CONST_PROP_IMMOVABLENOFIELDBLOCKPATH);
	registerEnum(L, CONST_PROP_NOFIELDBLOCKPATH);
	registerEnum(L, CONST_PROP_SUPPORTHANGABLE);

	registerEnum(L, CONST_SLOT_HEAD);
	registerEnum(L, CONST_SLOT_NECKLACE);
	registerEnum(L, CONST_SLOT_BACKPACK);
	registerEnum(L, CONST_SLOT_ARMOR);
	registerEnum(L, CONST_SLOT_RIGHT);
	registerEnum(L, CONST_SLOT_LEFT);
	registerEnum(L, CONST_SLOT_LEGS);
	registerEnum(L, CONST_SLOT_FEET);
	registerEnum(L, CONST_SLOT_RING);
	registerEnum(L, CONST_SLOT_AMMO);

	registerEnum(L, CREATURE_EVENT_NONE);
	registerEnum(L, CREATURE_EVENT_LOGIN);
	registerEnum(L, CREATURE_EVENT_LOGOUT);
	registerEnum(L, CREATURE_EVENT_RECONNECT);
	registerEnum(L, CREATURE_EVENT_THINK);
	registerEnum(L, CREATURE_EVENT_PREPAREDEATH);
	registerEnum(L, CREATURE_EVENT_DEATH);
	registerEnum(L, CREATURE_EVENT_KILL);
	registerEnum(L, CREATURE_EVENT_ADVANCE);
	registerEnum(L, CREATURE_EVENT_MODALWINDOW);
	registerEnum(L, CREATURE_EVENT_TEXTEDIT);
	registerEnum(L, CREATURE_EVENT_HEALTHCHANGE);
	registerEnum(L, CREATURE_EVENT_MANACHANGE);
	registerEnum(L, CREATURE_EVENT_EXTENDED_OPCODE);

	registerEnum(L, CREATURE_ID_MIN);
	registerEnum(L, CREATURE_ID_MAX);

	registerEnum(L, GAME_STATE_STARTUP);
	registerEnum(L, GAME_STATE_INIT);
	registerEnum(L, GAME_STATE_NORMAL);
	registerEnum(L, GAME_STATE_CLOSED);
	registerEnum(L, GAME_STATE_SHUTDOWN);
	registerEnum(L, GAME_STATE_CLOSING);
	registerEnum(L, GAME_STATE_MAINTAIN);

	registerEnum(L, ITEM_STACK_SIZE);

	registerEnum(L, MESSAGE_STATUS_DEFAULT);
	registerEnum(L, MESSAGE_STATUS_WARNING);
	registerEnum(L, MESSAGE_EVENT_ADVANCE);
	registerEnum(L, MESSAGE_STATUS_WARNING2);
	registerEnum(L, MESSAGE_STATUS_SMALL);
	registerEnum(L, MESSAGE_INFO_DESCR);
	registerEnum(L, MESSAGE_DAMAGE_DEALT);
	registerEnum(L, MESSAGE_DAMAGE_RECEIVED);
	registerEnum(L, MESSAGE_HEALED);
	registerEnum(L, MESSAGE_EXPERIENCE);
	registerEnum(L, MESSAGE_DAMAGE_OTHERS);
	registerEnum(L, MESSAGE_HEALED_OTHERS);
	registerEnum(L, MESSAGE_EXPERIENCE_OTHERS);
	registerEnum(L, MESSAGE_EVENT_DEFAULT);
	registerEnum(L, MESSAGE_LOOT);
	registerEnum(L, MESSAGE_TRADE);
	registerEnum(L, MESSAGE_GUILD);
	registerEnum(L, MESSAGE_PARTY_MANAGEMENT);
	registerEnum(L, MESSAGE_PARTY);
	registerEnum(L, MESSAGE_REPORT);
	registerEnum(L, MESSAGE_HOTKEY_PRESSED);
	registerEnum(L, MESSAGE_MARKET);
	registerEnum(L, MESSAGE_BEYOND_LAST);
	registerEnum(L, MESSAGE_TOURNAMENT_INFO);
	registerEnum(L, MESSAGE_ATTENTION);
	registerEnum(L, MESSAGE_BOOSTED_CREATURE);
	registerEnum(L, MESSAGE_OFFLINE_TRAINING);
	registerEnum(L, MESSAGE_TRANSACTION);

	registerEnum(L, CREATURETYPE_PLAYER);
	registerEnum(L, CREATURETYPE_MONSTER);
	registerEnum(L, CREATURETYPE_NPC);
	registerEnum(L, CREATURETYPE_SUMMON_OWN);
	registerEnum(L, CREATURETYPE_SUMMON_OTHERS);

	registerEnum(L, CLIENTOS_LINUX);
	registerEnum(L, CLIENTOS_WINDOWS);
	registerEnum(L, CLIENTOS_FLASH);
	registerEnum(L, CLIENTOS_OTCLIENT_LINUX);
	registerEnum(L, CLIENTOS_OTCLIENT_WINDOWS);
	registerEnum(L, CLIENTOS_OTCLIENT_MAC);

	registerEnum(L, FIGHTMODE_ATTACK);
	registerEnum(L, FIGHTMODE_BALANCED);
	registerEnum(L, FIGHTMODE_DEFENSE);

	registerEnum(L, ITEM_ATTRIBUTE_NONE);
	registerEnum(L, ITEM_ATTRIBUTE_ACTIONID);
	registerEnum(L, ITEM_ATTRIBUTE_UNIQUEID);
	registerEnum(L, ITEM_ATTRIBUTE_DESCRIPTION);
	registerEnum(L, ITEM_ATTRIBUTE_TEXT);
	registerEnum(L, ITEM_ATTRIBUTE_DATE);
	registerEnum(L, ITEM_ATTRIBUTE_WRITER);
	registerEnum(L, ITEM_ATTRIBUTE_NAME);
	registerEnum(L, ITEM_ATTRIBUTE_ARTICLE);
	registerEnum(L, ITEM_ATTRIBUTE_PLURALNAME);
	registerEnum(L, ITEM_ATTRIBUTE_WEIGHT);
	registerEnum(L, ITEM_ATTRIBUTE_ATTACK);
	registerEnum(L, ITEM_ATTRIBUTE_DEFENSE);
	registerEnum(L, ITEM_ATTRIBUTE_EXTRADEFENSE);
	registerEnum(L, ITEM_ATTRIBUTE_ARMOR);
	registerEnum(L, ITEM_ATTRIBUTE_HITCHANCE);
	registerEnum(L, ITEM_ATTRIBUTE_SHOOTRANGE);
	registerEnum(L, ITEM_ATTRIBUTE_OWNER);
	registerEnum(L, ITEM_ATTRIBUTE_DURATION);
	registerEnum(L, ITEM_ATTRIBUTE_DECAYSTATE);
	registerEnum(L, ITEM_ATTRIBUTE_CORPSEOWNER);
	registerEnum(L, ITEM_ATTRIBUTE_CHARGES);
	registerEnum(L, ITEM_ATTRIBUTE_FLUIDTYPE);
	registerEnum(L, ITEM_ATTRIBUTE_DOORID);
	registerEnum(L, ITEM_ATTRIBUTE_DECAYTO);
	registerEnum(L, ITEM_ATTRIBUTE_WRAPID);
	registerEnum(L, ITEM_ATTRIBUTE_STOREITEM);
	registerEnum(L, ITEM_ATTRIBUTE_ATTACK_SPEED);
	registerEnum(L, ITEM_ATTRIBUTE_OPENCONTAINER);
	registerEnum(L, ITEM_ATTRIBUTE_DURATION_MIN);
	registerEnum(L, ITEM_ATTRIBUTE_DURATION_MAX);

	registerEnum(L, ITEM_TYPE_DEPOT);
	registerEnum(L, ITEM_TYPE_MAILBOX);
	registerEnum(L, ITEM_TYPE_TRASHHOLDER);
	registerEnum(L, ITEM_TYPE_CONTAINER);
	registerEnum(L, ITEM_TYPE_DOOR);
	registerEnum(L, ITEM_TYPE_MAGICFIELD);
	registerEnum(L, ITEM_TYPE_TELEPORT);
	registerEnum(L, ITEM_TYPE_BED);
	registerEnum(L, ITEM_TYPE_KEY);
	registerEnum(L, ITEM_TYPE_RUNE);
	registerEnum(L, ITEM_TYPE_PODIUM);

	registerEnum(L, ITEM_GROUP_GROUND);
	registerEnum(L, ITEM_GROUP_CONTAINER);
	registerEnum(L, ITEM_GROUP_WEAPON);
	registerEnum(L, ITEM_GROUP_AMMUNITION);
	registerEnum(L, ITEM_GROUP_ARMOR);
	registerEnum(L, ITEM_GROUP_CHARGES);
	registerEnum(L, ITEM_GROUP_TELEPORT);
	registerEnum(L, ITEM_GROUP_MAGICFIELD);
	registerEnum(L, ITEM_GROUP_WRITEABLE);
	registerEnum(L, ITEM_GROUP_KEY);
	registerEnum(L, ITEM_GROUP_SPLASH);
	registerEnum(L, ITEM_GROUP_FLUID);
	registerEnum(L, ITEM_GROUP_DOOR);
	registerEnum(L, ITEM_GROUP_DEPRECATED);
	registerEnum(L, ITEM_GROUP_PODIUM);

	registerEnum(L, ITEM_BROWSEFIELD);
	registerEnum(L, ITEM_BAG);
	registerEnum(L, ITEM_SHOPPING_BAG);
	registerEnum(L, ITEM_GOLD_COIN);
	registerEnum(L, ITEM_PLATINUM_COIN);
	registerEnum(L, ITEM_CRYSTAL_COIN);
	registerEnum(L, ITEM_AMULETOFLOSS);
	registerEnum(L, ITEM_PARCEL);
	registerEnum(L, ITEM_LABEL);
	registerEnum(L, ITEM_FIREFIELD_PVP_FULL);
	registerEnum(L, ITEM_FIREFIELD_PVP_MEDIUM);
	registerEnum(L, ITEM_FIREFIELD_PVP_SMALL);
	registerEnum(L, ITEM_FIREFIELD_PERSISTENT_FULL);
	registerEnum(L, ITEM_FIREFIELD_PERSISTENT_MEDIUM);
	registerEnum(L, ITEM_FIREFIELD_PERSISTENT_SMALL);
	registerEnum(L, ITEM_FIREFIELD_NOPVP);
	registerEnum(L, ITEM_FIREFIELD_NOPVP_MEDIUM);
	registerEnum(L, ITEM_POISONFIELD_PVP);
	registerEnum(L, ITEM_POISONFIELD_PERSISTENT);
	registerEnum(L, ITEM_POISONFIELD_NOPVP);
	registerEnum(L, ITEM_ENERGYFIELD_PVP);
	registerEnum(L, ITEM_ENERGYFIELD_PERSISTENT);
	registerEnum(L, ITEM_ENERGYFIELD_NOPVP);
	registerEnum(L, ITEM_MAGICWALL);
	registerEnum(L, ITEM_MAGICWALL_PERSISTENT);
	registerEnum(L, ITEM_MAGICWALL_SAFE);
	registerEnum(L, ITEM_WILDGROWTH);
	registerEnum(L, ITEM_WILDGROWTH_PERSISTENT);
	registerEnum(L, ITEM_WILDGROWTH_SAFE);
	registerEnum(L, ITEM_DECORATION_KIT);

	registerEnum(L, WIELDINFO_NONE);
	registerEnum(L, WIELDINFO_LEVEL);
	registerEnum(L, WIELDINFO_MAGLV);
	registerEnum(L, WIELDINFO_VOCREQ);
	registerEnum(L, WIELDINFO_PREMIUM);

	registerEnum(L, PlayerFlag_CannotUseCombat);
	registerEnum(L, PlayerFlag_CannotAttackPlayer);
	registerEnum(L, PlayerFlag_CannotAttackMonster);
	registerEnum(L, PlayerFlag_CannotBeAttacked);
	registerEnum(L, PlayerFlag_CanConvinceAll);
	registerEnum(L, PlayerFlag_CanSummonAll);
	registerEnum(L, PlayerFlag_CanIllusionAll);
	registerEnum(L, PlayerFlag_CanSenseInvisibility);
	registerEnum(L, PlayerFlag_IgnoredByMonsters);
	registerEnum(L, PlayerFlag_NotGainInFight);
	registerEnum(L, PlayerFlag_HasInfiniteMana);
	registerEnum(L, PlayerFlag_HasInfiniteSoul);
	registerEnum(L, PlayerFlag_HasNoExhaustion);
	registerEnum(L, PlayerFlag_CannotUseSpells);
	registerEnum(L, PlayerFlag_CannotPickupItem);
	registerEnum(L, PlayerFlag_CanAlwaysLogin);
	registerEnum(L, PlayerFlag_CanBroadcast);
	registerEnum(L, PlayerFlag_CanEditHouses);
	registerEnum(L, PlayerFlag_CannotBeBanned);
	registerEnum(L, PlayerFlag_CannotBePushed);
	registerEnum(L, PlayerFlag_HasInfiniteCapacity);
	registerEnum(L, PlayerFlag_CanPushAllCreatures);
	registerEnum(L, PlayerFlag_CanTalkRedPrivate);
	registerEnum(L, PlayerFlag_CanTalkRedChannel);
	registerEnum(L, PlayerFlag_TalkOrangeHelpChannel);
	registerEnum(L, PlayerFlag_NotGainExperience);
	registerEnum(L, PlayerFlag_NotGainMana);
	registerEnum(L, PlayerFlag_NotGainHealth);
	registerEnum(L, PlayerFlag_NotGainSkill);
	registerEnum(L, PlayerFlag_SetMaxSpeed);
	registerEnum(L, PlayerFlag_SpecialVIP);
	registerEnum(L, PlayerFlag_NotGenerateLoot);
	registerEnum(L, PlayerFlag_IgnoreProtectionZone);
	registerEnum(L, PlayerFlag_IgnoreSpellCheck);
	registerEnum(L, PlayerFlag_IgnoreWeaponCheck);
	registerEnum(L, PlayerFlag_CannotBeMuted);
	registerEnum(L, PlayerFlag_IsAlwaysPremium);
	registerEnum(L, PlayerFlag_IgnoreYellCheck);
	registerEnum(L, PlayerFlag_IgnoreSendPrivateCheck);

	registerEnum(L, PODIUM_SHOW_PLATFORM);
	registerEnum(L, PODIUM_SHOW_OUTFIT);
	registerEnum(L, PODIUM_SHOW_MOUNT);

	registerEnum(L, PLAYERSEX_FEMALE);
	registerEnum(L, PLAYERSEX_MALE);

	registerEnum(L, REPORT_REASON_NAMEINAPPROPRIATE);
	registerEnum(L, REPORT_REASON_NAMEPOORFORMATTED);
	registerEnum(L, REPORT_REASON_NAMEADVERTISING);
	registerEnum(L, REPORT_REASON_NAMEUNFITTING);
	registerEnum(L, REPORT_REASON_NAMERULEVIOLATION);
	registerEnum(L, REPORT_REASON_INSULTINGSTATEMENT);
	registerEnum(L, REPORT_REASON_SPAMMING);
	registerEnum(L, REPORT_REASON_ADVERTISINGSTATEMENT);
	registerEnum(L, REPORT_REASON_UNFITTINGSTATEMENT);
	registerEnum(L, REPORT_REASON_LANGUAGESTATEMENT);
	registerEnum(L, REPORT_REASON_DISCLOSURE);
	registerEnum(L, REPORT_REASON_RULEVIOLATION);
	registerEnum(L, REPORT_REASON_STATEMENT_BUGABUSE);
	registerEnum(L, REPORT_REASON_UNOFFICIALSOFTWARE);
	registerEnum(L, REPORT_REASON_PRETENDING);
	registerEnum(L, REPORT_REASON_HARASSINGOWNERS);
	registerEnum(L, REPORT_REASON_FALSEINFO);
	registerEnum(L, REPORT_REASON_ACCOUNTSHARING);
	registerEnum(L, REPORT_REASON_STEALINGDATA);
	registerEnum(L, REPORT_REASON_SERVICEATTACKING);
	registerEnum(L, REPORT_REASON_SERVICEAGREEMENT);

	registerEnum(L, REPORT_TYPE_NAME);
	registerEnum(L, REPORT_TYPE_STATEMENT);
	registerEnum(L, REPORT_TYPE_BOT);

	registerEnum(L, VOCATION_NONE);

	registerEnum(L, SKILL_FIST);
	registerEnum(L, SKILL_CLUB);
	registerEnum(L, SKILL_SWORD);
	registerEnum(L, SKILL_AXE);
	registerEnum(L, SKILL_DISTANCE);
	registerEnum(L, SKILL_SHIELD);
	registerEnum(L, SKILL_FISHING);
	registerEnum(L, SKILL_MAGLEVEL);
	registerEnum(L, SKILL_LEVEL);

	registerEnum(L, SPECIALSKILL_CRITICALHITCHANCE);
	registerEnum(L, SPECIALSKILL_CRITICALHITAMOUNT);
	registerEnum(L, SPECIALSKILL_LIFELEECHCHANCE);
	registerEnum(L, SPECIALSKILL_LIFELEECHAMOUNT);
	registerEnum(L, SPECIALSKILL_MANALEECHCHANCE);
	registerEnum(L, SPECIALSKILL_MANALEECHAMOUNT);

	registerEnum(L, STAT_MAXHITPOINTS);
	registerEnum(L, STAT_MAXMANAPOINTS);
	registerEnum(L, STAT_SOULPOINTS);
	registerEnum(L, STAT_MAGICPOINTS);

	registerEnum(L, SKULL_NONE);
	registerEnum(L, SKULL_YELLOW);
	registerEnum(L, SKULL_GREEN);
	registerEnum(L, SKULL_WHITE);
	registerEnum(L, SKULL_RED);
	registerEnum(L, SKULL_BLACK);
	registerEnum(L, SKULL_ORANGE);

	registerEnum(L, FLUID_NONE);
	registerEnum(L, FLUID_WATER);
	registerEnum(L, FLUID_BLOOD);
	registerEnum(L, FLUID_BEER);
	registerEnum(L, FLUID_SLIME);
	registerEnum(L, FLUID_LEMONADE);
	registerEnum(L, FLUID_MILK);
	registerEnum(L, FLUID_MANA);
	registerEnum(L, FLUID_LIFE);
	registerEnum(L, FLUID_OIL);
	registerEnum(L, FLUID_URINE);
	registerEnum(L, FLUID_COCONUTMILK);
	registerEnum(L, FLUID_WINE);
	registerEnum(L, FLUID_MUD);
	registerEnum(L, FLUID_FRUITJUICE);
	registerEnum(L, FLUID_LAVA);
	registerEnum(L, FLUID_RUM);
	registerEnum(L, FLUID_SWAMP);
	registerEnum(L, FLUID_TEA);
	registerEnum(L, FLUID_MEAD);

	registerEnum(L, TALKTYPE_SAY);
	registerEnum(L, TALKTYPE_WHISPER);
	registerEnum(L, TALKTYPE_YELL);
	registerEnum(L, TALKTYPE_PRIVATE_FROM);
	registerEnum(L, TALKTYPE_PRIVATE_TO);
	registerEnum(L, TALKTYPE_CHANNEL_Y);
	registerEnum(L, TALKTYPE_CHANNEL_O);
	registerEnum(L, TALKTYPE_SPELL);
	registerEnum(L, TALKTYPE_PRIVATE_NP);
	registerEnum(L, TALKTYPE_PRIVATE_NP_CONSOLE);
	registerEnum(L, TALKTYPE_PRIVATE_PN);
	registerEnum(L, TALKTYPE_BROADCAST);
	registerEnum(L, TALKTYPE_CHANNEL_R1);
	registerEnum(L, TALKTYPE_PRIVATE_RED_FROM);
	registerEnum(L, TALKTYPE_PRIVATE_RED_TO);
	registerEnum(L, TALKTYPE_MONSTER_SAY);
	registerEnum(L, TALKTYPE_MONSTER_YELL);
	registerEnum(L, TALKTYPE_POTION);

	registerEnum(L, TEXTCOLOR_BLUE);
	registerEnum(L, TEXTCOLOR_LIGHTGREEN);
	registerEnum(L, TEXTCOLOR_LIGHTBLUE);
	registerEnum(L, TEXTCOLOR_MAYABLUE);
	registerEnum(L, TEXTCOLOR_DARKRED);
	registerEnum(L, TEXTCOLOR_LIGHTGREY);
	registerEnum(L, TEXTCOLOR_SKYBLUE);
	registerEnum(L, TEXTCOLOR_PURPLE);
	registerEnum(L, TEXTCOLOR_ELECTRICPURPLE);
	registerEnum(L, TEXTCOLOR_RED);
	registerEnum(L, TEXTCOLOR_PASTELRED);
	registerEnum(L, TEXTCOLOR_ORANGE);
	registerEnum(L, TEXTCOLOR_YELLOW);
	registerEnum(L, TEXTCOLOR_WHITE_EXP);
	registerEnum(L, TEXTCOLOR_NONE);

	registerEnum(L, TILESTATE_NONE);
	registerEnum(L, TILESTATE_PROTECTIONZONE);
	registerEnum(L, TILESTATE_NOPVPZONE);
	registerEnum(L, TILESTATE_NOLOGOUT);
	registerEnum(L, TILESTATE_PVPZONE);
	registerEnum(L, TILESTATE_FLOORCHANGE);
	registerEnum(L, TILESTATE_FLOORCHANGE_DOWN);
	registerEnum(L, TILESTATE_FLOORCHANGE_NORTH);
	registerEnum(L, TILESTATE_FLOORCHANGE_SOUTH);
	registerEnum(L, TILESTATE_FLOORCHANGE_EAST);
	registerEnum(L, TILESTATE_FLOORCHANGE_WEST);
	registerEnum(L, TILESTATE_TELEPORT);
	registerEnum(L, TILESTATE_MAGICFIELD);
	registerEnum(L, TILESTATE_MAILBOX);
	registerEnum(L, TILESTATE_TRASHHOLDER);
	registerEnum(L, TILESTATE_BED);
	registerEnum(L, TILESTATE_DEPOT);
	registerEnum(L, TILESTATE_BLOCKSOLID);
	registerEnum(L, TILESTATE_BLOCKPATH);
	registerEnum(L, TILESTATE_IMMOVABLEBLOCKSOLID);
	registerEnum(L, TILESTATE_IMMOVABLEBLOCKPATH);
	registerEnum(L, TILESTATE_IMMOVABLENOFIELDBLOCKPATH);
	registerEnum(L, TILESTATE_NOFIELDBLOCKPATH);
	registerEnum(L, TILESTATE_FLOORCHANGE_SOUTH_ALT);
	registerEnum(L, TILESTATE_FLOORCHANGE_EAST_ALT);
	registerEnum(L, TILESTATE_SUPPORTS_HANGABLE);

	registerEnum(L, WEAPON_NONE);
	registerEnum(L, WEAPON_SWORD);
	registerEnum(L, WEAPON_CLUB);
	registerEnum(L, WEAPON_AXE);
	registerEnum(L, WEAPON_SHIELD);
	registerEnum(L, WEAPON_DISTANCE);
	registerEnum(L, WEAPON_WAND);
	registerEnum(L, WEAPON_AMMO);
	registerEnum(L, WEAPON_QUIVER);

	registerEnum(L, WORLD_TYPE_NO_PVP);
	registerEnum(L, WORLD_TYPE_PVP);
	registerEnum(L, WORLD_TYPE_PVP_ENFORCED);

	// Use with container:addItem, container:addItemEx and possibly other functions.
	registerEnum(L, FLAG_NOLIMIT);
	registerEnum(L, FLAG_IGNOREBLOCKITEM);
	registerEnum(L, FLAG_IGNOREBLOCKCREATURE);
	registerEnum(L, FLAG_CHILDISOWNER);
	registerEnum(L, FLAG_PATHFINDING);
	registerEnum(L, FLAG_IGNOREFIELDDAMAGE);
	registerEnum(L, FLAG_IGNORENOTMOVEABLE);
	registerEnum(L, FLAG_IGNOREAUTOSTACK);

	// Use with itemType:getSlotPosition
	registerEnum(L, SLOTP_WHEREEVER);
	registerEnum(L, SLOTP_HEAD);
	registerEnum(L, SLOTP_NECKLACE);
	registerEnum(L, SLOTP_BACKPACK);
	registerEnum(L, SLOTP_ARMOR);
	registerEnum(L, SLOTP_RIGHT);
	registerEnum(L, SLOTP_LEFT);
	registerEnum(L, SLOTP_LEGS);
	registerEnum(L, SLOTP_FEET);
	registerEnum(L, SLOTP_RING);
	registerEnum(L, SLOTP_AMMO);
	registerEnum(L, SLOTP_DEPOT);
	registerEnum(L, SLOTP_TWO_HAND);

	// Use with combat functions
	registerEnum(L, ORIGIN_NONE);
	registerEnum(L, ORIGIN_CONDITION);
	registerEnum(L, ORIGIN_SPELL);
	registerEnum(L, ORIGIN_MELEE);
	registerEnum(L, ORIGIN_RANGED);
	registerEnum(L, ORIGIN_WAND);

	// Use with house:getAccessList, house:setAccessList
	registerEnum(L, GUEST_LIST);
	registerEnum(L, SUBOWNER_LIST);

	// Use with npc:setSpeechBubble
	registerEnum(L, SPEECHBUBBLE_NONE);
	registerEnum(L, SPEECHBUBBLE_NORMAL);
	registerEnum(L, SPEECHBUBBLE_TRADE);
	registerEnum(L, SPEECHBUBBLE_QUEST);
	registerEnum(L, SPEECHBUBBLE_COMPASS);
	registerEnum(L, SPEECHBUBBLE_NORMAL2);
	registerEnum(L, SPEECHBUBBLE_NORMAL3);
	registerEnum(L, SPEECHBUBBLE_HIRELING);

	// Use with player:addMapMark
	registerEnum(L, MAPMARK_TICK);
	registerEnum(L, MAPMARK_QUESTION);
	registerEnum(L, MAPMARK_EXCLAMATION);
	registerEnum(L, MAPMARK_STAR);
	registerEnum(L, MAPMARK_CROSS);
	registerEnum(L, MAPMARK_TEMPLE);
	registerEnum(L, MAPMARK_KISS);
	registerEnum(L, MAPMARK_SHOVEL);
	registerEnum(L, MAPMARK_SWORD);
	registerEnum(L, MAPMARK_FLAG);
	registerEnum(L, MAPMARK_LOCK);
	registerEnum(L, MAPMARK_BAG);
	registerEnum(L, MAPMARK_SKULL);
	registerEnum(L, MAPMARK_DOLLAR);
	registerEnum(L, MAPMARK_REDNORTH);
	registerEnum(L, MAPMARK_REDSOUTH);
	registerEnum(L, MAPMARK_REDEAST);
	registerEnum(L, MAPMARK_REDWEST);
	registerEnum(L, MAPMARK_GREENNORTH);
	registerEnum(L, MAPMARK_GREENSOUTH);

	// Use with Game.getReturnMessage
	registerEnum(L, RETURNVALUE_NOERROR);
	registerEnum(L, RETURNVALUE_NOTPOSSIBLE);
	registerEnum(L, RETURNVALUE_NOTENOUGHROOM);
	registerEnum(L, RETURNVALUE_PLAYERISPZLOCKED);
	registerEnum(L, RETURNVALUE_PLAYERISNOTINVITED);
	registerEnum(L, RETURNVALUE_CANNOTTHROW);
	registerEnum(L, RETURNVALUE_THEREISNOWAY);
	registerEnum(L, RETURNVALUE_DESTINATIONOUTOFREACH);
	registerEnum(L, RETURNVALUE_CREATUREBLOCK);
	registerEnum(L, RETURNVALUE_NOTMOVEABLE);
	registerEnum(L, RETURNVALUE_DROPTWOHANDEDITEM);
	registerEnum(L, RETURNVALUE_BOTHHANDSNEEDTOBEFREE);
	registerEnum(L, RETURNVALUE_CANONLYUSEONEWEAPON);
	registerEnum(L, RETURNVALUE_NEEDEXCHANGE);
	registerEnum(L, RETURNVALUE_CANNOTBEDRESSED);
	registerEnum(L, RETURNVALUE_PUTTHISOBJECTINYOURHAND);
	registerEnum(L, RETURNVALUE_PUTTHISOBJECTINBOTHHANDS);
	registerEnum(L, RETURNVALUE_TOOFARAWAY);
	registerEnum(L, RETURNVALUE_FIRSTGODOWNSTAIRS);
	registerEnum(L, RETURNVALUE_FIRSTGOUPSTAIRS);
	registerEnum(L, RETURNVALUE_CONTAINERNOTENOUGHROOM);
	registerEnum(L, RETURNVALUE_NOTENOUGHCAPACITY);
	registerEnum(L, RETURNVALUE_CANNOTPICKUP);
	registerEnum(L, RETURNVALUE_THISISIMPOSSIBLE);
	registerEnum(L, RETURNVALUE_DEPOTISFULL);
	registerEnum(L, RETURNVALUE_CREATUREDOESNOTEXIST);
	registerEnum(L, RETURNVALUE_CANNOTUSETHISOBJECT);
	registerEnum(L, RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE);
	registerEnum(L, RETURNVALUE_NOTREQUIREDLEVELTOUSERUNE);
	registerEnum(L, RETURNVALUE_YOUAREALREADYTRADING);
	registerEnum(L, RETURNVALUE_THISPLAYERISALREADYTRADING);
	registerEnum(L, RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT);
	registerEnum(L, RETURNVALUE_DIRECTPLAYERSHOOT);
	registerEnum(L, RETURNVALUE_NOTENOUGHLEVEL);
	registerEnum(L, RETURNVALUE_NOTENOUGHMAGICLEVEL);
	registerEnum(L, RETURNVALUE_NOTENOUGHMANA);
	registerEnum(L, RETURNVALUE_NOTENOUGHSOUL);
	registerEnum(L, RETURNVALUE_YOUAREEXHAUSTED);
	registerEnum(L, RETURNVALUE_YOUCANNOTUSEOBJECTSTHATFAST);
	registerEnum(L, RETURNVALUE_PLAYERISNOTREACHABLE);
	registerEnum(L, RETURNVALUE_CANONLYUSETHISRUNEONCREATURES);
	registerEnum(L, RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE);
	registerEnum(L, RETURNVALUE_YOUMAYNOTATTACKTHISPLAYER);
	registerEnum(L, RETURNVALUE_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE);
	registerEnum(L, RETURNVALUE_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE);
	registerEnum(L, RETURNVALUE_YOUMAYNOTATTACKTHISCREATURE);
	registerEnum(L, RETURNVALUE_YOUCANONLYUSEITONCREATURES);
	registerEnum(L, RETURNVALUE_CREATUREISNOTREACHABLE);
	registerEnum(L, RETURNVALUE_TURNSECUREMODETOATTACKUNMARKEDPLAYERS);
	registerEnum(L, RETURNVALUE_YOUNEEDPREMIUMACCOUNT);
	registerEnum(L, RETURNVALUE_YOUNEEDTOLEARNTHISSPELL);
	registerEnum(L, RETURNVALUE_YOURVOCATIONCANNOTUSETHISSPELL);
	registerEnum(L, RETURNVALUE_YOUNEEDAWEAPONTOUSETHISSPELL);
	registerEnum(L, RETURNVALUE_PLAYERISPZLOCKEDLEAVEPVPZONE);
	registerEnum(L, RETURNVALUE_PLAYERISPZLOCKEDENTERPVPZONE);
	registerEnum(L, RETURNVALUE_ACTIONNOTPERMITTEDINANOPVPZONE);
	registerEnum(L, RETURNVALUE_YOUCANNOTLOGOUTHERE);
	registerEnum(L, RETURNVALUE_YOUNEEDAMAGICITEMTOCASTSPELL);
	registerEnum(L, RETURNVALUE_NAMEISTOOAMBIGUOUS);
	registerEnum(L, RETURNVALUE_CANONLYUSEONESHIELD);
	registerEnum(L, RETURNVALUE_NOPARTYMEMBERSINRANGE);
	registerEnum(L, RETURNVALUE_YOUARENOTTHEOWNER);
	registerEnum(L, RETURNVALUE_TRADEPLAYERFARAWAY);
	registerEnum(L, RETURNVALUE_YOUDONTOWNTHISHOUSE);
	registerEnum(L, RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE);
	registerEnum(L, RETURNVALUE_TRADEPLAYERHIGHESTBIDDER);
	registerEnum(L, RETURNVALUE_YOUCANNOTTRADETHISHOUSE);
	registerEnum(L, RETURNVALUE_YOUDONTHAVEREQUIREDPROFESSION);
	registerEnum(L, RETURNVALUE_YOUCANNOTUSETHISBED);

	registerEnum(L, RELOAD_TYPE_ALL);
	registerEnum(L, RELOAD_TYPE_ACTIONS);
	registerEnum(L, RELOAD_TYPE_CHAT);
	registerEnum(L, RELOAD_TYPE_CONFIG);
	registerEnum(L, RELOAD_TYPE_CREATURESCRIPTS);
	registerEnum(L, RELOAD_TYPE_EVENTS);
	registerEnum(L, RELOAD_TYPE_GLOBAL);
	registerEnum(L, RELOAD_TYPE_GLOBALEVENTS);
	registerEnum(L, RELOAD_TYPE_ITEMS);
	registerEnum(L, RELOAD_TYPE_MONSTERS);
	registerEnum(L, RELOAD_TYPE_MOUNTS);
	registerEnum(L, RELOAD_TYPE_MOVEMENTS);
	registerEnum(L, RELOAD_TYPE_NPCS);
	registerEnum(L, RELOAD_TYPE_QUESTS);
	registerEnum(L, RELOAD_TYPE_SCRIPTS);
	registerEnum(L, RELOAD_TYPE_SPELLS);
	registerEnum(L, RELOAD_TYPE_TALKACTIONS);
	registerEnum(L, RELOAD_TYPE_WEAPONS);

	registerEnum(L, ZONE_PROTECTION);
	registerEnum(L, ZONE_NOPVP);
	registerEnum(L, ZONE_PVP);
	registerEnum(L, ZONE_NOLOGOUT);
	registerEnum(L, ZONE_NORMAL);

	registerEnum(L, MAX_LOOTCHANCE);

	registerEnum(L, SPELL_INSTANT);
	registerEnum(L, SPELL_RUNE);

	registerEnum(L, MONSTERS_EVENT_THINK);
	registerEnum(L, MONSTERS_EVENT_APPEAR);
	registerEnum(L, MONSTERS_EVENT_DISAPPEAR);
	registerEnum(L, MONSTERS_EVENT_MOVE);
	registerEnum(L, MONSTERS_EVENT_SAY);

	registerEnum(L, DECAYING_FALSE);
	registerEnum(L, DECAYING_TRUE);
	registerEnum(L, DECAYING_PENDING);

	registerEnum(L, RESOURCE_BANK_BALANCE);
	registerEnum(L, RESOURCE_GOLD_EQUIPPED);
	registerEnum(L, RESOURCE_PREY_WILDCARDS);
	registerEnum(L, RESOURCE_DAILYREWARD_STREAK);
	registerEnum(L, RESOURCE_DAILYREWARD_JOKERS);

	registerEnum(L, CREATURE_ICON_CROSS_WHITE);
	registerEnum(L, CREATURE_ICON_CROSS_WHITE_RED);
	registerEnum(L, CREATURE_ICON_ORB_RED);
	registerEnum(L, CREATURE_ICON_ORB_GREEN);
	registerEnum(L, CREATURE_ICON_ORB_RED_GREEN);
	registerEnum(L, CREATURE_ICON_GEM_GREEN);
	registerEnum(L, CREATURE_ICON_GEM_YELLOW);
	registerEnum(L, CREATURE_ICON_GEM_BLUE);
	registerEnum(L, CREATURE_ICON_GEM_PURPLE);
	registerEnum(L, CREATURE_ICON_GEM_RED);
	registerEnum(L, CREATURE_ICON_PIGEON);
	registerEnum(L, CREATURE_ICON_ENERGY);
	registerEnum(L, CREATURE_ICON_POISON);
	registerEnum(L, CREATURE_ICON_WATER);
	registerEnum(L, CREATURE_ICON_FIRE);
	registerEnum(L, CREATURE_ICON_ICE);
	registerEnum(L, CREATURE_ICON_ARROW_UP);
	registerEnum(L, CREATURE_ICON_ARROW_DOWN);
	registerEnum(L, CREATURE_ICON_WARNING);
	registerEnum(L, CREATURE_ICON_QUESTION);
	registerEnum(L, CREATURE_ICON_CROSS_RED);
	registerEnum(L, CREATURE_ICON_FIRST);
	registerEnum(L, CREATURE_ICON_LAST);

	registerEnum(L, MONSTER_ICON_VULNERABLE);
	registerEnum(L, MONSTER_ICON_WEAKENED);
	registerEnum(L, MONSTER_ICON_MELEE);
	registerEnum(L, MONSTER_ICON_INFLUENCED);
	registerEnum(L, MONSTER_ICON_FIENDISH);
	registerEnum(L, MONSTER_ICON_FIRST);
	registerEnum(L, MONSTER_ICON_LAST);

	// _G
	registerGlobalVariable(L, "INDEX_WHEREEVER", INDEX_WHEREEVER);
	registerGlobalBoolean(L, "VIRTUAL_PARENT", true);

	registerGlobalMethod(L, "isType", LuaScriptInterface::luaIsType);
	registerGlobalMethod(L, "rawgetmetatable", LuaScriptInterface::luaRawGetMetatable);

	// configKeys
	registerTable(L, "configKeys");

	registerEnumIn(L, "configKeys", ConfigManager::ALLOW_CHANGEOUTFIT);
	registerEnumIn(L, "configKeys", ConfigManager::ONE_PLAYER_ON_ACCOUNT);
	registerEnumIn(L, "configKeys", ConfigManager::AIMBOT_HOTKEY_ENABLED);
	registerEnumIn(L, "configKeys", ConfigManager::REMOVE_RUNE_CHARGES);
	registerEnumIn(L, "configKeys", ConfigManager::REMOVE_WEAPON_AMMO);
	registerEnumIn(L, "configKeys", ConfigManager::REMOVE_WEAPON_CHARGES);
	registerEnumIn(L, "configKeys", ConfigManager::REMOVE_POTION_CHARGES);
	registerEnumIn(L, "configKeys", ConfigManager::EXPERIENCE_FROM_PLAYERS);
	registerEnumIn(L, "configKeys", ConfigManager::FREE_PREMIUM);
	registerEnumIn(L, "configKeys", ConfigManager::REPLACE_KICK_ON_LOGIN);
	registerEnumIn(L, "configKeys", ConfigManager::ALLOW_CLONES);
	registerEnumIn(L, "configKeys", ConfigManager::BIND_ONLY_GLOBAL_ADDRESS);
	registerEnumIn(L, "configKeys", ConfigManager::OPTIMIZE_DATABASE);
	registerEnumIn(L, "configKeys", ConfigManager::MARKET_PREMIUM);
	registerEnumIn(L, "configKeys", ConfigManager::EMOTE_SPELLS);
	registerEnumIn(L, "configKeys", ConfigManager::STAMINA_SYSTEM);
	registerEnumIn(L, "configKeys", ConfigManager::WARN_UNSAFE_SCRIPTS);
	registerEnumIn(L, "configKeys", ConfigManager::CONVERT_UNSAFE_SCRIPTS);
	registerEnumIn(L, "configKeys", ConfigManager::CLASSIC_EQUIPMENT_SLOTS);
	registerEnumIn(L, "configKeys", ConfigManager::CLASSIC_ATTACK_SPEED);
	registerEnumIn(L, "configKeys", ConfigManager::SERVER_SAVE_NOTIFY_MESSAGE);
	registerEnumIn(L, "configKeys", ConfigManager::SERVER_SAVE_NOTIFY_DURATION);
	registerEnumIn(L, "configKeys", ConfigManager::SERVER_SAVE_CLEAN_MAP);
	registerEnumIn(L, "configKeys", ConfigManager::SERVER_SAVE_CLOSE);
	registerEnumIn(L, "configKeys", ConfigManager::SERVER_SAVE_SHUTDOWN);
	registerEnumIn(L, "configKeys", ConfigManager::ONLINE_OFFLINE_CHARLIST);
	registerEnumIn(L, "configKeys", ConfigManager::CHECK_DUPLICATE_STORAGE_KEYS);

	registerEnumIn(L, "configKeys", ConfigManager::MAP_NAME);
	registerEnumIn(L, "configKeys", ConfigManager::HOUSE_RENT_PERIOD);
	registerEnumIn(L, "configKeys", ConfigManager::SERVER_NAME);
	registerEnumIn(L, "configKeys", ConfigManager::OWNER_NAME);
	registerEnumIn(L, "configKeys", ConfigManager::OWNER_EMAIL);
	registerEnumIn(L, "configKeys", ConfigManager::URL);
	registerEnumIn(L, "configKeys", ConfigManager::LOCATION);
	registerEnumIn(L, "configKeys", ConfigManager::IP);
	registerEnumIn(L, "configKeys", ConfigManager::WORLD_TYPE);
	registerEnumIn(L, "configKeys", ConfigManager::MYSQL_HOST);
	registerEnumIn(L, "configKeys", ConfigManager::MYSQL_USER);
	registerEnumIn(L, "configKeys", ConfigManager::MYSQL_PASS);
	registerEnumIn(L, "configKeys", ConfigManager::MYSQL_DB);
	registerEnumIn(L, "configKeys", ConfigManager::MYSQL_SOCK);
	registerEnumIn(L, "configKeys", ConfigManager::DEFAULT_PRIORITY);
	registerEnumIn(L, "configKeys", ConfigManager::MAP_AUTHOR);

	registerEnumIn(L, "configKeys", ConfigManager::SQL_PORT);
	registerEnumIn(L, "configKeys", ConfigManager::MAX_PLAYERS);
	registerEnumIn(L, "configKeys", ConfigManager::PZ_LOCKED);
	registerEnumIn(L, "configKeys", ConfigManager::DEFAULT_DESPAWNRANGE);
	registerEnumIn(L, "configKeys", ConfigManager::DEFAULT_DESPAWNRADIUS);
	registerEnumIn(L, "configKeys", ConfigManager::DEFAULT_WALKTOSPAWNRADIUS);
	registerEnumIn(L, "configKeys", ConfigManager::REMOVE_ON_DESPAWN);
	registerEnumIn(L, "configKeys", ConfigManager::RATE_EXPERIENCE);
	registerEnumIn(L, "configKeys", ConfigManager::RATE_SKILL);
	registerEnumIn(L, "configKeys", ConfigManager::RATE_LOOT);
	registerEnumIn(L, "configKeys", ConfigManager::RATE_MAGIC);
	registerEnumIn(L, "configKeys", ConfigManager::RATE_SPAWN);
	registerEnumIn(L, "configKeys", ConfigManager::HOUSE_PRICE);
	registerEnumIn(L, "configKeys", ConfigManager::KILLS_TO_RED);
	registerEnumIn(L, "configKeys", ConfigManager::KILLS_TO_BLACK);
	registerEnumIn(L, "configKeys", ConfigManager::MAX_MESSAGEBUFFER);
	registerEnumIn(L, "configKeys", ConfigManager::ACTIONS_DELAY_INTERVAL);
	registerEnumIn(L, "configKeys", ConfigManager::EX_ACTIONS_DELAY_INTERVAL);
	registerEnumIn(L, "configKeys", ConfigManager::KICK_AFTER_MINUTES);
	registerEnumIn(L, "configKeys", ConfigManager::PROTECTION_LEVEL);
	registerEnumIn(L, "configKeys", ConfigManager::DEATH_LOSE_PERCENT);
	registerEnumIn(L, "configKeys", ConfigManager::STATUSQUERY_TIMEOUT);
	registerEnumIn(L, "configKeys", ConfigManager::FRAG_TIME);
	registerEnumIn(L, "configKeys", ConfigManager::WHITE_SKULL_TIME);
	registerEnumIn(L, "configKeys", ConfigManager::GAME_PORT);
	registerEnumIn(L, "configKeys", ConfigManager::LOGIN_PORT);
	registerEnumIn(L, "configKeys", ConfigManager::STATUS_PORT);
	registerEnumIn(L, "configKeys", ConfigManager::STAIRHOP_DELAY);
	registerEnumIn(L, "configKeys", ConfigManager::MARKET_OFFER_DURATION);
	registerEnumIn(L, "configKeys", ConfigManager::CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES);
	registerEnumIn(L, "configKeys", ConfigManager::MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER);
	registerEnumIn(L, "configKeys", ConfigManager::EXP_FROM_PLAYERS_LEVEL_RANGE);
	registerEnumIn(L, "configKeys", ConfigManager::MAX_PACKETS_PER_SECOND);
	registerEnumIn(L, "configKeys", ConfigManager::TWO_FACTOR_AUTH);
	registerEnumIn(L, "configKeys", ConfigManager::MANASHIELD_BREAKABLE);
	registerEnumIn(L, "configKeys", ConfigManager::STAMINA_REGEN_MINUTE);
	registerEnumIn(L, "configKeys", ConfigManager::STAMINA_REGEN_PREMIUM);
	registerEnumIn(L, "configKeys", ConfigManager::HOUSE_DOOR_SHOW_PRICE);
	registerEnumIn(L, "configKeys", ConfigManager::MONSTER_OVERSPAWN);

	registerEnumIn(L, "configKeys", ConfigManager::QUEST_TRACKER_FREE_LIMIT);
	registerEnumIn(L, "configKeys", ConfigManager::QUEST_TRACKER_PREMIUM_LIMIT);

	// os
	registerMethod(L, "os", "mtime", LuaScriptInterface::luaSystemTime);

	// table
	registerMethod(L, "table", "create", LuaScriptInterface::luaTableCreate);
	registerMethod(L, "table", "pack", LuaScriptInterface::luaTablePack);

	// DB Insert
	registerClass(L, "DBInsert", "", LuaScriptInterface::luaDBInsertCreate);
	registerMetaMethod(L, "DBInsert", "__gc", LuaScriptInterface::luaDBInsertDelete);

	registerMethod(L, "DBInsert", "addRow", LuaScriptInterface::luaDBInsertAddRow);
	registerMethod(L, "DBInsert", "execute", LuaScriptInterface::luaDBInsertExecute);

	// DB Transaction
	registerClass(L, "DBTransaction", "", LuaScriptInterface::luaDBTransactionCreate);
	registerMetaMethod(L, "DBTransaction", "__eq", LuaScriptInterface::luaUserdataCompare);
	registerMetaMethod(L, "DBTransaction", "__gc", LuaScriptInterface::luaDBTransactionDelete);

	registerMethod(L, "DBTransaction", "begin", LuaScriptInterface::luaDBTransactionBegin);
	registerMethod(L, "DBTransaction", "commit", LuaScriptInterface::luaDBTransactionCommit);
	registerMethod(L, "DBTransaction", "rollback", LuaScriptInterface::luaDBTransactionDelete);

	// Game
	registerTable(L, "Game");

	registerMethod(L, "Game", "getSpectators", LuaScriptInterface::luaGameGetSpectators);
	registerMethod(L, "Game", "getPlayers", LuaScriptInterface::luaGameGetPlayers);
	registerMethod(L, "Game", "getNpcs", LuaScriptInterface::luaGameGetNpcs);
	registerMethod(L, "Game", "getMonsters", LuaScriptInterface::luaGameGetMonsters);
	registerMethod(L, "Game", "loadMap", LuaScriptInterface::luaGameLoadMap);

	registerMethod(L, "Game", "getExperienceStage", LuaScriptInterface::luaGameGetExperienceStage);
	registerMethod(L, "Game", "getExperienceForLevel", LuaScriptInterface::luaGameGetExperienceForLevel);
	registerMethod(L, "Game", "getMonsterCount", LuaScriptInterface::luaGameGetMonsterCount);
	registerMethod(L, "Game", "getPlayerCount", LuaScriptInterface::luaGameGetPlayerCount);
	registerMethod(L, "Game", "getNpcCount", LuaScriptInterface::luaGameGetNpcCount);
	registerMethod(L, "Game", "getMonsterTypes", LuaScriptInterface::luaGameGetMonsterTypes);
	registerMethod(L, "Game", "getBestiary", LuaScriptInterface::luaGameGetBestiary);
	registerMethod(L, "Game", "getCurrencyItems", LuaScriptInterface::luaGameGetCurrencyItems);
	registerMethod(L, "Game", "getItemTypeByClientId", LuaScriptInterface::luaGameGetItemTypeByClientId);
	registerMethod(L, "Game", "getMountIdByLookType", LuaScriptInterface::luaGameGetMountIdByLookType);

	registerMethod(L, "Game", "getTowns", LuaScriptInterface::luaGameGetTowns);
	registerMethod(L, "Game", "getHouses", LuaScriptInterface::luaGameGetHouses);
	registerMethod(L, "Game", "getOutfits", LuaScriptInterface::luaGameGetOutfits);
	registerMethod(L, "Game", "getMounts", LuaScriptInterface::luaGameGetMounts);
	registerMethod(L, "Game", "getVocations", LuaScriptInterface::luaGameGetVocations);
	registerMethod(L, "Game", "getRuneSpells", LuaScriptInterface::luaGameGetRuneSpells);
	registerMethod(L, "Game", "getInstantSpells", LuaScriptInterface::luaGameGetInstantSpells);

	registerMethod(L, "Game", "getGameState", LuaScriptInterface::luaGameGetGameState);
	registerMethod(L, "Game", "setGameState", LuaScriptInterface::luaGameSetGameState);

	registerMethod(L, "Game", "getWorldType", LuaScriptInterface::luaGameGetWorldType);
	registerMethod(L, "Game", "setWorldType", LuaScriptInterface::luaGameSetWorldType);

	registerMethod(L, "Game", "getItemAttributeByName", LuaScriptInterface::luaGameGetItemAttributeByName);
	registerMethod(L, "Game", "getReturnMessage", LuaScriptInterface::luaGameGetReturnMessage);

	registerMethod(L, "Game", "createItem", LuaScriptInterface::luaGameCreateItem);
	registerMethod(L, "Game", "createContainer", LuaScriptInterface::luaGameCreateContainer);
	registerMethod(L, "Game", "createMonster", LuaScriptInterface::luaGameCreateMonster);
	registerMethod(L, "Game", "createNpc", LuaScriptInterface::luaGameCreateNpc);
	registerMethod(L, "Game", "createTile", LuaScriptInterface::luaGameCreateTile);
	registerMethod(L, "Game", "createMonsterType", LuaScriptInterface::luaGameCreateMonsterType);

	registerMethod(L, "Game", "startEvent", LuaScriptInterface::luaGameStartEvent);

	registerMethod(L, "Game", "getClientVersion", LuaScriptInterface::luaGameGetClientVersion);

	registerMethod(L, "Game", "reload", LuaScriptInterface::luaGameReload);

	// Variant
	registerClass(L, "Variant", "", LuaScriptInterface::luaVariantCreate);

	registerMethod(L, "Variant", "getNumber", LuaScriptInterface::luaVariantGetNumber);
	registerMethod(L, "Variant", "getString", LuaScriptInterface::luaVariantGetString);
	registerMethod(L, "Variant", "getPosition", LuaScriptInterface::luaVariantGetPosition);

	// Position
	registerClass(L, "Position", "", LuaScriptInterface::luaPositionCreate);

	registerMethod(L, "Position", "isSightClear", LuaScriptInterface::luaPositionIsSightClear);

	registerMethod(L, "Position", "sendMagicEffect", LuaScriptInterface::luaPositionSendMagicEffect);
	registerMethod(L, "Position", "sendDistanceEffect", LuaScriptInterface::luaPositionSendDistanceEffect);

	// Tile
	registerClass(L, "Tile", "", LuaScriptInterface::luaTileCreate);
	registerMetaMethod(L, "Tile", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Tile", "remove", LuaScriptInterface::luaTileRemove);

	registerMethod(L, "Tile", "getPosition", LuaScriptInterface::luaTileGetPosition);
	registerMethod(L, "Tile", "getGround", LuaScriptInterface::luaTileGetGround);
	registerMethod(L, "Tile", "getThing", LuaScriptInterface::luaTileGetThing);
	registerMethod(L, "Tile", "getThingCount", LuaScriptInterface::luaTileGetThingCount);
	registerMethod(L, "Tile", "getTopVisibleThing", LuaScriptInterface::luaTileGetTopVisibleThing);

	registerMethod(L, "Tile", "getTopTopItem", LuaScriptInterface::luaTileGetTopTopItem);
	registerMethod(L, "Tile", "getTopDownItem", LuaScriptInterface::luaTileGetTopDownItem);
	registerMethod(L, "Tile", "getFieldItem", LuaScriptInterface::luaTileGetFieldItem);

	registerMethod(L, "Tile", "getItemById", LuaScriptInterface::luaTileGetItemById);
	registerMethod(L, "Tile", "getItemByType", LuaScriptInterface::luaTileGetItemByType);
	registerMethod(L, "Tile", "getItemByTopOrder", LuaScriptInterface::luaTileGetItemByTopOrder);
	registerMethod(L, "Tile", "getItemCountById", LuaScriptInterface::luaTileGetItemCountById);

	registerMethod(L, "Tile", "getBottomCreature", LuaScriptInterface::luaTileGetBottomCreature);
	registerMethod(L, "Tile", "getTopCreature", LuaScriptInterface::luaTileGetTopCreature);
	registerMethod(L, "Tile", "getBottomVisibleCreature", LuaScriptInterface::luaTileGetBottomVisibleCreature);
	registerMethod(L, "Tile", "getTopVisibleCreature", LuaScriptInterface::luaTileGetTopVisibleCreature);

	registerMethod(L, "Tile", "getItems", LuaScriptInterface::luaTileGetItems);
	registerMethod(L, "Tile", "getItemCount", LuaScriptInterface::luaTileGetItemCount);
	registerMethod(L, "Tile", "getDownItemCount", LuaScriptInterface::luaTileGetDownItemCount);
	registerMethod(L, "Tile", "getTopItemCount", LuaScriptInterface::luaTileGetTopItemCount);

	registerMethod(L, "Tile", "getCreatures", LuaScriptInterface::luaTileGetCreatures);
	registerMethod(L, "Tile", "getCreatureCount", LuaScriptInterface::luaTileGetCreatureCount);

	registerMethod(L, "Tile", "getThingIndex", LuaScriptInterface::luaTileGetThingIndex);

	registerMethod(L, "Tile", "hasProperty", LuaScriptInterface::luaTileHasProperty);
	registerMethod(L, "Tile", "hasFlag", LuaScriptInterface::luaTileHasFlag);

	registerMethod(L, "Tile", "queryAdd", LuaScriptInterface::luaTileQueryAdd);
	registerMethod(L, "Tile", "addItem", LuaScriptInterface::luaTileAddItem);
	registerMethod(L, "Tile", "addItemEx", LuaScriptInterface::luaTileAddItemEx);

	registerMethod(L, "Tile", "getHouse", LuaScriptInterface::luaTileGetHouse);

	// NetworkMessage
	registerClass(L, "NetworkMessage", "", LuaScriptInterface::luaNetworkMessageCreate);
	registerMetaMethod(L, "NetworkMessage", "__eq", LuaScriptInterface::luaUserdataCompare);
	registerMetaMethod(L, "NetworkMessage", "__gc", LuaScriptInterface::luaNetworkMessageDelete);
	registerMethod(L, "NetworkMessage", "delete", LuaScriptInterface::luaNetworkMessageDelete);

	registerMethod(L, "NetworkMessage", "getByte", LuaScriptInterface::luaNetworkMessageGetByte);
	registerMethod(L, "NetworkMessage", "getU16", LuaScriptInterface::luaNetworkMessageGetU16);
	registerMethod(L, "NetworkMessage", "getU32", LuaScriptInterface::luaNetworkMessageGetU32);
	registerMethod(L, "NetworkMessage", "getU64", LuaScriptInterface::luaNetworkMessageGetU64);
	registerMethod(L, "NetworkMessage", "getString", LuaScriptInterface::luaNetworkMessageGetString);
	registerMethod(L, "NetworkMessage", "getPosition", LuaScriptInterface::luaNetworkMessageGetPosition);

	registerMethod(L, "NetworkMessage", "addByte", LuaScriptInterface::luaNetworkMessageAddByte);
	registerMethod(L, "NetworkMessage", "addU16", LuaScriptInterface::luaNetworkMessageAddU16);
	registerMethod(L, "NetworkMessage", "addU32", LuaScriptInterface::luaNetworkMessageAddU32);
	registerMethod(L, "NetworkMessage", "addU64", LuaScriptInterface::luaNetworkMessageAddU64);
	registerMethod(L, "NetworkMessage", "addString", LuaScriptInterface::luaNetworkMessageAddString);
	registerMethod(L, "NetworkMessage", "addPosition", LuaScriptInterface::luaNetworkMessageAddPosition);
	registerMethod(L, "NetworkMessage", "addDouble", LuaScriptInterface::luaNetworkMessageAddDouble);
	registerMethod(L, "NetworkMessage", "addItem", LuaScriptInterface::luaNetworkMessageAddItem);
	registerMethod(L, "NetworkMessage", "addItemId", LuaScriptInterface::luaNetworkMessageAddItemId);

	registerMethod(L, "NetworkMessage", "reset", LuaScriptInterface::luaNetworkMessageReset);
	registerMethod(L, "NetworkMessage", "seek", LuaScriptInterface::luaNetworkMessageSeek);
	registerMethod(L, "NetworkMessage", "tell", LuaScriptInterface::luaNetworkMessageTell);
	registerMethod(L, "NetworkMessage", "len", LuaScriptInterface::luaNetworkMessageLength);
	registerMethod(L, "NetworkMessage", "skipBytes", LuaScriptInterface::luaNetworkMessageSkipBytes);
	registerMethod(L, "NetworkMessage", "sendToPlayer", LuaScriptInterface::luaNetworkMessageSendToPlayer);

	// ModalWindow
	registerClass(L, "ModalWindow", "", LuaScriptInterface::luaModalWindowCreate);
	registerMetaMethod(L, "ModalWindow", "__eq", LuaScriptInterface::luaUserdataCompare);
	registerMetaMethod(L, "ModalWindow", "__gc", LuaScriptInterface::luaModalWindowDelete);
	registerMethod(L, "ModalWindow", "delete", LuaScriptInterface::luaModalWindowDelete);

	registerMethod(L, "ModalWindow", "getId", LuaScriptInterface::luaModalWindowGetId);
	registerMethod(L, "ModalWindow", "getTitle", LuaScriptInterface::luaModalWindowGetTitle);
	registerMethod(L, "ModalWindow", "getMessage", LuaScriptInterface::luaModalWindowGetMessage);

	registerMethod(L, "ModalWindow", "setTitle", LuaScriptInterface::luaModalWindowSetTitle);
	registerMethod(L, "ModalWindow", "setMessage", LuaScriptInterface::luaModalWindowSetMessage);

	registerMethod(L, "ModalWindow", "getButtonCount", LuaScriptInterface::luaModalWindowGetButtonCount);
	registerMethod(L, "ModalWindow", "getChoiceCount", LuaScriptInterface::luaModalWindowGetChoiceCount);

	registerMethod(L, "ModalWindow", "addButton", LuaScriptInterface::luaModalWindowAddButton);
	registerMethod(L, "ModalWindow", "addChoice", LuaScriptInterface::luaModalWindowAddChoice);

	registerMethod(L, "ModalWindow", "getDefaultEnterButton", LuaScriptInterface::luaModalWindowGetDefaultEnterButton);
	registerMethod(L, "ModalWindow", "setDefaultEnterButton", LuaScriptInterface::luaModalWindowSetDefaultEnterButton);

	registerMethod(L, "ModalWindow", "getDefaultEscapeButton",
	               LuaScriptInterface::luaModalWindowGetDefaultEscapeButton);
	registerMethod(L, "ModalWindow", "setDefaultEscapeButton",
	               LuaScriptInterface::luaModalWindowSetDefaultEscapeButton);

	registerMethod(L, "ModalWindow", "hasPriority", LuaScriptInterface::luaModalWindowHasPriority);
	registerMethod(L, "ModalWindow", "setPriority", LuaScriptInterface::luaModalWindowSetPriority);

	registerMethod(L, "ModalWindow", "sendToPlayer", LuaScriptInterface::luaModalWindowSendToPlayer);

	// Item
	registerClass(L, "Item", "", LuaScriptInterface::luaItemCreate);
	registerMetaMethod(L, "Item", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Item", "isItem", LuaScriptInterface::luaItemIsItem);

	registerMethod(L, "Item", "hasParent", LuaScriptInterface::luaItemHasParent);
	registerMethod(L, "Item", "getParent", LuaScriptInterface::luaItemGetParent);
	registerMethod(L, "Item", "getTopParent", LuaScriptInterface::luaItemGetTopParent);

	registerMethod(L, "Item", "getId", LuaScriptInterface::luaItemGetId);

	registerMethod(L, "Item", "clone", LuaScriptInterface::luaItemClone);
	registerMethod(L, "Item", "split", LuaScriptInterface::luaItemSplit);
	registerMethod(L, "Item", "remove", LuaScriptInterface::luaItemRemove);

	registerMethod(L, "Item", "getUniqueId", LuaScriptInterface::luaItemGetUniqueId);
	registerMethod(L, "Item", "getActionId", LuaScriptInterface::luaItemGetActionId);
	registerMethod(L, "Item", "setActionId", LuaScriptInterface::luaItemSetActionId);

	registerMethod(L, "Item", "getCount", LuaScriptInterface::luaItemGetCount);
	registerMethod(L, "Item", "getCharges", LuaScriptInterface::luaItemGetCharges);
	registerMethod(L, "Item", "getFluidType", LuaScriptInterface::luaItemGetFluidType);
	registerMethod(L, "Item", "getWeight", LuaScriptInterface::luaItemGetWeight);
	registerMethod(L, "Item", "getWorth", LuaScriptInterface::luaItemGetWorth);

	registerMethod(L, "Item", "getSubType", LuaScriptInterface::luaItemGetSubType);

	registerMethod(L, "Item", "getName", LuaScriptInterface::luaItemGetName);
	registerMethod(L, "Item", "getPluralName", LuaScriptInterface::luaItemGetPluralName);
	registerMethod(L, "Item", "getArticle", LuaScriptInterface::luaItemGetArticle);

	registerMethod(L, "Item", "getPosition", LuaScriptInterface::luaItemGetPosition);
	registerMethod(L, "Item", "getTile", LuaScriptInterface::luaItemGetTile);

	registerMethod(L, "Item", "hasAttribute", LuaScriptInterface::luaItemHasAttribute);
	registerMethod(L, "Item", "getAttribute", LuaScriptInterface::luaItemGetAttribute);
	registerMethod(L, "Item", "setAttribute", LuaScriptInterface::luaItemSetAttribute);
	registerMethod(L, "Item", "removeAttribute", LuaScriptInterface::luaItemRemoveAttribute);
	registerMethod(L, "Item", "getCustomAttribute", LuaScriptInterface::luaItemGetCustomAttribute);
	registerMethod(L, "Item", "setCustomAttribute", LuaScriptInterface::luaItemSetCustomAttribute);
	registerMethod(L, "Item", "removeCustomAttribute", LuaScriptInterface::luaItemRemoveCustomAttribute);

	registerMethod(L, "Item", "moveTo", LuaScriptInterface::luaItemMoveTo);
	registerMethod(L, "Item", "transform", LuaScriptInterface::luaItemTransform);
	registerMethod(L, "Item", "decay", LuaScriptInterface::luaItemDecay);

	registerMethod(L, "Item", "getSpecialDescription", LuaScriptInterface::luaItemGetSpecialDescription);

	registerMethod(L, "Item", "hasProperty", LuaScriptInterface::luaItemHasProperty);
	registerMethod(L, "Item", "isLoadedFromMap", LuaScriptInterface::luaItemIsLoadedFromMap);

	registerMethod(L, "Item", "setStoreItem", LuaScriptInterface::luaItemSetStoreItem);
	registerMethod(L, "Item", "isStoreItem", LuaScriptInterface::luaItemIsStoreItem);

	registerMethod(L, "Item", "setReflect", LuaScriptInterface::luaItemSetReflect);
	registerMethod(L, "Item", "getReflect", LuaScriptInterface::luaItemGetReflect);

	registerMethod(L, "Item", "setBoostPercent", LuaScriptInterface::luaItemSetBoostPercent);
	registerMethod(L, "Item", "getBoostPercent", LuaScriptInterface::luaItemGetBoostPercent);

	// Container
	registerClass(L, "Container", "Item", LuaScriptInterface::luaContainerCreate);
	registerMetaMethod(L, "Container", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Container", "getSize", LuaScriptInterface::luaContainerGetSize);
	registerMethod(L, "Container", "getCapacity", LuaScriptInterface::luaContainerGetCapacity);
	registerMethod(L, "Container", "getEmptySlots", LuaScriptInterface::luaContainerGetEmptySlots);
	registerMethod(L, "Container", "getItems", LuaScriptInterface::luaContainerGetItems);
	registerMethod(L, "Container", "getItemHoldingCount", LuaScriptInterface::luaContainerGetItemHoldingCount);
	registerMethod(L, "Container", "getItemCountById", LuaScriptInterface::luaContainerGetItemCountById);

	registerMethod(L, "Container", "getItem", LuaScriptInterface::luaContainerGetItem);
	registerMethod(L, "Container", "hasItem", LuaScriptInterface::luaContainerHasItem);
	registerMethod(L, "Container", "addItem", LuaScriptInterface::luaContainerAddItem);
	registerMethod(L, "Container", "addItemEx", LuaScriptInterface::luaContainerAddItemEx);
	registerMethod(L, "Container", "getCorpseOwner", LuaScriptInterface::luaContainerGetCorpseOwner);

	// Teleport
	registerClass(L, "Teleport", "Item", LuaScriptInterface::luaTeleportCreate);
	registerMetaMethod(L, "Teleport", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Teleport", "getDestination", LuaScriptInterface::luaTeleportGetDestination);
	registerMethod(L, "Teleport", "setDestination", LuaScriptInterface::luaTeleportSetDestination);

	// Podium
	registerClass(L, "Podium", "Item", LuaScriptInterface::luaPodiumCreate);
	registerMetaMethod(L, "Podium", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Podium", "getOutfit", LuaScriptInterface::luaPodiumGetOutfit);
	registerMethod(L, "Podium", "setOutfit", LuaScriptInterface::luaPodiumSetOutfit);
	registerMethod(L, "Podium", "hasFlag", LuaScriptInterface::luaPodiumHasFlag);
	registerMethod(L, "Podium", "setFlag", LuaScriptInterface::luaPodiumSetFlag);
	registerMethod(L, "Podium", "getDirection", LuaScriptInterface::luaPodiumGetDirection);
	registerMethod(L, "Podium", "setDirection", LuaScriptInterface::luaPodiumSetDirection);

	// Creature
	registerClass(L, "Creature", "", LuaScriptInterface::luaCreatureCreate);
	registerMetaMethod(L, "Creature", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Creature", "getEvents", LuaScriptInterface::luaCreatureGetEvents);
	registerMethod(L, "Creature", "registerEvent", LuaScriptInterface::luaCreatureRegisterEvent);
	registerMethod(L, "Creature", "unregisterEvent", LuaScriptInterface::luaCreatureUnregisterEvent);

	registerMethod(L, "Creature", "isRemoved", LuaScriptInterface::luaCreatureIsRemoved);
	registerMethod(L, "Creature", "isCreature", LuaScriptInterface::luaCreatureIsCreature);
	registerMethod(L, "Creature", "isInGhostMode", LuaScriptInterface::luaCreatureIsInGhostMode);
	registerMethod(L, "Creature", "isHealthHidden", LuaScriptInterface::luaCreatureIsHealthHidden);
	registerMethod(L, "Creature", "isMovementBlocked", LuaScriptInterface::luaCreatureIsMovementBlocked);
	registerMethod(L, "Creature", "isImmune", LuaScriptInterface::luaCreatureIsImmune);

	registerMethod(L, "Creature", "canSee", LuaScriptInterface::luaCreatureCanSee);
	registerMethod(L, "Creature", "canSeeCreature", LuaScriptInterface::luaCreatureCanSeeCreature);
	registerMethod(L, "Creature", "canSeeGhostMode", LuaScriptInterface::luaCreatureCanSeeGhostMode);
	registerMethod(L, "Creature", "canSeeInvisibility", LuaScriptInterface::luaCreatureCanSeeInvisibility);

	registerMethod(L, "Creature", "hasParent", LuaScriptInterface::luaCreatureHasParent);
	registerMethod(L, "Creature", "getParent", LuaScriptInterface::luaCreatureGetParent);

	registerMethod(L, "Creature", "getId", LuaScriptInterface::luaCreatureGetId);
	registerMethod(L, "Creature", "getName", LuaScriptInterface::luaCreatureGetName);

	registerMethod(L, "Creature", "getTarget", LuaScriptInterface::luaCreatureGetTarget);
	registerMethod(L, "Creature", "setTarget", LuaScriptInterface::luaCreatureSetTarget);

	registerMethod(L, "Creature", "getFollowCreature", LuaScriptInterface::luaCreatureGetFollowCreature);
	registerMethod(L, "Creature", "setFollowCreature", LuaScriptInterface::luaCreatureSetFollowCreature);

	registerMethod(L, "Creature", "getMaster", LuaScriptInterface::luaCreatureGetMaster);
	registerMethod(L, "Creature", "setMaster", LuaScriptInterface::luaCreatureSetMaster);

	registerMethod(L, "Creature", "getLight", LuaScriptInterface::luaCreatureGetLight);
	registerMethod(L, "Creature", "setLight", LuaScriptInterface::luaCreatureSetLight);

	registerMethod(L, "Creature", "getSpeed", LuaScriptInterface::luaCreatureGetSpeed);
	registerMethod(L, "Creature", "getBaseSpeed", LuaScriptInterface::luaCreatureGetBaseSpeed);
	registerMethod(L, "Creature", "changeSpeed", LuaScriptInterface::luaCreatureChangeSpeed);

	registerMethod(L, "Creature", "setDropLoot", LuaScriptInterface::luaCreatureSetDropLoot);
	registerMethod(L, "Creature", "setSkillLoss", LuaScriptInterface::luaCreatureSetSkillLoss);

	registerMethod(L, "Creature", "getPosition", LuaScriptInterface::luaCreatureGetPosition);
	registerMethod(L, "Creature", "getTile", LuaScriptInterface::luaCreatureGetTile);
	registerMethod(L, "Creature", "getDirection", LuaScriptInterface::luaCreatureGetDirection);
	registerMethod(L, "Creature", "setDirection", LuaScriptInterface::luaCreatureSetDirection);

	registerMethod(L, "Creature", "getHealth", LuaScriptInterface::luaCreatureGetHealth);
	registerMethod(L, "Creature", "setHealth", LuaScriptInterface::luaCreatureSetHealth);
	registerMethod(L, "Creature", "addHealth", LuaScriptInterface::luaCreatureAddHealth);
	registerMethod(L, "Creature", "getMaxHealth", LuaScriptInterface::luaCreatureGetMaxHealth);
	registerMethod(L, "Creature", "setMaxHealth", LuaScriptInterface::luaCreatureSetMaxHealth);
	registerMethod(L, "Creature", "setHiddenHealth", LuaScriptInterface::luaCreatureSetHiddenHealth);
	registerMethod(L, "Creature", "setMovementBlocked", LuaScriptInterface::luaCreatureSetMovementBlocked);

	registerMethod(L, "Creature", "getSkull", LuaScriptInterface::luaCreatureGetSkull);
	registerMethod(L, "Creature", "setSkull", LuaScriptInterface::luaCreatureSetSkull);

	registerMethod(L, "Creature", "getOutfit", LuaScriptInterface::luaCreatureGetOutfit);
	registerMethod(L, "Creature", "setOutfit", LuaScriptInterface::luaCreatureSetOutfit);

	registerMethod(L, "Creature", "getCondition", LuaScriptInterface::luaCreatureGetCondition);
	registerMethod(L, "Creature", "addCondition", LuaScriptInterface::luaCreatureAddCondition);
	registerMethod(L, "Creature", "removeCondition", LuaScriptInterface::luaCreatureRemoveCondition);
	registerMethod(L, "Creature", "hasCondition", LuaScriptInterface::luaCreatureHasCondition);

	registerMethod(L, "Creature", "remove", LuaScriptInterface::luaCreatureRemove);
	registerMethod(L, "Creature", "teleportTo", LuaScriptInterface::luaCreatureTeleportTo);
	registerMethod(L, "Creature", "say", LuaScriptInterface::luaCreatureSay);

	registerMethod(L, "Creature", "getDamageMap", LuaScriptInterface::luaCreatureGetDamageMap);

	registerMethod(L, "Creature", "getSummons", LuaScriptInterface::luaCreatureGetSummons);

	registerMethod(L, "Creature", "getDescription", LuaScriptInterface::luaCreatureGetDescription);

	registerMethod(L, "Creature", "getPathTo", LuaScriptInterface::luaCreatureGetPathTo);
	registerMethod(L, "Creature", "move", LuaScriptInterface::luaCreatureMove);

	registerMethod(L, "Creature", "getZone", LuaScriptInterface::luaCreatureGetZone);

	registerMethod(L, "Creature", "hasIcon", LuaScriptInterface::luaCreatureHasIcon);
	registerMethod(L, "Creature", "setIcon", LuaScriptInterface::luaCreatureSetIcon);
	registerMethod(L, "Creature", "getIcon", LuaScriptInterface::luaCreatureGetIcon);
	registerMethod(L, "Creature", "removeIcon", LuaScriptInterface::luaCreatureRemoveIcon);

	registerMethod(L, "Creature", "getStorageValue", LuaScriptInterface::luaCreatureGetStorageValue);
	registerMethod(L, "Creature", "setStorageValue", LuaScriptInterface::luaCreatureSetStorageValue);

	// Player
	registerClass(L, "Player", "Creature", LuaScriptInterface::luaPlayerCreate);
	registerMetaMethod(L, "Player", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Player", "isPlayer", LuaScriptInterface::luaPlayerIsPlayer);

	registerMethod(L, "Player", "getGuid", LuaScriptInterface::luaPlayerGetGuid);
	registerMethod(L, "Player", "getIp", LuaScriptInterface::luaPlayerGetIp);
	registerMethod(L, "Player", "getAccountId", LuaScriptInterface::luaPlayerGetAccountId);
	registerMethod(L, "Player", "getLastLoginSaved", LuaScriptInterface::luaPlayerGetLastLoginSaved);
	registerMethod(L, "Player", "getLastLogout", LuaScriptInterface::luaPlayerGetLastLogout);

	registerMethod(L, "Player", "getAccountType", LuaScriptInterface::luaPlayerGetAccountType);
	registerMethod(L, "Player", "setAccountType", LuaScriptInterface::luaPlayerSetAccountType);

	registerMethod(L, "Player", "getCapacity", LuaScriptInterface::luaPlayerGetCapacity);
	registerMethod(L, "Player", "setCapacity", LuaScriptInterface::luaPlayerSetCapacity);

	registerMethod(L, "Player", "getFreeCapacity", LuaScriptInterface::luaPlayerGetFreeCapacity);

	registerMethod(L, "Player", "getDepotChest", LuaScriptInterface::luaPlayerGetDepotChest);
	registerMethod(L, "Player", "getInbox", LuaScriptInterface::luaPlayerGetInbox);

	registerMethod(L, "Player", "getSkullTime", LuaScriptInterface::luaPlayerGetSkullTime);
	registerMethod(L, "Player", "setSkullTime", LuaScriptInterface::luaPlayerSetSkullTime);
	registerMethod(L, "Player", "getDeathPenalty", LuaScriptInterface::luaPlayerGetDeathPenalty);

	registerMethod(L, "Player", "getExperience", LuaScriptInterface::luaPlayerGetExperience);
	registerMethod(L, "Player", "addExperience", LuaScriptInterface::luaPlayerAddExperience);
	registerMethod(L, "Player", "removeExperience", LuaScriptInterface::luaPlayerRemoveExperience);
	registerMethod(L, "Player", "getLevel", LuaScriptInterface::luaPlayerGetLevel);
	registerMethod(L, "Player", "getLevelPercent", LuaScriptInterface::luaPlayerGetLevelPercent);

	registerMethod(L, "Player", "getMagicLevel", LuaScriptInterface::luaPlayerGetMagicLevel);
	registerMethod(L, "Player", "getMagicLevelPercent", LuaScriptInterface::luaPlayerGetMagicLevelPercent);
	registerMethod(L, "Player", "getBaseMagicLevel", LuaScriptInterface::luaPlayerGetBaseMagicLevel);
	registerMethod(L, "Player", "getMana", LuaScriptInterface::luaPlayerGetMana);
	registerMethod(L, "Player", "addMana", LuaScriptInterface::luaPlayerAddMana);
	registerMethod(L, "Player", "getMaxMana", LuaScriptInterface::luaPlayerGetMaxMana);
	registerMethod(L, "Player", "setMaxMana", LuaScriptInterface::luaPlayerSetMaxMana);
	registerMethod(L, "Player", "setManaShieldBar", LuaScriptInterface::luaPlayerSetManaShieldBar);
	registerMethod(L, "Player", "getManaSpent", LuaScriptInterface::luaPlayerGetManaSpent);
	registerMethod(L, "Player", "addManaSpent", LuaScriptInterface::luaPlayerAddManaSpent);
	registerMethod(L, "Player", "removeManaSpent", LuaScriptInterface::luaPlayerRemoveManaSpent);

	registerMethod(L, "Player", "getBaseMaxHealth", LuaScriptInterface::luaPlayerGetBaseMaxHealth);
	registerMethod(L, "Player", "getBaseMaxMana", LuaScriptInterface::luaPlayerGetBaseMaxMana);

	registerMethod(L, "Player", "getSkillLevel", LuaScriptInterface::luaPlayerGetSkillLevel);
	registerMethod(L, "Player", "getEffectiveSkillLevel", LuaScriptInterface::luaPlayerGetEffectiveSkillLevel);
	registerMethod(L, "Player", "getSkillPercent", LuaScriptInterface::luaPlayerGetSkillPercent);
	registerMethod(L, "Player", "getSkillTries", LuaScriptInterface::luaPlayerGetSkillTries);
	registerMethod(L, "Player", "addSkillTries", LuaScriptInterface::luaPlayerAddSkillTries);
	registerMethod(L, "Player", "removeSkillTries", LuaScriptInterface::luaPlayerRemoveSkillTries);
	registerMethod(L, "Player", "getSpecialSkill", LuaScriptInterface::luaPlayerGetSpecialSkill);
	registerMethod(L, "Player", "addSpecialSkill", LuaScriptInterface::luaPlayerAddSpecialSkill);

	registerMethod(L, "Player", "addOfflineTrainingTime", LuaScriptInterface::luaPlayerAddOfflineTrainingTime);
	registerMethod(L, "Player", "getOfflineTrainingTime", LuaScriptInterface::luaPlayerGetOfflineTrainingTime);
	registerMethod(L, "Player", "removeOfflineTrainingTime", LuaScriptInterface::luaPlayerRemoveOfflineTrainingTime);

	registerMethod(L, "Player", "addOfflineTrainingTries", LuaScriptInterface::luaPlayerAddOfflineTrainingTries);

	registerMethod(L, "Player", "getOfflineTrainingSkill", LuaScriptInterface::luaPlayerGetOfflineTrainingSkill);
	registerMethod(L, "Player", "setOfflineTrainingSkill", LuaScriptInterface::luaPlayerSetOfflineTrainingSkill);

	registerMethod(L, "Player", "getItemCount", LuaScriptInterface::luaPlayerGetItemCount);
	registerMethod(L, "Player", "getItemById", LuaScriptInterface::luaPlayerGetItemById);

	registerMethod(L, "Player", "getVocation", LuaScriptInterface::luaPlayerGetVocation);
	registerMethod(L, "Player", "setVocation", LuaScriptInterface::luaPlayerSetVocation);

	registerMethod(L, "Player", "getSex", LuaScriptInterface::luaPlayerGetSex);
	registerMethod(L, "Player", "setSex", LuaScriptInterface::luaPlayerSetSex);

	registerMethod(L, "Player", "getTown", LuaScriptInterface::luaPlayerGetTown);
	registerMethod(L, "Player", "setTown", LuaScriptInterface::luaPlayerSetTown);

	registerMethod(L, "Player", "getGuild", LuaScriptInterface::luaPlayerGetGuild);
	registerMethod(L, "Player", "setGuild", LuaScriptInterface::luaPlayerSetGuild);

	registerMethod(L, "Player", "getGuildLevel", LuaScriptInterface::luaPlayerGetGuildLevel);
	registerMethod(L, "Player", "setGuildLevel", LuaScriptInterface::luaPlayerSetGuildLevel);

	registerMethod(L, "Player", "getGuildNick", LuaScriptInterface::luaPlayerGetGuildNick);
	registerMethod(L, "Player", "setGuildNick", LuaScriptInterface::luaPlayerSetGuildNick);

	registerMethod(L, "Player", "getGroup", LuaScriptInterface::luaPlayerGetGroup);
	registerMethod(L, "Player", "setGroup", LuaScriptInterface::luaPlayerSetGroup);

	registerMethod(L, "Player", "getStamina", LuaScriptInterface::luaPlayerGetStamina);
	registerMethod(L, "Player", "setStamina", LuaScriptInterface::luaPlayerSetStamina);

	registerMethod(L, "Player", "getSoul", LuaScriptInterface::luaPlayerGetSoul);
	registerMethod(L, "Player", "addSoul", LuaScriptInterface::luaPlayerAddSoul);
	registerMethod(L, "Player", "getMaxSoul", LuaScriptInterface::luaPlayerGetMaxSoul);

	registerMethod(L, "Player", "getBankBalance", LuaScriptInterface::luaPlayerGetBankBalance);
	registerMethod(L, "Player", "setBankBalance", LuaScriptInterface::luaPlayerSetBankBalance);

	registerMethod(L, "Player", "addItem", LuaScriptInterface::luaPlayerAddItem);
	registerMethod(L, "Player", "addItemEx", LuaScriptInterface::luaPlayerAddItemEx);
	registerMethod(L, "Player", "removeItem", LuaScriptInterface::luaPlayerRemoveItem);
	registerMethod(L, "Player", "sendSupplyUsed", LuaScriptInterface::luaPlayerSendSupplyUsed);

	registerMethod(L, "Player", "getMoney", LuaScriptInterface::luaPlayerGetMoney);
	registerMethod(L, "Player", "addMoney", LuaScriptInterface::luaPlayerAddMoney);
	registerMethod(L, "Player", "removeMoney", LuaScriptInterface::luaPlayerRemoveMoney);

	registerMethod(L, "Player", "showTextDialog", LuaScriptInterface::luaPlayerShowTextDialog);

	registerMethod(L, "Player", "sendTextMessage", LuaScriptInterface::luaPlayerSendTextMessage);
	registerMethod(L, "Player", "sendChannelMessage", LuaScriptInterface::luaPlayerSendChannelMessage);
	registerMethod(L, "Player", "sendPrivateMessage", LuaScriptInterface::luaPlayerSendPrivateMessage);
	registerMethod(L, "Player", "channelSay", LuaScriptInterface::luaPlayerChannelSay);
	registerMethod(L, "Player", "openChannel", LuaScriptInterface::luaPlayerOpenChannel);
	registerMethod(L, "Player", "leaveChannel", LuaScriptInterface::luaPlayerLeaveChannel);

	registerMethod(L, "Player", "getSlotItem", LuaScriptInterface::luaPlayerGetSlotItem);

	registerMethod(L, "Player", "getParty", LuaScriptInterface::luaPlayerGetParty);

	registerMethod(L, "Player", "addOutfit", LuaScriptInterface::luaPlayerAddOutfit);
	registerMethod(L, "Player", "addOutfitAddon", LuaScriptInterface::luaPlayerAddOutfitAddon);
	registerMethod(L, "Player", "removeOutfit", LuaScriptInterface::luaPlayerRemoveOutfit);
	registerMethod(L, "Player", "removeOutfitAddon", LuaScriptInterface::luaPlayerRemoveOutfitAddon);
	registerMethod(L, "Player", "hasOutfit", LuaScriptInterface::luaPlayerHasOutfit);
	registerMethod(L, "Player", "canWearOutfit", LuaScriptInterface::luaPlayerCanWearOutfit);
	registerMethod(L, "Player", "sendOutfitWindow", LuaScriptInterface::luaPlayerSendOutfitWindow);

	registerMethod(L, "Player", "sendEditPodium", LuaScriptInterface::luaPlayerSendEditPodium);

	registerMethod(L, "Player", "addMount", LuaScriptInterface::luaPlayerAddMount);
	registerMethod(L, "Player", "removeMount", LuaScriptInterface::luaPlayerRemoveMount);
	registerMethod(L, "Player", "hasMount", LuaScriptInterface::luaPlayerHasMount);
	registerMethod(L, "Player", "toggleMount", LuaScriptInterface::luaPlayerToggleMount);

	registerMethod(L, "Player", "getPremiumEndsAt", LuaScriptInterface::luaPlayerGetPremiumEndsAt);
	registerMethod(L, "Player", "setPremiumEndsAt", LuaScriptInterface::luaPlayerSetPremiumEndsAt);

	registerMethod(L, "Player", "hasBlessing", LuaScriptInterface::luaPlayerHasBlessing);
	registerMethod(L, "Player", "addBlessing", LuaScriptInterface::luaPlayerAddBlessing);
	registerMethod(L, "Player", "removeBlessing", LuaScriptInterface::luaPlayerRemoveBlessing);

	registerMethod(L, "Player", "canLearnSpell", LuaScriptInterface::luaPlayerCanLearnSpell);
	registerMethod(L, "Player", "learnSpell", LuaScriptInterface::luaPlayerLearnSpell);
	registerMethod(L, "Player", "forgetSpell", LuaScriptInterface::luaPlayerForgetSpell);
	registerMethod(L, "Player", "hasLearnedSpell", LuaScriptInterface::luaPlayerHasLearnedSpell);

	registerMethod(L, "Player", "sendTutorial", LuaScriptInterface::luaPlayerSendTutorial);
	registerMethod(L, "Player", "addMapMark", LuaScriptInterface::luaPlayerAddMapMark);

	registerMethod(L, "Player", "save", LuaScriptInterface::luaPlayerSave);
	registerMethod(L, "Player", "popupFYI", LuaScriptInterface::luaPlayerPopupFYI);

	registerMethod(L, "Player", "isPzLocked", LuaScriptInterface::luaPlayerIsPzLocked);

	registerMethod(L, "Player", "getClient", LuaScriptInterface::luaPlayerGetClient);

	registerMethod(L, "Player", "getHouse", LuaScriptInterface::luaPlayerGetHouse);
	registerMethod(L, "Player", "sendHouseWindow", LuaScriptInterface::luaPlayerSendHouseWindow);
	registerMethod(L, "Player", "setEditHouse", LuaScriptInterface::luaPlayerSetEditHouse);

	registerMethod(L, "Player", "setGhostMode", LuaScriptInterface::luaPlayerSetGhostMode);

	registerMethod(L, "Player", "getContainerId", LuaScriptInterface::luaPlayerGetContainerId);
	registerMethod(L, "Player", "getContainerById", LuaScriptInterface::luaPlayerGetContainerById);
	registerMethod(L, "Player", "getContainerIndex", LuaScriptInterface::luaPlayerGetContainerIndex);

	registerMethod(L, "Player", "getRuneSpells", LuaScriptInterface::luaPlayerGetRuneSpells);
	registerMethod(L, "Player", "getInstantSpells", LuaScriptInterface::luaPlayerGetInstantSpells);
	registerMethod(L, "Player", "canCast", LuaScriptInterface::luaPlayerCanCast);

	registerMethod(L, "Player", "hasChaseMode", LuaScriptInterface::luaPlayerHasChaseMode);
	registerMethod(L, "Player", "hasSecureMode", LuaScriptInterface::luaPlayerHasSecureMode);
	registerMethod(L, "Player", "getFightMode", LuaScriptInterface::luaPlayerGetFightMode);

	registerMethod(L, "Player", "getStoreInbox", LuaScriptInterface::luaPlayerGetStoreInbox);

	registerMethod(L, "Player", "isNearDepotBox", LuaScriptInterface::luaPlayerIsNearDepotBox);

	registerMethod(L, "Player", "getIdleTime", LuaScriptInterface::luaPlayerGetIdleTime);
	registerMethod(L, "Player", "resetIdleTime", LuaScriptInterface::luaPlayerResetIdleTime);

	registerMethod(L, "Player", "sendCreatureSquare", LuaScriptInterface::luaPlayerSendCreatureSquare);

	registerMethod(L, "Player", "getClientExpDisplay", LuaScriptInterface::luaPlayerGetClientExpDisplay);
	registerMethod(L, "Player", "setClientExpDisplay", LuaScriptInterface::luaPlayerSetClientExpDisplay);

	registerMethod(L, "Player", "getClientStaminaBonusDisplay",
	               LuaScriptInterface::luaPlayerGetClientStaminaBonusDisplay);
	registerMethod(L, "Player", "setClientStaminaBonusDisplay",
	               LuaScriptInterface::luaPlayerSetClientStaminaBonusDisplay);

	registerMethod(L, "Player", "getClientLowLevelBonusDisplay",
	               LuaScriptInterface::luaPlayerGetClientLowLevelBonusDisplay);
	registerMethod(L, "Player", "setClientLowLevelBonusDisplay",
	               LuaScriptInterface::luaPlayerSetClientLowLevelBonusDisplay);

	registerMethod(L, "Player", "sendResourceBalance", LuaScriptInterface::luaPlayerSendResourceBalance);

	registerMethod(L, "Player", "sendEnterMarket", LuaScriptInterface::luaPlayerSendEnterMarket);

	// Monster
	registerClass(L, "Monster", "Creature", LuaScriptInterface::luaMonsterCreate);
	registerMetaMethod(L, "Monster", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Monster", "isMonster", LuaScriptInterface::luaMonsterIsMonster);

	registerMethod(L, "Monster", "getId", LuaScriptInterface::luaMonsterGetId);
	registerMethod(L, "Monster", "getType", LuaScriptInterface::luaMonsterGetType);

	registerMethod(L, "Monster", "rename", LuaScriptInterface::luaMonsterRename);

	registerMethod(L, "Monster", "getSpawnPosition", LuaScriptInterface::luaMonsterGetSpawnPosition);
	registerMethod(L, "Monster", "isInSpawnRange", LuaScriptInterface::luaMonsterIsInSpawnRange);

	registerMethod(L, "Monster", "isIdle", LuaScriptInterface::luaMonsterIsIdle);
	registerMethod(L, "Monster", "setIdle", LuaScriptInterface::luaMonsterSetIdle);

	registerMethod(L, "Monster", "isTarget", LuaScriptInterface::luaMonsterIsTarget);
	registerMethod(L, "Monster", "isOpponent", LuaScriptInterface::luaMonsterIsOpponent);
	registerMethod(L, "Monster", "isFriend", LuaScriptInterface::luaMonsterIsFriend);

	registerMethod(L, "Monster", "addFriend", LuaScriptInterface::luaMonsterAddFriend);
	registerMethod(L, "Monster", "removeFriend", LuaScriptInterface::luaMonsterRemoveFriend);
	registerMethod(L, "Monster", "getFriendList", LuaScriptInterface::luaMonsterGetFriendList);
	registerMethod(L, "Monster", "getFriendCount", LuaScriptInterface::luaMonsterGetFriendCount);

	registerMethod(L, "Monster", "addTarget", LuaScriptInterface::luaMonsterAddTarget);
	registerMethod(L, "Monster", "removeTarget", LuaScriptInterface::luaMonsterRemoveTarget);
	registerMethod(L, "Monster", "getTargetList", LuaScriptInterface::luaMonsterGetTargetList);
	registerMethod(L, "Monster", "getTargetCount", LuaScriptInterface::luaMonsterGetTargetCount);

	registerMethod(L, "Monster", "selectTarget", LuaScriptInterface::luaMonsterSelectTarget);
	registerMethod(L, "Monster", "searchTarget", LuaScriptInterface::luaMonsterSearchTarget);

	registerMethod(L, "Monster", "isWalkingToSpawn", LuaScriptInterface::luaMonsterIsWalkingToSpawn);
	registerMethod(L, "Monster", "walkToSpawn", LuaScriptInterface::luaMonsterWalkToSpawn);

	registerMethod(L, "Monster", "hasSpecialIcon", LuaScriptInterface::luaMonsterHasIcon);
	registerMethod(L, "Monster", "setSpecialIcon", LuaScriptInterface::luaMonsterSetIcon);
	registerMethod(L, "Monster", "getSpecialIcon", LuaScriptInterface::luaMonsterGetIcon);
	registerMethod(L, "Monster", "removeSpecialIcon", LuaScriptInterface::luaMonsterRemoveIcon);

	// Npc
	registerClass(L, "Npc", "Creature", LuaScriptInterface::luaNpcCreate);
	registerMetaMethod(L, "Npc", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Npc", "isNpc", LuaScriptInterface::luaNpcIsNpc);

	registerMethod(L, "Npc", "setMasterPos", LuaScriptInterface::luaNpcSetMasterPos);

	registerMethod(L, "Npc", "getSpeechBubble", LuaScriptInterface::luaNpcGetSpeechBubble);
	registerMethod(L, "Npc", "setSpeechBubble", LuaScriptInterface::luaNpcSetSpeechBubble);

	registerMethod(L, "Npc", "getSpectators", LuaScriptInterface::luaNpcGetSpectators);

	// Guild
	registerClass(L, "Guild", "", LuaScriptInterface::luaGuildCreate);
	registerMetaMethod(L, "Guild", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Guild", "getId", LuaScriptInterface::luaGuildGetId);
	registerMethod(L, "Guild", "getName", LuaScriptInterface::luaGuildGetName);
	registerMethod(L, "Guild", "getMembersOnline", LuaScriptInterface::luaGuildGetMembersOnline);

	registerMethod(L, "Guild", "addRank", LuaScriptInterface::luaGuildAddRank);
	registerMethod(L, "Guild", "getRankById", LuaScriptInterface::luaGuildGetRankById);
	registerMethod(L, "Guild", "getRankByLevel", LuaScriptInterface::luaGuildGetRankByLevel);

	registerMethod(L, "Guild", "getMotd", LuaScriptInterface::luaGuildGetMotd);
	registerMethod(L, "Guild", "setMotd", LuaScriptInterface::luaGuildSetMotd);

	// Group
	registerClass(L, "Group", "", LuaScriptInterface::luaGroupCreate);
	registerMetaMethod(L, "Group", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Group", "getId", LuaScriptInterface::luaGroupGetId);
	registerMethod(L, "Group", "getName", LuaScriptInterface::luaGroupGetName);
	registerMethod(L, "Group", "getFlags", LuaScriptInterface::luaGroupGetFlags);
	registerMethod(L, "Group", "getAccess", LuaScriptInterface::luaGroupGetAccess);
	registerMethod(L, "Group", "getMaxDepotItems", LuaScriptInterface::luaGroupGetMaxDepotItems);
	registerMethod(L, "Group", "getMaxVipEntries", LuaScriptInterface::luaGroupGetMaxVipEntries);
	registerMethod(L, "Group", "hasFlag", LuaScriptInterface::luaGroupHasFlag);

	// Vocation
	registerClass(L, "Vocation", "", LuaScriptInterface::luaVocationCreate);
	registerMetaMethod(L, "Vocation", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Vocation", "getId", LuaScriptInterface::luaVocationGetId);
	registerMethod(L, "Vocation", "getClientId", LuaScriptInterface::luaVocationGetClientId);
	registerMethod(L, "Vocation", "getName", LuaScriptInterface::luaVocationGetName);
	registerMethod(L, "Vocation", "getDescription", LuaScriptInterface::luaVocationGetDescription);

	registerMethod(L, "Vocation", "getRequiredSkillTries", LuaScriptInterface::luaVocationGetRequiredSkillTries);
	registerMethod(L, "Vocation", "getRequiredManaSpent", LuaScriptInterface::luaVocationGetRequiredManaSpent);

	registerMethod(L, "Vocation", "getCapacityGain", LuaScriptInterface::luaVocationGetCapacityGain);

	registerMethod(L, "Vocation", "getHealthGain", LuaScriptInterface::luaVocationGetHealthGain);
	registerMethod(L, "Vocation", "getHealthGainTicks", LuaScriptInterface::luaVocationGetHealthGainTicks);
	registerMethod(L, "Vocation", "getHealthGainAmount", LuaScriptInterface::luaVocationGetHealthGainAmount);

	registerMethod(L, "Vocation", "getManaGain", LuaScriptInterface::luaVocationGetManaGain);
	registerMethod(L, "Vocation", "getManaGainTicks", LuaScriptInterface::luaVocationGetManaGainTicks);
	registerMethod(L, "Vocation", "getManaGainAmount", LuaScriptInterface::luaVocationGetManaGainAmount);

	registerMethod(L, "Vocation", "getMaxSoul", LuaScriptInterface::luaVocationGetMaxSoul);
	registerMethod(L, "Vocation", "getSoulGainTicks", LuaScriptInterface::luaVocationGetSoulGainTicks);

	registerMethod(L, "Vocation", "getAttackSpeed", LuaScriptInterface::luaVocationGetAttackSpeed);
	registerMethod(L, "Vocation", "getBaseSpeed", LuaScriptInterface::luaVocationGetBaseSpeed);

	registerMethod(L, "Vocation", "getDemotion", LuaScriptInterface::luaVocationGetDemotion);
	registerMethod(L, "Vocation", "getPromotion", LuaScriptInterface::luaVocationGetPromotion);

	registerMethod(L, "Vocation", "allowsPvp", LuaScriptInterface::luaVocationAllowsPvp);

	// House
	registerClass(L, "House", "", LuaScriptInterface::luaHouseCreate);
	registerMetaMethod(L, "House", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "House", "getId", LuaScriptInterface::luaHouseGetId);
	registerMethod(L, "House", "getName", LuaScriptInterface::luaHouseGetName);
	registerMethod(L, "House", "getTown", LuaScriptInterface::luaHouseGetTown);
	registerMethod(L, "House", "getExitPosition", LuaScriptInterface::luaHouseGetExitPosition);

	registerMethod(L, "House", "getRent", LuaScriptInterface::luaHouseGetRent);
	registerMethod(L, "House", "setRent", LuaScriptInterface::luaHouseSetRent);

	registerMethod(L, "House", "getPaidUntil", LuaScriptInterface::luaHouseGetPaidUntil);
	registerMethod(L, "House", "setPaidUntil", LuaScriptInterface::luaHouseSetPaidUntil);

	registerMethod(L, "House", "getPayRentWarnings", LuaScriptInterface::luaHouseGetPayRentWarnings);
	registerMethod(L, "House", "setPayRentWarnings", LuaScriptInterface::luaHouseSetPayRentWarnings);

	registerMethod(L, "House", "getOwnerName", LuaScriptInterface::luaHouseGetOwnerName);
	registerMethod(L, "House", "getOwnerGuid", LuaScriptInterface::luaHouseGetOwnerGuid);
	registerMethod(L, "House", "setOwnerGuid", LuaScriptInterface::luaHouseSetOwnerGuid);
	registerMethod(L, "House", "startTrade", LuaScriptInterface::luaHouseStartTrade);

	registerMethod(L, "House", "getBeds", LuaScriptInterface::luaHouseGetBeds);
	registerMethod(L, "House", "getBedCount", LuaScriptInterface::luaHouseGetBedCount);

	registerMethod(L, "House", "getDoors", LuaScriptInterface::luaHouseGetDoors);
	registerMethod(L, "House", "getDoorCount", LuaScriptInterface::luaHouseGetDoorCount);
	registerMethod(L, "House", "getDoorIdByPosition", LuaScriptInterface::luaHouseGetDoorIdByPosition);

	registerMethod(L, "House", "getTiles", LuaScriptInterface::luaHouseGetTiles);
	registerMethod(L, "House", "getItems", LuaScriptInterface::luaHouseGetItems);
	registerMethod(L, "House", "getTileCount", LuaScriptInterface::luaHouseGetTileCount);

	registerMethod(L, "House", "canEditAccessList", LuaScriptInterface::luaHouseCanEditAccessList);
	registerMethod(L, "House", "getAccessList", LuaScriptInterface::luaHouseGetAccessList);
	registerMethod(L, "House", "setAccessList", LuaScriptInterface::luaHouseSetAccessList);

	registerMethod(L, "House", "kickPlayer", LuaScriptInterface::luaHouseKickPlayer);

	registerMethod(L, "House", "save", LuaScriptInterface::luaHouseSave);

	// ItemType
	registerClass(L, "ItemType", "", LuaScriptInterface::luaItemTypeCreate);
	registerMetaMethod(L, "ItemType", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "ItemType", "isCorpse", LuaScriptInterface::luaItemTypeIsCorpse);
	registerMethod(L, "ItemType", "isDoor", LuaScriptInterface::luaItemTypeIsDoor);
	registerMethod(L, "ItemType", "isContainer", LuaScriptInterface::luaItemTypeIsContainer);
	registerMethod(L, "ItemType", "isFluidContainer", LuaScriptInterface::luaItemTypeIsFluidContainer);
	registerMethod(L, "ItemType", "isMovable", LuaScriptInterface::luaItemTypeIsMovable);
	registerMethod(L, "ItemType", "isRune", LuaScriptInterface::luaItemTypeIsRune);
	registerMethod(L, "ItemType", "isStackable", LuaScriptInterface::luaItemTypeIsStackable);
	registerMethod(L, "ItemType", "isReadable", LuaScriptInterface::luaItemTypeIsReadable);
	registerMethod(L, "ItemType", "isWritable", LuaScriptInterface::luaItemTypeIsWritable);
	registerMethod(L, "ItemType", "isBlocking", LuaScriptInterface::luaItemTypeIsBlocking);
	registerMethod(L, "ItemType", "isGroundTile", LuaScriptInterface::luaItemTypeIsGroundTile);
	registerMethod(L, "ItemType", "isMagicField", LuaScriptInterface::luaItemTypeIsMagicField);
	registerMethod(L, "ItemType", "isUseable", LuaScriptInterface::luaItemTypeIsUseable);
	registerMethod(L, "ItemType", "isPickupable", LuaScriptInterface::luaItemTypeIsPickupable);
	registerMethod(L, "ItemType", "isRotatable", LuaScriptInterface::luaItemTypeIsRotatable);

	registerMethod(L, "ItemType", "getType", LuaScriptInterface::luaItemTypeGetType);
	registerMethod(L, "ItemType", "getGroup", LuaScriptInterface::luaItemTypeGetGroup);
	registerMethod(L, "ItemType", "getId", LuaScriptInterface::luaItemTypeGetId);
	registerMethod(L, "ItemType", "getClientId", LuaScriptInterface::luaItemTypeGetClientId);
	registerMethod(L, "ItemType", "getName", LuaScriptInterface::luaItemTypeGetName);
	registerMethod(L, "ItemType", "getPluralName", LuaScriptInterface::luaItemTypeGetPluralName);
	registerMethod(L, "ItemType", "getRotateTo", LuaScriptInterface::luaItemTypeGetRotateTo);
	registerMethod(L, "ItemType", "getArticle", LuaScriptInterface::luaItemTypeGetArticle);
	registerMethod(L, "ItemType", "getDescription", LuaScriptInterface::luaItemTypeGetDescription);
	registerMethod(L, "ItemType", "getSlotPosition", LuaScriptInterface::luaItemTypeGetSlotPosition);

	registerMethod(L, "ItemType", "getCharges", LuaScriptInterface::luaItemTypeGetCharges);
	registerMethod(L, "ItemType", "getFluidSource", LuaScriptInterface::luaItemTypeGetFluidSource);
	registerMethod(L, "ItemType", "getCapacity", LuaScriptInterface::luaItemTypeGetCapacity);
	registerMethod(L, "ItemType", "getWeight", LuaScriptInterface::luaItemTypeGetWeight);
	registerMethod(L, "ItemType", "getWorth", LuaScriptInterface::luaItemTypeGetWorth);

	registerMethod(L, "ItemType", "getHitChance", LuaScriptInterface::luaItemTypeGetHitChance);
	registerMethod(L, "ItemType", "getShootRange", LuaScriptInterface::luaItemTypeGetShootRange);

	registerMethod(L, "ItemType", "getAttack", LuaScriptInterface::luaItemTypeGetAttack);
	registerMethod(L, "ItemType", "getAttackSpeed", LuaScriptInterface::luaItemTypeGetAttackSpeed);
	registerMethod(L, "ItemType", "getDefense", LuaScriptInterface::luaItemTypeGetDefense);
	registerMethod(L, "ItemType", "getExtraDefense", LuaScriptInterface::luaItemTypeGetExtraDefense);
	registerMethod(L, "ItemType", "getArmor", LuaScriptInterface::luaItemTypeGetArmor);
	registerMethod(L, "ItemType", "getWeaponType", LuaScriptInterface::luaItemTypeGetWeaponType);

	registerMethod(L, "ItemType", "getElementType", LuaScriptInterface::luaItemTypeGetElementType);
	registerMethod(L, "ItemType", "getElementDamage", LuaScriptInterface::luaItemTypeGetElementDamage);

	registerMethod(L, "ItemType", "getTransformEquipId", LuaScriptInterface::luaItemTypeGetTransformEquipId);
	registerMethod(L, "ItemType", "getTransformDeEquipId", LuaScriptInterface::luaItemTypeGetTransformDeEquipId);
	registerMethod(L, "ItemType", "getDestroyId", LuaScriptInterface::luaItemTypeGetDestroyId);
	registerMethod(L, "ItemType", "getDecayId", LuaScriptInterface::luaItemTypeGetDecayId);
	registerMethod(L, "ItemType", "getRequiredLevel", LuaScriptInterface::luaItemTypeGetRequiredLevel);
	registerMethod(L, "ItemType", "getAmmoType", LuaScriptInterface::luaItemTypeGetAmmoType);
	registerMethod(L, "ItemType", "getCorpseType", LuaScriptInterface::luaItemTypeGetCorpseType);
	registerMethod(L, "ItemType", "getClassification", LuaScriptInterface::luaItemTypeGetClassification);

	registerMethod(L, "ItemType", "getAbilities", LuaScriptInterface::luaItemTypeGetAbilities);

	registerMethod(L, "ItemType", "hasShowAttributes", LuaScriptInterface::luaItemTypeHasShowAttributes);
	registerMethod(L, "ItemType", "hasShowCount", LuaScriptInterface::luaItemTypeHasShowCount);
	registerMethod(L, "ItemType", "hasShowCharges", LuaScriptInterface::luaItemTypeHasShowCharges);
	registerMethod(L, "ItemType", "hasShowDuration", LuaScriptInterface::luaItemTypeHasShowDuration);
	registerMethod(L, "ItemType", "hasAllowDistRead", LuaScriptInterface::luaItemTypeHasAllowDistRead);
	registerMethod(L, "ItemType", "getWieldInfo", LuaScriptInterface::luaItemTypeGetWieldInfo);
	registerMethod(L, "ItemType", "getDurationMin", LuaScriptInterface::luaItemTypeGetDurationMin);
	registerMethod(L, "ItemType", "getDurationMax", LuaScriptInterface::luaItemTypeGetDurationMax);
	registerMethod(L, "ItemType", "getLevelDoor", LuaScriptInterface::luaItemTypeGetLevelDoor);
	registerMethod(L, "ItemType", "getRuneSpellName", LuaScriptInterface::luaItemTypeGetRuneSpellName);
	registerMethod(L, "ItemType", "getVocationString", LuaScriptInterface::luaItemTypeGetVocationString);
	registerMethod(L, "ItemType", "getMinReqLevel", LuaScriptInterface::luaItemTypeGetMinReqLevel);
	registerMethod(L, "ItemType", "getMinReqMagicLevel", LuaScriptInterface::luaItemTypeGetMinReqMagicLevel);
	registerMethod(L, "ItemType", "getMarketBuyStatistics", LuaScriptInterface::luaItemTypeGetMarketBuyStatistics);
	registerMethod(L, "ItemType", "getMarketSellStatistics", LuaScriptInterface::luaItemTypeGetMarketSellStatistics);

	registerMethod(L, "ItemType", "hasSubType", LuaScriptInterface::luaItemTypeHasSubType);

	registerMethod(L, "ItemType", "isStoreItem", LuaScriptInterface::luaItemTypeIsStoreItem);

	// Combat
	registerClass(L, "Combat", "", LuaScriptInterface::luaCombatCreate);
	registerMetaMethod(L, "Combat", "__eq", LuaScriptInterface::luaUserdataCompare);
	registerMetaMethod(L, "Combat", "__gc", LuaScriptInterface::luaCombatDelete);
	registerMethod(L, "Combat", "delete", LuaScriptInterface::luaCombatDelete);

	registerMethod(L, "Combat", "setParameter", LuaScriptInterface::luaCombatSetParameter);
	registerMethod(L, "Combat", "getParameter", LuaScriptInterface::luaCombatGetParameter);

	registerMethod(L, "Combat", "setFormula", LuaScriptInterface::luaCombatSetFormula);

	registerMethod(L, "Combat", "setArea", LuaScriptInterface::luaCombatSetArea);
	registerMethod(L, "Combat", "addCondition", LuaScriptInterface::luaCombatAddCondition);
	registerMethod(L, "Combat", "clearConditions", LuaScriptInterface::luaCombatClearConditions);
	registerMethod(L, "Combat", "setCallback", LuaScriptInterface::luaCombatSetCallback);
	registerMethod(L, "Combat", "setOrigin", LuaScriptInterface::luaCombatSetOrigin);

	registerMethod(L, "Combat", "execute", LuaScriptInterface::luaCombatExecute);

	// Condition
	registerClass(L, "Condition", "", LuaScriptInterface::luaConditionCreate);
	registerMetaMethod(L, "Condition", "__eq", LuaScriptInterface::luaUserdataCompare);
	registerMetaMethod(L, "Condition", "__gc", LuaScriptInterface::luaConditionDelete);

	registerMethod(L, "Condition", "getId", LuaScriptInterface::luaConditionGetId);
	registerMethod(L, "Condition", "getSubId", LuaScriptInterface::luaConditionGetSubId);
	registerMethod(L, "Condition", "getType", LuaScriptInterface::luaConditionGetType);
	registerMethod(L, "Condition", "getIcons", LuaScriptInterface::luaConditionGetIcons);
	registerMethod(L, "Condition", "getEndTime", LuaScriptInterface::luaConditionGetEndTime);

	registerMethod(L, "Condition", "clone", LuaScriptInterface::luaConditionClone);

	registerMethod(L, "Condition", "getTicks", LuaScriptInterface::luaConditionGetTicks);
	registerMethod(L, "Condition", "setTicks", LuaScriptInterface::luaConditionSetTicks);

	registerMethod(L, "Condition", "setParameter", LuaScriptInterface::luaConditionSetParameter);
	registerMethod(L, "Condition", "getParameter", LuaScriptInterface::luaConditionGetParameter);

	registerMethod(L, "Condition", "setFormula", LuaScriptInterface::luaConditionSetFormula);
	registerMethod(L, "Condition", "setOutfit", LuaScriptInterface::luaConditionSetOutfit);

	registerMethod(L, "Condition", "addDamage", LuaScriptInterface::luaConditionAddDamage);

	// Outfit
	registerClass(L, "Outfit", "", LuaScriptInterface::luaOutfitCreate);
	registerMetaMethod(L, "Outfit", "__eq", LuaScriptInterface::luaOutfitCompare);

	// MonsterType
	registerClass(L, "MonsterType", "", LuaScriptInterface::luaMonsterTypeCreate);
	registerMetaMethod(L, "MonsterType", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "MonsterType", "isAttackable", LuaScriptInterface::luaMonsterTypeIsAttackable);
	registerMethod(L, "MonsterType", "isChallengeable", LuaScriptInterface::luaMonsterTypeIsChallengeable);
	registerMethod(L, "MonsterType", "isConvinceable", LuaScriptInterface::luaMonsterTypeIsConvinceable);
	registerMethod(L, "MonsterType", "isSummonable", LuaScriptInterface::luaMonsterTypeIsSummonable);
	registerMethod(L, "MonsterType", "isIgnoringSpawnBlock", LuaScriptInterface::luaMonsterTypeIsIgnoringSpawnBlock);
	registerMethod(L, "MonsterType", "isIllusionable", LuaScriptInterface::luaMonsterTypeIsIllusionable);
	registerMethod(L, "MonsterType", "isHostile", LuaScriptInterface::luaMonsterTypeIsHostile);
	registerMethod(L, "MonsterType", "isPushable", LuaScriptInterface::luaMonsterTypeIsPushable);
	registerMethod(L, "MonsterType", "isHealthHidden", LuaScriptInterface::luaMonsterTypeIsHealthHidden);
	registerMethod(L, "MonsterType", "isBoss", LuaScriptInterface::luaMonsterTypeIsBoss);

	registerMethod(L, "MonsterType", "canPushItems", LuaScriptInterface::luaMonsterTypeCanPushItems);
	registerMethod(L, "MonsterType", "canPushCreatures", LuaScriptInterface::luaMonsterTypeCanPushCreatures);

	registerMethod(L, "MonsterType", "canWalkOnEnergy", LuaScriptInterface::luaMonsterTypeCanWalkOnEnergy);
	registerMethod(L, "MonsterType", "canWalkOnFire", LuaScriptInterface::luaMonsterTypeCanWalkOnFire);
	registerMethod(L, "MonsterType", "canWalkOnPoison", LuaScriptInterface::luaMonsterTypeCanWalkOnPoison);

	registerMethod(L, "MonsterType", "name", LuaScriptInterface::luaMonsterTypeName);
	registerMethod(L, "MonsterType", "nameDescription", LuaScriptInterface::luaMonsterTypeNameDescription);

	registerMethod(L, "MonsterType", "health", LuaScriptInterface::luaMonsterTypeHealth);
	registerMethod(L, "MonsterType", "maxHealth", LuaScriptInterface::luaMonsterTypeMaxHealth);
	registerMethod(L, "MonsterType", "runHealth", LuaScriptInterface::luaMonsterTypeRunHealth);
	registerMethod(L, "MonsterType", "experience", LuaScriptInterface::luaMonsterTypeExperience);
	registerMethod(L, "MonsterType", "skull", LuaScriptInterface::luaMonsterTypeSkull);

	registerMethod(L, "MonsterType", "combatImmunities", LuaScriptInterface::luaMonsterTypeCombatImmunities);
	registerMethod(L, "MonsterType", "conditionImmunities", LuaScriptInterface::luaMonsterTypeConditionImmunities);

	registerMethod(L, "MonsterType", "getAttackList", LuaScriptInterface::luaMonsterTypeGetAttackList);
	registerMethod(L, "MonsterType", "addAttack", LuaScriptInterface::luaMonsterTypeAddAttack);

	registerMethod(L, "MonsterType", "getDefenseList", LuaScriptInterface::luaMonsterTypeGetDefenseList);
	registerMethod(L, "MonsterType", "addDefense", LuaScriptInterface::luaMonsterTypeAddDefense);

	registerMethod(L, "MonsterType", "getElementList", LuaScriptInterface::luaMonsterTypeGetElementList);
	registerMethod(L, "MonsterType", "addElement", LuaScriptInterface::luaMonsterTypeAddElement);

	registerMethod(L, "MonsterType", "getVoices", LuaScriptInterface::luaMonsterTypeGetVoices);
	registerMethod(L, "MonsterType", "addVoice", LuaScriptInterface::luaMonsterTypeAddVoice);

	registerMethod(L, "MonsterType", "getLoot", LuaScriptInterface::luaMonsterTypeGetLoot);
	registerMethod(L, "MonsterType", "addLoot", LuaScriptInterface::luaMonsterTypeAddLoot);

	registerMethod(L, "MonsterType", "getCreatureEvents", LuaScriptInterface::luaMonsterTypeGetCreatureEvents);
	registerMethod(L, "MonsterType", "registerEvent", LuaScriptInterface::luaMonsterTypeRegisterEvent);

	registerMethod(L, "MonsterType", "eventType", LuaScriptInterface::luaMonsterTypeEventType);
	registerMethod(L, "MonsterType", "onThink", LuaScriptInterface::luaMonsterTypeEventOnCallback);
	registerMethod(L, "MonsterType", "onAppear", LuaScriptInterface::luaMonsterTypeEventOnCallback);
	registerMethod(L, "MonsterType", "onDisappear", LuaScriptInterface::luaMonsterTypeEventOnCallback);
	registerMethod(L, "MonsterType", "onMove", LuaScriptInterface::luaMonsterTypeEventOnCallback);
	registerMethod(L, "MonsterType", "onSay", LuaScriptInterface::luaMonsterTypeEventOnCallback);

	registerMethod(L, "MonsterType", "getSummonList", LuaScriptInterface::luaMonsterTypeGetSummonList);
	registerMethod(L, "MonsterType", "addSummon", LuaScriptInterface::luaMonsterTypeAddSummon);

	registerMethod(L, "MonsterType", "maxSummons", LuaScriptInterface::luaMonsterTypeMaxSummons);

	registerMethod(L, "MonsterType", "armor", LuaScriptInterface::luaMonsterTypeArmor);
	registerMethod(L, "MonsterType", "defense", LuaScriptInterface::luaMonsterTypeDefense);
	registerMethod(L, "MonsterType", "outfit", LuaScriptInterface::luaMonsterTypeOutfit);
	registerMethod(L, "MonsterType", "race", LuaScriptInterface::luaMonsterTypeRace);
	registerMethod(L, "MonsterType", "corpseId", LuaScriptInterface::luaMonsterTypeCorpseId);
	registerMethod(L, "MonsterType", "manaCost", LuaScriptInterface::luaMonsterTypeManaCost);
	registerMethod(L, "MonsterType", "baseSpeed", LuaScriptInterface::luaMonsterTypeBaseSpeed);
	registerMethod(L, "MonsterType", "light", LuaScriptInterface::luaMonsterTypeLight);

	registerMethod(L, "MonsterType", "staticAttackChance", LuaScriptInterface::luaMonsterTypeStaticAttackChance);
	registerMethod(L, "MonsterType", "targetDistance", LuaScriptInterface::luaMonsterTypeTargetDistance);
	registerMethod(L, "MonsterType", "yellChance", LuaScriptInterface::luaMonsterTypeYellChance);
	registerMethod(L, "MonsterType", "yellSpeedTicks", LuaScriptInterface::luaMonsterTypeYellSpeedTicks);
	registerMethod(L, "MonsterType", "changeTargetChance", LuaScriptInterface::luaMonsterTypeChangeTargetChance);
	registerMethod(L, "MonsterType", "changeTargetSpeed", LuaScriptInterface::luaMonsterTypeChangeTargetSpeed);

	registerMethod(L, "MonsterType", "bestiaryInfo", LuaScriptInterface::luaMonsterTypeBestiaryInfo);

	// Loot
	registerClass(L, "Loot", "", LuaScriptInterface::luaCreateLoot);
	registerMetaMethod(L, "Loot", "__gc", LuaScriptInterface::luaDeleteLoot);
	registerMethod(L, "Loot", "delete", LuaScriptInterface::luaDeleteLoot);

	registerMethod(L, "Loot", "setId", LuaScriptInterface::luaLootSetId);
	registerMethod(L, "Loot", "setMaxCount", LuaScriptInterface::luaLootSetMaxCount);
	registerMethod(L, "Loot", "setSubType", LuaScriptInterface::luaLootSetSubType);
	registerMethod(L, "Loot", "setChance", LuaScriptInterface::luaLootSetChance);
	registerMethod(L, "Loot", "setActionId", LuaScriptInterface::luaLootSetActionId);
	registerMethod(L, "Loot", "setDescription", LuaScriptInterface::luaLootSetDescription);
	registerMethod(L, "Loot", "addChildLoot", LuaScriptInterface::luaLootAddChildLoot);

	// MonsterSpell
	registerClass(L, "MonsterSpell", "", LuaScriptInterface::luaCreateMonsterSpell);
	registerMetaMethod(L, "MonsterSpell", "__gc", LuaScriptInterface::luaDeleteMonsterSpell);
	registerMethod(L, "MonsterSpell", "delete", LuaScriptInterface::luaDeleteMonsterSpell);

	registerMethod(L, "MonsterSpell", "setType", LuaScriptInterface::luaMonsterSpellSetType);
	registerMethod(L, "MonsterSpell", "setScriptName", LuaScriptInterface::luaMonsterSpellSetScriptName);
	registerMethod(L, "MonsterSpell", "setChance", LuaScriptInterface::luaMonsterSpellSetChance);
	registerMethod(L, "MonsterSpell", "setInterval", LuaScriptInterface::luaMonsterSpellSetInterval);
	registerMethod(L, "MonsterSpell", "setRange", LuaScriptInterface::luaMonsterSpellSetRange);
	registerMethod(L, "MonsterSpell", "setCombatValue", LuaScriptInterface::luaMonsterSpellSetCombatValue);
	registerMethod(L, "MonsterSpell", "setCombatType", LuaScriptInterface::luaMonsterSpellSetCombatType);
	registerMethod(L, "MonsterSpell", "setAttackValue", LuaScriptInterface::luaMonsterSpellSetAttackValue);
	registerMethod(L, "MonsterSpell", "setNeedTarget", LuaScriptInterface::luaMonsterSpellSetNeedTarget);
	registerMethod(L, "MonsterSpell", "setNeedDirection", LuaScriptInterface::luaMonsterSpellSetNeedDirection);
	registerMethod(L, "MonsterSpell", "setCombatLength", LuaScriptInterface::luaMonsterSpellSetCombatLength);
	registerMethod(L, "MonsterSpell", "setCombatSpread", LuaScriptInterface::luaMonsterSpellSetCombatSpread);
	registerMethod(L, "MonsterSpell", "setCombatRadius", LuaScriptInterface::luaMonsterSpellSetCombatRadius);
	registerMethod(L, "MonsterSpell", "setCombatRing", LuaScriptInterface::luaMonsterSpellSetCombatRing);
	registerMethod(L, "MonsterSpell", "setConditionType", LuaScriptInterface::luaMonsterSpellSetConditionType);
	registerMethod(L, "MonsterSpell", "setConditionDamage", LuaScriptInterface::luaMonsterSpellSetConditionDamage);
	registerMethod(L, "MonsterSpell", "setConditionSpeedChange",
	               LuaScriptInterface::luaMonsterSpellSetConditionSpeedChange);
	registerMethod(L, "MonsterSpell", "setConditionDuration", LuaScriptInterface::luaMonsterSpellSetConditionDuration);
	registerMethod(L, "MonsterSpell", "setConditionDrunkenness",
	               LuaScriptInterface::luaMonsterSpellSetConditionDrunkenness);
	registerMethod(L, "MonsterSpell", "setConditionTickInterval",
	               LuaScriptInterface::luaMonsterSpellSetConditionTickInterval);
	registerMethod(L, "MonsterSpell", "setCombatShootEffect", LuaScriptInterface::luaMonsterSpellSetCombatShootEffect);
	registerMethod(L, "MonsterSpell", "setCombatEffect", LuaScriptInterface::luaMonsterSpellSetCombatEffect);
	registerMethod(L, "MonsterSpell", "setOutfit", LuaScriptInterface::luaMonsterSpellSetOutfit);

	// Party
	registerClass(L, "Party", "", LuaScriptInterface::luaPartyCreate);
	registerMetaMethod(L, "Party", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Party", "disband", LuaScriptInterface::luaPartyDisband);

	registerMethod(L, "Party", "getLeader", LuaScriptInterface::luaPartyGetLeader);
	registerMethod(L, "Party", "setLeader", LuaScriptInterface::luaPartySetLeader);

	registerMethod(L, "Party", "getMembers", LuaScriptInterface::luaPartyGetMembers);
	registerMethod(L, "Party", "getMemberCount", LuaScriptInterface::luaPartyGetMemberCount);

	registerMethod(L, "Party", "getInvitees", LuaScriptInterface::luaPartyGetInvitees);
	registerMethod(L, "Party", "getInviteeCount", LuaScriptInterface::luaPartyGetInviteeCount);

	registerMethod(L, "Party", "addInvite", LuaScriptInterface::luaPartyAddInvite);
	registerMethod(L, "Party", "removeInvite", LuaScriptInterface::luaPartyRemoveInvite);

	registerMethod(L, "Party", "addMember", LuaScriptInterface::luaPartyAddMember);
	registerMethod(L, "Party", "removeMember", LuaScriptInterface::luaPartyRemoveMember);

	registerMethod(L, "Party", "isSharedExperienceActive", LuaScriptInterface::luaPartyIsSharedExperienceActive);
	registerMethod(L, "Party", "isSharedExperienceEnabled", LuaScriptInterface::luaPartyIsSharedExperienceEnabled);
	registerMethod(L, "Party", "isMemberSharingExp", LuaScriptInterface::luaPartyIsMemberSharingExp);
	registerMethod(L, "Party", "shareExperience", LuaScriptInterface::luaPartyShareExperience);
	registerMethod(L, "Party", "setSharedExperience", LuaScriptInterface::luaPartySetSharedExperience);

	// Spells
	registerClass(L, "Spell", "", LuaScriptInterface::luaSpellCreate);
	registerMetaMethod(L, "Spell", "__eq", LuaScriptInterface::luaUserdataCompare);

	registerMethod(L, "Spell", "onCastSpell", LuaScriptInterface::luaSpellOnCastSpell);
	registerMethod(L, "Spell", "register", LuaScriptInterface::luaSpellRegister);
	registerMethod(L, "Spell", "name", LuaScriptInterface::luaSpellName);
	registerMethod(L, "Spell", "id", LuaScriptInterface::luaSpellId);
	registerMethod(L, "Spell", "group", LuaScriptInterface::luaSpellGroup);
	registerMethod(L, "Spell", "cooldown", LuaScriptInterface::luaSpellCooldown);
	registerMethod(L, "Spell", "groupCooldown", LuaScriptInterface::luaSpellGroupCooldown);
	registerMethod(L, "Spell", "level", LuaScriptInterface::luaSpellLevel);
	registerMethod(L, "Spell", "magicLevel", LuaScriptInterface::luaSpellMagicLevel);
	registerMethod(L, "Spell", "mana", LuaScriptInterface::luaSpellMana);
	registerMethod(L, "Spell", "manaPercent", LuaScriptInterface::luaSpellManaPercent);
	registerMethod(L, "Spell", "soul", LuaScriptInterface::luaSpellSoul);
	registerMethod(L, "Spell", "range", LuaScriptInterface::luaSpellRange);
	registerMethod(L, "Spell", "isPremium", LuaScriptInterface::luaSpellPremium);
	registerMethod(L, "Spell", "isEnabled", LuaScriptInterface::luaSpellEnabled);
	registerMethod(L, "Spell", "needTarget", LuaScriptInterface::luaSpellNeedTarget);
	registerMethod(L, "Spell", "needWeapon", LuaScriptInterface::luaSpellNeedWeapon);
	registerMethod(L, "Spell", "needLearn", LuaScriptInterface::luaSpellNeedLearn);
	registerMethod(L, "Spell", "isSelfTarget", LuaScriptInterface::luaSpellSelfTarget);
	registerMethod(L, "Spell", "isBlocking", LuaScriptInterface::luaSpellBlocking);
	registerMethod(L, "Spell", "isAggressive", LuaScriptInterface::luaSpellAggressive);
	registerMethod(L, "Spell", "isPzLock", LuaScriptInterface::luaSpellPzLock);
	registerMethod(L, "Spell", "vocation", LuaScriptInterface::luaSpellVocation);

	// only for InstantSpell
	registerMethod(L, "Spell", "words", LuaScriptInterface::luaSpellWords);
	registerMethod(L, "Spell", "needDirection", LuaScriptInterface::luaSpellNeedDirection);
	registerMethod(L, "Spell", "hasParams", LuaScriptInterface::luaSpellHasParams);
	registerMethod(L, "Spell", "hasPlayerNameParam", LuaScriptInterface::luaSpellHasPlayerNameParam);
	registerMethod(L, "Spell", "needCasterTargetOrDirection", LuaScriptInterface::luaSpellNeedCasterTargetOrDirection);
	registerMethod(L, "Spell", "isBlockingWalls", LuaScriptInterface::luaSpellIsBlockingWalls);

	// only for RuneSpells
	registerMethod(L, "Spell", "runeLevel", LuaScriptInterface::luaSpellRuneLevel);
	registerMethod(L, "Spell", "runeMagicLevel", LuaScriptInterface::luaSpellRuneMagicLevel);
	registerMethod(L, "Spell", "runeId", LuaScriptInterface::luaSpellRuneId);
	registerMethod(L, "Spell", "charges", LuaScriptInterface::luaSpellCharges);
	registerMethod(L, "Spell", "allowFarUse", LuaScriptInterface::luaSpellAllowFarUse);
	registerMethod(L, "Spell", "blockWalls", LuaScriptInterface::luaSpellBlockWalls);
	registerMethod(L, "Spell", "checkFloor", LuaScriptInterface::luaSpellCheckFloor);

	// Action
	registerClass(L, "Action", "", LuaScriptInterface::luaCreateAction);
	registerMethod(L, "Action", "onUse", LuaScriptInterface::luaActionOnUse);
	registerMethod(L, "Action", "register", LuaScriptInterface::luaActionRegister);
	registerMethod(L, "Action", "id", LuaScriptInterface::luaActionItemId);
	registerMethod(L, "Action", "aid", LuaScriptInterface::luaActionActionId);
	registerMethod(L, "Action", "uid", LuaScriptInterface::luaActionUniqueId);
	registerMethod(L, "Action", "allowFarUse", LuaScriptInterface::luaActionAllowFarUse);
	registerMethod(L, "Action", "blockWalls", LuaScriptInterface::luaActionBlockWalls);
	registerMethod(L, "Action", "checkFloor", LuaScriptInterface::luaActionCheckFloor);

	// TalkAction
	registerClass(L, "TalkAction", "", LuaScriptInterface::luaCreateTalkaction);
	registerMethod(L, "TalkAction", "onSay", LuaScriptInterface::luaTalkactionOnSay);
	registerMethod(L, "TalkAction", "register", LuaScriptInterface::luaTalkactionRegister);
	registerMethod(L, "TalkAction", "separator", LuaScriptInterface::luaTalkactionSeparator);
	registerMethod(L, "TalkAction", "access", LuaScriptInterface::luaTalkactionAccess);
	registerMethod(L, "TalkAction", "accountType", LuaScriptInterface::luaTalkactionAccountType);

	// CreatureEvent
	registerClass(L, "CreatureEvent", "", LuaScriptInterface::luaCreateCreatureEvent);
	registerMethod(L, "CreatureEvent", "type", LuaScriptInterface::luaCreatureEventType);
	registerMethod(L, "CreatureEvent", "register", LuaScriptInterface::luaCreatureEventRegister);
	registerMethod(L, "CreatureEvent", "onLogin", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onLogout", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onReconnect", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onThink", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onPrepareDeath", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onDeath", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onKill", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onAdvance", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onModalWindow", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onTextEdit", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onHealthChange", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onManaChange", LuaScriptInterface::luaCreatureEventOnCallback);
	registerMethod(L, "CreatureEvent", "onExtendedOpcode", LuaScriptInterface::luaCreatureEventOnCallback);

	// MoveEvent
	registerClass(L, "MoveEvent", "", LuaScriptInterface::luaCreateMoveEvent);
	registerMethod(L, "MoveEvent", "type", LuaScriptInterface::luaMoveEventType);
	registerMethod(L, "MoveEvent", "register", LuaScriptInterface::luaMoveEventRegister);
	registerMethod(L, "MoveEvent", "level", LuaScriptInterface::luaMoveEventLevel);
	registerMethod(L, "MoveEvent", "magicLevel", LuaScriptInterface::luaMoveEventMagLevel);
	registerMethod(L, "MoveEvent", "slot", LuaScriptInterface::luaMoveEventSlot);
	registerMethod(L, "MoveEvent", "id", LuaScriptInterface::luaMoveEventItemId);
	registerMethod(L, "MoveEvent", "aid", LuaScriptInterface::luaMoveEventActionId);
	registerMethod(L, "MoveEvent", "uid", LuaScriptInterface::luaMoveEventUniqueId);
	registerMethod(L, "MoveEvent", "position", LuaScriptInterface::luaMoveEventPosition);
	registerMethod(L, "MoveEvent", "premium", LuaScriptInterface::luaMoveEventPremium);
	registerMethod(L, "MoveEvent", "vocation", LuaScriptInterface::luaMoveEventVocation);
	registerMethod(L, "MoveEvent", "tileItem", LuaScriptInterface::luaMoveEventTileItem);
	registerMethod(L, "MoveEvent", "onEquip", LuaScriptInterface::luaMoveEventOnCallback);
	registerMethod(L, "MoveEvent", "onDeEquip", LuaScriptInterface::luaMoveEventOnCallback);
	registerMethod(L, "MoveEvent", "onStepIn", LuaScriptInterface::luaMoveEventOnCallback);
	registerMethod(L, "MoveEvent", "onStepOut", LuaScriptInterface::luaMoveEventOnCallback);
	registerMethod(L, "MoveEvent", "onAddItem", LuaScriptInterface::luaMoveEventOnCallback);
	registerMethod(L, "MoveEvent", "onRemoveItem", LuaScriptInterface::luaMoveEventOnCallback);

	// GlobalEvent
	registerClass(L, "GlobalEvent", "", LuaScriptInterface::luaCreateGlobalEvent);
	registerMethod(L, "GlobalEvent", "type", LuaScriptInterface::luaGlobalEventType);
	registerMethod(L, "GlobalEvent", "register", LuaScriptInterface::luaGlobalEventRegister);
	registerMethod(L, "GlobalEvent", "time", LuaScriptInterface::luaGlobalEventTime);
	registerMethod(L, "GlobalEvent", "interval", LuaScriptInterface::luaGlobalEventInterval);
	registerMethod(L, "GlobalEvent", "onThink", LuaScriptInterface::luaGlobalEventOnCallback);
	registerMethod(L, "GlobalEvent", "onTime", LuaScriptInterface::luaGlobalEventOnCallback);
	registerMethod(L, "GlobalEvent", "onStartup", LuaScriptInterface::luaGlobalEventOnCallback);
	registerMethod(L, "GlobalEvent", "onShutdown", LuaScriptInterface::luaGlobalEventOnCallback);
	registerMethod(L, "GlobalEvent", "onRecord", LuaScriptInterface::luaGlobalEventOnCallback);
	registerMethod(L, "GlobalEvent", "onSave", LuaScriptInterface::luaGlobalEventOnCallback);

	// Weapon
	registerClass(L, "Weapon", "", LuaScriptInterface::luaCreateWeapon);
	registerMethod(L, "Weapon", "action", LuaScriptInterface::luaWeaponAction);
	registerMethod(L, "Weapon", "register", LuaScriptInterface::luaWeaponRegister);
	registerMethod(L, "Weapon", "id", LuaScriptInterface::luaWeaponId);
	registerMethod(L, "Weapon", "level", LuaScriptInterface::luaWeaponLevel);
	registerMethod(L, "Weapon", "magicLevel", LuaScriptInterface::luaWeaponMagicLevel);
	registerMethod(L, "Weapon", "mana", LuaScriptInterface::luaWeaponMana);
	registerMethod(L, "Weapon", "manaPercent", LuaScriptInterface::luaWeaponManaPercent);
	registerMethod(L, "Weapon", "health", LuaScriptInterface::luaWeaponHealth);
	registerMethod(L, "Weapon", "healthPercent", LuaScriptInterface::luaWeaponHealthPercent);
	registerMethod(L, "Weapon", "soul", LuaScriptInterface::luaWeaponSoul);
	registerMethod(L, "Weapon", "breakChance", LuaScriptInterface::luaWeaponBreakChance);
	registerMethod(L, "Weapon", "premium", LuaScriptInterface::luaWeaponPremium);
	registerMethod(L, "Weapon", "wieldUnproperly", LuaScriptInterface::luaWeaponUnproperly);
	registerMethod(L, "Weapon", "vocation", LuaScriptInterface::luaWeaponVocation);
	registerMethod(L, "Weapon", "onUseWeapon", LuaScriptInterface::luaWeaponOnUseWeapon);
	registerMethod(L, "Weapon", "element", LuaScriptInterface::luaWeaponElement);
	registerMethod(L, "Weapon", "attack", LuaScriptInterface::luaWeaponAttack);
	registerMethod(L, "Weapon", "defense", LuaScriptInterface::luaWeaponDefense);
	registerMethod(L, "Weapon", "range", LuaScriptInterface::luaWeaponRange);
	registerMethod(L, "Weapon", "charges", LuaScriptInterface::luaWeaponCharges);
	registerMethod(L, "Weapon", "duration", LuaScriptInterface::luaWeaponDuration);
	registerMethod(L, "Weapon", "decayTo", LuaScriptInterface::luaWeaponDecayTo);
	registerMethod(L, "Weapon", "transformEquipTo", LuaScriptInterface::luaWeaponTransformEquipTo);
	registerMethod(L, "Weapon", "transformDeEquipTo", LuaScriptInterface::luaWeaponTransformDeEquipTo);
	registerMethod(L, "Weapon", "slotType", LuaScriptInterface::luaWeaponSlotType);
	registerMethod(L, "Weapon", "hitChance", LuaScriptInterface::luaWeaponHitChance);
	registerMethod(L, "Weapon", "extraElement", LuaScriptInterface::luaWeaponExtraElement);

	// exclusively for distance weapons
	registerMethod(L, "Weapon", "ammoType", LuaScriptInterface::luaWeaponAmmoType);
	registerMethod(L, "Weapon", "maxHitChance", LuaScriptInterface::luaWeaponMaxHitChance);

	// exclusively for wands
	registerMethod(L, "Weapon", "damage", LuaScriptInterface::luaWeaponWandDamage);

	// exclusively for wands & distance weapons
	registerMethod(L, "Weapon", "shootType", LuaScriptInterface::luaWeaponShootType);

	// XML
	registerClass(L, "XMLDocument", "", LuaScriptInterface::luaCreateXmlDocument);
	registerMetaMethod(L, "XMLDocument", "__gc", LuaScriptInterface::luaDeleteXmlDocument);
	registerMethod(L, "XMLDocument", "delete", LuaScriptInterface::luaDeleteXmlDocument);

	registerMethod(L, "XMLDocument", "child", LuaScriptInterface::luaXmlDocumentChild);

	registerClass(L, "XMLNode", "");
	registerMetaMethod(L, "XMLNode", "__gc", LuaScriptInterface::luaDeleteXmlNode);
	registerMethod(L, "XMLNode", "delete", LuaScriptInterface::luaDeleteXmlNode);

	registerMethod(L, "XMLNode", "attribute", LuaScriptInterface::luaXmlNodeAttribute);
	registerMethod(L, "XMLNode", "name", LuaScriptInterface::luaXmlNodeName);
	registerMethod(L, "XMLNode", "firstChild", LuaScriptInterface::luaXmlNodeFirstChild);
	registerMethod(L, "XMLNode", "nextSibling", LuaScriptInterface::luaXmlNodeNextSibling);
}

#undef registerEnum
#undef registerEnumIn

ScriptEnvironment* tfs::lua::getScriptEnv()
{
	assert(scriptEnvIndex >= 0 && scriptEnvIndex < static_cast<int32_t>(scriptEnv.size()));
	return &scriptEnv[scriptEnvIndex];
}

bool tfs::lua::reserveScriptEnv() { return ++scriptEnvIndex < static_cast<int32_t>(scriptEnv.size()); }

void tfs::lua::resetScriptEnv()
{
	assert(scriptEnvIndex >= 0);
	scriptEnv[scriptEnvIndex--].resetEnv();
}

// Get
bool tfs::lua::getBoolean(lua_State* L, int32_t arg) { return lua_toboolean(L, arg) != 0; }
bool tfs::lua::getBoolean(lua_State* L, int32_t arg, bool defaultValue)
{
	if (lua_isboolean(L, arg) == 0) {
		return defaultValue;
	}
	return lua_toboolean(L, arg) != 0;
}

// Push

void tfs::lua::registerMethod(lua_State* L, std::string_view globalName, std::string_view methodName,
                              lua_CFunction func)
{
	// globalName.methodName = func
	lua_getglobal(L, globalName.data());
	lua_pushcfunction(L, func);
	lua_setfield(L, -2, methodName.data());

	// pop globalName
	lua_pop(L, 1);
}

int LuaScriptInterface::luaDoPlayerAddItem(lua_State* L)
{
	// doPlayerAddItem(cid, itemid, <optional: default: 1> count/subtype, <optional: default: 1> canDropOnMap)
	// doPlayerAddItem(cid, itemid, <optional: default: 1> count, <optional: default: 1> canDropOnMap, <optional:
	// default: 1>subtype)
	Player* player = tfs::lua::getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	uint16_t itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	int32_t count = tfs::lua::getNumber<int32_t>(L, 3, 1);
	bool canDropOnMap = tfs::lua::getBoolean(L, 4, true);
	uint16_t subType = tfs::lua::getNumber<uint16_t>(L, 5, 1);

	const ItemType& it = Item::items[itemId];
	int32_t itemCount;

	auto parameters = lua_gettop(L);
	if (parameters > 4) {
		// subtype already supplied, count then is the amount
		itemCount = std::max<int32_t>(1, count);
	} else if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = static_cast<int32_t>(std::ceil(static_cast<float>(count) / ITEM_STACK_SIZE));
		} else {
			itemCount = 1;
		}
		subType = count;
	} else {
		itemCount = std::max<int32_t>(1, count);
	}

	while (itemCount > 0) {
		uint16_t stackCount = subType;
		if (it.stackable && stackCount > ITEM_STACK_SIZE) {
			stackCount = ITEM_STACK_SIZE;
		}

		Item* newItem = Item::CreateItem(itemId, stackCount);
		if (!newItem) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		if (it.stackable) {
			subType -= stackCount;
		}

		ReturnValue ret = g_game.internalPlayerAddItem(player, newItem, canDropOnMap);
		if (ret != RETURNVALUE_NOERROR) {
			delete newItem;
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		if (--itemCount == 0) {
			if (newItem->hasParent()) {
				uint32_t uid = tfs::lua::getScriptEnv()->addThing(newItem);
				lua_pushnumber(L, uid);
				return 1;
			} else {
				// stackable item stacked with existing object, newItem will be released
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
		}
	}

	tfs::lua::pushBoolean(L, false);
	return 1;
}

int LuaScriptInterface::luaDebugPrint(lua_State* L)
{
	// debugPrint(text)
	reportErrorFunc(L, tfs::lua::getString(L, -1));
	return 0;
}

int LuaScriptInterface::luaGetWorldUpTime(lua_State* L)
{
	// getWorldUpTime()
	uint64_t uptime = (OTSYS_TIME() - ProtocolStatus::start) / 1000;
	lua_pushnumber(L, uptime);
	return 1;
}

int LuaScriptInterface::luaGetSubTypeName(lua_State* L)
{
	// getSubTypeName(subType)
	int32_t subType = tfs::lua::getNumber<int32_t>(L, 1);
	if (subType > 0) {
		tfs::lua::pushString(L, Item::items[subType].name);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreateCombatArea(lua_State* L)
{
	// createCombatArea({area}, <optional> {extArea})
	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	if (env->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc(L, "This function can only be used while loading the script.");
		tfs::lua::pushBoolean(L, false);
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
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		area->setupExtArea(vecExtArea, rowsExtArea);
	}

	uint32_t rowsArea = 0;
	std::vector<uint32_t> vecArea;
	if (!lua_istable(L, 1) || !getArea(L, vecArea, rowsArea)) {
		reportErrorFunc(L, "Invalid area table.");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	area->setupArea(vecArea, rowsArea);
	lua_pushnumber(L, areaId);
	return 1;
}

int LuaScriptInterface::luaDoAreaCombat(lua_State* L)
{
	// doAreaCombat(cid, type, pos, area, min, max, effect[, origin = ORIGIN_SPELL[, blockArmor = false[, blockShield =
	// false[, ignoreResistances = false]]]])
	Creature* creature = tfs::lua::getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || tfs::lua::getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	uint32_t areaId = tfs::lua::getNumber<uint32_t>(L, 4);
	const AreaCombat* area = g_luaEnvironment.getAreaObject(areaId);
	if (area || areaId == 0) {
		CombatType_t combatType = tfs::lua::getNumber<CombatType_t>(L, 2);

		CombatParams params;
		params.combatType = combatType;
		params.impactEffect = tfs::lua::getNumber<uint8_t>(L, 7);

		params.blockedByArmor = tfs::lua::getBoolean(L, 9, false);
		params.blockedByShield = tfs::lua::getBoolean(L, 10, false);
		params.ignoreResistances = tfs::lua::getBoolean(L, 11, false);

		CombatDamage damage;
		damage.origin = tfs::lua::getNumber<CombatOrigin>(L, 8, ORIGIN_SPELL);
		damage.primary.type = combatType;
		damage.primary.value = normal_random(tfs::lua::getNumber<int32_t>(L, 5), tfs::lua::getNumber<int32_t>(L, 6));

		Combat::doAreaCombat(creature, tfs::lua::getPosition(L, 3), area, damage, params);
		tfs::lua::pushBoolean(L, true);
	} else {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_AREA_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int LuaScriptInterface::luaDoTargetCombat(lua_State* L)
{
	// doTargetCombat(cid, target, type, min, max, effect[, origin = ORIGIN_SPELL[, blockArmor = false[, blockShield =
	// false[, ignoreResistances = false]]]])
	Creature* creature = tfs::lua::getCreature(L, 1);
	if (!creature && (!isNumber(L, 1) || tfs::lua::getNumber<uint32_t>(L, 1) != 0)) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	Creature* target = tfs::lua::getCreature(L, 2);
	if (!target) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	CombatType_t combatType = tfs::lua::getNumber<CombatType_t>(L, 3);

	CombatParams params{
	    .combatType = combatType,
	    .impactEffect = tfs::lua::getNumber<uint8_t>(L, 6),
	    .blockedByArmor = tfs::lua::getBoolean(L, 8, false),
	    .blockedByShield = tfs::lua::getBoolean(L, 9, false),
	    .ignoreResistances = tfs::lua::getBoolean(L, 10, false),
	};

	CombatDamage damage{
	    .primary =
	        {
	            .type = combatType,
	            .value = normal_random(tfs::lua::getNumber<int32_t>(L, 4), tfs::lua::getNumber<int32_t>(L, 5)),
	        },
	    .origin = tfs::lua::getNumber<CombatOrigin>(L, 7, ORIGIN_SPELL),
	};

	Combat::doTargetCombat(creature, target, damage, params);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaDoChallengeCreature(lua_State* L)
{
	// doChallengeCreature(cid, target[, force = false])
	Creature* creature = tfs::lua::getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	Creature* target = tfs::lua::getCreature(L, 2);
	if (!target) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	target->challengeCreature(creature, tfs::lua::getBoolean(L, 3, false));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaIsValidUID(lua_State* L)
{
	// isValidUID(uid)
	tfs::lua::pushBoolean(L, tfs::lua::getScriptEnv()->getThingByUID(tfs::lua::getNumber<uint32_t>(L, -1)) != nullptr);
	return 1;
}

int LuaScriptInterface::luaIsDepot(lua_State* L)
{
	// isDepot(uid)
	Container* container = tfs::lua::getScriptEnv()->getContainerByUID(tfs::lua::getNumber<uint32_t>(L, -1));
	tfs::lua::pushBoolean(L, container && container->getDepotLocker());
	return 1;
}

int LuaScriptInterface::luaIsMoveable(lua_State* L)
{
	// isMoveable(uid)
	// isMovable(uid)
	Thing* thing = tfs::lua::getScriptEnv()->getThingByUID(tfs::lua::getNumber<uint32_t>(L, -1));
	tfs::lua::pushBoolean(L, thing && thing->isPushable());
	return 1;
}

int LuaScriptInterface::luaGetDepotId(lua_State* L)
{
	// getDepotId(uid)
	uint32_t uid = tfs::lua::getNumber<uint32_t>(L, -1);

	Container* container = tfs::lua::getScriptEnv()->getContainerByUID(uid);
	if (!container) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CONTAINER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	DepotLocker* depotLocker = container->getDepotLocker();
	if (!depotLocker) {
		reportErrorFunc(L, "Depot not found");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	lua_pushnumber(L, depotLocker->getDepotId());
	return 1;
}

int LuaScriptInterface::luaAddEvent(lua_State* L)
{
	// addEvent(callback, delay, ...)
	int parameters = lua_gettop(L);
	if (parameters < 2) {
		reportErrorFunc(L, fmt::format("Not enough parameters: {:d}.", parameters));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (!lua_isfunction(L, 1)) {
		reportErrorFunc(L, "callback parameter should be a function.");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (!isNumber(L, 2)) {
		reportErrorFunc(L, "delay parameter should be a number.");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (ConfigManager::getBoolean(ConfigManager::WARN_UNSAFE_SCRIPTS) ||
	    ConfigManager::getBoolean(ConfigManager::CONVERT_UNSAFE_SCRIPTS)) {
		std::vector<std::pair<int32_t, LuaDataType>> indexes;
		for (int i = 3; i <= parameters; ++i) {
			if (lua_getmetatable(L, i) == 0) {
				continue;
			}
			lua_rawgeti(L, -1, 't');

			LuaDataType type = tfs::lua::getNumber<LuaDataType>(L, -1);
			if (type != LuaData_Unknown && type != LuaData_Tile) {
				indexes.push_back({i, type});
			}
			lua_pop(L, 2);
		}

		if (!indexes.empty()) {
			if (ConfigManager::getBoolean(ConfigManager::WARN_UNSAFE_SCRIPTS)) {
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

			if (ConfigManager::getBoolean(ConfigManager::CONVERT_UNSAFE_SCRIPTS)) {
				for (const auto& entry : indexes) {
					switch (entry.second) {
						case LuaData_Item:
						case LuaData_Container:
						case LuaData_Teleport:
						case LuaData_Podium: {
							lua_getglobal(L, "Item");
							lua_getfield(L, -1, "getUniqueId");
							break;
						}
						case LuaData_Player:
						case LuaData_Monster:
						case LuaData_Npc: {
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

	uint32_t delay = std::max<uint32_t>(100, tfs::lua::getNumber<uint32_t>(L, 2));
	lua_pop(L, 1);

	eventDesc.function = luaL_ref(L, LUA_REGISTRYINDEX);
	eventDesc.scriptId = tfs::lua::getScriptEnv()->getScriptId();

	auto& lastTimerEventId = g_luaEnvironment.lastEventTimerId;
	eventDesc.eventId = g_scheduler.addEvent(
	    createSchedulerTask(delay, [=]() { g_luaEnvironment.executeTimerEvent(lastTimerEventId); }));

	g_luaEnvironment.timerEvents.emplace(lastTimerEventId, std::move(eventDesc));
	lua_pushnumber(L, lastTimerEventId++);
	return 1;
}

int LuaScriptInterface::luaStopEvent(lua_State* L)
{
	// stopEvent(eventid)
	uint32_t eventId = tfs::lua::getNumber<uint32_t>(L, 1);

	auto& timerEvents = g_luaEnvironment.timerEvents;
	auto it = timerEvents.find(eventId);
	if (it == timerEvents.end()) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	LuaTimerEventDesc timerEventDesc = std::move(it->second);
	timerEvents.erase(it);

	g_scheduler.stopEvent(timerEventDesc.eventId);
	luaL_unref(L, LUA_REGISTRYINDEX, timerEventDesc.function);

	for (auto parameter : timerEventDesc.parameters) {
		luaL_unref(L, LUA_REGISTRYINDEX, parameter);
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaSaveServer(lua_State* L)
{
	g_globalEvents->save();
	g_game.saveGameState();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCleanMap(lua_State* L)
{
	lua_pushnumber(L, g_game.map.clean());
	return 1;
}

int LuaScriptInterface::luaIsInWar(lua_State* L)
{
	// isInWar(cid, target)
	Player* player = tfs::lua::getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	Player* targetPlayer = tfs::lua::getPlayer(L, 2);
	if (!targetPlayer) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	tfs::lua::pushBoolean(L, player->isInWar(targetPlayer));
	return 1;
}

int LuaScriptInterface::luaGetWaypointPositionByName(lua_State* L)
{
	// getWaypointPositionByName(name)
	auto& waypoints = g_game.map.waypoints;

	auto it = waypoints.find(tfs::lua::getString(L, -1));
	if (it != waypoints.end()) {
		tfs::lua::pushPosition(L, it->second);
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int LuaScriptInterface::luaSendChannelMessage(lua_State* L)
{
	// sendChannelMessage(channelId, type, message)
	uint32_t channelId = tfs::lua::getNumber<uint32_t>(L, 1);
	ChatChannel* channel = g_chat->getChannelById(channelId);
	if (!channel) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 2);
	std::string message = tfs::lua::getString(L, 3);
	channel->sendToAll(message, type);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaSendGuildChannelMessage(lua_State* L)
{
	// sendGuildChannelMessage(guildId, type, message)
	uint32_t guildId = tfs::lua::getNumber<uint32_t>(L, 1);
	ChatChannel* channel = g_chat->getGuildChannelById(guildId);
	if (!channel) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 2);
	std::string message = tfs::lua::getString(L, 3);
	channel->sendToAll(message, type);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaIsScriptsInterface(lua_State* L)
{
	// isScriptsInterface()
	if (tfs::lua::getScriptEnv()->getScriptInterface() == &g_scripts->getScriptInterface()) {
		tfs::lua::pushBoolean(L, true);
	} else {
		reportErrorFunc(L, "Event: can only be called inside (data/scripts/)");
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

#ifndef LUAJIT_VERSION
const luaL_Reg LuaScriptInterface::luaBitReg[] = {
    //{"tobit", LuaScriptInterface::luaBitToBit},
    {"bnot", LuaScriptInterface::luaBitNot},
    {"band", LuaScriptInterface::luaBitAnd},
    {"bor", LuaScriptInterface::luaBitOr},
    {"bxor", LuaScriptInterface::luaBitXor},
    {"lshift", LuaScriptInterface::luaBitLeftShift},
    {"rshift", LuaScriptInterface::luaBitRightShift},
    //{"arshift", LuaScriptInterface::luaBitArithmeticalRightShift},
    //{"rol", LuaScriptInterface::luaBitRotateLeft},
    //{"ror", LuaScriptInterface::luaBitRotateRight},
    //{"bswap", LuaScriptInterface::luaBitSwapEndian},
    //{"tohex", LuaScriptInterface::luaBitToHex},
    {nullptr, nullptr}};

int LuaScriptInterface::luaBitNot(lua_State* L)
{
	lua_pushnumber(L, ~tfs::lua::getNumber<uint32_t>(L, -1));
	return 1;
}

#define MULTIOP(name, op) \
	int LuaScriptInterface::luaBit##name(lua_State* L) \
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
	int LuaScriptInterface::luaBit##name(lua_State* L) \
	{ \
		uint32_t n1 = tfs::lua::getNumber<uint32_t>(L, 1), n2 = tfs::lua::getNumber<uint32_t>(L, 2); \
		lua_pushnumber(L, (n1 op n2)); \
		return 1; \
	}

SHIFTOP(LeftShift, <<)
SHIFTOP(RightShift, >>)
#endif

const luaL_Reg LuaScriptInterface::luaConfigManagerTable[] = {
    {"getString", LuaScriptInterface::luaConfigManagerGetString},
    {"getNumber", LuaScriptInterface::luaConfigManagerGetNumber},
    {"getBoolean", LuaScriptInterface::luaConfigManagerGetBoolean},
    {nullptr, nullptr}};

int LuaScriptInterface::luaConfigManagerGetString(lua_State* L)
{
	tfs::lua::pushString(L, ConfigManager::getString(tfs::lua::getNumber<ConfigManager::string_config_t>(L, -1)));
	return 1;
}

int LuaScriptInterface::luaConfigManagerGetNumber(lua_State* L)
{
	lua_pushnumber(L, ConfigManager::getNumber(tfs::lua::getNumber<ConfigManager::integer_config_t>(L, -1)));
	return 1;
}

int LuaScriptInterface::luaConfigManagerGetBoolean(lua_State* L)
{
	tfs::lua::pushBoolean(L, ConfigManager::getBoolean(tfs::lua::getNumber<ConfigManager::boolean_config_t>(L, -1)));
	return 1;
}

const luaL_Reg LuaScriptInterface::luaDatabaseTable[] = {
    {"query", LuaScriptInterface::luaDatabaseExecute},
    {"asyncQuery", LuaScriptInterface::luaDatabaseAsyncExecute},
    {"storeQuery", LuaScriptInterface::luaDatabaseStoreQuery},
    {"asyncStoreQuery", LuaScriptInterface::luaDatabaseAsyncStoreQuery},
    {"escapeString", LuaScriptInterface::luaDatabaseEscapeString},
    {"escapeBlob", LuaScriptInterface::luaDatabaseEscapeBlob},
    {"lastInsertId", LuaScriptInterface::luaDatabaseLastInsertId},
    {"tableExists", LuaScriptInterface::luaDatabaseTableExists},
    {nullptr, nullptr}};

int LuaScriptInterface::luaDatabaseExecute(lua_State* L)
{
	// db.query(query)
	tfs::lua::pushBoolean(L, Database::getInstance().executeQuery(tfs::lua::getString(L, -1)));
	return 1;
}

int LuaScriptInterface::luaDatabaseAsyncExecute(lua_State* L)
{
	// db.asyncQuery(query, callback)
	std::function<void(const DBResult_ptr&, bool)> callback;
	if (lua_gettop(L) > 1) {
		int32_t ref = luaL_ref(L, LUA_REGISTRYINDEX);
		auto scriptId = tfs::lua::getScriptEnv()->getScriptId();
		callback = [ref, scriptId](const DBResult_ptr&, bool success) {
			lua_State* L = g_luaEnvironment.getLuaState();
			if (!L) {
				return;
			}

			if (!tfs::lua::reserveScriptEnv()) {
				luaL_unref(L, LUA_REGISTRYINDEX, ref);
				return;
			}

			lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
			tfs::lua::pushBoolean(L, success);
			auto env = tfs::lua::getScriptEnv();
			env->setScriptId(scriptId, &g_luaEnvironment);
			g_luaEnvironment.callFunction(1);

			luaL_unref(L, LUA_REGISTRYINDEX, ref);
		};
	}
	g_databaseTasks.addTask(tfs::lua::getString(L, -1), callback);
	return 0;
}

int LuaScriptInterface::luaDatabaseStoreQuery(lua_State* L)
{
	// db.storeQuery(query)
	if (DBResult_ptr res = Database::getInstance().storeQuery(tfs::lua::getString(L, -1))) {
		lua_pushnumber(L, addResult(res));
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int LuaScriptInterface::luaDatabaseAsyncStoreQuery(lua_State* L)
{
	// db.asyncStoreQuery(query, callback)
	std::function<void(const DBResult_ptr&, bool)> callback;
	if (lua_gettop(L) > 1) {
		int32_t ref = luaL_ref(L, LUA_REGISTRYINDEX);
		auto scriptId = tfs::lua::getScriptEnv()->getScriptId();
		callback = [ref, scriptId](const DBResult_ptr& result, bool) {
			lua_State* L = g_luaEnvironment.getLuaState();
			if (!L) {
				return;
			}

			if (!tfs::lua::reserveScriptEnv()) {
				luaL_unref(L, LUA_REGISTRYINDEX, ref);
				return;
			}

			lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
			if (result) {
				lua_pushnumber(L, addResult(result));
			} else {
				tfs::lua::pushBoolean(L, false);
			}
			auto env = tfs::lua::getScriptEnv();
			env->setScriptId(scriptId, &g_luaEnvironment);
			g_luaEnvironment.callFunction(1);

			luaL_unref(L, LUA_REGISTRYINDEX, ref);
		};
	}
	g_databaseTasks.addTask(tfs::lua::getString(L, -1), callback, true);
	return 0;
}

int LuaScriptInterface::luaDatabaseEscapeString(lua_State* L)
{
	// db.escapeString(s)
	tfs::lua::pushString(L, Database::getInstance().escapeString(tfs::lua::getString(L, -1)));
	return 1;
}

int LuaScriptInterface::luaDatabaseEscapeBlob(lua_State* L)
{
	// db.escapeBlob(s, length)
	uint32_t length = tfs::lua::getNumber<uint32_t>(L, 2);
	tfs::lua::pushString(L, Database::getInstance().escapeBlob(tfs::lua::getString(L, 1).data(), length));
	return 1;
}

int LuaScriptInterface::luaDatabaseLastInsertId(lua_State* L)
{
	// db.lastInsertId()
	lua_pushnumber(L, Database::getInstance().getLastInsertId());
	return 1;
}

int LuaScriptInterface::luaDatabaseTableExists(lua_State* L)
{
	// db.tableExists(tableName)
	tfs::lua::pushBoolean(L, DatabaseManager::tableExists(tfs::lua::getString(L, -1)));
	return 1;
}

const luaL_Reg LuaScriptInterface::luaResultTable[] = {
    {"getNumber", LuaScriptInterface::luaResultGetNumber}, {"getString", LuaScriptInterface::luaResultGetString},
    {"getStream", LuaScriptInterface::luaResultGetStream}, {"next", LuaScriptInterface::luaResultNext},
    {"free", LuaScriptInterface::luaResultFree},           {nullptr, nullptr}};

int LuaScriptInterface::luaResultGetNumber(lua_State* L)
{
	DBResult_ptr res = getResultByID(tfs::lua::getNumber<uint32_t>(L, 1));
	if (!res) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const std::string& s = tfs::lua::getString(L, 2);
	lua_pushnumber(L, res->getNumber<int64_t>(s));
	return 1;
}

int LuaScriptInterface::luaResultGetString(lua_State* L)
{
	DBResult_ptr res = getResultByID(tfs::lua::getNumber<uint32_t>(L, 1));
	if (!res) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const std::string& s = tfs::lua::getString(L, 2);
	tfs::lua::pushString(L, res->getString(s));
	return 1;
}

int LuaScriptInterface::luaResultGetStream(lua_State* L)
{
	DBResult_ptr res = getResultByID(tfs::lua::getNumber<uint32_t>(L, 1));
	if (!res) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	auto stream = res->getString(tfs::lua::getString(L, 2));
	lua_pushlstring(L, stream.data(), stream.size());
	lua_pushnumber(L, stream.size());
	return 2;
}

int LuaScriptInterface::luaResultNext(lua_State* L)
{
	DBResult_ptr res = getResultByID(tfs::lua::getNumber<uint32_t>(L, -1));
	if (!res) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	tfs::lua::pushBoolean(L, res->next());
	return 1;
}

int LuaScriptInterface::luaResultFree(lua_State* L)
{
	tfs::lua::pushBoolean(L, removeResult(tfs::lua::getNumber<uint32_t>(L, -1)));
	return 1;
}

// Userdata
int LuaScriptInterface::luaUserdataCompare(lua_State* L)
{
	// userdataA == userdataB
	tfs::lua::pushBoolean(L, tfs::lua::getUserdata<void>(L, 1) == tfs::lua::getUserdata<void>(L, 2));
	return 1;
}

// _G
int LuaScriptInterface::luaIsType(lua_State* L)
{
	// isType(derived, base)
	lua_getmetatable(L, -2);
	lua_getmetatable(L, -2);

	lua_rawgeti(L, -2, 'p');
	uint_fast8_t parentsB = tfs::lua::getNumber<uint_fast8_t>(L, 1);

	lua_rawgeti(L, -3, 'h');
	size_t hashB = tfs::lua::getNumber<size_t>(L, 1);

	lua_rawgeti(L, -3, 'p');
	uint_fast8_t parentsA = tfs::lua::getNumber<uint_fast8_t>(L, 1);
	for (uint_fast8_t i = parentsA; i < parentsB; ++i) {
		lua_getfield(L, -3, "__index");
		lua_replace(L, -4);
	}

	lua_rawgeti(L, -4, 'h');
	size_t hashA = tfs::lua::getNumber<size_t>(L, 1);

	tfs::lua::pushBoolean(L, hashA == hashB);
	return 1;
}

int LuaScriptInterface::luaRawGetMetatable(lua_State* L)
{
	// rawgetmetatable(metatableName)
	luaL_getmetatable(L, tfs::lua::getString(L, 1).data());
	return 1;
}

// os
int LuaScriptInterface::luaSystemTime(lua_State* L)
{
	// os.mtime()
	lua_pushnumber(L, OTSYS_TIME());
	return 1;
}

// table
int LuaScriptInterface::luaTableCreate(lua_State* L)
{
	// table.create(arrayLength, keyLength)
	lua_createtable(L, tfs::lua::getNumber<int32_t>(L, 1), tfs::lua::getNumber<int32_t>(L, 2));
	return 1;
}

int LuaScriptInterface::luaTablePack(lua_State* L)
{
	// table.pack(...)
	int n = lua_gettop(L);         /* number of elements to pack */
	lua_createtable(L, n, 1);      /* create result table */
	lua_insert(L, 1);              /* put it at index 1 */
	for (int i = n; i >= 1; i--) { /* assign elements */
		lua_rawseti(L, 1, i);
	}
	if (luaL_callmeta(L, -1, "__index") != 0) {
		lua_replace(L, -2);
	}
	lua_pushinteger(L, n);
	lua_setfield(L, 1, "n"); /* t.n = number of elements */
	return 1;                /* return table */
}

// DB Insert
int LuaScriptInterface::luaDBInsertCreate(lua_State* L)
{
	// DBInsert(query)
	if (lua_isstring(L, 2)) {
		tfs::lua::pushUserdata(L, new DBInsert(tfs::lua::getString(L, 2)));
		tfs::lua::setMetatable(L, -1, "DBInsert");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaDBInsertAddRow(lua_State* L)
{
	// insert:addRow(row)
	DBInsert* insert = tfs::lua::getUserdata<DBInsert>(L, 1);
	if (insert) {
		tfs::lua::pushBoolean(L, insert->addRow(tfs::lua::getString(L, 2)));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaDBInsertExecute(lua_State* L)
{
	// insert:execute()
	DBInsert* insert = tfs::lua::getUserdata<DBInsert>(L, 1);
	if (insert) {
		tfs::lua::pushBoolean(L, insert->execute());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaDBInsertDelete(lua_State* L)
{
	DBInsert** insertPtr = tfs::lua::getRawUserdata<DBInsert>(L, 1);
	if (insertPtr && *insertPtr) {
		delete *insertPtr;
		*insertPtr = nullptr;
	}
	return 0;
}

// DB Transaction
int LuaScriptInterface::luaDBTransactionCreate(lua_State* L)
{
	// DBTransaction()
	tfs::lua::pushUserdata(L, new DBTransaction);
	tfs::lua::setMetatable(L, -1, "DBTransaction");
	return 1;
}

int LuaScriptInterface::luaDBTransactionBegin(lua_State* L)
{
	// transaction:begin()
	DBTransaction* transaction = tfs::lua::getUserdata<DBTransaction>(L, 1);
	if (transaction) {
		tfs::lua::pushBoolean(L, transaction->begin());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaDBTransactionCommit(lua_State* L)
{
	// transaction:commit()
	DBTransaction* transaction = tfs::lua::getUserdata<DBTransaction>(L, 1);
	if (transaction) {
		tfs::lua::pushBoolean(L, transaction->commit());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaDBTransactionDelete(lua_State* L)
{
	DBTransaction** transactionPtr = tfs::lua::getRawUserdata<DBTransaction>(L, 1);
	if (transactionPtr && *transactionPtr) {
		delete *transactionPtr;
		*transactionPtr = nullptr;
	}
	return 0;
}

// Game
int LuaScriptInterface::luaGameGetSpectators(lua_State* L)
{
	// Game.getSpectators(position[, multifloor = false[, onlyPlayer = false[, minRangeX = 0[, maxRangeX = 0[, minRangeY
	// = 0[, maxRangeY = 0]]]]]])
	const Position& position = tfs::lua::getPosition(L, 1);
	bool multifloor = tfs::lua::getBoolean(L, 2, false);
	bool onlyPlayers = tfs::lua::getBoolean(L, 3, false);
	int32_t minRangeX = tfs::lua::getNumber<int32_t>(L, 4, 0);
	int32_t maxRangeX = tfs::lua::getNumber<int32_t>(L, 5, 0);
	int32_t minRangeY = tfs::lua::getNumber<int32_t>(L, 6, 0);
	int32_t maxRangeY = tfs::lua::getNumber<int32_t>(L, 7, 0);

	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, position, multifloor, onlyPlayers, minRangeX, maxRangeX, minRangeY, maxRangeY);

	lua_createtable(L, spectators.size(), 0);

	int index = 0;
	for (Creature* creature : spectators) {
		tfs::lua::pushUserdata(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaGameGetPlayers(lua_State* L)
{
	// Game.getPlayers()
	lua_createtable(L, g_game.getPlayersOnline(), 0);

	int index = 0;
	for (const auto& playerEntry : g_game.getPlayers()) {
		tfs::lua::pushUserdata(L, playerEntry.second);
		tfs::lua::setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaGameGetNpcs(lua_State* L)
{
	// Game.getNpcs()
	lua_createtable(L, g_game.getNpcsOnline(), 0);

	int index = 0;
	for (const auto& npcEntry : g_game.getNpcs()) {
		tfs::lua::pushUserdata(L, npcEntry.second);
		tfs::lua::setMetatable(L, -1, "Npc");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaGameGetMonsters(lua_State* L)
{
	// Game.getMonsters()
	lua_createtable(L, g_game.getMonstersOnline(), 0);

	int index = 0;
	for (const auto& monsterEntry : g_game.getMonsters()) {
		tfs::lua::pushUserdata(L, monsterEntry.second);
		tfs::lua::setMetatable(L, -1, "Monster");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaGameLoadMap(lua_State* L)
{
	// Game.loadMap(path)
	const std::string& path = tfs::lua::getString(L, 1);
	g_dispatcher.addTask([path]() {
		try {
			g_game.loadMap(path, true);
		} catch (const std::exception& e) {
			// FIXME: Should only catch some exceptions
			std::cout << "[Error - LuaScriptInterface::luaGameLoadMap] Failed to load map: " << e.what() << '\n';
		}
	});
	return 0;
}

int LuaScriptInterface::luaGameGetExperienceStage(lua_State* L)
{
	// Game.getExperienceStage(level)
	uint32_t level = tfs::lua::getNumber<uint32_t>(L, 1);
	lua_pushnumber(L, ConfigManager::getExperienceStage(level));
	return 1;
}

int LuaScriptInterface::luaGameGetExperienceForLevel(lua_State* L)
{
	// Game.getExperienceForLevel(level)
	const uint32_t level = tfs::lua::getNumber<uint32_t>(L, 1);
	if (level == 0) {
		lua_pushnumber(L, 0);
	} else {
		lua_pushnumber(L, Player::getExpForLevel(level));
	}
	return 1;
}

int LuaScriptInterface::luaGameGetMonsterCount(lua_State* L)
{
	// Game.getMonsterCount()
	lua_pushnumber(L, g_game.getMonstersOnline());
	return 1;
}

int LuaScriptInterface::luaGameGetPlayerCount(lua_State* L)
{
	// Game.getPlayerCount()
	lua_pushnumber(L, g_game.getPlayersOnline());
	return 1;
}

int LuaScriptInterface::luaGameGetNpcCount(lua_State* L)
{
	// Game.getNpcCount()
	lua_pushnumber(L, g_game.getNpcsOnline());
	return 1;
}

int LuaScriptInterface::luaGameGetMonsterTypes(lua_State* L)
{
	// Game.getMonsterTypes()
	auto& type = g_monsters.monsters;
	lua_createtable(L, type.size(), 0);

	for (const auto& [name, mType] : type) {
		tfs::lua::pushUserdata(L, &mType);
		tfs::lua::setMetatable(L, -1, "MonsterType");
		lua_setfield(L, -2, name.data());
	}
	return 1;
}

int LuaScriptInterface::luaGameGetBestiary(lua_State* L)
{
	// Game.getBestiary()
	lua_createtable(L, 0, g_monsters.bestiary.size());
	int classIndex = 0;
	for (const auto& [className, monsters] : g_monsters.bestiary) {
		lua_createtable(L, 0, 2);
		tfs::lua::pushString(L, className);
		lua_setfield(L, -2, "name");

		lua_createtable(L, 0, monsters.size());
		int index = 0;
		for (const auto& monsterName : monsters) {
			tfs::lua::pushUserdata(L, g_monsters.getMonsterType(monsterName));
			tfs::lua::setMetatable(L, -1, "MonsterType");
			lua_rawseti(L, -2, ++index);
		}

		lua_setfield(L, -2, "monsterTypes");
		lua_rawseti(L, -2, ++classIndex);
	}
	return 1;
}

int LuaScriptInterface::luaGameGetCurrencyItems(lua_State* L)
{
	// Game.getCurrencyItems()
	const auto& currencyItems = Item::items.currencyItems;
	size_t size = currencyItems.size();
	lua_createtable(L, size, 0);

	for (const auto& it : currencyItems) {
		const ItemType& itemType = Item::items[it.second];
		tfs::lua::pushUserdata(L, &itemType);
		tfs::lua::setMetatable(L, -1, "ItemType");
		lua_rawseti(L, -2, size--);
	}
	return 1;
}

int LuaScriptInterface::luaGameGetItemTypeByClientId(lua_State* L)
{
	// Game.getItemTypeByClientId(clientId)
	uint16_t spriteId = tfs::lua::getNumber<uint16_t>(L, 1);
	const ItemType& itemType = Item::items.getItemIdByClientId(spriteId);
	if (itemType.id != 0) {
		tfs::lua::pushUserdata(L, &itemType);
		tfs::lua::setMetatable(L, -1, "ItemType");
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int LuaScriptInterface::luaGameGetMountIdByLookType(lua_State* L)
{
	// Game.getMountIdByLookType(lookType)
	Mount* mount = nullptr;
	if (isNumber(L, 1)) {
		mount = g_game.mounts.getMountByClientID(tfs::lua::getNumber<uint16_t>(L, 1));
	}

	if (mount) {
		lua_pushnumber(L, mount->id);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGameGetTowns(lua_State* L)
{
	// Game.getTowns()
	const auto& towns = g_game.map.towns.getTowns();
	lua_createtable(L, towns.size(), 0);

	int index = 0;
	for (const auto& [_, town] : towns) {
		pushTown(L, *town);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaGameGetHouses(lua_State* L)
{
	// Game.getHouses()
	const auto& houses = g_game.map.houses.getHouses();
	lua_createtable(L, houses.size(), 0);

	int index = 0;
	for (const auto& house : houses | std::views::values) {
		tfs::lua::pushUserdata(L, house.get());
		tfs::lua::setMetatable(L, -1, "House");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaGameGetOutfits(lua_State* L)
{
	// Game.getOutfits(playerSex)
	if (!isNumber(L, 1)) {
		lua_pushnil(L);
		return 1;
	}

	PlayerSex_t playerSex = tfs::lua::getNumber<PlayerSex_t>(L, 1);
	if (playerSex > PLAYERSEX_LAST) {
		lua_pushnil(L);
		return 1;
	}

	const auto& outfits = Outfits::getInstance().getOutfits(playerSex);
	lua_createtable(L, outfits.size(), 0);

	int index = 0;
	for (const auto& outfit : outfits) {
		tfs::lua::pushOutfit(L, &outfit);
		lua_rawseti(L, -2, ++index);
	}

	return 1;
}

int LuaScriptInterface::luaGameGetMounts(lua_State* L)
{
	// Game.getMounts()
	const auto& mounts = g_game.mounts.getMounts();
	lua_createtable(L, mounts.size(), 0);

	int index = 0;
	for (const auto& mount : mounts) {
		lua_createtable(L, 0, 5);

		setField(L, "name", mount.name);
		setField(L, "speed", mount.speed);
		setField(L, "clientId", mount.clientId);
		setField(L, "id", mount.id);
		setField(L, "premium", mount.premium);

		lua_rawseti(L, -2, ++index);
	}

	return 1;
}

int LuaScriptInterface::luaGameGetVocations(lua_State* L)
{
	// Game.getVocations()
	const auto& vocations = g_vocations.getVocations();
	lua_createtable(L, vocations.size(), 0);

	int index = 0;
	for (const auto& [id, vocation] : vocations) {
		tfs::lua::pushUserdata(L, &vocation);
		tfs::lua::setMetatable(L, -1, "Vocation");
		lua_rawseti(L, -2, ++index);
	}

	return 1;
}

int LuaScriptInterface::luaGameGetRuneSpells(lua_State* L)
{
	// Game.getRuneSpells()
	auto runeSpells = g_spells->getRuneSpells();

	lua_createtable(L, runeSpells.size(), 0);

	int index = 0;
	for (auto& spell : runeSpells | std::views::values) {
		tfs::lua::pushUserdata<Spell>(L, &spell);
		tfs::lua::setMetatable(L, -1, "Spell");
		lua_rawseti(L, -2, ++index);
	}

	return 1;
}

int LuaScriptInterface::luaGameGetInstantSpells(lua_State* L)
{
	// Game.getInstantSpells()
	auto instantSpells = g_spells->getInstantSpells();

	lua_createtable(L, instantSpells.size(), 0);

	int index = 0;
	for (auto& spell : instantSpells | std::views::values) {
		tfs::lua::pushUserdata<Spell>(L, &spell);
		tfs::lua::setMetatable(L, -1, "Spell");
		lua_rawseti(L, -2, ++index);
	}

	return 1;
}

int LuaScriptInterface::luaGameGetGameState(lua_State* L)
{
	// Game.getGameState()
	lua_pushnumber(L, g_game.getGameState());
	return 1;
}

int LuaScriptInterface::luaGameSetGameState(lua_State* L)
{
	// Game.setGameState(state)
	GameState_t state = tfs::lua::getNumber<GameState_t>(L, 1);
	g_game.setGameState(state);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaGameGetWorldType(lua_State* L)
{
	// Game.getWorldType()
	lua_pushnumber(L, g_game.getWorldType());
	return 1;
}

int LuaScriptInterface::luaGameSetWorldType(lua_State* L)
{
	// Game.setWorldType(type)
	WorldType_t type = tfs::lua::getNumber<WorldType_t>(L, 1);
	g_game.setWorldType(type);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaGameGetReturnMessage(lua_State* L)
{
	// Game.getReturnMessage(value)
	ReturnValue value = tfs::lua::getNumber<ReturnValue>(L, 1);
	tfs::lua::pushString(L, getReturnMessage(value));
	return 1;
}

int LuaScriptInterface::luaGameGetItemAttributeByName(lua_State* L)
{
	// Game.getItemAttributeByName(name)
	lua_pushnumber(L, stringToItemAttribute(tfs::lua::getString(L, 1)));
	return 1;
}

int LuaScriptInterface::luaGameCreateItem(lua_State* L)
{
	// Game.createItem(itemId[, count[, position]])
	uint16_t count = tfs::lua::getNumber<uint16_t>(L, 2, 1);
	uint16_t id;
	if (isNumber(L, 1)) {
		id = tfs::lua::getNumber<uint16_t>(L, 1);
	} else {
		id = Item::items.getItemIdByName(tfs::lua::getString(L, 1));
		if (id == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	const ItemType& it = Item::items[id];
	if (it.stackable) {
		count = std::min<uint16_t>(count, ITEM_STACK_SIZE);
	}

	Item* item = Item::CreateItem(id, count);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	if (lua_gettop(L) >= 3) {
		const Position& position = tfs::lua::getPosition(L, 3);
		Tile* tile = g_game.map.getTile(position);
		if (!tile) {
			delete item;
			lua_pushnil(L);
			return 1;
		}

		g_game.internalAddItem(tile, item, INDEX_WHEREEVER, FLAG_NOLIMIT);
	} else {
		addTempItem(item);
		item->setParent(VirtualCylinder::virtualCylinder);
	}

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);
	return 1;
}

int LuaScriptInterface::luaGameCreateContainer(lua_State* L)
{
	// Game.createContainer(itemId, size[, position])
	uint16_t size = tfs::lua::getNumber<uint16_t>(L, 2);
	uint16_t id;
	if (isNumber(L, 1)) {
		id = tfs::lua::getNumber<uint16_t>(L, 1);
	} else {
		id = Item::items.getItemIdByName(tfs::lua::getString(L, 1));
		if (id == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	Container* container = Item::CreateItemAsContainer(id, size);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	if (lua_gettop(L) >= 3) {
		const Position& position = tfs::lua::getPosition(L, 3);
		Tile* tile = g_game.map.getTile(position);
		if (!tile) {
			delete container;
			lua_pushnil(L);
			return 1;
		}

		g_game.internalAddItem(tile, container, INDEX_WHEREEVER, FLAG_NOLIMIT);
	} else {
		addTempItem(container);
		container->setParent(VirtualCylinder::virtualCylinder);
	}

	tfs::lua::pushUserdata(L, container);
	tfs::lua::setMetatable(L, -1, "Container");
	return 1;
}

int LuaScriptInterface::luaGameCreateMonster(lua_State* L)
{
	// Game.createMonster(monsterName, position[, extended = false[, force = false[, magicEffect = CONST_ME_TELEPORT]]])
	Monster* monster = Monster::createMonster(tfs::lua::getString(L, 1));
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	const Position& position = tfs::lua::getPosition(L, 2);
	bool extended = tfs::lua::getBoolean(L, 3, false);
	bool force = tfs::lua::getBoolean(L, 4, false);
	MagicEffectClasses magicEffect = tfs::lua::getNumber<MagicEffectClasses>(L, 5, CONST_ME_TELEPORT);
	if (tfs::events::monster::onSpawn(monster, position, false, true) || force) {
		if (g_game.placeCreature(monster, position, extended, force, magicEffect)) {
			tfs::lua::pushUserdata(L, monster);
			tfs::lua::setMetatable(L, -1, "Monster");
		} else {
			delete monster;
			lua_pushnil(L);
		}
	} else {
		delete monster;
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGameCreateNpc(lua_State* L)
{
	// Game.createNpc(npcName, position[, extended = false[, force = false[, magicEffect = CONST_ME_TELEPORT]]])
	Npc* npc = Npc::createNpc(tfs::lua::getString(L, 1));
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	const Position& position = tfs::lua::getPosition(L, 2);
	bool extended = tfs::lua::getBoolean(L, 3, false);
	bool force = tfs::lua::getBoolean(L, 4, false);
	MagicEffectClasses magicEffect = tfs::lua::getNumber<MagicEffectClasses>(L, 5, CONST_ME_TELEPORT);
	if (g_game.placeCreature(npc, position, extended, force, magicEffect)) {
		tfs::lua::pushUserdata(L, npc);
		tfs::lua::setMetatable(L, -1, "Npc");
	} else {
		delete npc;
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGameCreateTile(lua_State* L)
{
	// Game.createTile(x, y, z[, isDynamic = false])
	// Game.createTile(position[, isDynamic = false])
	Position position;
	bool isDynamic;
	if (lua_istable(L, 1)) {
		position = tfs::lua::getPosition(L, 1);
		isDynamic = tfs::lua::getBoolean(L, 2, false);
	} else {
		position.x = tfs::lua::getNumber<uint16_t>(L, 1);
		position.y = tfs::lua::getNumber<uint16_t>(L, 2);
		position.z = tfs::lua::getNumber<uint16_t>(L, 3);
		isDynamic = tfs::lua::getBoolean(L, 4, false);
	}

	Tile* tile = g_game.map.getTile(position);
	if (!tile) {
		if (isDynamic) {
			tile = new DynamicTile(position.x, position.y, position.z);
		} else {
			tile = new StaticTile(position.x, position.y, position.z);
		}

		g_game.map.setTile(position, tile);
	}

	tfs::lua::pushUserdata(L, tile);
	tfs::lua::setMetatable(L, -1, "Tile");
	return 1;
}

int LuaScriptInterface::luaGameCreateMonsterType(lua_State* L)
{
	// Game.createMonsterType(name)
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		reportErrorFunc(L, "MonsterTypes can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	const std::string& name = tfs::lua::getString(L, 1);
	if (name.length() == 0) {
		lua_pushnil(L);
		return 1;
	}

	MonsterType* monsterType = g_monsters.getMonsterType(name, false);
	if (!monsterType) {
		monsterType = &g_monsters.monsters[boost::algorithm::to_lower_copy(name)];
		monsterType->name = name;
		monsterType->nameDescription = "a " + name;
	} else {
		monsterType->info.lootItems.clear();
		monsterType->info.attackSpells.clear();
		monsterType->info.defenseSpells.clear();
		monsterType->info.scripts.clear();
		monsterType->info.thinkEvent = -1;
		monsterType->info.creatureAppearEvent = -1;
		monsterType->info.creatureDisappearEvent = -1;
		monsterType->info.creatureMoveEvent = -1;
		monsterType->info.creatureSayEvent = -1;
	}

	tfs::lua::pushUserdata(L, monsterType);
	tfs::lua::setMetatable(L, -1, "MonsterType");
	return 1;
}

int LuaScriptInterface::luaGameStartEvent(lua_State* L)
{
	// Game.startEvent(event)
	const std::string& eventName = tfs::lua::getString(L, 1);

	const auto& eventMap = g_globalEvents->getEventMap(GLOBALEVENT_TIMER);
	if (auto it = eventMap.find(eventName); it != eventMap.end()) {
		tfs::lua::pushBoolean(L, it->second.executeEvent());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGameGetClientVersion(lua_State* L)
{
	// Game.getClientVersion()
	lua_createtable(L, 0, 3);
	setField(L, "min", CLIENT_VERSION_MIN);
	setField(L, "max", CLIENT_VERSION_MAX);
	setField(L, "string", CLIENT_VERSION_STR);
	return 1;
}

int LuaScriptInterface::luaGameReload(lua_State* L)
{
	// Game.reload(reloadType)
	ReloadTypes_t reloadType = tfs::lua::getNumber<ReloadTypes_t>(L, 1);
	if (reloadType == RELOAD_TYPE_GLOBAL) {
		tfs::lua::pushBoolean(L, g_luaEnvironment.loadFile("data/global.lua") == 0);
		tfs::lua::pushBoolean(L, g_scripts->loadScripts("scripts/lib", true, true));
		lua_gc(g_luaEnvironment.getLuaState(), LUA_GCCOLLECT, 0);
		return 2;
	}

	tfs::lua::pushBoolean(L, g_game.reload(reloadType));
	lua_gc(g_luaEnvironment.getLuaState(), LUA_GCCOLLECT, 0);
	return 1;
}

// Variant
int LuaScriptInterface::luaVariantCreate(lua_State* L)
{
	// Variant(number or string or position or thing)
	LuaVariant variant;
	if (lua_isuserdata(L, 2)) {
		if (Thing* thing = tfs::lua::getThing(L, 2)) {
			variant.setTargetPosition(thing->getPosition());
		}
	} else if (lua_istable(L, 2)) {
		variant.setPosition(tfs::lua::getPosition(L, 2));
	} else if (isNumber(L, 2)) {
		variant.setNumber(tfs::lua::getNumber<uint32_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		variant.setString(tfs::lua::getString(L, 2));
	}
	tfs::lua::pushVariant(L, variant);
	return 1;
}

int LuaScriptInterface::luaVariantGetNumber(lua_State* L)
{
	// Variant:getNumber()
	const LuaVariant& variant = getVariant(L, 1);
	if (variant.isNumber()) {
		lua_pushnumber(L, variant.getNumber());
	} else {
		lua_pushnumber(L, 0);
	}
	return 1;
}

int LuaScriptInterface::luaVariantGetString(lua_State* L)
{
	// Variant:getString()
	const LuaVariant& variant = getVariant(L, 1);
	if (variant.isString()) {
		tfs::lua::pushString(L, variant.getString());
	} else {
		tfs::lua::pushString(L, std::string());
	}
	return 1;
}

int LuaScriptInterface::luaVariantGetPosition(lua_State* L)
{
	// Variant:getPosition()
	const LuaVariant& variant = getVariant(L, 1);
	if (variant.isPosition()) {
		tfs::lua::pushPosition(L, variant.getPosition());
	} else if (variant.isTargetPosition()) {
		tfs::lua::pushPosition(L, variant.getTargetPosition());
	} else {
		tfs::lua::pushPosition(L, Position());
	}
	return 1;
}

// Position
int LuaScriptInterface::luaPositionCreate(lua_State* L)
{
	// Position([x = 0[, y = 0[, z = 0[, stackpos = 0]]]])
	// Position([position])
	if (lua_gettop(L) <= 1) {
		tfs::lua::pushPosition(L, Position());
		return 1;
	}

	int32_t stackpos;
	if (lua_istable(L, 2)) {
		const Position& position = tfs::lua::getPosition(L, 2, stackpos);
		tfs::lua::pushPosition(L, position, stackpos);
	} else {
		uint16_t x = tfs::lua::getNumber<uint16_t>(L, 2, 0);
		uint16_t y = tfs::lua::getNumber<uint16_t>(L, 3, 0);
		uint8_t z = tfs::lua::getNumber<uint8_t>(L, 4, 0);
		stackpos = tfs::lua::getNumber<int32_t>(L, 5, 0);

		tfs::lua::pushPosition(L, Position(x, y, z), stackpos);
	}
	return 1;
}

int LuaScriptInterface::luaPositionIsSightClear(lua_State* L)
{
	// position:isSightClear(positionEx[, sameFloor = true])
	bool sameFloor = tfs::lua::getBoolean(L, 3, true);
	const Position& positionEx = tfs::lua::getPosition(L, 2);
	const Position& position = tfs::lua::getPosition(L, 1);
	tfs::lua::pushBoolean(L, g_game.isSightClear(position, positionEx, sameFloor));
	return 1;
}

int LuaScriptInterface::luaPositionSendMagicEffect(lua_State* L)
{
	// position:sendMagicEffect(magicEffect[, player = nullptr])
	SpectatorVec spectators;
	if (lua_gettop(L) >= 3) {
		Player* player = tfs::lua::getPlayer(L, 3);
		if (player) {
			spectators.insert(player);
		}
	}

	MagicEffectClasses magicEffect = tfs::lua::getNumber<MagicEffectClasses>(L, 2);
	if (magicEffect == CONST_ME_NONE) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const Position& position = tfs::lua::getPosition(L, 1);
	if (!spectators.empty()) {
		Game::addMagicEffect(spectators, position, magicEffect);
	} else {
		g_game.addMagicEffect(position, magicEffect);
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPositionSendDistanceEffect(lua_State* L)
{
	// position:sendDistanceEffect(positionEx, distanceEffect[, player = nullptr])
	SpectatorVec spectators;
	if (lua_gettop(L) >= 4) {
		Player* player = tfs::lua::getPlayer(L, 4);
		if (player) {
			spectators.insert(player);
		}
	}

	ShootType_t distanceEffect = tfs::lua::getNumber<ShootType_t>(L, 3);
	const Position& positionEx = tfs::lua::getPosition(L, 2);
	const Position& position = tfs::lua::getPosition(L, 1);
	if (!spectators.empty()) {
		Game::addDistanceEffect(spectators, position, positionEx, distanceEffect);
	} else {
		g_game.addDistanceEffect(position, positionEx, distanceEffect);
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

// Tile
int LuaScriptInterface::luaTileCreate(lua_State* L)
{
	// Tile(x, y, z)
	// Tile(position)
	Tile* tile;
	if (lua_istable(L, 2)) {
		tile = g_game.map.getTile(tfs::lua::getPosition(L, 2));
	} else {
		uint8_t z = tfs::lua::getNumber<uint8_t>(L, 4);
		uint16_t y = tfs::lua::getNumber<uint16_t>(L, 3);
		uint16_t x = tfs::lua::getNumber<uint16_t>(L, 2);
		tile = g_game.map.getTile(x, y, z);
	}

	if (tile) {
		tfs::lua::pushUserdata(L, tile);
		tfs::lua::setMetatable(L, -1, "Tile");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileRemove(lua_State* L)
{
	// tile:remove()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (g_game.isTileInCleanList(tile)) {
		g_game.removeTileToClean(tile);
	}

	g_game.map.removeTile(tile->getPosition());
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaTileGetPosition(lua_State* L)
{
	// tile:getPosition()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (tile) {
		tfs::lua::pushPosition(L, tile->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetGround(lua_State* L)
{
	// tile:getGround()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (tile && tile->getGround()) {
		tfs::lua::pushUserdata(L, tile->getGround());
		tfs::lua::setItemMetatable(L, -1, tile->getGround());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetThing(lua_State* L)
{
	// tile:getThing(index)
	int32_t index = tfs::lua::getNumber<int32_t>(L, 2);
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Thing* thing = tile->getThing(index);
	if (!thing) {
		lua_pushnil(L);
		return 1;
	}

	if (Creature* creature = thing->getCreature()) {
		tfs::lua::pushUserdata(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else if (Item* item = thing->getItem()) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetThingCount(lua_State* L)
{
	// tile:getThingCount()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (tile) {
		lua_pushnumber(L, tile->getThingCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetTopVisibleThing(lua_State* L)
{
	// tile:getTopVisibleThing(creature)
	Creature* creature = tfs::lua::getCreature(L, 2);
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Thing* thing = tile->getTopVisibleThing(creature);
	if (!thing) {
		lua_pushnil(L);
		return 1;
	}

	if (Creature* visibleCreature = thing->getCreature()) {
		tfs::lua::pushUserdata(L, visibleCreature);
		tfs::lua::setCreatureMetatable(L, -1, visibleCreature);
	} else if (Item* visibleItem = thing->getItem()) {
		tfs::lua::pushUserdata(L, visibleItem);
		tfs::lua::setItemMetatable(L, -1, visibleItem);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetTopTopItem(lua_State* L)
{
	// tile:getTopTopItem()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Item* item = tile->getTopTopItem();
	if (item) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetTopDownItem(lua_State* L)
{
	// tile:getTopDownItem()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Item* item = tile->getTopDownItem();
	if (item) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetFieldItem(lua_State* L)
{
	// tile:getFieldItem()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Item* item = tile->getFieldItem();
	if (item) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetItemById(lua_State* L)
{
	// tile:getItemById(itemId[, subType = -1])
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}
	int32_t subType = tfs::lua::getNumber<int32_t>(L, 3, -1);

	Item* item = g_game.findItemOfType(tile, itemId, false, subType);
	if (item) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetItemByType(lua_State* L)
{
	// tile:getItemByType(itemType)
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	bool found;

	ItemTypes_t itemType = tfs::lua::getNumber<ItemTypes_t>(L, 2);
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

	if (!found) {
		lua_pushnil(L);
		return 1;
	}

	if (Item* item = tile->getGround()) {
		const ItemType& it = Item::items[item->getID()];
		if (it.type == itemType) {
			tfs::lua::pushUserdata(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
			return 1;
		}
	}

	if (const TileItemVector* items = tile->getItemList()) {
		for (Item* item : *items) {
			const ItemType& it = Item::items[item->getID()];
			if (it.type == itemType) {
				tfs::lua::pushUserdata(L, item);
				tfs::lua::setItemMetatable(L, -1, item);
				return 1;
			}
		}
	}

	lua_pushnil(L);
	return 1;
}

int LuaScriptInterface::luaTileGetItemByTopOrder(lua_State* L)
{
	// tile:getItemByTopOrder(topOrder)
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	int32_t topOrder = tfs::lua::getNumber<int32_t>(L, 2);

	Item* item = tile->getItemByTopOrder(topOrder);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);
	return 1;
}

int LuaScriptInterface::luaTileGetItemCountById(lua_State* L)
{
	// tile:getItemCountById(itemId[, subType = -1])
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	int32_t subType = tfs::lua::getNumber<int32_t>(L, 3, -1);

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	lua_pushnumber(L, tile->getItemTypeCount(itemId, subType));
	return 1;
}

int LuaScriptInterface::luaTileGetBottomCreature(lua_State* L)
{
	// tile:getBottomCreature()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	const Creature* creature = tile->getBottomCreature();
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushUserdata(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);
	return 1;
}

int LuaScriptInterface::luaTileGetTopCreature(lua_State* L)
{
	// tile:getTopCreature()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = tile->getTopCreature();
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushUserdata(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);
	return 1;
}

int LuaScriptInterface::luaTileGetBottomVisibleCreature(lua_State* L)
{
	// tile:getBottomVisibleCreature(creature)
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = tfs::lua::getCreature(L, 2);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const Creature* visibleCreature = tile->getBottomVisibleCreature(creature);
	if (visibleCreature) {
		tfs::lua::pushUserdata(L, visibleCreature);
		tfs::lua::setCreatureMetatable(L, -1, visibleCreature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetTopVisibleCreature(lua_State* L)
{
	// tile:getTopVisibleCreature(creature)
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = tfs::lua::getCreature(L, 2);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Creature* visibleCreature = tile->getTopVisibleCreature(creature);
	if (visibleCreature) {
		tfs::lua::pushUserdata(L, visibleCreature);
		tfs::lua::setCreatureMetatable(L, -1, visibleCreature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetItems(lua_State* L)
{
	// tile:getItems()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
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

	int index = 0;
	for (Item* item : *itemVector) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetItemCount(lua_State* L)
{
	// tile:getItemCount()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, tile->getItemCount());
	return 1;
}

int LuaScriptInterface::luaTileGetDownItemCount(lua_State* L)
{
	// tile:getDownItemCount()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (tile) {
		lua_pushnumber(L, tile->getDownItemCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetTopItemCount(lua_State* L)
{
	// tile:getTopItemCount()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, tile->getTopItemCount());
	return 1;
}

int LuaScriptInterface::luaTileGetCreatures(lua_State* L)
{
	// tile:getCreatures()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
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

	int index = 0;
	for (Creature* creature : *creatureVector) {
		tfs::lua::pushUserdata(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaTileGetCreatureCount(lua_State* L)
{
	// tile:getCreatureCount()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, tile->getCreatureCount());
	return 1;
}

int LuaScriptInterface::luaTileHasProperty(lua_State* L)
{
	// tile:hasProperty(property[, item])
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Item* item;
	if (lua_gettop(L) >= 3) {
		item = tfs::lua::getUserdata<Item>(L, 3);
	} else {
		item = nullptr;
	}

	ITEMPROPERTY property = tfs::lua::getNumber<ITEMPROPERTY>(L, 2);
	if (item) {
		tfs::lua::pushBoolean(L, tile->hasProperty(item, property));
	} else {
		tfs::lua::pushBoolean(L, tile->hasProperty(property));
	}
	return 1;
}

int LuaScriptInterface::luaTileGetThingIndex(lua_State* L)
{
	// tile:getThingIndex(thing)
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Thing* thing = tfs::lua::getThing(L, 2);
	if (thing) {
		lua_pushnumber(L, tile->getThingIndex(thing));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileHasFlag(lua_State* L)
{
	// tile:hasFlag(flag)
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (tile) {
		tileflags_t flag = tfs::lua::getNumber<tileflags_t>(L, 2);
		tfs::lua::pushBoolean(L, tile->hasFlag(flag));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileQueryAdd(lua_State* L)
{
	// tile:queryAdd(thing[, flags])
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	Thing* thing = tfs::lua::getThing(L, 2);
	if (thing) {
		uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 3, 0);
		lua_pushnumber(L, tile->queryAdd(0, *thing, 1, flags));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileAddItem(lua_State* L)
{
	// tile:addItem(itemId[, count/subType = 1[, flags = 0]])
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	uint32_t subType = tfs::lua::getNumber<uint32_t>(L, 3, 1);

	Item* item = Item::CreateItem(itemId, std::min<uint32_t>(subType, ITEM_STACK_SIZE));
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 4, 0);

	ReturnValue ret = g_game.internalAddItem(tile, item, INDEX_WHEREEVER, flags);
	if (ret == RETURNVALUE_NOERROR) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		delete item;
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTileAddItemEx(lua_State* L)
{
	// tile:addItemEx(item[, flags = 0])
	Item* item = tfs::lua::getUserdata<Item>(L, 2);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (item->getParent() != VirtualCylinder::virtualCylinder) {
		reportErrorFunc(L, "Item already has a parent");
		lua_pushnil(L);
		return 1;
	}

	uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 3, 0);
	ReturnValue ret = g_game.internalAddItem(tile, item, INDEX_WHEREEVER, flags);
	if (ret == RETURNVALUE_NOERROR) {
		tfs::lua::removeTempItem(item);
	}
	lua_pushnumber(L, ret);
	return 1;
}

int LuaScriptInterface::luaTileGetHouse(lua_State* L)
{
	// tile:getHouse()
	Tile* tile = tfs::lua::getUserdata<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (HouseTile* houseTile = dynamic_cast<HouseTile*>(tile)) {
		tfs::lua::pushUserdata(L, houseTile->getHouse());
		tfs::lua::setMetatable(L, -1, "House");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// NetworkMessage
int LuaScriptInterface::luaNetworkMessageCreate(lua_State* L)
{
	// NetworkMessage()
	tfs::lua::pushUserdata(L, new NetworkMessage);
	tfs::lua::setMetatable(L, -1, "NetworkMessage");
	return 1;
}

int LuaScriptInterface::luaNetworkMessageDelete(lua_State* L)
{
	NetworkMessage** messagePtr = tfs::lua::getRawUserdata<NetworkMessage>(L, 1);
	if (messagePtr && *messagePtr) {
		delete *messagePtr;
		*messagePtr = nullptr;
	}
	return 0;
}

int LuaScriptInterface::luaNetworkMessageGetByte(lua_State* L)
{
	// networkMessage:getByte()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		lua_pushnumber(L, message->getByte());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageGetU16(lua_State* L)
{
	// networkMessage:getU16()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		lua_pushnumber(L, message->get<uint16_t>());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageGetU32(lua_State* L)
{
	// networkMessage:getU32()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		lua_pushnumber(L, message->get<uint32_t>());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageGetU64(lua_State* L)
{
	// networkMessage:getU64()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		lua_pushnumber(L, message->get<uint64_t>());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageGetString(lua_State* L)
{
	// networkMessage:getString()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		tfs::lua::pushString(L, message->getString());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageGetPosition(lua_State* L)
{
	// networkMessage:getPosition()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		tfs::lua::pushPosition(L, message->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageAddByte(lua_State* L)
{
	// networkMessage:addByte(number)
	uint8_t number = tfs::lua::getNumber<uint8_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addByte(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageAddU16(lua_State* L)
{
	// networkMessage:addU16(number)
	uint16_t number = tfs::lua::getNumber<uint16_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->add<uint16_t>(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageAddU32(lua_State* L)
{
	// networkMessage:addU32(number)
	uint32_t number = tfs::lua::getNumber<uint32_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->add<uint32_t>(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageAddU64(lua_State* L)
{
	// networkMessage:addU64(number)
	uint64_t number = tfs::lua::getNumber<uint64_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->add<uint64_t>(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageAddString(lua_State* L)
{
	// networkMessage:addString(string)
	const std::string& string = tfs::lua::getString(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addString(string);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageAddPosition(lua_State* L)
{
	// networkMessage:addPosition(position)
	const Position& position = tfs::lua::getPosition(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addPosition(position);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageAddDouble(lua_State* L)
{
	// networkMessage:addDouble(number)
	double number = tfs::lua::getNumber<double>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addDouble(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageAddItem(lua_State* L)
{
	// networkMessage:addItem(item)
	Item* item = tfs::lua::getUserdata<Item>(L, 2);
	if (!item) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addItem(item);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageAddItemId(lua_State* L)
{
	// networkMessage:addItemId(itemId)
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (!message) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	message->addItemId(itemId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaNetworkMessageReset(lua_State* L)
{
	// networkMessage:reset()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->reset();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageSeek(lua_State* L)
{
	// networkMessage:seek(position)
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message && isNumber(L, 2)) {
		tfs::lua::pushBoolean(L, message->setBufferPosition(tfs::lua::getNumber<uint16_t>(L, 2)));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageTell(lua_State* L)
{
	// networkMessage:tell()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		lua_pushnumber(L, message->getBufferPosition() - message->INITIAL_BUFFER_POSITION);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageLength(lua_State* L)
{
	// networkMessage:len()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		lua_pushnumber(L, message->getLength());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageSkipBytes(lua_State* L)
{
	// networkMessage:skipBytes(number)
	int16_t number = tfs::lua::getNumber<int16_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->skipBytes(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNetworkMessageSendToPlayer(lua_State* L)
{
	// networkMessage:sendToPlayer(player)
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (!message) {
		lua_pushnil(L);
		return 1;
	}

	Player* player = tfs::lua::getPlayer(L, 2);
	if (player) {
		player->sendNetworkMessage(*message);
		tfs::lua::pushBoolean(L, true);
	} else {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		lua_pushnil(L);
	}
	return 1;
}

// ModalWindow
int LuaScriptInterface::luaModalWindowCreate(lua_State* L)
{
	// ModalWindow(id, title, message)
	const std::string& message = tfs::lua::getString(L, 4);
	const std::string& title = tfs::lua::getString(L, 3);
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	tfs::lua::pushUserdata(L, new ModalWindow(id, title, message));
	tfs::lua::setMetatable(L, -1, "ModalWindow");
	return 1;
}

int LuaScriptInterface::luaModalWindowDelete(lua_State* L)
{
	ModalWindow** windowPtr = tfs::lua::getRawUserdata<ModalWindow>(L, 1);
	if (windowPtr && *windowPtr) {
		delete *windowPtr;
		*windowPtr = nullptr;
	}
	return 0;
}

int LuaScriptInterface::luaModalWindowGetId(lua_State* L)
{
	// modalWindow:getId()
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		lua_pushnumber(L, window->id);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowGetTitle(lua_State* L)
{
	// modalWindow:getTitle()
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		tfs::lua::pushString(L, window->title);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowGetMessage(lua_State* L)
{
	// modalWindow:getMessage()
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		tfs::lua::pushString(L, window->message);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowSetTitle(lua_State* L)
{
	// modalWindow:setTitle(text)
	const std::string& text = tfs::lua::getString(L, 2);
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->title = text;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowSetMessage(lua_State* L)
{
	// modalWindow:setMessage(text)
	const std::string& text = tfs::lua::getString(L, 2);
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->message = text;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowGetButtonCount(lua_State* L)
{
	// modalWindow:getButtonCount()
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		lua_pushnumber(L, window->buttons.size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowGetChoiceCount(lua_State* L)
{
	// modalWindow:getChoiceCount()
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		lua_pushnumber(L, window->choices.size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowAddButton(lua_State* L)
{
	// modalWindow:addButton(id, text)
	const std::string& text = tfs::lua::getString(L, 3);
	uint8_t id = tfs::lua::getNumber<uint8_t>(L, 2);
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->buttons.emplace_back(text, id);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowAddChoice(lua_State* L)
{
	// modalWindow:addChoice(id, text)
	const std::string& text = tfs::lua::getString(L, 3);
	uint8_t id = tfs::lua::getNumber<uint8_t>(L, 2);
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->choices.emplace_back(text, id);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowGetDefaultEnterButton(lua_State* L)
{
	// modalWindow:getDefaultEnterButton()
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		lua_pushnumber(L, window->defaultEnterButton);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowSetDefaultEnterButton(lua_State* L)
{
	// modalWindow:setDefaultEnterButton(buttonId)
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->defaultEnterButton = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowGetDefaultEscapeButton(lua_State* L)
{
	// modalWindow:getDefaultEscapeButton()
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		lua_pushnumber(L, window->defaultEscapeButton);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowSetDefaultEscapeButton(lua_State* L)
{
	// modalWindow:setDefaultEscapeButton(buttonId)
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->defaultEscapeButton = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowHasPriority(lua_State* L)
{
	// modalWindow:hasPriority()
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		tfs::lua::pushBoolean(L, window->priority);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowSetPriority(lua_State* L)
{
	// modalWindow:setPriority(priority)
	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		window->priority = tfs::lua::getBoolean(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaModalWindowSendToPlayer(lua_State* L)
{
	// modalWindow:sendToPlayer(player)
	Player* player = tfs::lua::getPlayer(L, 2);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	ModalWindow* window = tfs::lua::getUserdata<ModalWindow>(L, 1);
	if (window) {
		if (!player->hasModalWindowOpen(window->id)) {
			player->sendModalWindow(*window);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Item
int LuaScriptInterface::luaItemCreate(lua_State* L)
{
	// Item(uid)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	Item* item = tfs::lua::getScriptEnv()->getItemByUID(id);
	if (item) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemIsItem(lua_State* L)
{
	// item:isItem()
	tfs::lua::pushBoolean(L, tfs::lua::getUserdata<const Item>(L, 1) != nullptr);
	return 1;
}

int LuaScriptInterface::luaItemHasParent(lua_State* L)
{
	// item:hasParent()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, item->hasParent());
	return 1;
}

int LuaScriptInterface::luaItemGetParent(lua_State* L)
{
	// item:getParent()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Cylinder* parent = item->getParent();
	if (!parent) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushCylinder(L, parent);
	return 1;
}

int LuaScriptInterface::luaItemGetTopParent(lua_State* L)
{
	// item:getTopParent()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Cylinder* topParent = item->getTopParent();
	if (!topParent) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushCylinder(L, topParent);
	return 1;
}

int LuaScriptInterface::luaItemGetId(lua_State* L)
{
	// item:getId()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemClone(lua_State* L)
{
	// item:clone()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Item* clone = item->clone();
	if (!clone) {
		lua_pushnil(L);
		return 1;
	}

	addTempItem(clone);
	clone->setParent(VirtualCylinder::virtualCylinder);

	tfs::lua::pushUserdata(L, clone);
	tfs::lua::setItemMetatable(L, -1, clone);
	return 1;
}

int LuaScriptInterface::luaItemSplit(lua_State* L)
{
	// item:split([count = 1])
	Item** itemPtr = tfs::lua::getRawUserdata<Item>(L, 1);
	if (!itemPtr) {
		lua_pushnil(L);
		return 1;
	}

	Item* item = *itemPtr;
	if (!item || !item->isStackable()) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t count = std::min<uint16_t>(tfs::lua::getNumber<uint16_t>(L, 2, 1), item->getItemCount());
	uint16_t diff = item->getItemCount() - count;

	Item* splitItem = item->clone();
	if (!splitItem) {
		lua_pushnil(L);
		return 1;
	}

	splitItem->setItemCount(count);

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	uint32_t uid = env->addThing(item);

	Item* newItem = g_game.transformItem(item, item->getID(), diff);
	if (item->isRemoved()) {
		env->removeItemByUID(uid);
	}

	if (newItem && newItem != item) {
		env->insertItem(uid, newItem);
	}

	*itemPtr = newItem;

	splitItem->setParent(VirtualCylinder::virtualCylinder);
	addTempItem(splitItem);

	tfs::lua::pushUserdata(L, splitItem);
	tfs::lua::setItemMetatable(L, -1, splitItem);
	return 1;
}

int LuaScriptInterface::luaItemRemove(lua_State* L)
{
	// item:remove([count = -1])
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		int32_t count = tfs::lua::getNumber<int32_t>(L, 2, -1);
		tfs::lua::pushBoolean(L, g_game.internalRemoveItem(item, count) == RETURNVALUE_NOERROR);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetUniqueId(lua_State* L)
{
	// item:getUniqueId()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		uint32_t uniqueId = item->getUniqueId();
		if (uniqueId == 0) {
			uniqueId = tfs::lua::getScriptEnv()->addThing(item);
		}
		lua_pushnumber(L, uniqueId);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetActionId(lua_State* L)
{
	// item:getActionId()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getActionId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemSetActionId(lua_State* L)
{
	// item:setActionId(actionId)
	uint16_t actionId = tfs::lua::getNumber<uint16_t>(L, 2);
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		item->setActionId(actionId);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetCount(lua_State* L)
{
	// item:getCount()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getItemCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetCharges(lua_State* L)
{
	// item:getCharges()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getCharges());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetFluidType(lua_State* L)
{
	// item:getFluidType()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getFluidType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetWeight(lua_State* L)
{
	// item:getWeight()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getWeight());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetWorth(lua_State* L)
{
	// item:getWorth()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getWorth());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetSubType(lua_State* L)
{
	// item:getSubType()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(L, item->getSubType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetName(lua_State* L)
{
	// item:getName()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		tfs::lua::pushString(L, item->getName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetPluralName(lua_State* L)
{
	// item:getPluralName()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		tfs::lua::pushString(L, item->getPluralName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetArticle(lua_State* L)
{
	// item:getArticle()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		tfs::lua::pushString(L, item->getArticle());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetPosition(lua_State* L)
{
	// item:getPosition()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		tfs::lua::pushPosition(L, item->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetTile(lua_State* L)
{
	// item:getTile()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Tile* tile = item->getTile();
	if (tile) {
		tfs::lua::pushUserdata(L, tile);
		tfs::lua::setMetatable(L, -1, "Tile");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemHasAttribute(lua_State* L)
{
	// item:hasAttribute(key)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	itemAttrTypes attribute;
	if (isNumber(L, 2)) {
		attribute = tfs::lua::getNumber<itemAttrTypes>(L, 2);
	} else if (lua_isstring(L, 2)) {
		attribute = stringToItemAttribute(tfs::lua::getString(L, 2));
	} else {
		attribute = ITEM_ATTRIBUTE_NONE;
	}

	tfs::lua::pushBoolean(L, item->hasAttribute(attribute));
	return 1;
}

int LuaScriptInterface::luaItemGetAttribute(lua_State* L)
{
	// item:getAttribute(key)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	itemAttrTypes attribute;
	if (isNumber(L, 2)) {
		attribute = tfs::lua::getNumber<itemAttrTypes>(L, 2);
	} else if (lua_isstring(L, 2)) {
		attribute = stringToItemAttribute(tfs::lua::getString(L, 2));
	} else {
		attribute = ITEM_ATTRIBUTE_NONE;
	}

	if (ItemAttributes::isIntAttrType(attribute)) {
		lua_pushnumber(L, item->getIntAttr(attribute));
	} else if (ItemAttributes::isStrAttrType(attribute)) {
		tfs::lua::pushString(L, item->getStrAttr(attribute));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemSetAttribute(lua_State* L)
{
	// item:setAttribute(key, value)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	itemAttrTypes attribute;
	if (isNumber(L, 2)) {
		attribute = tfs::lua::getNumber<itemAttrTypes>(L, 2);
	} else if (lua_isstring(L, 2)) {
		attribute = stringToItemAttribute(tfs::lua::getString(L, 2));
	} else {
		attribute = ITEM_ATTRIBUTE_NONE;
	}

	if (ItemAttributes::isIntAttrType(attribute)) {
		if (attribute == ITEM_ATTRIBUTE_UNIQUEID) {
			reportErrorFunc(L, "Attempt to set protected key \"uid\"");
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		item->setIntAttr(attribute, tfs::lua::getNumber<int32_t>(L, 3));
		tfs::lua::pushBoolean(L, true);
	} else if (ItemAttributes::isStrAttrType(attribute)) {
		item->setStrAttr(attribute, tfs::lua::getString(L, 3));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemRemoveAttribute(lua_State* L)
{
	// item:removeAttribute(key)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	itemAttrTypes attribute;
	if (isNumber(L, 2)) {
		attribute = tfs::lua::getNumber<itemAttrTypes>(L, 2);
	} else if (lua_isstring(L, 2)) {
		attribute = stringToItemAttribute(tfs::lua::getString(L, 2));
	} else {
		attribute = ITEM_ATTRIBUTE_NONE;
	}

	bool ret = attribute != ITEM_ATTRIBUTE_UNIQUEID;
	if (ret) {
		item->removeAttribute(attribute);
	} else {
		reportErrorFunc(L, "Attempt to erase protected key \"uid\"");
	}
	tfs::lua::pushBoolean(L, ret);
	return 1;
}

int LuaScriptInterface::luaItemGetCustomAttribute(lua_State* L)
{
	// item:getCustomAttribute(key)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	const ItemAttributes::CustomAttribute* attr;
	if (isNumber(L, 2)) {
		attr = item->getCustomAttribute(tfs::lua::getNumber<int64_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		attr = item->getCustomAttribute(tfs::lua::getString(L, 2));
	} else {
		lua_pushnil(L);
		return 1;
	}

	if (attr) {
		attr->pushToLua(L);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemSetCustomAttribute(lua_State* L)
{
	// item:setCustomAttribute(key, value)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	std::string key;
	if (isNumber(L, 2)) {
		key = std::to_string(tfs::lua::getNumber<int64_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		key = tfs::lua::getString(L, 2);
	} else {
		lua_pushnil(L);
		return 1;
	}

	ItemAttributes::CustomAttribute val;
	if (isNumber(L, 3)) {
		double tmp = tfs::lua::getNumber<double>(L, 3);
		if (std::floor(tmp) < tmp) {
			val.set<double>(tmp);
		} else {
			val.set<int64_t>(tmp);
		}
	} else if (lua_isstring(L, 3)) {
		val.set<std::string>(tfs::lua::getString(L, 3));
	} else if (lua_isboolean(L, 3)) {
		val.set<bool>(tfs::lua::getBoolean(L, 3));
	} else {
		lua_pushnil(L);
		return 1;
	}

	item->setCustomAttribute(key, val);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaItemRemoveCustomAttribute(lua_State* L)
{
	// item:removeCustomAttribute(key)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	if (isNumber(L, 2)) {
		tfs::lua::pushBoolean(L, item->removeCustomAttribute(tfs::lua::getNumber<int64_t>(L, 2)));
	} else if (lua_isstring(L, 2)) {
		tfs::lua::pushBoolean(L, item->removeCustomAttribute(tfs::lua::getString(L, 2)));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemMoveTo(lua_State* L)
{
	// item:moveTo(position or cylinder[, flags])
	Item** itemPtr = tfs::lua::getRawUserdata<Item>(L, 1);
	if (!itemPtr) {
		lua_pushnil(L);
		return 1;
	}

	Item* item = *itemPtr;
	if (!item || item->isRemoved()) {
		lua_pushnil(L);
		return 1;
	}

	Cylinder* toCylinder;
	if (lua_isuserdata(L, 2)) {
		const LuaDataType type = getUserdataType(L, 2);
		switch (type) {
			case LuaData_Container:
				toCylinder = tfs::lua::getUserdata<Container>(L, 2);
				break;
			case LuaData_Player:
				toCylinder = tfs::lua::getUserdata<Player>(L, 2);
				break;
			case LuaData_Tile:
				toCylinder = tfs::lua::getUserdata<Tile>(L, 2);
				break;
			default:
				toCylinder = nullptr;
				break;
		}
	} else {
		toCylinder = g_game.map.getTile(tfs::lua::getPosition(L, 2));
	}

	if (!toCylinder) {
		lua_pushnil(L);
		return 1;
	}

	if (item->getParent() == toCylinder) {
		tfs::lua::pushBoolean(L, true);
		return 1;
	}

	uint32_t flags = tfs::lua::getNumber<uint32_t>(
	    L, 3, FLAG_NOLIMIT | FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE | FLAG_IGNORENOTMOVEABLE);

	if (item->getParent() == VirtualCylinder::virtualCylinder) {
		tfs::lua::pushBoolean(L,
		                      g_game.internalAddItem(toCylinder, item, INDEX_WHEREEVER, flags) == RETURNVALUE_NOERROR);
	} else {
		Item* moveItem = nullptr;
		ReturnValue ret = g_game.internalMoveItem(item->getParent(), toCylinder, INDEX_WHEREEVER, item,
		                                          item->getItemCount(), &moveItem, flags);
		if (moveItem) {
			*itemPtr = moveItem;
		}
		tfs::lua::pushBoolean(L, ret == RETURNVALUE_NOERROR);
	}
	return 1;
}

int LuaScriptInterface::luaItemTransform(lua_State* L)
{
	// item:transform(itemId[, count/subType = -1])
	Item** itemPtr = tfs::lua::getRawUserdata<Item>(L, 1);
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
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t subType = tfs::lua::getNumber<int32_t>(L, 3, -1);
	if (item->getID() == itemId && (subType == -1 || subType == item->getSubType())) {
		tfs::lua::pushBoolean(L, true);
		return 1;
	}

	const ItemType& it = Item::items[itemId];
	if (it.stackable) {
		subType = std::min<int32_t>(subType, ITEM_STACK_SIZE);
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	uint32_t uid = env->addThing(item);

	Item* newItem = g_game.transformItem(item, itemId, subType);
	if (item->isRemoved()) {
		env->removeItemByUID(uid);
	}

	if (newItem && newItem != item) {
		env->insertItem(uid, newItem);
	}

	item = newItem;
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaItemDecay(lua_State* L)
{
	// item:decay(decayId)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		if (isNumber(L, 2)) {
			item->setDecayTo(tfs::lua::getNumber<int32_t>(L, 2));
		}

		g_game.startDecay(item);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemGetSpecialDescription(lua_State* L)
{
	// item:getSpecialDescription()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		tfs::lua::pushString(L, item->getSpecialDescription());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemHasProperty(lua_State* L)
{
	// item:hasProperty(property)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		ITEMPROPERTY property = tfs::lua::getNumber<ITEMPROPERTY>(L, 2);
		tfs::lua::pushBoolean(L, item->hasProperty(property));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemIsLoadedFromMap(lua_State* L)
{
	// item:isLoadedFromMap()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		tfs::lua::pushBoolean(L, item->isLoadedFromMap());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemSetStoreItem(lua_State* L)
{
	// item:setStoreItem(storeItem)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	item->setStoreItem(tfs::lua::getBoolean(L, 2, false));
	return 1;
}

int LuaScriptInterface::luaItemIsStoreItem(lua_State* L)
{
	// item:isStoreItem()
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		tfs::lua::pushBoolean(L, item->isStoreItem());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemSetReflect(lua_State* L)
{
	// item:setReflect(combatType, reflect)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Reflect reflect{
	    tfs::lua::getField<uint16_t>(L, 3, "percent"),
	    tfs::lua::getField<uint16_t>(L, 3, "chance"),
	};
	lua_pop(L, 2);

	item->setReflect(tfs::lua::getNumber<CombatType_t>(L, 2), reflect);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaItemGetReflect(lua_State* L)
{
	// item:getReflect(combatType[, total = true])
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		const Reflect& reflect =
		    item->getReflect(tfs::lua::getNumber<CombatType_t>(L, 2), tfs::lua::getBoolean(L, 3, true));

		lua_createtable(L, 0, 2);
		setField(L, "percent", reflect.percent);
		setField(L, "chance", reflect.chance);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemSetBoostPercent(lua_State* L)
{
	// item:setBoostPercent(combatType, percent)
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	item->setBoostPercent(tfs::lua::getNumber<CombatType_t>(L, 2), tfs::lua::getNumber<uint16_t>(L, 3));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaItemGetBoostPercent(lua_State* L)
{
	// item:getBoostPercent(combatType[, total = true])
	Item* item = tfs::lua::getUserdata<Item>(L, 1);
	if (item) {
		lua_pushnumber(
		    L, item->getBoostPercent(tfs::lua::getNumber<CombatType_t>(L, 2), tfs::lua::getBoolean(L, 3, true)));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Container
int LuaScriptInterface::luaContainerCreate(lua_State* L)
{
	// Container(uid)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	Container* container = tfs::lua::getScriptEnv()->getContainerByUID(id);
	if (container) {
		tfs::lua::pushUserdata(L, container);
		tfs::lua::setMetatable(L, -1, "Container");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaContainerGetSize(lua_State* L)
{
	// container:getSize()
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (container) {
		lua_pushnumber(L, container->size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaContainerGetCapacity(lua_State* L)
{
	// container:getCapacity()
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (container) {
		lua_pushnumber(L, container->capacity());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaContainerGetEmptySlots(lua_State* L)
{
	// container:getEmptySlots([recursive = false])
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t slots = container->capacity() - container->size();
	bool recursive = tfs::lua::getBoolean(L, 2, false);
	if (recursive) {
		for (ContainerIterator it = container->iterator(); it.hasNext(); it.advance()) {
			if (Container* tmpContainer = (*it)->getContainer()) {
				slots += tmpContainer->capacity() - tmpContainer->size();
			}
		}
	}
	lua_pushnumber(L, slots);
	return 1;
}

int LuaScriptInterface::luaContainerGetItemHoldingCount(lua_State* L)
{
	// container:getItemHoldingCount()
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (container) {
		lua_pushnumber(L, container->getItemHoldingCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaContainerGetItem(lua_State* L)
{
	// container:getItem(index)
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t index = tfs::lua::getNumber<uint32_t>(L, 2);
	Item* item = container->getItemByIndex(index);
	if (item) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaContainerHasItem(lua_State* L)
{
	// container:hasItem(item)
	Item* item = tfs::lua::getUserdata<Item>(L, 2);
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (container) {
		tfs::lua::pushBoolean(L, container->isHoldingItem(item));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaContainerAddItem(lua_State* L)
{
	// container:addItem(itemId[, count/subType = 1[, index = INDEX_WHEREEVER[, flags = 0]]])
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	const ItemType& it = Item::items[itemId];

	int32_t itemCount = 1;
	int32_t subType = 1;
	uint32_t count = tfs::lua::getNumber<uint32_t>(L, 3, 1);

	if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = std::ceil(count / static_cast<float>(ITEM_STACK_SIZE));
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

	int32_t index = tfs::lua::getNumber<int32_t>(L, 4, INDEX_WHEREEVER);
	uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 5, 0);

	for (int32_t i = 1; i <= itemCount; ++i) {
		int32_t stackCount = std::min<int32_t>(subType, ITEM_STACK_SIZE);
		Item* item = Item::CreateItem(itemId, stackCount);
		if (!item) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
			if (!hasTable) {
				lua_pushnil(L);
			}
			return 1;
		}

		if (it.stackable) {
			subType -= stackCount;
		}

		ReturnValue ret = g_game.internalAddItem(container, item, index, flags);
		if (ret != RETURNVALUE_NOERROR) {
			delete item;
			if (!hasTable) {
				lua_pushnil(L);
			}
			return 1;
		}

		if (hasTable) {
			lua_pushnumber(L, i);
			tfs::lua::pushUserdata(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
			lua_settable(L, -3);
		} else {
			tfs::lua::pushUserdata(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
		}
	}
	return 1;
}

int LuaScriptInterface::luaContainerAddItemEx(lua_State* L)
{
	// container:addItemEx(item[, index = INDEX_WHEREEVER[, flags = 0]])
	Item* item = tfs::lua::getUserdata<Item>(L, 2);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	if (item->getParent() != VirtualCylinder::virtualCylinder) {
		reportErrorFunc(L, "Item already has a parent");
		lua_pushnil(L);
		return 1;
	}

	int32_t index = tfs::lua::getNumber<int32_t>(L, 3, INDEX_WHEREEVER);
	uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 4, 0);
	ReturnValue ret = g_game.internalAddItem(container, item, index, flags);
	if (ret == RETURNVALUE_NOERROR) {
		tfs::lua::removeTempItem(item);
	}
	lua_pushnumber(L, ret);
	return 1;
}

int LuaScriptInterface::luaContainerGetCorpseOwner(lua_State* L)
{
	// container:getCorpseOwner()
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (container) {
		lua_pushnumber(L, container->getCorpseOwner());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaContainerGetItemCountById(lua_State* L)
{
	// container:getItemCountById(itemId[, subType = -1])
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t subType = tfs::lua::getNumber<int32_t>(L, 3, -1);
	lua_pushnumber(L, container->getItemTypeCount(itemId, subType));
	return 1;
}

int LuaScriptInterface::luaContainerGetItems(lua_State* L)
{
	// container:getItems([recursive = false])
	Container* container = tfs::lua::getUserdata<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	bool recursive = tfs::lua::getBoolean(L, 2, false);
	std::vector<Item*> items = container->getItems(recursive);

	lua_createtable(L, items.size(), 0);

	int index = 0;
	for (Item* item : items) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

// Teleport
int LuaScriptInterface::luaTeleportCreate(lua_State* L)
{
	// Teleport(uid)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	Item* item = tfs::lua::getScriptEnv()->getItemByUID(id);
	if (item && item->getTeleport()) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setMetatable(L, -1, "Teleport");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTeleportGetDestination(lua_State* L)
{
	// teleport:getDestination()
	Teleport* teleport = tfs::lua::getUserdata<Teleport>(L, 1);
	if (teleport) {
		tfs::lua::pushPosition(L, teleport->getDestPos());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTeleportSetDestination(lua_State* L)
{
	// teleport:setDestination(position)
	Teleport* teleport = tfs::lua::getUserdata<Teleport>(L, 1);
	if (teleport) {
		teleport->setDestPos(tfs::lua::getPosition(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Podium
int LuaScriptInterface::luaPodiumCreate(lua_State* L)
{
	// Podium(uid)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	Item* item = tfs::lua::getScriptEnv()->getItemByUID(id);
	if (item && item->getPodium()) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setMetatable(L, -1, "Podium");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPodiumGetOutfit(lua_State* L)
{
	// podium:getOutfit()
	const Podium* podium = tfs::lua::getUserdata<const Podium>(L, 1);
	if (podium) {
		tfs::lua::pushOutfit(L, podium->getOutfit());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPodiumSetOutfit(lua_State* L)
{
	// podium:setOutfit(outfit)
	Podium* podium = tfs::lua::getUserdata<Podium>(L, 1);
	if (podium) {
		podium->setOutfit(getOutfit(L, 2));
		g_game.updatePodium(podium);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPodiumHasFlag(lua_State* L)
{
	// podium:hasFlag(flag)
	Podium* podium = tfs::lua::getUserdata<Podium>(L, 1);
	if (podium) {
		PodiumFlags flag = tfs::lua::getNumber<PodiumFlags>(L, 2);
		tfs::lua::pushBoolean(L, podium->hasFlag(flag));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPodiumSetFlag(lua_State* L)
{
	// podium:setFlag(flag, value)
	uint8_t value = tfs::lua::getBoolean(L, 3);
	PodiumFlags flag = tfs::lua::getNumber<PodiumFlags>(L, 2);
	Podium* podium = tfs::lua::getUserdata<Podium>(L, 1);

	if (podium) {
		podium->setFlagValue(flag, value);
		g_game.updatePodium(podium);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPodiumGetDirection(lua_State* L)
{
	// podium:getDirection()
	const Podium* podium = tfs::lua::getUserdata<const Podium>(L, 1);
	if (podium) {
		lua_pushnumber(L, podium->getDirection());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPodiumSetDirection(lua_State* L)
{
	// podium:setDirection(direction)
	Podium* podium = tfs::lua::getUserdata<Podium>(L, 1);
	if (podium) {
		podium->setDirection(tfs::lua::getNumber<Direction>(L, 2));
		g_game.updatePodium(podium);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Creature
int LuaScriptInterface::luaCreatureCreate(lua_State* L)
{
	// Creature(id or name or userdata)
	Creature* creature;
	if (isNumber(L, 2)) {
		creature = g_game.getCreatureByID(tfs::lua::getNumber<uint32_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		creature = g_game.getCreatureByName(tfs::lua::getString(L, 2));
	} else if (lua_isuserdata(L, 2)) {
		LuaDataType type = getUserdataType(L, 2);
		if (type != LuaData_Player && type != LuaData_Monster && type != LuaData_Npc) {
			lua_pushnil(L);
			return 1;
		}
		creature = tfs::lua::getUserdata<Creature>(L, 2);
	} else {
		creature = nullptr;
	}

	if (creature) {
		tfs::lua::pushUserdata(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetEvents(lua_State* L)
{
	// creature:getEvents(type)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	CreatureEventType_t eventType = tfs::lua::getNumber<CreatureEventType_t>(L, 2);
	const auto& eventList = creature->getCreatureEvents(eventType);
	lua_createtable(L, eventList.size(), 0);

	int index = 0;
	for (CreatureEvent* event : eventList) {
		tfs::lua::pushString(L, event->getName());
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureRegisterEvent(lua_State* L)
{
	// creature:registerEvent(name)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		const std::string& name = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, creature->registerCreatureEvent(name));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureUnregisterEvent(lua_State* L)
{
	// creature:unregisterEvent(name)
	const std::string& name = tfs::lua::getString(L, 2);
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		tfs::lua::pushBoolean(L, creature->unregisterCreatureEvent(name));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureIsRemoved(lua_State* L)
{
	// creature:isRemoved()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		tfs::lua::pushBoolean(L, creature->isRemoved());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureIsCreature(lua_State* L)
{
	// creature:isCreature()
	tfs::lua::pushBoolean(L, tfs::lua::getUserdata<const Creature>(L, 1) != nullptr);
	return 1;
}

int LuaScriptInterface::luaCreatureIsInGhostMode(lua_State* L)
{
	// creature:isInGhostMode()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		tfs::lua::pushBoolean(L, creature->isInGhostMode());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureIsHealthHidden(lua_State* L)
{
	// creature:isHealthHidden()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		tfs::lua::pushBoolean(L, creature->isHealthHidden());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureIsMovementBlocked(lua_State* L)
{
	// creature:isMovementBlocked()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		tfs::lua::pushBoolean(L, creature->isMovementBlocked());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureCanSee(lua_State* L)
{
	// creature:canSee(position)
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		const Position& position = tfs::lua::getPosition(L, 2);
		tfs::lua::pushBoolean(L, creature->canSee(position));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureCanSeeCreature(lua_State* L)
{
	// creature:canSeeCreature(creature)
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		const Creature* otherCreature = tfs::lua::getCreature(L, 2);
		if (!otherCreature) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, creature->canSeeCreature(otherCreature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureCanSeeGhostMode(lua_State* L)
{
	// creature:canSeeGhostMode(creature)
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		const Creature* otherCreature = tfs::lua::getCreature(L, 2);
		if (!otherCreature) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, creature->canSeeGhostMode(otherCreature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureCanSeeInvisibility(lua_State* L)
{
	// creature:canSeeInvisibility()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		tfs::lua::pushBoolean(L, creature->canSeeInvisibility());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureHasParent(lua_State* L)
{
	// creature:hasParent()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, creature->hasParent());
	return 1;
}

int LuaScriptInterface::luaCreatureGetParent(lua_State* L)
{
	// creature:getParent()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Cylinder* parent = creature->getParent();
	if (!parent) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushCylinder(L, parent);
	return 1;
}

int LuaScriptInterface::luaCreatureGetId(lua_State* L)
{
	// creature:getId()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetName(lua_State* L)
{
	// creature:getName()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		tfs::lua::pushString(L, creature->getName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetTarget(lua_State* L)
{
	// creature:getTarget()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Creature* target = creature->getAttackedCreature();
	if (target) {
		tfs::lua::pushUserdata(L, target);
		tfs::lua::setCreatureMetatable(L, -1, target);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetTarget(lua_State* L)
{
	// creature:setTarget(target)
	auto creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);

		return 1;
	}

	auto target = tfs::lua::getCreature(L, 2);
	if (target) {
		creature->setAttackedCreature(target);
		tfs::lua::pushBoolean(L, creature->canAttackCreature(target));
	} else {
		creature->removeAttackedCreature();
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetFollowCreature(lua_State* L)
{
	// creature:getFollowCreature()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Creature* followCreature = creature->getFollowCreature();
	if (followCreature) {
		tfs::lua::pushUserdata(L, followCreature);
		tfs::lua::setCreatureMetatable(L, -1, followCreature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetFollowCreature(lua_State* L)
{
	// creature:setFollowCreature(followedCreature)
	auto creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	auto followedCreature = tfs::lua::getCreature(L, 2);
	if (followedCreature) {
		creature->setFollowCreature(followedCreature);
		tfs::lua::pushBoolean(L, creature->canFollowCreature(followedCreature));
	} else {
		creature->removeFollowCreature();
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetMaster(lua_State* L)
{
	// creature:getMaster()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Creature* master = creature->getMaster();
	if (!master) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushUserdata(L, master);
	tfs::lua::setCreatureMetatable(L, -1, master);
	return 1;
}

int LuaScriptInterface::luaCreatureSetMaster(lua_State* L)
{
	// creature:setMaster(master)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, creature->setMaster(tfs::lua::getCreature(L, 2)));

	// update summon icon
	g_game.updateKnownCreature(creature);
	return 1;
}

int LuaScriptInterface::luaCreatureGetLight(lua_State* L)
{
	// creature:getLight()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	LightInfo lightInfo = creature->getCreatureLight();
	lua_pushnumber(L, lightInfo.level);
	lua_pushnumber(L, lightInfo.color);
	return 2;
}

int LuaScriptInterface::luaCreatureSetLight(lua_State* L)
{
	// creature:setLight(color, level)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	LightInfo light;
	light.color = tfs::lua::getNumber<uint8_t>(L, 2);
	light.level = tfs::lua::getNumber<uint8_t>(L, 3);
	creature->setCreatureLight(light);
	g_game.changeLight(creature);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCreatureGetSpeed(lua_State* L)
{
	// creature:getSpeed()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetBaseSpeed(lua_State* L)
{
	// creature:getBaseSpeed()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getBaseSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureChangeSpeed(lua_State* L)
{
	// creature:changeSpeed(delta)
	Creature* creature = tfs::lua::getCreature(L, 1);
	if (!creature) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	int32_t delta = tfs::lua::getNumber<int32_t>(L, 2);
	g_game.changeSpeed(creature, delta);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCreatureSetDropLoot(lua_State* L)
{
	// creature:setDropLoot(doDrop)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		creature->setDropLoot(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetSkillLoss(lua_State* L)
{
	// creature:setSkillLoss(skillLoss)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		creature->setSkillLoss(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetPosition(lua_State* L)
{
	// creature:getPosition()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		tfs::lua::pushPosition(L, creature->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetTile(lua_State* L)
{
	// creature:getTile()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Tile* tile = creature->getTile();
	if (tile) {
		tfs::lua::pushUserdata(L, tile);
		tfs::lua::setMetatable(L, -1, "Tile");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetDirection(lua_State* L)
{
	// creature:getDirection()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getDirection());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetDirection(lua_State* L)
{
	// creature:setDirection(direction)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		tfs::lua::pushBoolean(L, g_game.internalCreatureTurn(creature, tfs::lua::getNumber<Direction>(L, 2)));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetHealth(lua_State* L)
{
	// creature:getHealth()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getHealth());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetHealth(lua_State* L)
{
	// creature:setHealth(health)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	creature->health = std::min<int32_t>(tfs::lua::getNumber<uint32_t>(L, 2), creature->healthMax);
	g_game.addCreatureHealth(creature);

	Player* player = creature->getPlayer();
	if (player) {
		player->sendStats();
	}
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCreatureAddHealth(lua_State* L)
{
	// creature:addHealth(healthChange)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	CombatDamage damage;
	damage.primary.value = tfs::lua::getNumber<int32_t>(L, 2);
	if (damage.primary.value >= 0) {
		damage.primary.type = COMBAT_HEALING;
	} else {
		damage.primary.type = COMBAT_UNDEFINEDDAMAGE;
	}
	tfs::lua::pushBoolean(L, g_game.combatChangeHealth(nullptr, creature, damage));
	return 1;
}

int LuaScriptInterface::luaCreatureGetMaxHealth(lua_State* L)
{
	// creature:getMaxHealth()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getMaxHealth());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetMaxHealth(lua_State* L)
{
	// creature:setMaxHealth(maxHealth)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	creature->healthMax = tfs::lua::getNumber<uint32_t>(L, 2);
	creature->health = std::min<int32_t>(creature->health, creature->healthMax);
	g_game.addCreatureHealth(creature);

	Player* player = creature->getPlayer();
	if (player) {
		player->sendStats();
	}
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCreatureSetHiddenHealth(lua_State* L)
{
	// creature:setHiddenHealth(hide)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		creature->setHiddenHealth(tfs::lua::getBoolean(L, 2));
		g_game.addCreatureHealth(creature);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetMovementBlocked(lua_State* L)
{
	// creature:setMovementBlocked(state)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		creature->setMovementBlocked(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetSkull(lua_State* L)
{
	// creature:getSkull()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getSkull());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetSkull(lua_State* L)
{
	// creature:setSkull(skull)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		creature->setSkull(tfs::lua::getNumber<Skulls_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetOutfit(lua_State* L)
{
	// creature:getOutfit()
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		tfs::lua::pushOutfit(L, creature->getCurrentOutfit());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetOutfit(lua_State* L)
{
	// creature:setOutfit(outfit)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		creature->defaultOutfit = getOutfit(L, 2);
		g_game.internalCreatureChangeOutfit(creature, creature->defaultOutfit);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetCondition(lua_State* L)
{
	// creature:getCondition(conditionType[, conditionId = CONDITIONID_COMBAT[, subId = 0]])
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	ConditionType_t conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
	ConditionId_t conditionId = tfs::lua::getNumber<ConditionId_t>(L, 3, CONDITIONID_COMBAT);
	uint32_t subId = tfs::lua::getNumber<uint32_t>(L, 4, 0);

	Condition* condition = creature->getCondition(conditionType, conditionId, subId);
	if (condition) {
		tfs::lua::pushUserdata(L, condition);
		setWeakMetatable(L, -1, "Condition");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureAddCondition(lua_State* L)
{
	// creature:addCondition(condition[, force = false])
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 2);
	if (creature && condition) {
		bool force = tfs::lua::getBoolean(L, 3, false);
		tfs::lua::pushBoolean(L, creature->addCondition(condition->clone(), force));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureRemoveCondition(lua_State* L)
{
	// creature:removeCondition(conditionType[, conditionId = CONDITIONID_COMBAT[, subId = 0[, force = false]]])
	// creature:removeCondition(condition[, force = false])
	Creature* const creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Condition* creatureCondition = nullptr;
	bool force = false;

	if (lua_isuserdata(L, 2)) {
		const Condition* const condition = tfs::lua::getUserdata<Condition>(L, 2);
		const ConditionType_t conditionType = condition->getType();
		const ConditionId_t conditionId = condition->getId();
		const uint32_t subId = condition->getSubId();
		creatureCondition = creature->getCondition(conditionType, conditionId, subId);
		force = tfs::lua::getBoolean(L, 3, false);
	} else {
		const ConditionType_t conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
		const ConditionId_t conditionId = tfs::lua::getNumber<ConditionId_t>(L, 3, CONDITIONID_COMBAT);
		const uint32_t subId = tfs::lua::getNumber<uint32_t>(L, 4, 0);
		creatureCondition = creature->getCondition(conditionType, conditionId, subId);
		force = tfs::lua::getBoolean(L, 5, false);
	}

	if (creatureCondition) {
		creature->removeCondition(creatureCondition, force);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureHasCondition(lua_State* L)
{
	// creature:hasCondition(conditionType[, subId = 0])
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	ConditionType_t conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
	uint32_t subId = tfs::lua::getNumber<uint32_t>(L, 3, 0);
	tfs::lua::pushBoolean(L, creature->hasCondition(conditionType, subId));
	return 1;
}

int LuaScriptInterface::luaCreatureIsImmune(lua_State* L)
{
	// creature:isImmune(condition or conditionType)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (isNumber(L, 2)) {
		tfs::lua::pushBoolean(L, creature->isImmune(tfs::lua::getNumber<ConditionType_t>(L, 2)));
	} else if (Condition* condition = tfs::lua::getUserdata<Condition>(L, 2)) {
		tfs::lua::pushBoolean(L, creature->isImmune(condition->getType()));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureRemove(lua_State* L)
{
	// creature:remove()
	Creature** creaturePtr = tfs::lua::getRawUserdata<Creature>(L, 1);
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
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCreatureTeleportTo(lua_State* L)
{
	// creature:teleportTo(position[, pushMovement = false])
	bool pushMovement = tfs::lua::getBoolean(L, 3, false);

	const Position& position = tfs::lua::getPosition(L, 2);
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const Position oldPosition = creature->getPosition();
	if (g_game.internalTeleport(creature, position, pushMovement) != RETURNVALUE_NOERROR) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (pushMovement) {
		if (oldPosition.x == position.x) {
			if (oldPosition.y < position.y) {
				g_game.internalCreatureTurn(creature, DIRECTION_SOUTH);
			} else {
				g_game.internalCreatureTurn(creature, DIRECTION_NORTH);
			}
		} else if (oldPosition.x > position.x) {
			g_game.internalCreatureTurn(creature, DIRECTION_WEST);
		} else if (oldPosition.x < position.x) {
			g_game.internalCreatureTurn(creature, DIRECTION_EAST);
		}
	}
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCreatureSay(lua_State* L)
{
	// creature:say(text[, type = TALKTYPE_MONSTER_SAY[, ghost = false[, target = nullptr[, position]]]])
	int parameters = lua_gettop(L);

	Position position;
	if (parameters >= 6) {
		position = tfs::lua::getPosition(L, 6);
		if (!position.x || !position.y) {
			reportErrorFunc(L, "Invalid position specified.");
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
	}

	Creature* target = nullptr;
	if (parameters >= 5) {
		target = tfs::lua::getCreature(L, 5);
	}

	bool ghost = tfs::lua::getBoolean(L, 4, false);

	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 3, TALKTYPE_MONSTER_SAY);
	const std::string& text = tfs::lua::getString(L, 2);
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	SpectatorVec spectators;
	if (target) {
		spectators.insert(target);
	}

	// Prevent infinity echo on event onHear
	bool echo = tfs::lua::getScriptEnv()->getScriptId() == tfs::events::getScriptId(EventInfoId::CREATURE_ONHEAR);

	if (position.x != 0) {
		tfs::lua::pushBoolean(L, g_game.internalCreatureSay(creature, type, text, ghost, &spectators, &position, echo));
	} else {
		tfs::lua::pushBoolean(L, g_game.internalCreatureSay(creature, type, text, ghost, &spectators, nullptr, echo));
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetDamageMap(lua_State* L)
{
	// creature:getDamageMap()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, creature->damageMap.size(), 0);
	for (const auto& damageEntry : creature->damageMap) {
		lua_createtable(L, 0, 2);
		setField(L, "total", damageEntry.second.total);
		setField(L, "ticks", damageEntry.second.ticks);
		lua_rawseti(L, -2, damageEntry.first);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetSummons(lua_State* L)
{
	// creature:getSummons()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, creature->getSummonCount(), 0);

	int index = 0;
	for (Creature* summon : creature->getSummons()) {
		tfs::lua::pushUserdata(L, summon);
		tfs::lua::setCreatureMetatable(L, -1, summon);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetDescription(lua_State* L)
{
	// creature:getDescription(distance)
	int32_t distance = tfs::lua::getNumber<int32_t>(L, 2);
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		tfs::lua::pushString(L, creature->getDescription(distance));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetPathTo(lua_State* L)
{
	// creature:getPathTo(pos[, minTargetDist = 0[, maxTargetDist = 1[, fullPathSearch = true[, clearSight = true[,
	// maxSearchDist = 0]]]]])
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const Position& position = tfs::lua::getPosition(L, 2);

	FindPathParams fpp;
	fpp.minTargetDist = tfs::lua::getNumber<int32_t>(L, 3, 0);
	fpp.maxTargetDist = tfs::lua::getNumber<int32_t>(L, 4, 1);
	fpp.fullPathSearch = tfs::lua::getBoolean(L, 5, fpp.fullPathSearch);
	fpp.clearSight = tfs::lua::getBoolean(L, 6, fpp.clearSight);
	fpp.maxSearchDist = tfs::lua::getNumber<int32_t>(L, 7, fpp.maxSearchDist);

	std::vector<Direction> dirList;
	if (creature->getPathTo(position, dirList, fpp)) {
		lua_newtable(L);

		int index = 0;
		for (auto it = dirList.rbegin(); it != dirList.rend(); ++it) {
			lua_pushnumber(L, *it);
			lua_rawseti(L, -2, ++index);
		}
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureMove(lua_State* L)
{
	// creature:move(direction)
	// creature:move(tile[, flags = 0])
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (isNumber(L, 2)) {
		Direction direction = tfs::lua::getNumber<Direction>(L, 2);
		if (direction > DIRECTION_LAST) {
			lua_pushnil(L);
			return 1;
		}
		lua_pushnumber(L, g_game.internalMoveCreature(creature, direction, FLAG_NOLIMIT));
	} else {
		Tile* tile = tfs::lua::getUserdata<Tile>(L, 2);
		if (!tile) {
			lua_pushnil(L);
			return 1;
		}
		lua_pushnumber(L, g_game.internalMoveCreature(*creature, *tile, tfs::lua::getNumber<uint32_t>(L, 3)));
	}
	return 1;
}

int LuaScriptInterface::luaCreatureGetZone(lua_State* L)
{
	// creature:getZone()
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (creature) {
		lua_pushnumber(L, creature->getZone());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureHasIcon(lua_State* L)
{
	// creature:hasIcon(iconId)
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (creature) {
		auto iconId = tfs::lua::getNumber<CreatureIcon_t>(L, 2);
		tfs::lua::pushBoolean(L, creature->getIcons().contains(iconId));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetIcon(lua_State* L)
{
	// creature:setIcon(iconId, value)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	auto iconId = tfs::lua::getNumber<CreatureIcon_t>(L, 2);
	if (iconId > CREATURE_ICON_LAST) {
		reportErrorFunc(L, "Invalid Creature Icon Id");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	creature->getIcons()[iconId] = tfs::lua::getNumber<uint16_t>(L, 3);
	creature->updateIcons();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCreatureGetIcon(lua_State* L)
{
	// creature:getIcon(iconId)
	const Creature* creature = tfs::lua::getUserdata<const Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	auto iconId = tfs::lua::getNumber<CreatureIcon_t>(L, 2);
	const auto& icons = creature->getIcons();
	auto it = icons.find(iconId);
	if (it != icons.end()) {
		lua_pushinteger(L, it->second);
	} else {
		lua_pushinteger(L, 0);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureRemoveIcon(lua_State* L)
{
	// creature:removeIcon(iconId)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	auto iconId = tfs::lua::getNumber<CreatureIcon_t>(L, 2);
	auto& icons = creature->getIcons();
	auto it = icons.find(iconId);
	if (it != icons.end()) {
		icons.erase(it);
		creature->updateIcons();
		tfs::lua::pushBoolean(L, true);
	} else {
		tfs::lua::pushBoolean(L, false);
	}

	return 1;
}

int LuaScriptInterface::luaCreatureGetStorageValue(lua_State* L)
{
	// creature:getStorageValue(key)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t key = tfs::lua::getNumber<uint32_t>(L, 2);
	if (auto storage = creature->getStorageValue(key)) {
		lua_pushnumber(L, storage.value());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureSetStorageValue(lua_State* L)
{
	// creature:setStorageValue(key, value)
	Creature* creature = tfs::lua::getUserdata<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t key = tfs::lua::getNumber<uint32_t>(L, 2);
	if (IS_IN_KEYRANGE(key, RESERVED_RANGE)) {
		reportErrorFunc(L, fmt::format("Accessing reserved range: {:d}", key));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (lua_isnoneornil(L, 3)) {
		creature->setStorageValue(key, std::nullopt);
	} else {
		int32_t value = tfs::lua::getNumber<int32_t>(L, 3);
		creature->setStorageValue(key, value);
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

// Player
int LuaScriptInterface::luaPlayerCreate(lua_State* L)
{
	// Player(id or guid or name or userdata)
	Player* player;
	if (isNumber(L, 2)) {
		uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);
		if (id >= CREATURE_ID_MIN && id <= Player::playerIDLimit) {
			player = g_game.getPlayerByID(id);
		} else {
			player = g_game.getPlayerByGUID(id);
		}
	} else if (lua_isstring(L, 2)) {
		ReturnValue ret = g_game.getPlayerByNameWildcard(tfs::lua::getString(L, 2), player);
		if (ret != RETURNVALUE_NOERROR) {
			lua_pushnil(L);
			lua_pushnumber(L, ret);
			return 2;
		}
	} else if (lua_isuserdata(L, 2)) {
		if (getUserdataType(L, 2) != LuaData_Player) {
			lua_pushnil(L);
			return 1;
		}
		player = tfs::lua::getUserdata<Player>(L, 2);
	} else {
		player = nullptr;
	}

	if (player) {
		tfs::lua::pushUserdata(L, player);
		tfs::lua::setMetatable(L, -1, "Player");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerIsPlayer(lua_State* L)
{
	// player:isPlayer()
	tfs::lua::pushBoolean(L, tfs::lua::getUserdata<const Player>(L, 1) != nullptr);
	return 1;
}

int LuaScriptInterface::luaPlayerGetGuid(lua_State* L)
{
	// player:getGuid()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getGUID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetIp(lua_State* L)
{
	// player:getIp()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		tfs::lua::pushString(L, player->getIP().to_string());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetAccountId(lua_State* L)
{
	// player:getAccountId()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getAccount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetLastLoginSaved(lua_State* L)
{
	// player:getLastLoginSaved()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getLastLoginSaved());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetLastLogout(lua_State* L)
{
	// player:getLastLogout()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getLastLogout());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetAccountType(lua_State* L)
{
	// player:getAccountType()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getAccountType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetAccountType(lua_State* L)
{
	// player:setAccountType(accountType)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->accountType = tfs::lua::getNumber<AccountType_t>(L, 2);
		IOLoginData::setAccountType(player->getAccount(), player->accountType);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetCapacity(lua_State* L)
{
	// player:getCapacity()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getCapacity());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetCapacity(lua_State* L)
{
	// player:setCapacity(capacity)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->capacity = tfs::lua::getNumber<uint32_t>(L, 2);
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetFreeCapacity(lua_State* L)
{
	// player:getFreeCapacity()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getFreeCapacity());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetDepotChest(lua_State* L)
{
	// player:getDepotChest(depotId[, autoCreate = false])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t depotId = tfs::lua::getNumber<uint32_t>(L, 2);
	bool autoCreate = tfs::lua::getBoolean(L, 3, false);
	const auto& depotChest = player->getDepotChest(depotId, autoCreate);
	if (depotChest) {
		pushSharedPtr(L, depotChest);
		tfs::lua::setItemMetatable(L, -1, depotChest.get());
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetInbox(lua_State* L)
{
	// player:getInbox()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const auto& inbox = player->getInbox();
	if (inbox) {
		pushSharedPtr(L, inbox);
		tfs::lua::setItemMetatable(L, -1, inbox.get());
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetSkullTime(lua_State* L)
{
	// player:getSkullTime()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getSkullTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetSkullTime(lua_State* L)
{
	// player:setSkullTime(skullTime)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->setSkullTicks(tfs::lua::getNumber<int64_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetDeathPenalty(lua_State* L)
{
	// player:getDeathPenalty()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getLostPercent() * 100);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetExperience(lua_State* L)
{
	// player:getExperience()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getExperience());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddExperience(lua_State* L)
{
	// player:addExperience(experience[, sendText = false])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		uint64_t experience = tfs::lua::getNumber<uint64_t>(L, 2);
		bool sendText = tfs::lua::getBoolean(L, 3, false);
		player->addExperience(nullptr, experience, sendText);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveExperience(lua_State* L)
{
	// player:removeExperience(experience[, sendText = false])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		uint64_t experience = tfs::lua::getNumber<uint64_t>(L, 2);
		bool sendText = tfs::lua::getBoolean(L, 3, false);
		player->removeExperience(experience, sendText);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetLevel(lua_State* L)
{
	// player:getLevel()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetLevelPercent(lua_State* L)
{
	// player:getLevelPercent()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getLevelPercent());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetMagicLevel(lua_State* L)
{
	// player:getMagicLevel()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getMagicLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetMagicLevelPercent(lua_State* L)
{
	// player:getMagicLevelPercent()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getMagicLevelPercent());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetBaseMagicLevel(lua_State* L)
{
	// player:getBaseMagicLevel()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getBaseMagicLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetMana(lua_State* L)
{
	// player:getMana()
	const Player* player = tfs::lua::getUserdata<const Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getMana());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddMana(lua_State* L)
{
	// player:addMana(manaChange[, animationOnLoss = false])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	int32_t manaChange = tfs::lua::getNumber<int32_t>(L, 2);
	bool animationOnLoss = tfs::lua::getBoolean(L, 3, false);
	if (!animationOnLoss && manaChange < 0) {
		player->changeMana(manaChange);
	} else {
		CombatDamage damage;
		damage.primary.value = manaChange;
		damage.origin = ORIGIN_NONE;
		g_game.combatChangeMana(nullptr, player, damage);
	}
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerGetMaxMana(lua_State* L)
{
	// player:getMaxMana()
	const Player* player = tfs::lua::getUserdata<const Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getMaxMana());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetMaxMana(lua_State* L)
{
	// player:setMaxMana(maxMana)
	Player* player = tfs::lua::getPlayer(L, 1);
	if (player) {
		player->manaMax = tfs::lua::getNumber<int32_t>(L, 2);
		player->mana = std::min<int32_t>(player->mana, player->manaMax);
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetManaShieldBar(lua_State* L)
{
	// player:setManaShieldBar(capacity, value)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->setMaxManaShieldBar(tfs::lua::getNumber<uint16_t>(L, 2));
		player->setManaShieldBar(tfs::lua::getNumber<uint16_t>(L, 3));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetManaSpent(lua_State* L)
{
	// player:getManaSpent()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getSpentMana());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddManaSpent(lua_State* L)
{
	// player:addManaSpent(amount)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->addManaSpent(tfs::lua::getNumber<uint64_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveManaSpent(lua_State* L)
{
	// player:removeManaSpent(amount[, notify = true])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->removeManaSpent(tfs::lua::getNumber<uint64_t>(L, 2), tfs::lua::getBoolean(L, 3, true));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetBaseMaxHealth(lua_State* L)
{
	// player:getBaseMaxHealth()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->healthMax);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetBaseMaxMana(lua_State* L)
{
	// player:getBaseMaxMana()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->manaMax);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetSkillLevel(lua_State* L)
{
	// player:getSkillLevel(skillType)
	skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		lua_pushnumber(L, player->skills[skillType].level);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetEffectiveSkillLevel(lua_State* L)
{
	// player:getEffectiveSkillLevel(skillType)
	skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		lua_pushnumber(L, player->getSkillLevel(skillType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetSkillPercent(lua_State* L)
{
	// player:getSkillPercent(skillType)
	skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		lua_pushnumber(L, player->skills[skillType].percent);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetSkillTries(lua_State* L)
{
	// player:getSkillTries(skillType)
	skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		lua_pushnumber(L, player->skills[skillType].tries);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddSkillTries(lua_State* L)
{
	// player:addSkillTries(skillType, tries)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
		uint64_t tries = tfs::lua::getNumber<uint64_t>(L, 3);
		player->addSkillAdvance(skillType, tries);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveSkillTries(lua_State* L)
{
	// player:removeSkillTries(skillType, tries[, notify = true])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
		uint64_t tries = tfs::lua::getNumber<uint64_t>(L, 3);
		player->removeSkillTries(skillType, tries, tfs::lua::getBoolean(L, 4, true));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetSpecialSkill(lua_State* L)
{
	// player:getSpecialSkill(specialSkillType)
	SpecialSkills_t specialSkillType = tfs::lua::getNumber<SpecialSkills_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player && specialSkillType <= SPECIALSKILL_LAST) {
		lua_pushnumber(L, player->getSpecialSkill(specialSkillType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddSpecialSkill(lua_State* L)
{
	// player:addSpecialSkill(specialSkillType, value)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	SpecialSkills_t specialSkillType = tfs::lua::getNumber<SpecialSkills_t>(L, 2);
	if (specialSkillType > SPECIALSKILL_LAST) {
		lua_pushnil(L);
		return 1;
	}

	player->setVarSpecialSkill(specialSkillType, tfs::lua::getNumber<int32_t>(L, 3));
	player->sendSkills();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerAddOfflineTrainingTime(lua_State* L)
{
	// player:addOfflineTrainingTime(time)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		int32_t time = tfs::lua::getNumber<int32_t>(L, 2);
		player->addOfflineTrainingTime(time);
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetOfflineTrainingTime(lua_State* L)
{
	// player:getOfflineTrainingTime()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getOfflineTrainingTime());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveOfflineTrainingTime(lua_State* L)
{
	// player:removeOfflineTrainingTime(time)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		int32_t time = tfs::lua::getNumber<int32_t>(L, 2);
		player->removeOfflineTrainingTime(time);
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddOfflineTrainingTries(lua_State* L)
{
	// player:addOfflineTrainingTries(skillType, tries)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
		uint64_t tries = tfs::lua::getNumber<uint64_t>(L, 3);
		tfs::lua::pushBoolean(L, player->addOfflineTrainingTries(skillType, tries));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetOfflineTrainingSkill(lua_State* L)
{
	// player:getOfflineTrainingSkill()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getOfflineTrainingSkill());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetOfflineTrainingSkill(lua_State* L)
{
	// player:setOfflineTrainingSkill(skillId)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		int32_t skillId = tfs::lua::getNumber<int32_t>(L, 2);
		player->setOfflineTrainingSkill(skillId);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetItemCount(lua_State* L)
{
	// player:getItemCount(itemId[, subType = -1])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t subType = tfs::lua::getNumber<int32_t>(L, 3, -1);
	lua_pushnumber(L, player->getItemTypeCount(itemId, subType));
	return 1;
}

int LuaScriptInterface::luaPlayerGetItemById(lua_State* L)
{
	// player:getItemById(itemId, deepSearch[, subType = -1])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}
	bool deepSearch = tfs::lua::getBoolean(L, 3);
	int32_t subType = tfs::lua::getNumber<int32_t>(L, 4, -1);

	Item* item = g_game.findItemOfType(player, itemId, deepSearch, subType);
	if (item) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetVocation(lua_State* L)
{
	// player:getVocation()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		tfs::lua::pushUserdata(L, player->getVocation());
		tfs::lua::setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetVocation(lua_State* L)
{
	// player:setVocation(id or name or userdata)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Vocation* vocation;
	if (isNumber(L, 2)) {
		vocation = g_vocations.getVocation(tfs::lua::getNumber<uint16_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		vocation = g_vocations.getVocation(g_vocations.getVocationId(tfs::lua::getString(L, 2)));
	} else if (lua_isuserdata(L, 2)) {
		vocation = tfs::lua::getUserdata<Vocation>(L, 2);
	} else {
		vocation = nullptr;
	}

	if (!vocation) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->setVocation(vocation->getId());
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerGetSex(lua_State* L)
{
	// player:getSex()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getSex());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetSex(lua_State* L)
{
	// player:setSex(newSex)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		PlayerSex_t newSex = tfs::lua::getNumber<PlayerSex_t>(L, 2);
		player->setSex(newSex);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetTown(lua_State* L)
{
	// player:getTown()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		pushTown(L, *player->getTown());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetTown(lua_State* L)
{
	// player:setTown(town)
	if (!lua_istable(L, 2)) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const Town* town = g_game.map.towns.getTown(tfs::lua::getField<uint32_t>(L, 2, "id", -1));
	if (!town) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->setTown(town);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetGuild(lua_State* L)
{
	// player:getGuild()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& guild = player->getGuild()) {
		pushSharedPtr(L, guild);
		tfs::lua::setMetatable(L, -1, "Guild");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetGuild(lua_State* L)
{
	// player:setGuild(guild)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	player->setGuild(getSharedPtr<Guild>(L, 2));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerGetGuildLevel(lua_State* L)
{
	// player:getGuildLevel()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player && player->getGuild()) {
		lua_pushnumber(L, player->getGuildRank()->level);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetGuildLevel(lua_State* L)
{
	// player:setGuildLevel(level)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const auto& guild = player->getGuild();
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t level = tfs::lua::getNumber<uint8_t>(L, 2);
	if (auto rank = guild->getRankByLevel(level)) {
		player->setGuildRank(rank);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetGuildNick(lua_State* L)
{
	// player:getGuildNick()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		tfs::lua::pushString(L, player->getGuildNick());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetGuildNick(lua_State* L)
{
	// player:setGuildNick(nick)
	const std::string& nick = tfs::lua::getString(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->setGuildNick(nick);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetGroup(lua_State* L)
{
	// player:getGroup()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		tfs::lua::pushUserdata(L, player->getGroup());
		tfs::lua::setMetatable(L, -1, "Group");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetGroup(lua_State* L)
{
	// player:setGroup(group)
	Group* group = tfs::lua::getUserdata<Group>(L, 2);
	if (!group) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->setGroup(group);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetStamina(lua_State* L)
{
	// player:getStamina()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getStaminaMinutes());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetStamina(lua_State* L)
{
	// player:setStamina(stamina)
	uint16_t stamina = tfs::lua::getNumber<uint16_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->staminaMinutes = std::min<uint16_t>(2520, stamina);
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetSoul(lua_State* L)
{
	// player:getSoul()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getSoul());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddSoul(lua_State* L)
{
	// player:addSoul(soulChange)
	int32_t soulChange = tfs::lua::getNumber<int32_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->changeSoul(soulChange);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetMaxSoul(lua_State* L)
{
	// player:getMaxSoul()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player && player->vocation) {
		lua_pushnumber(L, player->vocation->getSoulMax());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetBankBalance(lua_State* L)
{
	// player:getBankBalance()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getBankBalance());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetBankBalance(lua_State* L)
{
	// player:setBankBalance(bankBalance)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	int64_t balance = tfs::lua::getNumber<int64_t>(L, 2);
	if (balance < 0) {
		reportErrorFunc(L, "Invalid bank balance value.");
		lua_pushnil(L);
		return 1;
	}

	player->setBankBalance(balance);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerAddItem(lua_State* L)
{
	// player:addItem(itemId[, count = 1[, canDropOnMap = true[, subType = 1[, slot = CONST_SLOT_WHEREEVER]]]])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t count = tfs::lua::getNumber<int32_t>(L, 3, 1);
	int32_t subType = tfs::lua::getNumber<int32_t>(L, 5, 1);

	const ItemType& it = Item::items[itemId];

	int32_t itemCount = 1;
	int parameters = lua_gettop(L);
	if (parameters >= 5) {
		itemCount = std::max<int32_t>(1, count);
	} else if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = std::ceil(count / static_cast<float>(ITEM_STACK_SIZE));
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

	bool canDropOnMap = tfs::lua::getBoolean(L, 4, true);
	slots_t slot = tfs::lua::getNumber<slots_t>(L, 6, CONST_SLOT_WHEREEVER);
	for (int32_t i = 1; i <= itemCount; ++i) {
		int32_t stackCount = subType;
		if (it.stackable) {
			stackCount = std::min<int32_t>(stackCount, ITEM_STACK_SIZE);
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
		if (ret != RETURNVALUE_NOERROR) {
			delete item;
			if (!hasTable) {
				lua_pushnil(L);
			}
			return 1;
		}

		if (hasTable) {
			lua_pushnumber(L, i);
			tfs::lua::pushUserdata(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
			lua_settable(L, -3);
		} else {
			tfs::lua::pushUserdata(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
		}
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddItemEx(lua_State* L)
{
	// player:addItemEx(item[, canDropOnMap = false[, index = INDEX_WHEREEVER[, flags = 0]]])
	// player:addItemEx(item[, canDropOnMap = true[, slot = CONST_SLOT_WHEREEVER]])
	Item* item = tfs::lua::getUserdata<Item>(L, 2);
	if (!item) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (item->getParent() != VirtualCylinder::virtualCylinder) {
		reportErrorFunc(L, "Item already has a parent");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	bool canDropOnMap = tfs::lua::getBoolean(L, 3, false);
	ReturnValue returnValue;
	if (canDropOnMap) {
		slots_t slot = tfs::lua::getNumber<slots_t>(L, 4, CONST_SLOT_WHEREEVER);
		returnValue = g_game.internalPlayerAddItem(player, item, true, slot);
	} else {
		int32_t index = tfs::lua::getNumber<int32_t>(L, 4, INDEX_WHEREEVER);
		uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 5, 0);
		returnValue = g_game.internalAddItem(player, item, index, flags);
	}

	if (returnValue == RETURNVALUE_NOERROR) {
		tfs::lua::removeTempItem(item);
	}
	lua_pushnumber(L, returnValue);
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveItem(lua_State* L)
{
	// player:removeItem(itemId, count[, subType = -1[, ignoreEquipped = false]])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	uint32_t count = tfs::lua::getNumber<uint32_t>(L, 3);
	int32_t subType = tfs::lua::getNumber<int32_t>(L, 4, -1);
	bool ignoreEquipped = tfs::lua::getBoolean(L, 5, false);
	tfs::lua::pushBoolean(L, player->removeItemOfType(itemId, count, subType, ignoreEquipped));
	return 1;
}

int LuaScriptInterface::luaPlayerSendSupplyUsed(lua_State* L)
{
	// player:sendSupplyUsed(item)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	Item* item = tfs::lua::getUserdata<Item>(L, 2);
	if (!item) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->sendSupplyUsed(item->getClientID());
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerGetMoney(lua_State* L)
{
	// player:getMoney()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getMoney());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddMoney(lua_State* L)
{
	// player:addMoney(money)
	uint64_t money = tfs::lua::getNumber<uint64_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		g_game.addMoney(player, money);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveMoney(lua_State* L)
{
	// player:removeMoney(money)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		uint64_t money = tfs::lua::getNumber<uint64_t>(L, 2);
		tfs::lua::pushBoolean(L, g_game.removeMoney(player, money));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerShowTextDialog(lua_State* L)
{
	// player:showTextDialog(id or name or userdata[, text[, canWrite[, length]]])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	int32_t length = tfs::lua::getNumber<int32_t>(L, 5, -1);
	bool canWrite = tfs::lua::getBoolean(L, 4, false);
	std::string text;

	int parameters = lua_gettop(L);
	if (parameters >= 3) {
		text = tfs::lua::getString(L, 3);
	}

	Item* item;
	if (isNumber(L, 2)) {
		item = Item::CreateItem(tfs::lua::getNumber<uint16_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		item = Item::CreateItem(Item::items.getItemIdByName(tfs::lua::getString(L, 2)));
	} else if (lua_isuserdata(L, 2)) {
		if (getUserdataType(L, 2) != LuaData_Item) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		item = tfs::lua::getUserdata<Item>(L, 2);
	} else {
		item = nullptr;
	}

	if (!item) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_ITEM_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (length < 0) {
		length = Item::items[item->getID()].maxTextLen;
	}

	if (!text.empty()) {
		item->setText(text);
		length = std::max<int32_t>(text.size(), length);
	}

	item->setParent(player);
	player->windowTextId++;
	player->writeItem = item;
	player->maxWriteLen = length;
	player->sendTextWindow(item, length, canWrite);
	lua_pushinteger(L, player->windowTextId);
	return 1;
}

int LuaScriptInterface::luaPlayerSendTextMessage(lua_State* L)
{
	// player:sendTextMessage(type, text[, position, primaryValue = 0, primaryColor = TEXTCOLOR_NONE[,
	// secondaryValue = 0, secondaryColor = TEXTCOLOR_NONE]]) player:sendTextMessage(type, text, channelId)

	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	int parameters = lua_gettop(L);

	TextMessage message(tfs::lua::getNumber<MessageClasses>(L, 2), tfs::lua::getString(L, 3));
	if (parameters == 4) {
		uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 4);
		ChatChannel* channel = g_chat->getChannel(*player, channelId);
		if (!channel || !channel->hasUser(*player)) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		message.channelId = channelId;
	} else {
		if (parameters >= 6) {
			message.position = tfs::lua::getPosition(L, 4);
			message.primary.value = tfs::lua::getNumber<int32_t>(L, 5);
			message.primary.color = tfs::lua::getNumber<TextColor_t>(L, 6);
		}

		if (parameters >= 8) {
			message.secondary.value = tfs::lua::getNumber<int32_t>(L, 7);
			message.secondary.color = tfs::lua::getNumber<TextColor_t>(L, 8);
		}
	}

	player->sendTextMessage(message);
	tfs::lua::pushBoolean(L, true);

	return 1;
}

int LuaScriptInterface::luaPlayerSendChannelMessage(lua_State* L)
{
	// player:sendChannelMessage(author, text, type, channelId)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 5);
	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 4);
	const std::string& text = tfs::lua::getString(L, 3);
	const std::string& author = tfs::lua::getString(L, 2);
	player->sendChannelMessage(author, text, type, channelId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerSendPrivateMessage(lua_State* L)
{
	// player:sendPrivateMessage(speaker, text[, type])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const Player* speaker = tfs::lua::getUserdata<const Player>(L, 2);
	const std::string& text = tfs::lua::getString(L, 3);
	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 4, TALKTYPE_PRIVATE_FROM);
	player->sendPrivateMessage(speaker, type, text);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerChannelSay(lua_State* L)
{
	// player:channelSay(speaker, type, text, channelId)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Creature* speaker = tfs::lua::getCreature(L, 2);
	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 3);
	const std::string& text = tfs::lua::getString(L, 4);
	uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 5);
	player->sendToChannel(speaker, type, text, channelId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerOpenChannel(lua_State* L)
{
	// player:openChannel(channelId)
	uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		g_game.playerOpenChannel(player->getID(), channelId);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerLeaveChannel(lua_State* L)
{
	// player:leaveChannel(channelId)
	uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 2);
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		g_game.playerCloseChannel(player->getID(), channelId);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetSlotItem(lua_State* L)
{
	// player:getSlotItem(slot)
	const Player* player = tfs::lua::getUserdata<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t slot = tfs::lua::getNumber<uint32_t>(L, 2);
	Thing* thing = player->getThing(slot);
	if (!thing) {
		lua_pushnil(L);
		return 1;
	}

	Item* item = thing->getItem();
	if (item) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetParty(lua_State* L)
{
	// player:getParty()
	const Player* player = tfs::lua::getUserdata<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Party* party = player->getParty();
	if (party) {
		tfs::lua::pushUserdata(L, party);
		tfs::lua::setMetatable(L, -1, "Party");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddOutfit(lua_State* L)
{
	// player:addOutfit(lookType)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->addOutfit(tfs::lua::getNumber<uint16_t>(L, 2), 0);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddOutfitAddon(lua_State* L)
{
	// player:addOutfitAddon(lookType, addon)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		uint8_t addon = tfs::lua::getNumber<uint8_t>(L, 3);
		player->addOutfit(lookType, addon);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveOutfit(lua_State* L)
{
	// player:removeOutfit(lookType)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, player->removeOutfit(lookType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveOutfitAddon(lua_State* L)
{
	// player:removeOutfitAddon(lookType, addon)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		uint8_t addon = tfs::lua::getNumber<uint8_t>(L, 3);
		tfs::lua::pushBoolean(L, player->removeOutfitAddon(lookType, addon));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerHasOutfit(lua_State* L)
{
	// player:hasOutfit(lookType[, addon = 0])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		uint8_t addon = tfs::lua::getNumber<uint8_t>(L, 3, 0);
		tfs::lua::pushBoolean(L, player->hasOutfit(lookType, addon));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerCanWearOutfit(lua_State* L)
{
	// player:canWearOutfit(lookType[, addon = 0])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		uint8_t addon = tfs::lua::getNumber<uint8_t>(L, 3, 0);
		tfs::lua::pushBoolean(L, player->canWear(lookType, addon));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSendOutfitWindow(lua_State* L)
{
	// player:sendOutfitWindow()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->sendOutfitWindow();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSendEditPodium(lua_State* L)
{
	// player:sendEditPodium(item)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	Item* item = tfs::lua::getUserdata<Item>(L, 2);
	if (player && item) {
		player->sendPodiumWindow(item);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddMount(lua_State* L)
{
	// player:addMount(mountId or mountName)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t mountId;
	if (isNumber(L, 2)) {
		mountId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		Mount* mount = g_game.mounts.getMountByName(tfs::lua::getString(L, 2));
		if (!mount) {
			lua_pushnil(L);
			return 1;
		}
		mountId = mount->id;
	}
	tfs::lua::pushBoolean(L, player->tameMount(mountId));
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveMount(lua_State* L)
{
	// player:removeMount(mountId or mountName)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t mountId;
	if (isNumber(L, 2)) {
		mountId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		Mount* mount = g_game.mounts.getMountByName(tfs::lua::getString(L, 2));
		if (!mount) {
			lua_pushnil(L);
			return 1;
		}
		mountId = mount->id;
	}
	tfs::lua::pushBoolean(L, player->untameMount(mountId));
	return 1;
}

int LuaScriptInterface::luaPlayerHasMount(lua_State* L)
{
	// player:hasMount(mountId or mountName)
	const Player* player = tfs::lua::getUserdata<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Mount* mount = nullptr;
	if (isNumber(L, 2)) {
		mount = g_game.mounts.getMountByID(tfs::lua::getNumber<uint16_t>(L, 2));
	} else {
		mount = g_game.mounts.getMountByName(tfs::lua::getString(L, 2));
	}

	if (mount) {
		tfs::lua::pushBoolean(L, player->hasMount(mount));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerToggleMount(lua_State* L)
{
	// player:toggleMount(mount)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	bool mount = tfs::lua::getBoolean(L, 2);
	tfs::lua::pushBoolean(L, player->toggleMount(mount));
	return 1;
}

int LuaScriptInterface::luaPlayerGetPremiumEndsAt(lua_State* L)
{
	// player:getPremiumEndsAt()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->premiumEndsAt);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetPremiumEndsAt(lua_State* L)
{
	// player:setPremiumEndsAt(timestamp)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	time_t timestamp = tfs::lua::getNumber<time_t>(L, 2);

	player->setPremiumTime(timestamp);
	IOLoginData::updatePremiumTime(player->getAccount(), timestamp);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerHasBlessing(lua_State* L)
{
	// player:hasBlessing(blessing)
	uint8_t blessing = tfs::lua::getNumber<uint8_t>(L, 2) - 1;
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		tfs::lua::pushBoolean(L, player->hasBlessing(blessing));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddBlessing(lua_State* L)
{
	// player:addBlessing(blessing)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t blessing = tfs::lua::getNumber<uint8_t>(L, 2) - 1;
	if (player->hasBlessing(blessing)) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->addBlessing(blessing);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerRemoveBlessing(lua_State* L)
{
	// player:removeBlessing(blessing)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t blessing = tfs::lua::getNumber<uint8_t>(L, 2) - 1;
	if (!player->hasBlessing(blessing)) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->removeBlessing(blessing);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerCanLearnSpell(lua_State* L)
{
	// player:canLearnSpell(spellName)
	const Player* player = tfs::lua::getUserdata<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const std::string& spellName = tfs::lua::getString(L, 2);
	InstantSpell* spell = g_spells->getInstantSpellByName(spellName);
	if (!spell) {
		reportErrorFunc(L, "Spell \"" + spellName + "\" not found");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (player->hasFlag(PlayerFlag_IgnoreSpellCheck)) {
		tfs::lua::pushBoolean(L, true);
		return 1;
	}

	if (!spell->hasVocationSpellMap(player->getVocationId())) {
		tfs::lua::pushBoolean(L, false);
	} else if (player->getLevel() < spell->getLevel()) {
		tfs::lua::pushBoolean(L, false);
	} else if (player->getMagicLevel() < spell->getMagicLevel()) {
		tfs::lua::pushBoolean(L, false);
	} else {
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerLearnSpell(lua_State* L)
{
	// player:learnSpell(spellName)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		const std::string& spellName = tfs::lua::getString(L, 2);
		player->learnInstantSpell(spellName);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerForgetSpell(lua_State* L)
{
	// player:forgetSpell(spellName)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		const std::string& spellName = tfs::lua::getString(L, 2);
		player->forgetInstantSpell(spellName);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerHasLearnedSpell(lua_State* L)
{
	// player:hasLearnedSpell(spellName)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		const std::string& spellName = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, player->hasLearnedInstantSpell(spellName));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSendTutorial(lua_State* L)
{
	// player:sendTutorial(tutorialId)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		uint8_t tutorialId = tfs::lua::getNumber<uint8_t>(L, 2);
		player->sendTutorial(tutorialId);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerAddMapMark(lua_State* L)
{
	// player:addMapMark(position, type, description)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		const Position& position = tfs::lua::getPosition(L, 2);
		uint8_t type = tfs::lua::getNumber<uint8_t>(L, 3);
		const std::string& description = tfs::lua::getString(L, 4);
		player->sendAddMarker(position, type, description);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSave(lua_State* L)
{
	// player:save()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->loginPosition = player->getPosition();
		tfs::lua::pushBoolean(L, IOLoginData::savePlayer(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerPopupFYI(lua_State* L)
{
	// player:popupFYI(message)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		const std::string& message = tfs::lua::getString(L, 2);
		player->sendFYIBox(message);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerIsPzLocked(lua_State* L)
{
	// player:isPzLocked()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		tfs::lua::pushBoolean(L, player->isPzLocked());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetClient(lua_State* L)
{
	// player:getClient()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_createtable(L, 0, 2);
		setField(L, "version", player->getProtocolVersion());
		setField(L, "os", player->getOperatingSystem());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetHouse(lua_State* L)
{
	// player:getHouse()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	House* house = g_game.map.houses.getHouseByPlayerId(player->getGUID());
	if (house) {
		tfs::lua::pushUserdata(L, house);
		tfs::lua::setMetatable(L, -1, "House");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSendHouseWindow(lua_State* L)
{
	// player:sendHouseWindow(house, listId)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	House* house = tfs::lua::getUserdata<House>(L, 2);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 3);
	player->sendHouseWindow(house, listId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerSetEditHouse(lua_State* L)
{
	// player:setEditHouse(house, listId)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	House* house = tfs::lua::getUserdata<House>(L, 2);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 3);
	player->setEditHouse(house, listId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerSetGhostMode(lua_State* L)
{
	// player:setGhostMode(enabled[, magicEffect = CONST_ME_TELEPORT])
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	bool enabled = tfs::lua::getBoolean(L, 2);
	if (player->isInGhostMode() == enabled) {
		tfs::lua::pushBoolean(L, true);
		return 1;
	}

	MagicEffectClasses magicEffect = tfs::lua::getNumber<MagicEffectClasses>(L, 3, CONST_ME_TELEPORT);

	player->switchGhostMode();

	Tile* tile = player->getTile();
	const Position& position = player->getPosition();
	const bool isInvisible = player->isInvisible();

	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, position, true, true);
	for (Creature* spectator : spectators) {
		assert(dynamic_cast<Player*>(spectator) != nullptr);

		Player* spectatorPlayer = static_cast<Player*>(spectator);
		if (spectatorPlayer != player && !spectatorPlayer->isAccessPlayer()) {
			if (enabled) {
				spectatorPlayer->sendRemoveTileCreature(player, position,
				                                        tile->getClientIndexOfCreature(spectatorPlayer, player));
			} else {
				spectatorPlayer->sendCreatureAppear(player, position, magicEffect);
			}
		} else {
			if (isInvisible) {
				continue;
			}

			spectatorPlayer->sendCreatureChangeVisible(player, !enabled);
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
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerGetContainerId(lua_State* L)
{
	// player:getContainerId(container)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Container* container = tfs::lua::getUserdata<Container>(L, 2);
	if (container) {
		lua_pushnumber(L, player->getContainerID(container));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetContainerById(lua_State* L)
{
	// player:getContainerById(id)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Container* container = player->getContainerByID(tfs::lua::getNumber<uint8_t>(L, 2));
	if (container) {
		tfs::lua::pushUserdata(L, container);
		tfs::lua::setMetatable(L, -1, "Container");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetContainerIndex(lua_State* L)
{
	// player:getContainerIndex(id)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getContainerIndex(tfs::lua::getNumber<uint8_t>(L, 2)));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetRuneSpells(lua_State* L)
{
	// player:getRuneSpells()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	auto runeSpells = g_spells->getRuneSpells();

	std::vector<RuneSpell*> spells;
	for (auto& spell : runeSpells | std::views::values) {
		if (spell.canUse(player)) {
			spells.push_back(&spell);
		}
	}

	lua_createtable(L, spells.size(), 0);

	int index = 0;
	for (auto& spell : spells) {
		tfs::lua::pushUserdata<Spell>(L, spell);
		tfs::lua::setMetatable(L, -1, "Spell");
		lua_rawseti(L, -2, ++index);
	}

	return 1;
}

int LuaScriptInterface::luaPlayerGetInstantSpells(lua_State* L)
{
	// player:getInstantSpells()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	std::vector<const InstantSpell*> spells;
	for (auto& spell : g_spells->getInstantSpells()) {
		if (spell.second.canCast(player)) {
			spells.push_back(&spell.second);
		}
	}

	lua_createtable(L, spells.size(), 0);

	int index = 0;
	for (auto spell : spells) {
		lua_createtable(L, 0, 7);

		setField(L, "name", spell->getName());
		setField(L, "words", spell->getWords());
		setField(L, "level", spell->getLevel());
		setField(L, "mlevel", spell->getMagicLevel());
		setField(L, "mana", spell->getMana());
		setField(L, "manapercent", spell->getManaPercent());
		setField(L, "params", spell->getHasParam());

		tfs::lua::setMetatable(L, -1, "Spell");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerCanCast(lua_State* L)
{
	// player:canCast(spell)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	InstantSpell* spell = tfs::lua::getUserdata<InstantSpell>(L, 2);
	if (player && spell) {
		tfs::lua::pushBoolean(L, spell->canCast(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerHasChaseMode(lua_State* L)
{
	// player:hasChaseMode()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		tfs::lua::pushBoolean(L, player->chaseMode);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerHasSecureMode(lua_State* L)
{
	// player:hasSecureMode()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		tfs::lua::pushBoolean(L, player->secureMode);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetFightMode(lua_State* L)
{
	// player:getFightMode()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->fightMode);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetStoreInbox(lua_State* L)
{
	// player:getStoreInbox()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Container* storeInbox = player->getStoreInbox();
	if (!storeInbox) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushUserdata(L, storeInbox);
	tfs::lua::setMetatable(L, -1, "Container");
	return 1;
}

int LuaScriptInterface::luaPlayerIsNearDepotBox(lua_State* L)
{
	// player:isNearDepotBox()
	const Player* const player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, player->isNearDepotBox());
	return 1;
}

int LuaScriptInterface::luaPlayerGetIdleTime(lua_State* L)
{
	// player:getIdleTime()
	const Player* const player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, player->getIdleTime());
	return 1;
}

int LuaScriptInterface::luaPlayerResetIdleTime(lua_State* L)
{
	// player:resetIdleTime()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	player->resetIdleTime();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerSendCreatureSquare(lua_State* L)
{
	// player:sendCreatureSquare(creature, color)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	auto creature = tfs::lua::getCreature(L, 2);
	if (!creature) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->sendCreatureSquare(creature, tfs::lua::getNumber<SquareColor_t>(L, 3));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaPlayerGetClientExpDisplay(lua_State* L)
{
	// player:getClientExpDisplay()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getClientExpDisplay());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetClientExpDisplay(lua_State* L)
{
	// player:setClientExpDisplay(value)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->setClientExpDisplay(tfs::lua::getNumber<uint16_t>(L, 2));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetClientStaminaBonusDisplay(lua_State* L)
{
	// player:getClientStaminaBonusDisplay()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getClientStaminaBonusDisplay());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetClientStaminaBonusDisplay(lua_State* L)
{
	// player:setClientStaminaBonusDisplay(value)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->setClientStaminaBonusDisplay(tfs::lua::getNumber<uint16_t>(L, 2));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerGetClientLowLevelBonusDisplay(lua_State* L)
{
	// player:getClientLowLevelBonusDisplay()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		lua_pushnumber(L, player->getClientLowLevelBonusDisplay());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSetClientLowLevelBonusDisplay(lua_State* L)
{
	// player:setClientLowLevelBonusDisplay(value)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->setClientLowLevelBonusDisplay(tfs::lua::getNumber<uint16_t>(L, 2));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSendResourceBalance(lua_State* L)
{
	// player:sendResourceBalance(resource, amount)
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		const ResourceTypes_t resourceType = tfs::lua::getNumber<ResourceTypes_t>(L, 2);
		uint64_t amount = tfs::lua::getNumber<uint64_t>(L, 3);
		player->sendResourceBalance(resourceType, amount);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPlayerSendEnterMarket(lua_State* L)
{
	// player:sendEnterMarket()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (player) {
		player->sendMarketEnter();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Monster
int LuaScriptInterface::luaMonsterCreate(lua_State* L)
{
	// Monster(id or userdata)
	Monster* monster;
	if (isNumber(L, 2)) {
		monster = g_game.getMonsterByID(tfs::lua::getNumber<uint32_t>(L, 2));
	} else if (lua_isuserdata(L, 2)) {
		if (getUserdataType(L, 2) != LuaData_Monster) {
			lua_pushnil(L);
			return 1;
		}
		monster = tfs::lua::getUserdata<Monster>(L, 2);
	} else {
		monster = nullptr;
	}

	if (monster) {
		tfs::lua::pushUserdata(L, monster);
		tfs::lua::setMetatable(L, -1, "Monster");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterIsMonster(lua_State* L)
{
	// monster:isMonster()
	tfs::lua::pushBoolean(L, tfs::lua::getUserdata<const Monster>(L, 1) != nullptr);
	return 1;
}

int LuaScriptInterface::luaMonsterGetId(lua_State* L)
{
	// monster:getId()
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		// Set monster id if it's not set yet (only for onSpawn event)
		if (tfs::lua::getScriptEnv()->getScriptId() == tfs::events::getScriptId(EventInfoId::MONSTER_ONSPAWN)) {
			monster->setID();
		}

		lua_pushinteger(L, monster->getID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterGetType(lua_State* L)
{
	// monster:getType()
	const Monster* monster = tfs::lua::getUserdata<const Monster>(L, 1);
	if (monster) {
		tfs::lua::pushUserdata(L, monster->mType);
		tfs::lua::setMetatable(L, -1, "MonsterType");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterRename(lua_State* L)
{
	// monster:rename(name[, nameDescription])
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	monster->setName(tfs::lua::getString(L, 2));
	if (lua_gettop(L) >= 3) {
		monster->setNameDescription(tfs::lua::getString(L, 3));
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaMonsterGetSpawnPosition(lua_State* L)
{
	// monster:getSpawnPosition()
	const Monster* monster = tfs::lua::getUserdata<const Monster>(L, 1);
	if (monster) {
		tfs::lua::pushPosition(L, monster->getMasterPos());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterIsInSpawnRange(lua_State* L)
{
	// monster:isInSpawnRange([position])
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		tfs::lua::pushBoolean(
		    L, monster->isInSpawnRange(lua_gettop(L) >= 2 ? tfs::lua::getPosition(L, 2) : monster->getPosition()));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterIsIdle(lua_State* L)
{
	// monster:isIdle()
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		tfs::lua::pushBoolean(L, monster->getIdleStatus());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSetIdle(lua_State* L)
{
	// monster:setIdle(idle)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	monster->setIdle(tfs::lua::getBoolean(L, 2));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaMonsterIsTarget(lua_State* L)
{
	// monster:isTarget(creature)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		const Creature* creature = tfs::lua::getCreature(L, 2);
		if (!creature) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, monster->isTarget(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterIsOpponent(lua_State* L)
{
	// monster:isOpponent(creature)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		const Creature* creature = tfs::lua::getCreature(L, 2);
		if (!creature) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, monster->isOpponent(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterIsFriend(lua_State* L)
{
	// monster:isFriend(creature)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		const Creature* creature = tfs::lua::getCreature(L, 2);
		if (!creature) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, monster->isFriend(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterAddFriend(lua_State* L)
{
	// monster:addFriend(creature)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		Creature* creature = tfs::lua::getCreature(L, 2);
		if (!creature) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		monster->addFriend(creature);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterRemoveFriend(lua_State* L)
{
	// monster:removeFriend(creature)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		Creature* creature = tfs::lua::getCreature(L, 2);
		if (!creature) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		monster->removeFriend(creature);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterGetFriendList(lua_State* L)
{
	// monster:getFriendList()
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	const auto& friendList = monster->getFriendList();
	lua_createtable(L, friendList.size(), 0);

	int index = 0;
	for (Creature* creature : friendList) {
		tfs::lua::pushUserdata(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterGetFriendCount(lua_State* L)
{
	// monster:getFriendCount()
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		lua_pushnumber(L, monster->getFriendList().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterAddTarget(lua_State* L)
{
	// monster:addTarget(creature[, pushFront = false])
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = tfs::lua::getCreature(L, 2);
	if (!creature) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	bool pushFront = tfs::lua::getBoolean(L, 3, false);
	monster->addTarget(creature, pushFront);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaMonsterRemoveTarget(lua_State* L)
{
	// monster:removeTarget(creature)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	Creature* creature = tfs::lua::getCreature(L, 2);
	if (!creature) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	monster->removeTarget(creature);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaMonsterGetTargetList(lua_State* L)
{
	// monster:getTargetList()
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	const auto& targetList = monster->getTargetList();
	lua_createtable(L, targetList.size(), 0);

	int index = 0;
	for (Creature* creature : targetList) {
		tfs::lua::pushUserdata(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterGetTargetCount(lua_State* L)
{
	// monster:getTargetCount()
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		lua_pushnumber(L, monster->getTargetList().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSelectTarget(lua_State* L)
{
	// monster:selectTarget(creature)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		Creature* creature = tfs::lua::getCreature(L, 2);
		if (!creature) {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, monster->selectTarget(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSearchTarget(lua_State* L)
{
	// monster:searchTarget([searchType = TARGETSEARCH_DEFAULT])
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		TargetSearchType_t searchType = tfs::lua::getNumber<TargetSearchType_t>(L, 2, TARGETSEARCH_DEFAULT);
		tfs::lua::pushBoolean(L, monster->searchTarget(searchType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterIsWalkingToSpawn(lua_State* L)
{
	// monster:isWalkingToSpawn()
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		tfs::lua::pushBoolean(L, monster->isWalkingToSpawn());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterWalkToSpawn(lua_State* L)
{
	// monster:walkToSpawn()
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (monster) {
		tfs::lua::pushBoolean(L, monster->walkToSpawn());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterHasIcon(lua_State* L)
{
	// monster:hasSpecialIcon(iconId)
	const Monster* monster = tfs::lua::getUserdata<const Monster>(L, 1);
	if (monster) {
		auto iconId = tfs::lua::getNumber<MonsterIcon_t>(L, 2);
		tfs::lua::pushBoolean(L, monster->getSpecialIcons().contains(iconId));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSetIcon(lua_State* L)
{
	// monster:setSpecialIcon(iconId, value)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	auto iconId = tfs::lua::getNumber<MonsterIcon_t>(L, 2);
	if (iconId > MONSTER_ICON_LAST) {
		reportErrorFunc(L, "Invalid Monster Icon Id");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	monster->getSpecialIcons()[iconId] = tfs::lua::getNumber<uint16_t>(L, 3);
	monster->updateIcons();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaMonsterGetIcon(lua_State* L)
{
	// monster:getSpecialIcon(iconId)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	auto iconId = tfs::lua::getNumber<MonsterIcon_t>(L, 2);
	const auto& icons = monster->getSpecialIcons();
	auto it = icons.find(iconId);
	if (it != icons.end()) {
		lua_pushinteger(L, it->second);
	} else {
		lua_pushinteger(L, 0);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterRemoveIcon(lua_State* L)
{
	// monster:removeSpecialIcon(iconId)
	Monster* monster = tfs::lua::getUserdata<Monster>(L, 1);
	if (!monster) {
		lua_pushnil(L);
		return 1;
	}

	auto iconId = tfs::lua::getNumber<MonsterIcon_t>(L, 2);
	auto& icons = monster->getSpecialIcons();
	auto it = icons.find(iconId);
	if (it != icons.end()) {
		icons.erase(it);
		monster->updateIcons();
		tfs::lua::pushBoolean(L, true);
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

// Npc
int LuaScriptInterface::luaNpcCreate(lua_State* L)
{
	// Npc([id or name or userdata])
	Npc* npc;
	if (lua_gettop(L) >= 2) {
		if (isNumber(L, 2)) {
			npc = g_game.getNpcByID(tfs::lua::getNumber<uint32_t>(L, 2));
		} else if (lua_isstring(L, 2)) {
			npc = g_game.getNpcByName(tfs::lua::getString(L, 2));
		} else if (lua_isuserdata(L, 2)) {
			if (getUserdataType(L, 2) != LuaData_Npc) {
				lua_pushnil(L);
				return 1;
			}
			npc = tfs::lua::getUserdata<Npc>(L, 2);
		} else {
			npc = nullptr;
		}
	} else {
		npc = tfs::lua::getScriptEnv()->getNpc();
	}

	if (npc) {
		tfs::lua::pushUserdata(L, npc);
		tfs::lua::setMetatable(L, -1, "Npc");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNpcIsNpc(lua_State* L)
{
	// npc:isNpc()
	tfs::lua::pushBoolean(L, tfs::lua::getUserdata<const Npc>(L, 1) != nullptr);
	return 1;
}

int LuaScriptInterface::luaNpcSetMasterPos(lua_State* L)
{
	// npc:setMasterPos(pos[, radius])
	Npc* npc = tfs::lua::getUserdata<Npc>(L, 1);
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	const Position& pos = tfs::lua::getPosition(L, 2);
	int32_t radius = tfs::lua::getNumber<int32_t>(L, 3, 1);
	npc->setMasterPos(pos, radius);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaNpcGetSpeechBubble(lua_State* L)
{
	// npc:getSpeechBubble()
	Npc* npc = tfs::lua::getUserdata<Npc>(L, 1);
	if (npc) {
		lua_pushnumber(L, npc->getSpeechBubble());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaNpcSetSpeechBubble(lua_State* L)
{
	// npc:setSpeechBubble(speechBubble)
	Npc* npc = tfs::lua::getUserdata<Npc>(L, 1);
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	if (!isNumber(L, 2)) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t speechBubble = tfs::lua::getNumber<uint8_t>(L, 2);
	if (speechBubble > SPEECHBUBBLE_LAST) {
		lua_pushnil(L);
	} else {
		npc->setSpeechBubble(speechBubble);

		// update creature speech bubble
		g_game.updateKnownCreature(npc);
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int LuaScriptInterface::luaNpcGetSpectators(lua_State* L)
{
	// npc:getSpectators()
	Npc* npc = tfs::lua::getUserdata<Npc>(L, 1);
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	const auto& spectators = npc->getSpectators();
	lua_createtable(L, spectators.size(), 0);

	int index = 0;
	for (const auto& spectatorPlayer : npc->getSpectators()) {
		tfs::lua::pushUserdata(L, spectatorPlayer);
		tfs::lua::setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

// Guild
int LuaScriptInterface::luaGuildCreate(lua_State* L)
{
	// Guild(id)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	if (const auto& guild = g_game.getGuild(id)) {
		pushSharedPtr(L, guild);
		tfs::lua::setMetatable(L, -1, "Guild");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGuildGetId(lua_State* L)
{
	// guild:getId()
	if (const auto& guild = getSharedPtr<Guild>(L, 1)) {
		lua_pushnumber(L, guild->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGuildGetName(lua_State* L)
{
	// guild:getName()
	if (const auto& guild = getSharedPtr<Guild>(L, 1)) {
		tfs::lua::pushString(L, guild->getName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGuildGetMembersOnline(lua_State* L)
{
	// guild:getMembersOnline()
	const auto& guild = getSharedPtr<const Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	const auto& members = guild->getMembersOnline();
	lua_createtable(L, members.size(), 0);

	int index = 0;
	for (Player* player : members) {
		tfs::lua::pushUserdata(L, player);
		tfs::lua::setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaGuildAddRank(lua_State* L)
{
	// guild:addRank(id, name, level)
	if (const auto& guild = getSharedPtr<Guild>(L, 1)) {
		uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);
		const std::string& name = tfs::lua::getString(L, 3);
		uint8_t level = tfs::lua::getNumber<uint8_t>(L, 4);
		guild->addRank(id, name, level);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGuildGetRankById(lua_State* L)
{
	// guild:getRankById(id)
	const auto& guild = getSharedPtr<Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);
	if (auto rank = guild->getRankById(id)) {
		lua_createtable(L, 0, 3);
		setField(L, "id", rank->id);
		setField(L, "name", rank->name);
		setField(L, "level", rank->level);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGuildGetRankByLevel(lua_State* L)
{
	// guild:getRankByLevel(level)
	const auto& guild = getSharedPtr<const Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t level = tfs::lua::getNumber<uint8_t>(L, 2);
	if (auto rank = guild->getRankByLevel(level)) {
		lua_createtable(L, 0, 3);
		setField(L, "id", rank->id);
		setField(L, "name", rank->name);
		setField(L, "level", rank->level);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGuildGetMotd(lua_State* L)
{
	// guild:getMotd()
	if (const auto& guild = getSharedPtr<Guild>(L, 1)) {
		tfs::lua::pushString(L, guild->getMotd());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGuildSetMotd(lua_State* L)
{
	// guild:setMotd(motd)
	if (const auto& guild = getSharedPtr<Guild>(L, 1)) {
		const std::string& motd = tfs::lua::getString(L, 2);
		guild->setMotd(motd);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Group
int LuaScriptInterface::luaGroupCreate(lua_State* L)
{
	// Group(id)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	Group* group = g_game.groups.getGroup(id);
	if (group) {
		tfs::lua::pushUserdata(L, group);
		tfs::lua::setMetatable(L, -1, "Group");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGroupGetId(lua_State* L)
{
	// group:getId()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->id);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGroupGetName(lua_State* L)
{
	// group:getName()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		tfs::lua::pushString(L, group->name);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGroupGetFlags(lua_State* L)
{
	// group:getFlags()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->flags);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGroupGetAccess(lua_State* L)
{
	// group:getAccess()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		tfs::lua::pushBoolean(L, group->access);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGroupGetMaxDepotItems(lua_State* L)
{
	// group:getMaxDepotItems()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->maxDepotItems);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGroupGetMaxVipEntries(lua_State* L)
{
	// group:getMaxVipEntries()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->maxVipEntries);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGroupHasFlag(lua_State* L)
{
	// group:hasFlag(flag)
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		PlayerFlags flag = tfs::lua::getNumber<PlayerFlags>(L, 2);
		tfs::lua::pushBoolean(L, (group->flags & flag) != 0);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Vocation
int LuaScriptInterface::luaVocationCreate(lua_State* L)
{
	// Vocation(id or name)
	uint32_t id;
	if (isNumber(L, 2)) {
		id = tfs::lua::getNumber<uint32_t>(L, 2);
	} else {
		id = g_vocations.getVocationId(tfs::lua::getString(L, 2));
	}

	Vocation* vocation = g_vocations.getVocation(id);
	if (vocation) {
		tfs::lua::pushUserdata(L, vocation);
		tfs::lua::setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetId(lua_State* L)
{
	// vocation:getId()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetClientId(lua_State* L)
{
	// vocation:getClientId()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getClientId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetName(lua_State* L)
{
	// vocation:getName()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushString(L, vocation->getVocName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetDescription(lua_State* L)
{
	// vocation:getDescription()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushString(L, vocation->getVocDescription());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetRequiredSkillTries(lua_State* L)
{
	// vocation:getRequiredSkillTries(skillType, skillLevel)
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
		uint16_t skillLevel = tfs::lua::getNumber<uint16_t>(L, 3);
		lua_pushnumber(L, vocation->getReqSkillTries(skillType, skillLevel));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetRequiredManaSpent(lua_State* L)
{
	// vocation:getRequiredManaSpent(magicLevel)
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		uint32_t magicLevel = tfs::lua::getNumber<uint32_t>(L, 2);
		lua_pushnumber(L, vocation->getReqMana(magicLevel));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetCapacityGain(lua_State* L)
{
	// vocation:getCapacityGain()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getCapGain());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetHealthGain(lua_State* L)
{
	// vocation:getHealthGain()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getHPGain());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetHealthGainTicks(lua_State* L)
{
	// vocation:getHealthGainTicks()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getHealthGainTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetHealthGainAmount(lua_State* L)
{
	// vocation:getHealthGainAmount()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getHealthGainAmount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetManaGain(lua_State* L)
{
	// vocation:getManaGain()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getManaGain());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetManaGainTicks(lua_State* L)
{
	// vocation:getManaGainTicks()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getManaGainTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetManaGainAmount(lua_State* L)
{
	// vocation:getManaGainAmount()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getManaGainAmount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetMaxSoul(lua_State* L)
{
	// vocation:getMaxSoul()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getSoulMax());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetSoulGainTicks(lua_State* L)
{
	// vocation:getSoulGainTicks()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getSoulGainTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetAttackSpeed(lua_State* L)
{
	// vocation:getAttackSpeed()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getAttackSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetBaseSpeed(lua_State* L)
{
	// vocation:getBaseSpeed()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		lua_pushnumber(L, vocation->getBaseSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetDemotion(lua_State* L)
{
	// vocation:getDemotion()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (!vocation) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t fromId = vocation->getFromVocation();
	if (fromId == VOCATION_NONE) {
		lua_pushnil(L);
		return 1;
	}

	Vocation* demotedVocation = g_vocations.getVocation(fromId);
	if (demotedVocation && demotedVocation != vocation) {
		tfs::lua::pushUserdata(L, demotedVocation);
		tfs::lua::setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationGetPromotion(lua_State* L)
{
	// vocation:getPromotion()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (!vocation) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t promotedId = g_vocations.getPromotedVocation(vocation->getId());
	if (promotedId == VOCATION_NONE) {
		lua_pushnil(L);
		return 1;
	}

	Vocation* promotedVocation = g_vocations.getVocation(promotedId);
	if (promotedVocation && promotedVocation != vocation) {
		tfs::lua::pushUserdata(L, promotedVocation);
		tfs::lua::setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaVocationAllowsPvp(lua_State* L)
{
	// vocation:allowsPvp()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushBoolean(L, vocation->allowsPvp());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// House
int LuaScriptInterface::luaHouseCreate(lua_State* L)
{
	// House(id)
	House* house = g_game.map.houses.getHouse(tfs::lua::getNumber<uint32_t>(L, 2));
	if (house) {
		tfs::lua::pushUserdata(L, house);
		tfs::lua::setMetatable(L, -1, "House");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetId(lua_State* L)
{
	// house:getId()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetName(lua_State* L)
{
	// house:getName()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		tfs::lua::pushString(L, house->getName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetTown(lua_State* L)
{
	// house:getTown()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const Town* town = g_game.map.towns.getTown(house->getTownId());
	if (town) {
		pushTown(L, *town);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetExitPosition(lua_State* L)
{
	// house:getExitPosition()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		tfs::lua::pushPosition(L, house->getEntryPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetRent(lua_State* L)
{
	// house:getRent()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getRent());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseSetRent(lua_State* L)
{
	// house:setRent(rent)
	uint32_t rent = tfs::lua::getNumber<uint32_t>(L, 2);
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		house->setRent(rent);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetPaidUntil(lua_State* L)
{
	// house:getPaidUntil()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getPaidUntil());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseSetPaidUntil(lua_State* L)
{
	// house:setPaidUntil(timestamp)
	time_t timestamp = tfs::lua::getNumber<time_t>(L, 2);
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		house->setPaidUntil(timestamp);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetPayRentWarnings(lua_State* L)
{
	// house:getPayRentWarnings()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getPayRentWarnings());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseSetPayRentWarnings(lua_State* L)
{
	// house:setPayRentWarnings(warnings)
	uint32_t warnings = tfs::lua::getNumber<uint32_t>(L, 2);
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		house->setPayRentWarnings(warnings);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetOwnerName(lua_State* L)
{
	// house:getOwnerName()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		tfs::lua::pushString(L, house->getOwnerName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetOwnerGuid(lua_State* L)
{
	// house:getOwnerGuid()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getOwner());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseSetOwnerGuid(lua_State* L)
{
	// house:setOwnerGuid(guid[, updateDatabase = true])
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		uint32_t guid = tfs::lua::getNumber<uint32_t>(L, 2);
		bool updateDatabase = tfs::lua::getBoolean(L, 3, true);
		house->setOwner(guid, updateDatabase);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseStartTrade(lua_State* L)
{
	// house:startTrade(player, tradePartner)
	House* house = tfs::lua::getUserdata<House>(L, 1);
	Player* player = tfs::lua::getUserdata<Player>(L, 2);
	Player* tradePartner = tfs::lua::getUserdata<Player>(L, 3);

	if (!player || !tradePartner || !house) {
		lua_pushnil(L);
		return 1;
	}

	if (!tradePartner->getPosition().isInRange(player->getPosition(), 2, 2, 0)) {
		lua_pushnumber(L, RETURNVALUE_TRADEPLAYERFARAWAY);
		return 1;
	}

	if (house->getOwner() != player->getGUID()) {
		lua_pushnumber(L, RETURNVALUE_YOUDONTOWNTHISHOUSE);
		return 1;
	}

	if (g_game.map.houses.getHouseByPlayerId(tradePartner->getGUID())) {
		lua_pushnumber(L, RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE);
		return 1;
	}

	if (IOLoginData::hasBiddedOnHouse(tradePartner->getGUID())) {
		lua_pushnumber(L, RETURNVALUE_TRADEPLAYERHIGHESTBIDDER);
		return 1;
	}

	Item* transferItem = house->getTransferItem();
	if (!transferItem) {
		lua_pushnumber(L, RETURNVALUE_YOUCANNOTTRADETHISHOUSE);
		return 1;
	}

	transferItem->getParent()->setParent(player);
	if (!g_game.internalStartTrade(player, tradePartner, transferItem)) {
		house->resetTransferItem();
	}

	lua_pushnumber(L, RETURNVALUE_NOERROR);
	return 1;
}

int LuaScriptInterface::luaHouseGetBeds(lua_State* L)
{
	// house:getBeds()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& beds = house->getBeds();
	lua_createtable(L, beds.size(), 0);

	int index = 0;
	for (BedItem* bedItem : beds) {
		tfs::lua::pushUserdata<Item>(L, bedItem);
		tfs::lua::setItemMetatable(L, -1, bedItem);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetBedCount(lua_State* L)
{
	// house:getBedCount()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getBedCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetDoors(lua_State* L)
{
	// house:getDoors()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& doors = house->getDoors();
	lua_createtable(L, doors.size(), 0);

	int index = 0;
	for (Door* door : doors) {
		tfs::lua::pushUserdata<Item>(L, door);
		tfs::lua::setItemMetatable(L, -1, door);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetDoorCount(lua_State* L)
{
	// house:getDoorCount()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getDoors().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetDoorIdByPosition(lua_State* L)
{
	// house:getDoorIdByPosition(position)
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	Door* door = house->getDoorByPosition(tfs::lua::getPosition(L, 2));
	if (door) {
		lua_pushnumber(L, door->getDoorId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetTiles(lua_State* L)
{
	// house:getTiles()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& tiles = house->getTiles();
	lua_createtable(L, tiles.size(), 0);

	int index = 0;
	for (Tile* tile : tiles) {
		tfs::lua::pushUserdata(L, tile);
		tfs::lua::setMetatable(L, -1, "Tile");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetItems(lua_State* L)
{
	// house:getItems()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& tiles = house->getTiles();
	lua_newtable(L);

	int index = 0;
	for (Tile* tile : tiles) {
		TileItemVector* itemVector = tile->getItemList();
		if (itemVector) {
			for (Item* item : *itemVector) {
				tfs::lua::pushUserdata(L, item);
				tfs::lua::setItemMetatable(L, -1, item);
				lua_rawseti(L, -2, ++index);
			}
		}
	}
	return 1;
}

int LuaScriptInterface::luaHouseGetTileCount(lua_State* L)
{
	// house:getTileCount()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (house) {
		lua_pushnumber(L, house->getTiles().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaHouseCanEditAccessList(lua_State* L)
{
	// house:canEditAccessList(listId, player)
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 2);
	Player* player = tfs::lua::getPlayer(L, 3);

	tfs::lua::pushBoolean(L, house->canEditAccessList(listId, player));
	return 1;
}

int LuaScriptInterface::luaHouseGetAccessList(lua_State* L)
{
	// house:getAccessList(listId)
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	std::string list;
	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 2);
	if (house->getAccessList(listId, list)) {
		tfs::lua::pushString(L, list);
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int LuaScriptInterface::luaHouseSetAccessList(lua_State* L)
{
	// house:setAccessList(listId, list)
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 2);
	const std::string& list = tfs::lua::getString(L, 3);
	house->setAccessList(listId, list);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaHouseKickPlayer(lua_State* L)
{
	// house:kickPlayer(player, targetPlayer)
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, house->kickPlayer(tfs::lua::getPlayer(L, 2), tfs::lua::getPlayer(L, 3)));
	return 1;
}

int LuaScriptInterface::luaHouseSave(lua_State* L)
{
	// house:save()
	House* house = tfs::lua::getUserdata<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, IOMapSerialize::saveHouse(house));
	return 1;
}

// ItemType
int LuaScriptInterface::luaItemTypeCreate(lua_State* L)
{
	// ItemType(id or name)
	uint32_t id;
	if (isNumber(L, 2)) {
		id = tfs::lua::getNumber<uint32_t>(L, 2);
	} else if (lua_isstring(L, 2)) {
		id = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
	} else {
		lua_pushnil(L);
		return 1;
	}

	const ItemType& itemType = Item::items[id];
	tfs::lua::pushUserdata(L, &itemType);
	tfs::lua::setMetatable(L, -1, "ItemType");
	return 1;
}

int LuaScriptInterface::luaItemTypeIsCorpse(lua_State* L)
{
	// itemType:isCorpse()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->corpseType != RACE_NONE);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsDoor(lua_State* L)
{
	// itemType:isDoor()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isDoor());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsContainer(lua_State* L)
{
	// itemType:isContainer()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isContainer());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsFluidContainer(lua_State* L)
{
	// itemType:isFluidContainer()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isFluidContainer());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsMovable(lua_State* L)
{
	// itemType:isMovable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->moveable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsRune(lua_State* L)
{
	// itemType:isRune()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isRune());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsStackable(lua_State* L)
{
	// itemType:isStackable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->stackable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsReadable(lua_State* L)
{
	// itemType:isReadable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->canReadText);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsWritable(lua_State* L)
{
	// itemType:isWritable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->canWriteText);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsBlocking(lua_State* L)
{
	// itemType:isBlocking()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->blockProjectile || itemType->blockSolid);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsGroundTile(lua_State* L)
{
	// itemType:isGroundTile()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isGroundTile());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsMagicField(lua_State* L)
{
	// itemType:isMagicField()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isMagicField());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsUseable(lua_State* L)
{
	// itemType:isUseable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isUseable());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsPickupable(lua_State* L)
{
	// itemType:isPickupable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isPickupable());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsRotatable(lua_State* L)
{
	// itemType:isRotatable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->rotatable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetType(lua_State* L)
{
	// itemType:getType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->type);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetGroup(lua_State* L)
{
	// itemType:getGroup()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->group);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetId(lua_State* L)
{
	// itemType:getId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->id);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetClientId(lua_State* L)
{
	// itemType:getClientId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->clientId);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetName(lua_State* L)
{
	// itemType:getName()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->name);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetPluralName(lua_State* L)
{
	// itemType:getPluralName()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->getPluralName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetRotateTo(lua_State* L)
{
	// itemType:getRotateTo()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->rotateTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetArticle(lua_State* L)
{
	// itemType:getArticle()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->article);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetDescription(lua_State* L)
{
	// itemType:getDescription()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->description);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetSlotPosition(lua_State* L)
{
	// itemType:getSlotPosition()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->slotPosition);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetCharges(lua_State* L)
{
	// itemType:getCharges()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->charges);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetFluidSource(lua_State* L)
{
	// itemType:getFluidSource()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->fluidSource);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetCapacity(lua_State* L)
{
	// itemType:getCapacity()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->maxItems);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetWeight(lua_State* L)
{
	// itemType:getWeight([count = 1])
	uint16_t count = tfs::lua::getNumber<uint16_t>(L, 2, 1);

	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	uint64_t weight = static_cast<uint64_t>(itemType->weight) * std::max<int32_t>(1, count);
	lua_pushnumber(L, weight);
	return 1;
}

int LuaScriptInterface::luaItemTypeGetWorth(lua_State* L)
{
	// itemType:getWorth()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, itemType->worth);
	return 1;
}

int LuaScriptInterface::luaItemTypeGetHitChance(lua_State* L)
{
	// itemType:getHitChance()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->hitChance);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetShootRange(lua_State* L)
{
	// itemType:getShootRange()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->shootRange);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetAttack(lua_State* L)
{
	// itemType:getAttack()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->attack);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetAttackSpeed(lua_State* L)
{
	// itemType:getAttackSpeed()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->attackSpeed);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetDefense(lua_State* L)
{
	// itemType:getDefense()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->defense);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetExtraDefense(lua_State* L)
{
	// itemType:getExtraDefense()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->extraDefense);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetArmor(lua_State* L)
{
	// itemType:getArmor()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->armor);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetWeaponType(lua_State* L)
{
	// itemType:getWeaponType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->weaponType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetAmmoType(lua_State* L)
{
	// itemType:getAmmoType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->ammoType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetCorpseType(lua_State* L)
{
	// itemType:getCorpseType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->corpseType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetClassification(lua_State* L)
{
	// itemType:getClassification()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->classification);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetAbilities(lua_State* L)
{
	// itemType:getAbilities()
	ItemType* itemType = tfs::lua::getUserdata<ItemType>(L, 1);
	if (itemType) {
		Abilities& abilities = itemType->getAbilities();
		lua_createtable(L, 10, 12);
		setField(L, "healthGain", abilities.healthGain);
		setField(L, "healthTicks", abilities.healthTicks);
		setField(L, "manaGain", abilities.manaGain);
		setField(L, "manaTicks", abilities.manaTicks);
		setField(L, "conditionImmunities", abilities.conditionImmunities);
		setField(L, "conditionSuppressions", abilities.conditionSuppressions);
		setField(L, "speed", abilities.speed);
		setField(L, "elementDamage", abilities.elementDamage);
		setField(L, "elementType", abilities.elementType);

		lua_pushboolean(L, abilities.manaShield);
		lua_setfield(L, -2, "manaShield");
		lua_pushboolean(L, abilities.invisible);
		lua_setfield(L, -2, "invisible");
		lua_pushboolean(L, abilities.regeneration);
		lua_setfield(L, -2, "regeneration");

		// Stats
		lua_createtable(L, 0, STAT_LAST + 1);
		for (int32_t i = STAT_FIRST; i <= STAT_LAST; i++) {
			lua_pushnumber(L, abilities.stats[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "stats");

		// Stats percent
		lua_createtable(L, 0, STAT_LAST + 1);
		for (int32_t i = STAT_FIRST; i <= STAT_LAST; i++) {
			lua_pushnumber(L, abilities.statsPercent[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "statsPercent");

		// Skills
		lua_createtable(L, 0, SKILL_LAST + 1);
		for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; i++) {
			lua_pushnumber(L, abilities.skills[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "skills");

		// Special skills
		lua_createtable(L, 0, SPECIALSKILL_LAST + 1);
		for (int32_t i = SPECIALSKILL_FIRST; i <= SPECIALSKILL_LAST; i++) {
			lua_pushnumber(L, abilities.specialSkills[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "specialSkills");

		// Field absorb percent
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			lua_pushnumber(L, abilities.fieldAbsorbPercent[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "fieldAbsorbPercent");

		// Absorb percent
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			lua_pushnumber(L, abilities.absorbPercent[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "absorbPercent");

		// special magic level
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			lua_pushnumber(L, abilities.specialMagicLevelSkill[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "specialMagicLevel");

		// Damage boost percent
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			lua_pushnumber(L, abilities.boostPercent[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "boostPercent");

		// Reflect chance
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			lua_pushnumber(L, abilities.reflect[i].chance);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "reflectChance");

		// Reflect percent
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			lua_pushnumber(L, abilities.reflect[i].percent);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "reflectPercent");
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeHasShowAttributes(lua_State* L)
{
	// itemType:hasShowAttributes()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->showAttributes);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeHasShowCount(lua_State* L)
{
	// itemType:hasShowCount()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->showCount);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeHasShowCharges(lua_State* L)
{
	// itemType:hasShowCharges()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->showCharges);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeHasShowDuration(lua_State* L)
{
	// itemType:hasShowDuration()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->showDuration);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeHasAllowDistRead(lua_State* L)
{
	// itemType:hasAllowDistRead()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->allowDistRead);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetWieldInfo(lua_State* L)
{
	// itemType:getWieldInfo()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushinteger(L, itemType->wieldInfo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetDurationMin(lua_State* L)
{
	// itemType:getDurationMin()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushinteger(L, itemType->decayTimeMin);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetDurationMax(lua_State* L)
{
	// itemType:getDurationMax()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushinteger(L, itemType->decayTimeMax);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetLevelDoor(lua_State* L)
{
	// itemType:getLevelDoor()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushinteger(L, itemType->levelDoor);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetRuneSpellName(lua_State* L)
{
	// itemType:getRuneSpellName()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType && itemType->isRune()) {
		tfs::lua::pushString(L, itemType->runeSpellName);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetVocationString(lua_State* L)
{
	// itemType:getVocationString()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->vocationString);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetMinReqLevel(lua_State* L)
{
	// itemType:getMinReqLevel()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushinteger(L, itemType->minReqLevel);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetMinReqMagicLevel(lua_State* L)
{
	// itemType:getMinReqMagicLevel()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushinteger(L, itemType->minReqMagicLevel);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetMarketBuyStatistics(lua_State* L)
{
	// itemType:getMarketBuyStatistics()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		MarketStatistics* statistics = tfs::iomarket::getPurchaseStatistics(itemType->id);
		if (statistics) {
			lua_createtable(L, 4, 0);
			setField(L, "numTransactions", statistics->numTransactions);
			setField(L, "totalPrice", statistics->totalPrice);
			setField(L, "highestPrice", statistics->highestPrice);
			setField(L, "lowestPrice", statistics->lowestPrice);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetMarketSellStatistics(lua_State* L)
{
	// itemType:getMarketSellStatistics()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		MarketStatistics* statistics = tfs::iomarket::getSaleStatistics(itemType->id);
		if (statistics) {
			lua_createtable(L, 4, 0);
			setField(L, "numTransactions", statistics->numTransactions);
			setField(L, "totalPrice", statistics->totalPrice);
			setField(L, "highestPrice", statistics->highestPrice);
			setField(L, "lowestPrice", statistics->lowestPrice);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetElementType(lua_State* L)
{
	// itemType:getElementType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	auto& abilities = itemType->abilities;
	if (abilities) {
		lua_pushnumber(L, abilities->elementType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetElementDamage(lua_State* L)
{
	// itemType:getElementDamage()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	auto& abilities = itemType->abilities;
	if (abilities) {
		lua_pushnumber(L, abilities->elementDamage);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetTransformEquipId(lua_State* L)
{
	// itemType:getTransformEquipId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->transformEquipTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetTransformDeEquipId(lua_State* L)
{
	// itemType:getTransformDeEquipId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->transformDeEquipTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetDestroyId(lua_State* L)
{
	// itemType:getDestroyId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->destroyTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetDecayId(lua_State* L)
{
	// itemType:getDecayId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->decayTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeGetRequiredLevel(lua_State* L)
{
	// itemType:getRequiredLevel()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		lua_pushnumber(L, itemType->minReqLevel);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeHasSubType(lua_State* L)
{
	// itemType:hasSubType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->hasSubType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaItemTypeIsStoreItem(lua_State* L)
{
	// itemType:isStoreItem()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->storeItem);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Combat

int LuaScriptInterface::luaCombatCreate(lua_State* L)
{
	// Combat()
	pushSharedPtr(L, g_luaEnvironment.createCombatObject(tfs::lua::getScriptEnv()->getScriptInterface()));
	tfs::lua::setMetatable(L, -1, "Combat");
	return 1;
}

int LuaScriptInterface::luaCombatDelete(lua_State* L)
{
	Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (combat) {
		combat.reset();
	}
	return 0;
}

int LuaScriptInterface::luaCombatSetParameter(lua_State* L)
{
	// combat:setParameter(key, value)
	const Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (!combat) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	CombatParam_t key = tfs::lua::getNumber<CombatParam_t>(L, 2);
	uint32_t value;
	if (lua_isboolean(L, 3)) {
		value = tfs::lua::getBoolean(L, 3) ? 1 : 0;
	} else {
		value = tfs::lua::getNumber<uint32_t>(L, 3);
	}
	combat->setParam(key, value);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCombatGetParameter(lua_State* L)
{
	// combat:getParameter(key)
	const Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (!combat) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	int32_t value = combat->getParam(tfs::lua::getNumber<CombatParam_t>(L, 2));
	if (value == std::numeric_limits<int32_t>().max()) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, value);
	return 1;
}

int LuaScriptInterface::luaCombatSetFormula(lua_State* L)
{
	// combat:setFormula(type, mina, minb, maxa, maxb)
	const Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (!combat) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	formulaType_t type = tfs::lua::getNumber<formulaType_t>(L, 2);
	double mina = tfs::lua::getNumber<double>(L, 3);
	double minb = tfs::lua::getNumber<double>(L, 4);
	double maxa = tfs::lua::getNumber<double>(L, 5);
	double maxb = tfs::lua::getNumber<double>(L, 6);
	combat->setPlayerCombatValues(type, mina, minb, maxa, maxb);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCombatSetArea(lua_State* L)
{
	// combat:setArea(area)
	if (tfs::lua::getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		reportErrorFunc(L, "This function can only be used while loading the script.");
		lua_pushnil(L);
		return 1;
	}

	const AreaCombat* area = g_luaEnvironment.getAreaObject(tfs::lua::getNumber<uint32_t>(L, 2));
	if (!area) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_AREA_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	const Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (!combat) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	combat->setArea(new AreaCombat(*area));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCombatAddCondition(lua_State* L)
{
	// combat:addCondition(condition)
	const Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (!combat) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	Condition* condition = tfs::lua::getUserdata<Condition>(L, 2);
	if (condition) {
		combat->addCondition(condition->clone());
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCombatClearConditions(lua_State* L)
{
	// combat:clearConditions()
	const Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (!combat) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	combat->clearConditions();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCombatSetCallback(lua_State* L)
{
	// combat:setCallback(key, function)
	const Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (!combat) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	CallBackParam_t key = tfs::lua::getNumber<CallBackParam_t>(L, 2);
	if (!combat->setCallback(key)) {
		lua_pushnil(L);
		return 1;
	}

	CallBack* callback = combat->getCallback(key);
	if (!callback) {
		lua_pushnil(L);
		return 1;
	}

	const std::string& function = tfs::lua::getString(L, 3);
	tfs::lua::pushBoolean(L, callback->loadCallBack(tfs::lua::getScriptEnv()->getScriptInterface(), function));
	return 1;
}

int LuaScriptInterface::luaCombatSetOrigin(lua_State* L)
{
	// combat:setOrigin(origin)
	const Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (!combat) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	combat->setOrigin(tfs::lua::getNumber<CombatOrigin>(L, 2));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaCombatExecute(lua_State* L)
{
	// combat:execute(creature, variant)
	const Combat_ptr& combat = getSharedPtr<Combat>(L, 1);
	if (!combat) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	if (lua_isuserdata(L, 2)) {
		LuaDataType type = getUserdataType(L, 2);
		if (type != LuaData_Player && type != LuaData_Monster && type != LuaData_Npc) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
	}

	Creature* creature = tfs::lua::getCreature(L, 2);

	const LuaVariant& variant = getVariant(L, 3);
	switch (variant.type()) {
		case VARIANT_NUMBER: {
			Creature* target = g_game.getCreatureByID(variant.getNumber());
			if (!target) {
				tfs::lua::pushBoolean(L, false);
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
			combat->doCombat(creature, variant.getPosition());
			break;
		}

		case VARIANT_TARGETPOSITION: {
			if (combat->hasArea()) {
				combat->doCombat(creature, variant.getTargetPosition());
			} else {
				combat->postCombatEffects(creature, variant.getTargetPosition());
				g_game.addMagicEffect(variant.getTargetPosition(), CONST_ME_POFF);
			}
			break;
		}

		case VARIANT_STRING: {
			Player* target = g_game.getPlayerByName(variant.getString());
			if (!target) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}

			combat->doCombat(creature, target);
			break;
		}

		case VARIANT_NONE: {
			reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_VARIANT_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		default: {
			break;
		}
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

// Condition
int LuaScriptInterface::luaConditionCreate(lua_State* L)
{
	// Condition(conditionType[, conditionId = CONDITIONID_COMBAT])
	ConditionType_t conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
	ConditionId_t conditionId = tfs::lua::getNumber<ConditionId_t>(L, 3, CONDITIONID_COMBAT);

	Condition* condition = Condition::createCondition(conditionId, conditionType, 0, 0);
	if (condition) {
		tfs::lua::pushUserdata(L, condition);
		tfs::lua::setMetatable(L, -1, "Condition");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionDelete(lua_State* L)
{
	// condition:delete()
	Condition** conditionPtr = tfs::lua::getRawUserdata<Condition>(L, 1);
	if (conditionPtr && *conditionPtr) {
		delete *conditionPtr;
		*conditionPtr = nullptr;
	}
	return 0;
}

int LuaScriptInterface::luaConditionGetId(lua_State* L)
{
	// condition:getId()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionGetSubId(lua_State* L)
{
	// condition:getSubId()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getSubId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionGetType(lua_State* L)
{
	// condition:getType()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionGetIcons(lua_State* L)
{
	// condition:getIcons()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getIcons());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionGetEndTime(lua_State* L)
{
	// condition:getEndTime()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getEndTime());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionClone(lua_State* L)
{
	// condition:clone()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		tfs::lua::pushUserdata(L, condition->clone());
		tfs::lua::setMetatable(L, -1, "Condition");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionGetTicks(lua_State* L)
{
	// condition:getTicks()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		lua_pushnumber(L, condition->getTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionSetTicks(lua_State* L)
{
	// condition:setTicks(ticks)
	int32_t ticks = tfs::lua::getNumber<int32_t>(L, 2);
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		condition->setTicks(ticks);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionSetParameter(lua_State* L)
{
	// condition:setParameter(key, value)
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (!condition) {
		lua_pushnil(L);
		return 1;
	}

	ConditionParam_t key = tfs::lua::getNumber<ConditionParam_t>(L, 2);
	int32_t value;
	if (lua_isboolean(L, 3)) {
		value = tfs::lua::getBoolean(L, 3) ? 1 : 0;
	} else {
		value = tfs::lua::getNumber<int32_t>(L, 3);
	}
	condition->setParam(key, value);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaConditionGetParameter(lua_State* L)
{
	// condition:getParameter(key)
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (!condition) {
		lua_pushnil(L);
		return 1;
	}

	int32_t value = condition->getParam(tfs::lua::getNumber<ConditionParam_t>(L, 2));
	if (value == std::numeric_limits<int32_t>().max()) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushnumber(L, value);
	return 1;
}

int LuaScriptInterface::luaConditionSetFormula(lua_State* L)
{
	// condition:setFormula(mina, minb, maxa, maxb)
	double maxb = tfs::lua::getNumber<double>(L, 5);
	double maxa = tfs::lua::getNumber<double>(L, 4);
	double minb = tfs::lua::getNumber<double>(L, 3);
	double mina = tfs::lua::getNumber<double>(L, 2);
	ConditionSpeed* condition = dynamic_cast<ConditionSpeed*>(tfs::lua::getUserdata<Condition>(L, 1));
	if (condition) {
		condition->setFormulaVars(mina, minb, maxa, maxb);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionSetOutfit(lua_State* L)
{
	// condition:setOutfit(outfit)
	// condition:setOutfit(lookTypeEx, lookType, lookHead, lookBody, lookLegs, lookFeet[, lookAddons[, lookMount]])
	Outfit_t outfit;
	if (lua_istable(L, 2)) {
		outfit = getOutfit(L, 2);
	} else {
		outfit.lookMount = tfs::lua::getNumber<uint16_t>(L, 9, outfit.lookMount);
		outfit.lookAddons = tfs::lua::getNumber<uint8_t>(L, 8, outfit.lookAddons);
		outfit.lookFeet = tfs::lua::getNumber<uint8_t>(L, 7);
		outfit.lookLegs = tfs::lua::getNumber<uint8_t>(L, 6);
		outfit.lookBody = tfs::lua::getNumber<uint8_t>(L, 5);
		outfit.lookHead = tfs::lua::getNumber<uint8_t>(L, 4);
		outfit.lookType = tfs::lua::getNumber<uint16_t>(L, 3);
		outfit.lookTypeEx = tfs::lua::getNumber<uint16_t>(L, 2);
	}

	ConditionOutfit* condition = dynamic_cast<ConditionOutfit*>(tfs::lua::getUserdata<Condition>(L, 1));
	if (condition) {
		condition->setOutfit(outfit);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaConditionAddDamage(lua_State* L)
{
	// condition:addDamage(rounds, time, value)
	int32_t value = tfs::lua::getNumber<int32_t>(L, 4);
	int32_t time = tfs::lua::getNumber<int32_t>(L, 3);
	int32_t rounds = tfs::lua::getNumber<int32_t>(L, 2);
	ConditionDamage* condition = dynamic_cast<ConditionDamage*>(tfs::lua::getUserdata<Condition>(L, 1));
	if (condition) {
		tfs::lua::pushBoolean(L, condition->addDamage(rounds, time, value));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Outfit
int LuaScriptInterface::luaOutfitCreate(lua_State* L)
{
	// Outfit(looktype)
	const Outfit* outfit = Outfits::getInstance().getOutfitByLookType(tfs::lua::getNumber<uint16_t>(L, 2));
	if (outfit) {
		tfs::lua::pushOutfit(L, outfit);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaOutfitCompare(lua_State* L)
{
	// outfit == outfitEx
	Outfit outfitEx = getOutfitClass(L, 2);
	Outfit outfit = getOutfitClass(L, 1);
	tfs::lua::pushBoolean(L, outfit == outfitEx);
	return 1;
}

// MonsterType
int LuaScriptInterface::luaMonsterTypeCreate(lua_State* L)
{
	// MonsterType(name or raceId)
	MonsterType* monsterType;
	if (isNumber(L, 2)) {
		monsterType = g_monsters.getMonsterType(tfs::lua::getNumber<uint32_t>(L, 2));
	} else {
		monsterType = g_monsters.getMonsterType(tfs::lua::getString(L, 2));
	}

	if (monsterType) {
		tfs::lua::pushUserdata(L, monsterType);
		tfs::lua::setMetatable(L, -1, "MonsterType");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsAttackable(lua_State* L)
{
	// get: monsterType:isAttackable() set: monsterType:isAttackable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isAttackable);
		} else {
			monsterType->info.isAttackable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsChallengeable(lua_State* L)
{
	// get: monsterType:isChallengeable() set: monsterType:isChallengeable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isChallengeable);
		} else {
			monsterType->info.isChallengeable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsConvinceable(lua_State* L)
{
	// get: monsterType:isConvinceable() set: monsterType:isConvinceable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isConvinceable);
		} else {
			monsterType->info.isConvinceable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsSummonable(lua_State* L)
{
	// get: monsterType:isSummonable() set: monsterType:isSummonable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isSummonable);
		} else {
			monsterType->info.isSummonable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsIgnoringSpawnBlock(lua_State* L)
{
	// get: monsterType:isIgnoringSpawnBlock() set: monsterType:isIgnoringSpawnBlock(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isIgnoringSpawnBlock);
		} else {
			monsterType->info.isIgnoringSpawnBlock = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsIllusionable(lua_State* L)
{
	// get: monsterType:isIllusionable() set: monsterType:isIllusionable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isIllusionable);
		} else {
			monsterType->info.isIllusionable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsHostile(lua_State* L)
{
	// get: monsterType:isHostile() set: monsterType:isHostile(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isHostile);
		} else {
			monsterType->info.isHostile = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsPushable(lua_State* L)
{
	// get: monsterType:isPushable() set: monsterType:isPushable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.pushable);
		} else {
			monsterType->info.pushable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsHealthHidden(lua_State* L)
{
	// get: monsterType:isHealthHidden() set: monsterType:isHealthHidden(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.hiddenHealth);
		} else {
			monsterType->info.hiddenHealth = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeIsBoss(lua_State* L)
{
	// get: monsterType:isBoss() set: monsterType:isBoss(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isBoss);
		} else {
			monsterType->info.isBoss = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeCanPushItems(lua_State* L)
{
	// get: monsterType:canPushItems() set: monsterType:canPushItems(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canPushItems);
		} else {
			monsterType->info.canPushItems = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeCanPushCreatures(lua_State* L)
{
	// get: monsterType:canPushCreatures() set: monsterType:canPushCreatures(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canPushCreatures);
		} else {
			monsterType->info.canPushCreatures = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeCanWalkOnEnergy(lua_State* L)
{
	// get: monsterType:canWalkOnEnergy() set: monsterType:canWalkOnEnergy(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canWalkOnEnergy);
		} else {
			monsterType->info.canWalkOnEnergy = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeCanWalkOnFire(lua_State* L)
{
	// get: monsterType:canWalkOnFire() set: monsterType:canWalkOnFire(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canWalkOnFire);
		} else {
			monsterType->info.canWalkOnFire = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeCanWalkOnPoison(lua_State* L)
{
	// get: monsterType:canWalkOnPoison() set: monsterType:canWalkOnPoison(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canWalkOnPoison);
		} else {
			monsterType->info.canWalkOnPoison = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t LuaScriptInterface::luaMonsterTypeName(lua_State* L)
{
	// get: monsterType:name() set: monsterType:name(name)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushString(L, monsterType->name);
		} else {
			monsterType->name = tfs::lua::getString(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeNameDescription(lua_State* L)
{
	// get: monsterType:nameDescription() set: monsterType:nameDescription(desc)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushString(L, monsterType->nameDescription);
		} else {
			monsterType->nameDescription = tfs::lua::getString(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeHealth(lua_State* L)
{
	// get: monsterType:health() set: monsterType:health(health)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.health);
		} else {
			monsterType->info.health = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeMaxHealth(lua_State* L)
{
	// get: monsterType:maxHealth() set: monsterType:maxHealth(health)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.healthMax);
		} else {
			monsterType->info.healthMax = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeRunHealth(lua_State* L)
{
	// get: monsterType:runHealth() set: monsterType:runHealth(health)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.runAwayHealth);
		} else {
			monsterType->info.runAwayHealth = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeExperience(lua_State* L)
{
	// get: monsterType:experience() set: monsterType:experience(exp)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.experience);
		} else {
			monsterType->info.experience = tfs::lua::getNumber<uint64_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeSkull(lua_State* L)
{
	// get: monsterType:skull() set: monsterType:skull(str/constant)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.skull);
		} else {
			if (isNumber(L, 2)) {
				monsterType->info.skull = tfs::lua::getNumber<Skulls_t>(L, 2);
			} else {
				monsterType->info.skull = getSkullType(tfs::lua::getString(L, 2));
			}
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeCombatImmunities(lua_State* L)
{
	// get: monsterType:combatImmunities() set: monsterType:combatImmunities(immunity)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.damageImmunities);
		} else {
			std::string immunity = tfs::lua::getString(L, 2);
			if (immunity == "physical") {
				monsterType->info.damageImmunities |= COMBAT_PHYSICALDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "energy") {
				monsterType->info.damageImmunities |= COMBAT_ENERGYDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "fire") {
				monsterType->info.damageImmunities |= COMBAT_FIREDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "poison" || immunity == "earth") {
				monsterType->info.damageImmunities |= COMBAT_EARTHDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "drown") {
				monsterType->info.damageImmunities |= COMBAT_DROWNDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "ice") {
				monsterType->info.damageImmunities |= COMBAT_ICEDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "holy") {
				monsterType->info.damageImmunities |= COMBAT_HOLYDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "death") {
				monsterType->info.damageImmunities |= COMBAT_DEATHDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "lifedrain") {
				monsterType->info.damageImmunities |= COMBAT_LIFEDRAIN;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "manadrain") {
				monsterType->info.damageImmunities |= COMBAT_MANADRAIN;
				tfs::lua::pushBoolean(L, true);
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown immunity name " << immunity
				          << " for monster: " << monsterType->name << '\n';
				lua_pushnil(L);
			}
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeConditionImmunities(lua_State* L)
{
	// get: monsterType:conditionImmunities() set: monsterType:conditionImmunities(immunity)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.conditionImmunities);
		} else {
			std::string immunity = tfs::lua::getString(L, 2);
			if (immunity == "physical") {
				monsterType->info.conditionImmunities |= CONDITION_BLEEDING;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "energy") {
				monsterType->info.conditionImmunities |= CONDITION_ENERGY;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "fire") {
				monsterType->info.conditionImmunities |= CONDITION_FIRE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "poison" || immunity == "earth") {
				monsterType->info.conditionImmunities |= CONDITION_POISON;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "drown") {
				monsterType->info.conditionImmunities |= CONDITION_DROWN;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "ice") {
				monsterType->info.conditionImmunities |= CONDITION_FREEZING;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "holy") {
				monsterType->info.conditionImmunities |= CONDITION_DAZZLED;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "death") {
				monsterType->info.conditionImmunities |= CONDITION_CURSED;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "paralyze") {
				monsterType->info.conditionImmunities |= CONDITION_PARALYZE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "outfit") {
				monsterType->info.conditionImmunities |= CONDITION_OUTFIT;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "drunk") {
				monsterType->info.conditionImmunities |= CONDITION_DRUNK;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "invisible" || immunity == "invisibility") {
				monsterType->info.conditionImmunities |= CONDITION_INVISIBLE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "bleed") {
				monsterType->info.conditionImmunities |= CONDITION_BLEEDING;
				tfs::lua::pushBoolean(L, true);
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown immunity name " << immunity
				          << " for monster: " << monsterType->name << '\n';
				lua_pushnil(L);
			}
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeGetAttackList(lua_State* L)
{
	// monsterType:getAttackList()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, monsterType->info.attackSpells.size(), 0);

	int index = 0;
	for (const auto& spellBlock : monsterType->info.attackSpells) {
		lua_createtable(L, 0, 8);

		setField(L, "chance", spellBlock.chance);
		setField(L, "isCombatSpell", spellBlock.combatSpell ? 1 : 0);
		setField(L, "isMelee", spellBlock.isMelee ? 1 : 0);
		setField(L, "minCombatValue", spellBlock.minCombatValue);
		setField(L, "maxCombatValue", spellBlock.maxCombatValue);
		setField(L, "range", spellBlock.range);
		setField(L, "speed", spellBlock.speed);
		tfs::lua::pushUserdata(L, static_cast<CombatSpell*>(spellBlock.spell));
		lua_setfield(L, -2, "spell");

		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeAddAttack(lua_State* L)
{
	// monsterType:addAttack(monsterspell)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 2);
		if (spell) {
			spellBlock_t sb;
			if (g_monsters.deserializeSpell(spell, sb, monsterType->name)) {
				monsterType->info.attackSpells.push_back(std::move(sb));
			} else {
				std::cout << monsterType->name << '\n';
				std::cout << "[Warning - Monsters::loadMonster] Cant load spell. " << spell->name << '\n';
			}
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeGetDefenseList(lua_State* L)
{
	// monsterType:getDefenseList()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, monsterType->info.defenseSpells.size(), 0);

	int index = 0;
	for (const auto& spellBlock : monsterType->info.defenseSpells) {
		lua_createtable(L, 0, 8);

		setField(L, "chance", spellBlock.chance);
		setField(L, "isCombatSpell", spellBlock.combatSpell ? 1 : 0);
		setField(L, "isMelee", spellBlock.isMelee ? 1 : 0);
		setField(L, "minCombatValue", spellBlock.minCombatValue);
		setField(L, "maxCombatValue", spellBlock.maxCombatValue);
		setField(L, "range", spellBlock.range);
		setField(L, "speed", spellBlock.speed);
		tfs::lua::pushUserdata(L, static_cast<CombatSpell*>(spellBlock.spell));
		lua_setfield(L, -2, "spell");

		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeAddDefense(lua_State* L)
{
	// monsterType:addDefense(monsterspell)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 2);
		if (spell) {
			spellBlock_t sb;
			if (g_monsters.deserializeSpell(spell, sb, monsterType->name)) {
				monsterType->info.defenseSpells.push_back(std::move(sb));
			} else {
				std::cout << monsterType->name << '\n';
				std::cout << "[Warning - Monsters::loadMonster] Cant load spell. " << spell->name << '\n';
			}
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeGetElementList(lua_State* L)
{
	// monsterType:getElementList()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, monsterType->info.elementMap.size(), 0);
	for (const auto& elementEntry : monsterType->info.elementMap) {
		lua_pushnumber(L, elementEntry.second);
		lua_rawseti(L, -2, elementEntry.first);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeAddElement(lua_State* L)
{
	// monsterType:addElement(type, percent)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		CombatType_t element = tfs::lua::getNumber<CombatType_t>(L, 2);
		monsterType->info.elementMap[element] = tfs::lua::getNumber<int32_t>(L, 3);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeGetVoices(lua_State* L)
{
	// monsterType:getVoices()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	int index = 0;
	lua_createtable(L, monsterType->info.voiceVector.size(), 0);
	for (const auto& voiceBlock : monsterType->info.voiceVector) {
		lua_createtable(L, 0, 2);
		setField(L, "text", voiceBlock.text);
		setField(L, "yellText", voiceBlock.yellText);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeAddVoice(lua_State* L)
{
	// monsterType:addVoice(sentence, interval, chance, yell)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		voiceBlock_t voice;
		voice.text = tfs::lua::getString(L, 2);
		monsterType->info.yellSpeedTicks = tfs::lua::getNumber<uint32_t>(L, 3);
		monsterType->info.yellChance = tfs::lua::getNumber<uint32_t>(L, 4);
		voice.yellText = tfs::lua::getBoolean(L, 5);
		monsterType->info.voiceVector.push_back(voice);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeGetLoot(lua_State* L)
{
	// monsterType:getLoot()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	pushLoot(L, monsterType->info.lootItems);
	return 1;
}

int LuaScriptInterface::luaMonsterTypeAddLoot(lua_State* L)
{
	// monsterType:addLoot(loot)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		Loot* loot = tfs::lua::getUserdata<Loot>(L, 2);
		if (loot) {
			monsterType->loadLoot(monsterType, loot->lootBlock);
			tfs::lua::pushBoolean(L, true);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeGetCreatureEvents(lua_State* L)
{
	// monsterType:getCreatureEvents()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	int index = 0;
	lua_createtable(L, monsterType->info.scripts.size(), 0);
	for (const std::string& creatureEvent : monsterType->info.scripts) {
		tfs::lua::pushString(L, creatureEvent);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeRegisterEvent(lua_State* L)
{
	// monsterType:registerEvent(name)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		monsterType->info.scripts.push_back(tfs::lua::getString(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeEventOnCallback(lua_State* L)
{
	// monsterType:onThink(callback)
	// monsterType:onAppear(callback)
	// monsterType:onDisappear(callback)
	// monsterType:onMove(callback)
	// monsterType:onSay(callback)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (monsterType->loadCallback(&g_scripts->getScriptInterface())) {
			tfs::lua::pushBoolean(L, true);
			return 1;
		}
		tfs::lua::pushBoolean(L, false);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeEventType(lua_State* L)
{
	// monstertype:eventType(event)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		monsterType->info.eventType = tfs::lua::getNumber<MonstersEvent_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeGetSummonList(lua_State* L)
{
	// monsterType:getSummonList()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	int index = 0;
	lua_createtable(L, monsterType->info.summons.size(), 0);
	for (const auto& summonBlock : monsterType->info.summons) {
		lua_createtable(L, 0, 6);
		setField(L, "name", summonBlock.name);
		setField(L, "speed", summonBlock.speed);
		setField(L, "chance", summonBlock.chance);
		setField(L, "max", summonBlock.max);
		setField(L, "effect", summonBlock.effect);
		setField(L, "masterEffect", summonBlock.masterEffect);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeAddSummon(lua_State* L)
{
	// monsterType:addSummon(name, interval, chance[, max = -1[, effect = CONST_ME_TELEPORT[, masterEffect =
	// CONST_ME_NONE]]])
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		summonBlock_t summon;
		summon.name = tfs::lua::getString(L, 2);
		summon.speed = tfs::lua::getNumber<int32_t>(L, 3);
		summon.chance = tfs::lua::getNumber<int32_t>(L, 4);
		summon.max = tfs::lua::getNumber<int32_t>(L, 5, -1);
		summon.effect = tfs::lua::getNumber<MagicEffectClasses>(L, 6, CONST_ME_TELEPORT);
		summon.masterEffect = tfs::lua::getNumber<MagicEffectClasses>(L, 7, CONST_ME_NONE);
		monsterType->info.summons.push_back(summon);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeMaxSummons(lua_State* L)
{
	// get: monsterType:maxSummons() set: monsterType:maxSummons(ammount)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.maxSummons);
		} else {
			monsterType->info.maxSummons = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeArmor(lua_State* L)
{
	// get: monsterType:armor() set: monsterType:armor(armor)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.armor);
		} else {
			monsterType->info.armor = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeDefense(lua_State* L)
{
	// get: monsterType:defense() set: monsterType:defense(defense)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.defense);
		} else {
			monsterType->info.defense = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeOutfit(lua_State* L)
{
	// get: monsterType:outfit() set: monsterType:outfit(outfit)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushOutfit(L, monsterType->info.outfit);
		} else {
			monsterType->info.outfit = getOutfit(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeRace(lua_State* L)
{
	// get: monsterType:race() set: monsterType:race(race)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	std::string race = tfs::lua::getString(L, 2);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.race);
		} else {
			if (race == "venom") {
				monsterType->info.race = RACE_VENOM;
			} else if (race == "blood") {
				monsterType->info.race = RACE_BLOOD;
			} else if (race == "undead") {
				monsterType->info.race = RACE_UNDEAD;
			} else if (race == "fire") {
				monsterType->info.race = RACE_FIRE;
			} else if (race == "energy") {
				monsterType->info.race = RACE_ENERGY;
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown race type " << race << ".\n";
				lua_pushnil(L);
				return 1;
			}
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeCorpseId(lua_State* L)
{
	// get: monsterType:corpseId() set: monsterType:corpseId(id)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.lookcorpse);
		} else {
			monsterType->info.lookcorpse = tfs::lua::getNumber<uint16_t>(L, 2);
			lua_pushboolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeManaCost(lua_State* L)
{
	// get: monsterType:manaCost() set: monsterType:manaCost(mana)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.manaCost);
		} else {
			monsterType->info.manaCost = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeBaseSpeed(lua_State* L)
{
	// get: monsterType:baseSpeed() set: monsterType:baseSpeed(speed)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.baseSpeed);
		} else {
			monsterType->info.baseSpeed = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeLight(lua_State* L)
{
	// get: monsterType:light() set: monsterType:light(color, level)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}
	if (lua_gettop(L) == 1) {
		lua_pushnumber(L, monsterType->info.light.level);
		lua_pushnumber(L, monsterType->info.light.color);
		return 2;
	} else {
		monsterType->info.light.color = tfs::lua::getNumber<uint8_t>(L, 2);
		monsterType->info.light.level = tfs::lua::getNumber<uint8_t>(L, 3);
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeStaticAttackChance(lua_State* L)
{
	// get: monsterType:staticAttackChance() set: monsterType:staticAttackChance(chance)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.staticAttackChance);
		} else {
			monsterType->info.staticAttackChance = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeTargetDistance(lua_State* L)
{
	// get: monsterType:targetDistance() set: monsterType:targetDistance(distance)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.targetDistance);
		} else {
			monsterType->info.targetDistance = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeYellChance(lua_State* L)
{
	// get: monsterType:yellChance() set: monsterType:yellChance(chance)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.yellChance);
		} else {
			monsterType->info.yellChance = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeYellSpeedTicks(lua_State* L)
{
	// get: monsterType:yellSpeedTicks() set: monsterType:yellSpeedTicks(rate)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.yellSpeedTicks);
		} else {
			monsterType->info.yellSpeedTicks = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeChangeTargetChance(lua_State* L)
{
	// get: monsterType:changeTargetChance() set: monsterType:changeTargetChance(chance)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.changeTargetChance);
		} else {
			monsterType->info.changeTargetChance = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeChangeTargetSpeed(lua_State* L)
{
	// get: monsterType:changeTargetSpeed() set: monsterType:changeTargetSpeed(speed)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, monsterType->info.changeTargetSpeed);
		} else {
			monsterType->info.changeTargetSpeed = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterTypeBestiaryInfo(lua_State* L)
{
	// get: monsterType:bestiaryInfo() set: monsterType:bestiaryInfo(info)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	if (lua_gettop(L) == 1) {
		const auto& info = monsterType->bestiaryInfo;
		lua_createtable(L, 0, 9);

		setField(L, "class", info.className);
		setField(L, "raceId", info.raceId);
		setField(L, "prowess", info.prowess);
		setField(L, "expertise", info.expertise);
		setField(L, "mastery", info.mastery);
		setField(L, "charmPoints", info.charmPoints);
		setField(L, "difficulty", info.difficulty);
		setField(L, "occurrence", info.occurrence);
		setField(L, "locations", info.locations);
		return 1;
	}

	if (lua_istable(L, 2)) {
		BestiaryInfo info{
		    .className = tfs::lua::getFieldString(L, 2, "class"),
		    .raceId = tfs::lua::getField<uint32_t>(L, 2, "raceId"),
		    .prowess = tfs::lua::getField<uint32_t>(L, 2, "prowess"),
		    .expertise = tfs::lua::getField<uint32_t>(L, 2, "expertise"),
		    .mastery = tfs::lua::getField<uint32_t>(L, 2, "mastery"),
		    .charmPoints = tfs::lua::getField<uint32_t>(L, 2, "charmPoints"),
		    .difficulty = tfs::lua::getField<uint32_t>(L, 2, "difficulty"),
		    .occurrence = tfs::lua::getField<uint32_t>(L, 2, "occurrence"),
		    .locations = tfs::lua::getFieldString(L, 2, "locations"),
		};
		lua_pop(L, 9);

		if (g_monsters.isValidBestiaryInfo(info)) {
			monsterType->bestiaryInfo = std::move(info);
			tfs::lua::pushBoolean(L, g_monsters.addBestiaryMonsterType(monsterType));
		} else {
			tfs::lua::pushBoolean(L, false);
		}
		return 1;
	}

	std::cout << "[Warning - LuaScriptInterface::luaMonsterTypeBestiaryInfo] bestiaryInfo must be a table.\n";
	lua_pushnil(L);
	return 1;
}

// Loot
int LuaScriptInterface::luaCreateLoot(lua_State* L)
{
	// Loot() will create a new loot item
	tfs::lua::pushUserdata(L, new Loot);
	tfs::lua::setMetatable(L, -1, "Loot");
	return 1;
}

int LuaScriptInterface::luaDeleteLoot(lua_State* L)
{
	// loot:delete() loot:__gc()
	Loot** lootPtr = tfs::lua::getRawUserdata<Loot>(L, 1);
	if (lootPtr && *lootPtr) {
		delete *lootPtr;
		*lootPtr = nullptr;
	}
	return 0;
}

int LuaScriptInterface::luaLootSetId(lua_State* L)
{
	// loot:setId(id or name)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		if (isNumber(L, 2)) {
			loot->lootBlock.id = tfs::lua::getNumber<uint16_t>(L, 2);
		} else {
			auto name = tfs::lua::getString(L, 2);
			auto ids = Item::items.nameToItems.equal_range(boost::algorithm::to_lower_copy(name));

			if (ids.first == Item::items.nameToItems.cend()) {
				std::cout << "[Warning - Loot:setId] Unknown loot item \"" << name << "\".\n";
				tfs::lua::pushBoolean(L, false);
				return 1;
			}

			if (std::next(ids.first) != ids.second) {
				std::cout << "[Warning - Loot:setId] Non-unique loot item \"" << name << "\".\n";
				tfs::lua::pushBoolean(L, false);
				return 1;
			}

			loot->lootBlock.id = ids.first->second;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaLootSetSubType(lua_State* L)
{
	// loot:setSubType(type)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.subType = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaLootSetChance(lua_State* L)
{
	// loot:setChance(chance)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.chance = tfs::lua::getNumber<uint32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaLootSetMaxCount(lua_State* L)
{
	// loot:setMaxCount(max)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.countmax = tfs::lua::getNumber<uint32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaLootSetActionId(lua_State* L)
{
	// loot:setActionId(actionid)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.actionId = tfs::lua::getNumber<uint32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaLootSetDescription(lua_State* L)
{
	// loot:setDescription(desc)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.text = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaLootAddChildLoot(lua_State* L)
{
	// loot:addChildLoot(loot)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		Loot* childLoot = tfs::lua::getUserdata<Loot>(L, 2);
		if (childLoot) {
			loot->lootBlock.childLoot.push_back(childLoot->lootBlock);
			tfs::lua::pushBoolean(L, true);
		} else {
			tfs::lua::pushBoolean(L, false);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// MonsterSpell
int LuaScriptInterface::luaCreateMonsterSpell(lua_State* L)
{
	// MonsterSpell() will create a new Monster Spell
	tfs::lua::pushUserdata(L, new MonsterSpell);
	tfs::lua::setMetatable(L, -1, "MonsterSpell");
	return 1;
}

int LuaScriptInterface::luaDeleteMonsterSpell(lua_State* L)
{
	// monsterSpell:delete() monsterSpell:__gc()
	MonsterSpell** monsterSpellPtr = tfs::lua::getRawUserdata<MonsterSpell>(L, 1);
	if (monsterSpellPtr && *monsterSpellPtr) {
		delete *monsterSpellPtr;
		*monsterSpellPtr = nullptr;
	}
	return 0;
}

int LuaScriptInterface::luaMonsterSpellSetType(lua_State* L)
{
	// monsterSpell:setType(type)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->name = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetScriptName(lua_State* L)
{
	// monsterSpell:setScriptName(name)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->scriptName = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetChance(lua_State* L)
{
	// monsterSpell:setChance(chance)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->chance = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetInterval(lua_State* L)
{
	// monsterSpell:setInterval(interval)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->interval = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetRange(lua_State* L)
{
	// monsterSpell:setRange(range)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->range = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetCombatValue(lua_State* L)
{
	// monsterSpell:setCombatValue(min, max)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->minCombatValue = tfs::lua::getNumber<int32_t>(L, 2);
		spell->maxCombatValue = tfs::lua::getNumber<int32_t>(L, 3);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetCombatType(lua_State* L)
{
	// monsterSpell:setCombatType(combatType_t)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->combatType = tfs::lua::getNumber<CombatType_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetAttackValue(lua_State* L)
{
	// monsterSpell:setAttackValue(attack, skill)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->attack = tfs::lua::getNumber<int32_t>(L, 2);
		spell->skill = tfs::lua::getNumber<int32_t>(L, 3);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetNeedTarget(lua_State* L)
{
	// monsterSpell:setNeedTarget(bool)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->needTarget = tfs::lua::getBoolean(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetNeedDirection(lua_State* L)
{
	// monsterSpell:setNeedDirection(bool)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->needDirection = tfs::lua::getBoolean(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetCombatLength(lua_State* L)
{
	// monsterSpell:setCombatLength(length)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->length = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetCombatSpread(lua_State* L)
{
	// monsterSpell:setCombatSpread(spread)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->spread = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetCombatRadius(lua_State* L)
{
	// monsterSpell:setCombatRadius(radius)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->radius = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetCombatRing(lua_State* L)
{
	// monsterSpell:setCombatRing(ring)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->ring = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetConditionType(lua_State* L)
{
	// monsterSpell:setConditionType(type)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetConditionDamage(lua_State* L)
{
	// monsterSpell:setConditionDamage(min, max, start)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->conditionMinDamage = tfs::lua::getNumber<int32_t>(L, 2);
		spell->conditionMaxDamage = tfs::lua::getNumber<int32_t>(L, 3);
		spell->conditionStartDamage = tfs::lua::getNumber<int32_t>(L, 4);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetConditionSpeedChange(lua_State* L)
{
	// monsterSpell:setConditionSpeedChange(minSpeed[, maxSpeed])
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->minSpeedChange = tfs::lua::getNumber<int32_t>(L, 2);
		spell->maxSpeedChange = tfs::lua::getNumber<int32_t>(L, 3, 0);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetConditionDuration(lua_State* L)
{
	// monsterSpell:setConditionDuration(duration)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->duration = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetConditionDrunkenness(lua_State* L)
{
	// monsterSpell:setConditionDrunkenness(drunkenness)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->drunkenness = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetConditionTickInterval(lua_State* L)
{
	// monsterSpell:setConditionTickInterval(interval)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->tickInterval = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetCombatShootEffect(lua_State* L)
{
	// monsterSpell:setCombatShootEffect(effect)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->shoot = tfs::lua::getNumber<ShootType_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetCombatEffect(lua_State* L)
{
	// monsterSpell:setCombatEffect(effect)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->effect = tfs::lua::getNumber<MagicEffectClasses>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMonsterSpellSetOutfit(lua_State* L)
{
	// monsterSpell:setOutfit(outfit)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		if (lua_istable(L, 2)) {
			spell->outfit = getOutfit(L, 2);
		} else if (isNumber(L, 2)) {
			spell->outfit.lookTypeEx = tfs::lua::getNumber<uint16_t>(L, 2);
		} else if (lua_isstring(L, 2)) {
			MonsterType* mType = g_monsters.getMonsterType(tfs::lua::getString(L, 2));
			if (mType) {
				spell->outfit = mType->info.outfit;
			}
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Party
int32_t LuaScriptInterface::luaPartyCreate(lua_State* L)
{
	// Party(userdata)
	Player* player = tfs::lua::getUserdata<Player>(L, 2);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Party* party = player->getParty();
	if (!party) {
		party = new Party(player);
		g_game.updatePlayerShield(player);
		player->sendCreatureSkull(player);
		tfs::lua::pushUserdata(L, party);
		tfs::lua::setMetatable(L, -1, "Party");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyDisband(lua_State* L)
{
	// party:disband()
	Party** partyPtr = tfs::lua::getRawUserdata<Party>(L, 1);
	if (partyPtr && *partyPtr) {
		Party*& party = *partyPtr;
		party->disband();
		party = nullptr;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyGetLeader(lua_State* L)
{
	// party:getLeader()
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (!party) {
		lua_pushnil(L);
		return 1;
	}

	Player* leader = party->getLeader();
	if (leader) {
		tfs::lua::pushUserdata(L, leader);
		tfs::lua::setMetatable(L, -1, "Player");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartySetLeader(lua_State* L)
{
	// party:setLeader(player)
	Player* player = tfs::lua::getPlayer(L, 2);
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->passPartyLeadership(player, true));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyGetMembers(lua_State* L)
{
	// party:getMembers()
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (!party) {
		lua_pushnil(L);
		return 1;
	}

	int index = 0;
	lua_createtable(L, party->getMemberCount(), 0);
	for (Player* player : party->getMembers()) {
		tfs::lua::pushUserdata(L, player);
		tfs::lua::setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int LuaScriptInterface::luaPartyGetMemberCount(lua_State* L)
{
	// party:getMemberCount()
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party) {
		lua_pushnumber(L, party->getMemberCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyGetInvitees(lua_State* L)
{
	// party:getInvitees()
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party) {
		lua_createtable(L, party->getInvitationCount(), 0);

		int index = 0;
		for (Player* player : party->getInvitees()) {
			tfs::lua::pushUserdata(L, player);
			tfs::lua::setMetatable(L, -1, "Player");
			lua_rawseti(L, -2, ++index);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyGetInviteeCount(lua_State* L)
{
	// party:getInviteeCount()
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party) {
		lua_pushnumber(L, party->getInvitationCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyAddInvite(lua_State* L)
{
	// party:addInvite(player)
	Player* player = tfs::lua::getPlayer(L, 2);
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->invitePlayer(*player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyRemoveInvite(lua_State* L)
{
	// party:removeInvite(player)
	Player* player = tfs::lua::getPlayer(L, 2);
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->removeInvite(*player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyAddMember(lua_State* L)
{
	// party:addMember(player)
	Player* player = tfs::lua::getPlayer(L, 2);
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->joinParty(*player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyRemoveMember(lua_State* L)
{
	// party:removeMember(player)
	Player* player = tfs::lua::getPlayer(L, 2);
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->leaveParty(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyIsSharedExperienceActive(lua_State* L)
{
	// party:isSharedExperienceActive()
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party) {
		tfs::lua::pushBoolean(L, party->isSharedExperienceActive());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyIsSharedExperienceEnabled(lua_State* L)
{
	// party:isSharedExperienceEnabled()
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party) {
		tfs::lua::pushBoolean(L, party->isSharedExperienceEnabled());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyIsMemberSharingExp(lua_State* L)
{
	// party:isMemberSharingExp(player)
	const Player* player = tfs::lua::getUserdata<const Player>(L, 2);
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->getMemberSharedExperienceStatus(player) == SHAREDEXP_OK);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartyShareExperience(lua_State* L)
{
	// party:shareExperience(experience)
	uint64_t experience = tfs::lua::getNumber<uint64_t>(L, 2);
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party) {
		party->shareExperience(experience);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaPartySetSharedExperience(lua_State* L)
{
	// party:setSharedExperience(active)
	bool active = tfs::lua::getBoolean(L, 2);
	Party* party = tfs::lua::getUserdata<Party>(L, 1);
	if (party) {
		tfs::lua::pushBoolean(L, party->setSharedExperience(party->getLeader(), active));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Spells
int LuaScriptInterface::luaSpellCreate(lua_State* L)
{
	// Spell(words, name or id) to get an existing spell
	// Spell(type) ex: Spell(SPELL_INSTANT) or Spell(SPELL_RUNE) to create a new spell
	if (lua_gettop(L) == 1) {
		std::cout << "[Error - Spell::luaSpellCreate] There is no parameter set!\n";
		lua_pushnil(L);
		return 1;
	}

	SpellType_t spellType = SPELL_UNDEFINED;

	if (isNumber(L, 2)) {
		int32_t id = tfs::lua::getNumber<int32_t>(L, 2);
		RuneSpell* rune = g_spells->getRuneSpell(id);

		if (rune) {
			tfs::lua::pushUserdata<Spell>(L, rune);
			tfs::lua::setMetatable(L, -1, "Spell");
			return 1;
		}

		spellType = static_cast<SpellType_t>(id);
	} else if (lua_isstring(L, 2)) {
		std::string arg = tfs::lua::getString(L, 2);
		InstantSpell* instant = g_spells->getInstantSpellByName(arg);
		if (instant) {
			tfs::lua::pushUserdata<Spell>(L, instant);
			tfs::lua::setMetatable(L, -1, "Spell");
			return 1;
		}
		instant = g_spells->getInstantSpell(arg);
		if (instant) {
			tfs::lua::pushUserdata<Spell>(L, instant);
			tfs::lua::setMetatable(L, -1, "Spell");
			return 1;
		}
		RuneSpell* rune = g_spells->getRuneSpellByName(arg);
		if (rune) {
			tfs::lua::pushUserdata<Spell>(L, rune);
			tfs::lua::setMetatable(L, -1, "Spell");
			return 1;
		}

		std::string tmp = boost::algorithm::to_lower_copy(arg);
		if (tmp == "instant") {
			spellType = SPELL_INSTANT;
		} else if (tmp == "rune") {
			spellType = SPELL_RUNE;
		}
	}

	if (spellType == SPELL_INSTANT) {
		InstantSpell* spell = new InstantSpell(tfs::lua::getScriptEnv()->getScriptInterface());
		spell->fromLua = true;
		tfs::lua::pushUserdata<Spell>(L, spell);
		tfs::lua::setMetatable(L, -1, "Spell");
		spell->spellType = SPELL_INSTANT;
		return 1;
	} else if (spellType == SPELL_RUNE) {
		RuneSpell* spell = new RuneSpell(tfs::lua::getScriptEnv()->getScriptInterface());
		spell->fromLua = true;
		tfs::lua::pushUserdata<Spell>(L, spell);
		tfs::lua::setMetatable(L, -1, "Spell");
		spell->spellType = SPELL_RUNE;
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

int LuaScriptInterface::luaSpellOnCastSpell(lua_State* L)
{
	// spell:onCastSpell(callback)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (spell->spellType == SPELL_INSTANT) {
			InstantSpell* instant = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
			if (!instant->loadCallback()) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
			instant->scripted = true;
			tfs::lua::pushBoolean(L, true);
		} else if (spell->spellType == SPELL_RUNE) {
			RuneSpell* rune = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
			if (!rune->loadCallback()) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
			rune->scripted = true;
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellRegister(lua_State* L)
{
	// spell:register()
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (spell->spellType == SPELL_INSTANT) {
			InstantSpell* instant = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
			if (!instant->isScripted()) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
			tfs::lua::pushBoolean(L, g_spells->registerInstantLuaEvent(instant));
		} else if (spell->spellType == SPELL_RUNE) {
			RuneSpell* rune = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
			if (rune->getMagicLevel() != 0 || rune->getLevel() != 0) {
				// Change information in the ItemType to get accurate description
				ItemType& iType = Item::items.getItemType(rune->getRuneItemId());
				iType.name = rune->getName();
				iType.runeMagLevel = rune->getMagicLevel();
				iType.runeLevel = rune->getLevel();
				iType.charges = rune->getCharges();
			}
			if (!rune->isScripted()) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
			tfs::lua::pushBoolean(L, g_spells->registerRuneLuaEvent(rune));
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellName(lua_State* L)
{
	// spell:name(name)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushString(L, spell->getName());
		} else {
			spell->setName(tfs::lua::getString(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellId(lua_State* L)
{
	// spell:id(id)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getId());
		} else {
			spell->setId(tfs::lua::getNumber<uint8_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellGroup(lua_State* L)
{
	// spell:group(primaryGroup[, secondaryGroup])
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getGroup());
			lua_pushnumber(L, spell->getSecondaryGroup());
			return 2;
		} else if (lua_gettop(L) == 2) {
			SpellGroup_t group = tfs::lua::getNumber<SpellGroup_t>(L, 2);
			if (group) {
				spell->setGroup(group);
				tfs::lua::pushBoolean(L, true);
			} else if (lua_isstring(L, 2)) {
				group = stringToSpellGroup(tfs::lua::getString(L, 2));
				if (group != SPELLGROUP_NONE) {
					spell->setGroup(group);
				} else {
					std::cout << "[Warning - Spell::group] Unknown group: " << tfs::lua::getString(L, 2) << '\n';
					tfs::lua::pushBoolean(L, false);
					return 1;
				}
				tfs::lua::pushBoolean(L, true);
			} else {
				std::cout << "[Warning - Spell::group] Unknown group: " << tfs::lua::getString(L, 2) << '\n';
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
		} else {
			SpellGroup_t primaryGroup = tfs::lua::getNumber<SpellGroup_t>(L, 2);
			SpellGroup_t secondaryGroup = tfs::lua::getNumber<SpellGroup_t>(L, 3);
			if (primaryGroup && secondaryGroup) {
				spell->setGroup(primaryGroup);
				spell->setSecondaryGroup(secondaryGroup);
				tfs::lua::pushBoolean(L, true);
			} else if (lua_isstring(L, 2) && lua_isstring(L, 3)) {
				primaryGroup = stringToSpellGroup(tfs::lua::getString(L, 2));
				if (primaryGroup != SPELLGROUP_NONE) {
					spell->setGroup(primaryGroup);
				} else {
					std::cout << "[Warning - Spell::group] Unknown primaryGroup: " << tfs::lua::getString(L, 2) << '\n';
					tfs::lua::pushBoolean(L, false);
					return 1;
				}
				secondaryGroup = stringToSpellGroup(tfs::lua::getString(L, 3));
				if (secondaryGroup != SPELLGROUP_NONE) {
					spell->setSecondaryGroup(secondaryGroup);
				} else {
					std::cout << "[Warning - Spell::group] Unknown secondaryGroup: " << tfs::lua::getString(L, 3)
					          << '\n';
					tfs::lua::pushBoolean(L, false);
					return 1;
				}
				tfs::lua::pushBoolean(L, true);
			} else {
				std::cout << "[Warning - Spell::group] Unknown primaryGroup: " << tfs::lua::getString(L, 2)
				          << " or secondaryGroup: " << tfs::lua::getString(L, 3) << '\n';
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellCooldown(lua_State* L)
{
	// spell:cooldown(cooldown)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getCooldown());
		} else {
			spell->setCooldown(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellGroupCooldown(lua_State* L)
{
	// spell:groupCooldown(primaryGroupCd[, secondaryGroupCd])
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getGroupCooldown());
			lua_pushnumber(L, spell->getSecondaryCooldown());
			return 2;
		} else if (lua_gettop(L) == 2) {
			spell->setGroupCooldown(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		} else {
			spell->setGroupCooldown(tfs::lua::getNumber<uint32_t>(L, 2));
			spell->setSecondaryCooldown(tfs::lua::getNumber<uint32_t>(L, 3));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellLevel(lua_State* L)
{
	// spell:level(lvl)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getLevel());
		} else {
			spell->setLevel(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellMagicLevel(lua_State* L)
{
	// spell:magicLevel(lvl)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getMagicLevel());
		} else {
			spell->setMagicLevel(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellMana(lua_State* L)
{
	// spell:mana(mana)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getMana());
		} else {
			spell->setMana(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellManaPercent(lua_State* L)
{
	// spell:manaPercent(percent)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getManaPercent());
		} else {
			spell->setManaPercent(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellSoul(lua_State* L)
{
	// spell:soul(soul)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getSoulCost());
		} else {
			spell->setSoulCost(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellRange(lua_State* L)
{
	// spell:range(range)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getRange());
		} else {
			spell->setRange(tfs::lua::getNumber<int32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellPremium(lua_State* L)
{
	// spell:isPremium(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->isPremium());
		} else {
			spell->setPremium(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellEnabled(lua_State* L)
{
	// spell:isEnabled(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->isEnabled());
		} else {
			spell->setEnabled(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellNeedTarget(lua_State* L)
{
	// spell:needTarget(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedTarget());
		} else {
			spell->setNeedTarget(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellNeedWeapon(lua_State* L)
{
	// spell:needWeapon(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedWeapon());
		} else {
			spell->setNeedWeapon(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellNeedLearn(lua_State* L)
{
	// spell:needLearn(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedLearn());
		} else {
			spell->setNeedLearn(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellSelfTarget(lua_State* L)
{
	// spell:isSelfTarget(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getSelfTarget());
		} else {
			spell->setSelfTarget(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellBlocking(lua_State* L)
{
	// spell:isBlocking(blockingSolid, blockingCreature)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getBlockingSolid());
			tfs::lua::pushBoolean(L, spell->getBlockingCreature());
			return 2;
		} else {
			spell->setBlockingSolid(tfs::lua::getBoolean(L, 2));
			spell->setBlockingCreature(tfs::lua::getBoolean(L, 3));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellAggressive(lua_State* L)
{
	// spell:isAggressive(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getAggressive());
		} else {
			spell->setAggressive(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellPzLock(lua_State* L)
{
	// spell:isPzLock(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getPzLock());
		} else {
			spell->setPzLock(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaSpellVocation(lua_State* L)
{
	// spell:vocation(vocation)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (!spell) {
		lua_pushnil(L);
		return 1;
	}

	if (lua_gettop(L) == 1) {
		lua_createtable(L, 0, 0);
		int i = 0;
		for (auto& vocation : spell->getVocationSpellMap()) {
			std::string name = g_vocations.getVocation(vocation.first)->getVocName();
			tfs::lua::pushString(L, name);
			lua_rawseti(L, -2, ++i);
		}
	} else {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		for (int i = 0; i < parameters; ++i) {
			std::string vocStr = tfs::lua::getString(L, 2 + i);
			auto vocList = explodeString(vocStr, ";");
			spell->addVocationSpellMap(vocList[0], vocList.size() > 1 ? booleanString(vocList[1]) : false);
		}
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

// only for InstantSpells
int LuaScriptInterface::luaSpellWords(lua_State* L)
{
	// spell:words(words[, separator = ""])
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushString(L, spell->getWords());
			tfs::lua::pushString(L, spell->getSeparator());
			return 2;
		} else {
			std::string sep = "";
			if (lua_gettop(L) == 3) {
				sep = tfs::lua::getString(L, 3);
			}
			spell->setWords(tfs::lua::getString(L, 2));
			spell->setSeparator(sep);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int LuaScriptInterface::luaSpellNeedDirection(lua_State* L)
{
	// spell:needDirection(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedDirection());
		} else {
			spell->setNeedDirection(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int LuaScriptInterface::luaSpellHasParams(lua_State* L)
{
	// spell:hasParams(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getHasParam());
		} else {
			spell->setHasParam(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int LuaScriptInterface::luaSpellHasPlayerNameParam(lua_State* L)
{
	// spell:hasPlayerNameParam(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getHasPlayerNameParam());
		} else {
			spell->setHasPlayerNameParam(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int LuaScriptInterface::luaSpellNeedCasterTargetOrDirection(lua_State* L)
{
	// spell:needCasterTargetOrDirection(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedCasterTargetOrDirection());
		} else {
			spell->setNeedCasterTargetOrDirection(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int LuaScriptInterface::luaSpellIsBlockingWalls(lua_State* L)
{
	// spell:blockWalls(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getBlockWalls());
		} else {
			spell->setBlockWalls(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int LuaScriptInterface::luaSpellRuneLevel(lua_State* L)
{
	// spell:runeLevel(level)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	int32_t level = tfs::lua::getNumber<int32_t>(L, 2);
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getLevel());
		} else {
			spell->setLevel(level);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int LuaScriptInterface::luaSpellRuneMagicLevel(lua_State* L)
{
	// spell:runeMagicLevel(magLevel)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	int32_t magLevel = tfs::lua::getNumber<int32_t>(L, 2);
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getMagicLevel());
		} else {
			spell->setMagicLevel(magLevel);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int LuaScriptInterface::luaSpellRuneId(lua_State* L)
{
	// spell:runeId(id)
	RuneSpell* rune = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (rune) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (rune->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, rune->getRuneItemId());
		} else {
			rune->setRuneItemId(tfs::lua::getNumber<uint16_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int LuaScriptInterface::luaSpellCharges(lua_State* L)
{
	// spell:charges(charges)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			lua_pushnumber(L, spell->getCharges());
		} else {
			spell->setCharges(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int LuaScriptInterface::luaSpellAllowFarUse(lua_State* L)
{
	// spell:allowFarUse(bool)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getAllowFarUse());
		} else {
			spell->setAllowFarUse(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int LuaScriptInterface::luaSpellBlockWalls(lua_State* L)
{
	// spell:blockWalls(bool)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getCheckLineOfSight());
		} else {
			spell->setCheckLineOfSight(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int LuaScriptInterface::luaSpellCheckFloor(lua_State* L)
{
	// spell:checkFloor(bool)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getCheckFloor());
		} else {
			spell->setCheckFloor(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreateAction(lua_State* L)
{
	// Action()
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		reportErrorFunc(L, "Actions can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	Action* action = new Action(tfs::lua::getScriptEnv()->getScriptInterface());
	action->fromLua = true;
	tfs::lua::pushUserdata(L, action);
	tfs::lua::setMetatable(L, -1, "Action");
	return 1;
}

int LuaScriptInterface::luaActionOnUse(lua_State* L)
{
	// action:onUse(callback)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		if (!action->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		action->scripted = true;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaActionRegister(lua_State* L)
{
	// action:register()
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		if (!action->isScripted()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, g_actions->registerLuaEvent(action));
		g_actions->clearItemIdRange(action);
		g_actions->clearUniqueIdRange(action);
		g_actions->clearActionIdRange(action);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaActionItemId(lua_State* L)
{
	// action:id(ids)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_actions->addItemId(action, tfs::lua::getNumber<uint16_t>(L, 2 + i));
			}
		} else {
			g_actions->addItemId(action, tfs::lua::getNumber<uint16_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaActionActionId(lua_State* L)
{
	// action:aid(aids)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_actions->addActionId(action, tfs::lua::getNumber<uint16_t>(L, 2 + i));
			}
		} else {
			g_actions->addActionId(action, tfs::lua::getNumber<uint16_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaActionUniqueId(lua_State* L)
{
	// action:uid(uids)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_actions->addUniqueId(action, tfs::lua::getNumber<uint16_t>(L, 2 + i));
			}
		} else {
			g_actions->addUniqueId(action, tfs::lua::getNumber<uint16_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaActionAllowFarUse(lua_State* L)
{
	// action:allowFarUse(bool)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		action->setAllowFarUse(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaActionBlockWalls(lua_State* L)
{
	// action:blockWalls(bool)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		action->setCheckLineOfSight(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaActionCheckFloor(lua_State* L)
{
	// action:checkFloor(bool)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		action->setCheckFloor(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreateTalkaction(lua_State* L)
{
	// TalkAction(words)
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		reportErrorFunc(L, "TalkActions can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	TalkAction* talkAction = new TalkAction(tfs::lua::getScriptEnv()->getScriptInterface());
	for (int i = 2; i <= lua_gettop(L); i++) {
		talkAction->setWords(tfs::lua::getString(L, i));
	}
	talkAction->fromLua = true;
	tfs::lua::pushUserdata(L, talkAction);
	tfs::lua::setMetatable(L, -1, "TalkAction");
	return 1;
}

int LuaScriptInterface::luaTalkactionOnSay(lua_State* L)
{
	// talkAction:onSay(callback)
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		if (!talk->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTalkactionRegister(lua_State* L)
{
	// talkAction:register()
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		if (!talk->isScripted()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, g_talkActions->registerLuaEvent(talk));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTalkactionSeparator(lua_State* L)
{
	// talkAction:separator(sep)
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		talk->setSeparator(tfs::lua::getString(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTalkactionAccess(lua_State* L)
{
	// talkAction:access(needAccess = false)
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		talk->setNeedAccess(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaTalkactionAccountType(lua_State* L)
{
	// talkAction:accountType(AccountType_t = ACCOUNT_TYPE_NORMAL)
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		talk->setRequiredAccountType(tfs::lua::getNumber<AccountType_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreateCreatureEvent(lua_State* L)
{
	// CreatureEvent(eventName)
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		reportErrorFunc(L, "CreatureEvents can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	CreatureEvent* creatureEvent = new CreatureEvent(tfs::lua::getScriptEnv()->getScriptInterface());
	creatureEvent->setName(tfs::lua::getString(L, 2));
	creatureEvent->fromLua = true;
	tfs::lua::pushUserdata(L, creatureEvent);
	tfs::lua::setMetatable(L, -1, "CreatureEvent");
	return 1;
}

int LuaScriptInterface::luaCreatureEventType(lua_State* L)
{
	// creatureevent:type(callback)
	CreatureEvent* creature = tfs::lua::getUserdata<CreatureEvent>(L, 1);
	if (creature) {
		std::string typeName = tfs::lua::getString(L, 2);
		std::string tmpStr = boost::algorithm::to_lower_copy(typeName);
		if (tmpStr == "login") {
			creature->setEventType(CREATURE_EVENT_LOGIN);
		} else if (tmpStr == "logout") {
			creature->setEventType(CREATURE_EVENT_LOGOUT);
		} else if (tmpStr == "reconnect") {
			creature->setEventType(CREATURE_EVENT_RECONNECT);
		} else if (tmpStr == "think") {
			creature->setEventType(CREATURE_EVENT_THINK);
		} else if (tmpStr == "preparedeath") {
			creature->setEventType(CREATURE_EVENT_PREPAREDEATH);
		} else if (tmpStr == "death") {
			creature->setEventType(CREATURE_EVENT_DEATH);
		} else if (tmpStr == "kill") {
			creature->setEventType(CREATURE_EVENT_KILL);
		} else if (tmpStr == "advance") {
			creature->setEventType(CREATURE_EVENT_ADVANCE);
		} else if (tmpStr == "modalwindow") {
			creature->setEventType(CREATURE_EVENT_MODALWINDOW);
		} else if (tmpStr == "textedit") {
			creature->setEventType(CREATURE_EVENT_TEXTEDIT);
		} else if (tmpStr == "healthchange") {
			creature->setEventType(CREATURE_EVENT_HEALTHCHANGE);
		} else if (tmpStr == "manachange") {
			creature->setEventType(CREATURE_EVENT_MANACHANGE);
		} else if (tmpStr == "extendedopcode") {
			creature->setEventType(CREATURE_EVENT_EXTENDED_OPCODE);
		} else {
			std::cout << "[Error - CreatureEvent::configureLuaEvent] Invalid type for creature event: " << typeName
			          << '\n';
			tfs::lua::pushBoolean(L, false);
		}
		creature->setLoaded(true);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureEventRegister(lua_State* L)
{
	// creatureevent:register()
	CreatureEvent* creature = tfs::lua::getUserdata<CreatureEvent>(L, 1);
	if (creature) {
		if (!creature->isScripted()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, g_creatureEvents->registerLuaEvent(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
{
	// creatureevent:onLogin / logout / etc. (callback)
	CreatureEvent* creature = tfs::lua::getUserdata<CreatureEvent>(L, 1);
	if (creature) {
		if (!creature->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreateMoveEvent(lua_State* L)
{
	// MoveEvent()
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		reportErrorFunc(L, "MoveEvents can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	MoveEvent* moveevent = new MoveEvent(tfs::lua::getScriptEnv()->getScriptInterface());
	moveevent->fromLua = true;
	tfs::lua::pushUserdata(L, moveevent);
	tfs::lua::setMetatable(L, -1, "MoveEvent");
	return 1;
}

int LuaScriptInterface::luaMoveEventType(lua_State* L)
{
	// moveevent:type(callback)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		std::string typeName = tfs::lua::getString(L, 2);
		std::string tmpStr = boost::algorithm::to_lower_copy(typeName);
		if (tmpStr == "stepin") {
			moveevent->setEventType(MOVE_EVENT_STEP_IN);
			moveevent->stepFunction = moveevent->StepInField;
		} else if (tmpStr == "stepout") {
			moveevent->setEventType(MOVE_EVENT_STEP_OUT);
			moveevent->stepFunction = moveevent->StepOutField;
		} else if (tmpStr == "equip") {
			moveevent->setEventType(MOVE_EVENT_EQUIP);
			moveevent->equipFunction = moveevent->EquipItem;
		} else if (tmpStr == "deequip") {
			moveevent->setEventType(MOVE_EVENT_DEEQUIP);
			moveevent->equipFunction = moveevent->DeEquipItem;
		} else if (tmpStr == "additem") {
			moveevent->setEventType(MOVE_EVENT_ADD_ITEM);
			moveevent->moveFunction = moveevent->AddItemField;
		} else if (tmpStr == "removeitem") {
			moveevent->setEventType(MOVE_EVENT_REMOVE_ITEM);
			moveevent->moveFunction = moveevent->RemoveItemField;
		} else {
			std::cout << "Error: [MoveEvent::configureMoveEvent] No valid event name " << typeName << '\n';
			tfs::lua::pushBoolean(L, false);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventRegister(lua_State* L)
{
	// moveevent:register()
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		if ((moveevent->getEventType() == MOVE_EVENT_EQUIP || moveevent->getEventType() == MOVE_EVENT_DEEQUIP) &&
		    moveevent->getSlot() == SLOTP_WHEREEVER) {
			uint32_t id = g_moveEvents->getItemIdRange(moveevent).at(0);
			ItemType& it = Item::items.getItemType(id);
			moveevent->setSlot(it.slotPosition);
		}
		if (!moveevent->isScripted()) {
			tfs::lua::pushBoolean(L, g_moveEvents->registerLuaFunction(moveevent));
			g_moveEvents->clearItemIdRange(moveevent);
			return 1;
		}
		tfs::lua::pushBoolean(L, g_moveEvents->registerLuaEvent(moveevent));
		g_moveEvents->clearItemIdRange(moveevent);
		g_moveEvents->clearActionIdRange(moveevent);
		g_moveEvents->clearUniqueIdRange(moveevent);
		g_moveEvents->clearPosList(moveevent);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventOnCallback(lua_State* L)
{
	// moveevent:onEquip / deEquip / etc. (callback)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		if (!moveevent->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventSlot(lua_State* L)
{
	// moveevent:slot(slot)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (!moveevent) {
		lua_pushnil(L);
		return 1;
	}

	if (moveevent->getEventType() == MOVE_EVENT_EQUIP || moveevent->getEventType() == MOVE_EVENT_DEEQUIP) {
		std::string slotName = boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
		if (slotName == "head") {
			moveevent->setSlot(SLOTP_HEAD);
		} else if (slotName == "necklace") {
			moveevent->setSlot(SLOTP_NECKLACE);
		} else if (slotName == "backpack") {
			moveevent->setSlot(SLOTP_BACKPACK);
		} else if (slotName == "armor" || slotName == "body") {
			moveevent->setSlot(SLOTP_ARMOR);
		} else if (slotName == "right-hand") {
			moveevent->setSlot(SLOTP_RIGHT);
		} else if (slotName == "left-hand") {
			moveevent->setSlot(SLOTP_LEFT);
		} else if (slotName == "hand" || slotName == "shield") {
			moveevent->setSlot(SLOTP_RIGHT | SLOTP_LEFT);
		} else if (slotName == "legs") {
			moveevent->setSlot(SLOTP_LEGS);
		} else if (slotName == "feet") {
			moveevent->setSlot(SLOTP_FEET);
		} else if (slotName == "ring") {
			moveevent->setSlot(SLOTP_RING);
		} else if (slotName == "ammo") {
			moveevent->setSlot(SLOTP_AMMO);
		} else {
			std::cout << "[Warning - MoveEvent::configureMoveEvent] Unknown slot type: " << slotName << '\n';
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

int LuaScriptInterface::luaMoveEventLevel(lua_State* L)
{
	// moveevent:level(lvl)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->setRequiredLevel(tfs::lua::getNumber<uint32_t>(L, 2));
		moveevent->setWieldInfo(WIELDINFO_LEVEL);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventMagLevel(lua_State* L)
{
	// moveevent:magicLevel(lvl)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->setRequiredMagLevel(tfs::lua::getNumber<uint32_t>(L, 2));
		moveevent->setWieldInfo(WIELDINFO_MAGLV);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventPremium(lua_State* L)
{
	// moveevent:premium(bool)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->setNeedPremium(tfs::lua::getBoolean(L, 2));
		moveevent->setWieldInfo(WIELDINFO_PREMIUM);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventVocation(lua_State* L)
{
	// moveevent:vocation(vocName[, showInDescription = false, lastVoc = false])
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->addVocationEquipSet(tfs::lua::getString(L, 2));
		moveevent->setWieldInfo(WIELDINFO_VOCREQ);
		std::string tmp;
		bool showInDescription = false;
		bool lastVoc = false;
		if (tfs::lua::getBoolean(L, 3)) {
			showInDescription = tfs::lua::getBoolean(L, 3);
		}
		if (tfs::lua::getBoolean(L, 4)) {
			lastVoc = tfs::lua::getBoolean(L, 4);
		}
		if (showInDescription) {
			if (moveevent->getVocationString().empty()) {
				tmp = boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
				tmp += "s";
				moveevent->setVocationString(tmp);
			} else {
				tmp = moveevent->getVocationString();
				if (lastVoc) {
					tmp += " and ";
				} else {
					tmp += ", ";
				}
				tmp += boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
				tmp += "s";
				moveevent->setVocationString(tmp);
			}
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventTileItem(lua_State* L)
{
	// moveevent:tileItem(bool)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->setTileItem(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventItemId(lua_State* L)
{
	// moveevent:id(ids)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_moveEvents->addItemId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2 + i));
			}
		} else {
			g_moveEvents->addItemId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventActionId(lua_State* L)
{
	// moveevent:aid(ids)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_moveEvents->addActionId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2 + i));
			}
		} else {
			g_moveEvents->addActionId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventUniqueId(lua_State* L)
{
	// moveevent:uid(ids)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_moveEvents->addUniqueId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2 + i));
			}
		} else {
			g_moveEvents->addUniqueId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaMoveEventPosition(lua_State* L)
{
	// moveevent:position(positions)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_moveEvents->addPosList(moveevent, tfs::lua::getPosition(L, 2 + i));
			}
		} else {
			g_moveEvents->addPosList(moveevent, tfs::lua::getPosition(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaCreateGlobalEvent(lua_State* L)
{
	// GlobalEvent(eventName)
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		reportErrorFunc(L, "GlobalEvents can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	GlobalEvent* globalEvent = new GlobalEvent(tfs::lua::getScriptEnv()->getScriptInterface());
	globalEvent->setName(tfs::lua::getString(L, 2));
	globalEvent->setEventType(GLOBALEVENT_NONE);
	globalEvent->fromLua = true;
	tfs::lua::pushUserdata(L, globalEvent);
	tfs::lua::setMetatable(L, -1, "GlobalEvent");
	return 1;
}

int LuaScriptInterface::luaGlobalEventType(lua_State* L)
{
	// globalevent:type(callback)
	GlobalEvent* global = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (global) {
		std::string typeName = tfs::lua::getString(L, 2);
		std::string tmpStr = boost::algorithm::to_lower_copy(typeName);
		if (tmpStr == "startup") {
			global->setEventType(GLOBALEVENT_STARTUP);
		} else if (tmpStr == "shutdown") {
			global->setEventType(GLOBALEVENT_SHUTDOWN);
		} else if (tmpStr == "record") {
			global->setEventType(GLOBALEVENT_RECORD);
		} else if (tmpStr == "timer") {
			global->setEventType(GLOBALEVENT_TIMER);
		} else if (tmpStr == "save") {
			global->setEventType(GLOBALEVENT_SAVE);
		} else {
			std::cout << "[Error - CreatureEvent::configureLuaEvent] Invalid type for global event: " << typeName
			          << '\n';
			tfs::lua::pushBoolean(L, false);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGlobalEventRegister(lua_State* L)
{
	// globalevent:register()
	GlobalEvent* globalevent = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (globalevent) {
		if (!globalevent->isScripted()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		if (globalevent->getEventType() == GLOBALEVENT_NONE && globalevent->getInterval() == 0) {
			std::cout << "[Error - LuaScriptInterface::luaGlobalEventRegister] No interval for globalevent with name "
			          << globalevent->getName() << '\n';
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, g_globalEvents->registerLuaEvent(globalevent));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGlobalEventOnCallback(lua_State* L)
{
	// globalevent:onThink / record / etc. (callback)
	GlobalEvent* globalevent = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (globalevent) {
		if (!globalevent->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGlobalEventTime(lua_State* L)
{
	// globalevent:time(time)
	GlobalEvent* globalevent = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (globalevent) {
		std::string timer = tfs::lua::getString(L, 2);
		std::vector<int32_t> params = vectorAtoi(explodeString(timer, ":"));

		int32_t hour = params.front();
		if (hour < 0 || hour > 23) {
			std::cout << "[Error - GlobalEvent::configureEvent] Invalid hour \"" << timer
			          << "\" for globalevent with name: " << globalevent->getName() << '\n';
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		globalevent->setInterval(hour << 16);

		int32_t min = 0;
		int32_t sec = 0;
		if (params.size() > 1) {
			min = params[1];
			if (min < 0 || min > 59) {
				std::cout << "[Error - GlobalEvent::configureEvent] Invalid minute \"" << timer
				          << "\" for globalevent with name: " << globalevent->getName() << '\n';
				tfs::lua::pushBoolean(L, false);
				return 1;
			}

			if (params.size() > 2) {
				sec = params[2];
				if (sec < 0 || sec > 59) {
					std::cout << "[Error - GlobalEvent::configureEvent] Invalid second \"" << timer
					          << "\" for globalevent with name: " << globalevent->getName() << '\n';
					tfs::lua::pushBoolean(L, false);
					return 1;
				}
			}
		}

		time_t current_time = time(nullptr);
		tm* timeinfo = localtime(&current_time);
		timeinfo->tm_hour = hour;
		timeinfo->tm_min = min;
		timeinfo->tm_sec = sec;

		time_t difference = static_cast<time_t>(difftime(mktime(timeinfo), current_time));
		if (difference < 0) {
			difference += 86400;
		}

		globalevent->setNextExecution((current_time + difference) * 1000);
		globalevent->setEventType(GLOBALEVENT_TIMER);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaGlobalEventInterval(lua_State* L)
{
	// globalevent:interval(interval)
	GlobalEvent* globalevent = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (globalevent) {
		globalevent->setInterval(tfs::lua::getNumber<uint32_t>(L, 2));
		globalevent->setNextExecution(OTSYS_TIME() + tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// Weapon
int LuaScriptInterface::luaCreateWeapon(lua_State* L)
{
	// Weapon(type)
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		reportErrorFunc(L, "Weapons can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	WeaponType_t type = tfs::lua::getNumber<WeaponType_t>(L, 2);
	switch (type) {
		case WEAPON_SWORD:
		case WEAPON_AXE:
		case WEAPON_CLUB: {
			WeaponMelee* weapon = new WeaponMelee(tfs::lua::getScriptEnv()->getScriptInterface());
			tfs::lua::pushUserdata(L, weapon);
			tfs::lua::setMetatable(L, -1, "Weapon");
			weapon->weaponType = type;
			weapon->fromLua = true;
			break;
		}
		case WEAPON_DISTANCE:
		case WEAPON_AMMO: {
			WeaponDistance* weapon = new WeaponDistance(tfs::lua::getScriptEnv()->getScriptInterface());
			tfs::lua::pushUserdata(L, weapon);
			tfs::lua::setMetatable(L, -1, "Weapon");
			weapon->weaponType = type;
			weapon->fromLua = true;
			break;
		}
		case WEAPON_WAND: {
			WeaponWand* weapon = new WeaponWand(tfs::lua::getScriptEnv()->getScriptInterface());
			tfs::lua::pushUserdata(L, weapon);
			tfs::lua::setMetatable(L, -1, "Weapon");
			weapon->weaponType = type;
			weapon->fromLua = true;
			break;
		}
		default: {
			lua_pushnil(L);
			break;
		}
	}
	return 1;
}

int LuaScriptInterface::luaWeaponAction(lua_State* L)
{
	// weapon:action(callback)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		std::string typeName = tfs::lua::getString(L, 2);
		std::string tmpStr = boost::algorithm::to_lower_copy(typeName);
		if (tmpStr == "removecount") {
			weapon->action = WEAPONACTION_REMOVECOUNT;
		} else if (tmpStr == "removecharge") {
			weapon->action = WEAPONACTION_REMOVECHARGE;
		} else if (tmpStr == "move") {
			weapon->action = WEAPONACTION_MOVE;
		} else {
			std::cout << "Error: [Weapon::action] No valid action " << typeName << '\n';
			tfs::lua::pushBoolean(L, false);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponRegister(lua_State* L)
{
	// weapon:register()
	Weapon** weaponPtr = tfs::lua::getRawUserdata<Weapon>(L, 1);
	if (!weaponPtr) {
		lua_pushnil(L);
		return 1;
	}

	if (auto* weapon = *weaponPtr) {
		if (weapon->weaponType == WEAPON_DISTANCE || weapon->weaponType == WEAPON_AMMO) {
			weapon = tfs::lua::getUserdata<WeaponDistance>(L, 1);
		} else if (weapon->weaponType == WEAPON_WAND) {
			weapon = tfs::lua::getUserdata<WeaponWand>(L, 1);
		} else {
			weapon = tfs::lua::getUserdata<WeaponMelee>(L, 1);
		}

		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.weaponType = weapon->weaponType;

		if (weapon->getWieldInfo() != 0) {
			it.wieldInfo = weapon->getWieldInfo();
			it.vocationString = weapon->getVocationString();
			it.minReqLevel = weapon->getReqLevel();
			it.minReqMagicLevel = weapon->getReqMagLv();
		}

		weapon->configureWeapon(it);
		tfs::lua::pushBoolean(L, g_weapons->registerLuaEvent(weapon));
		*weaponPtr = nullptr; // Remove luascript reference
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponOnUseWeapon(lua_State* L)
{
	// weapon:onUseWeapon(callback)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		if (!weapon->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponUnproperly(lua_State* L)
{
	// weapon:wieldUnproperly(bool)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setWieldUnproperly(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponLevel(lua_State* L)
{
	// weapon:level(lvl)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setRequiredLevel(tfs::lua::getNumber<uint32_t>(L, 2));
		weapon->setWieldInfo(WIELDINFO_LEVEL);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponMagicLevel(lua_State* L)
{
	// weapon:magicLevel(lvl)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setRequiredMagLevel(tfs::lua::getNumber<uint32_t>(L, 2));
		weapon->setWieldInfo(WIELDINFO_MAGLV);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponMana(lua_State* L)
{
	// weapon:mana(mana)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setMana(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponManaPercent(lua_State* L)
{
	// weapon:manaPercent(percent)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setManaPercent(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponHealth(lua_State* L)
{
	// weapon:health(health)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setHealth(tfs::lua::getNumber<int32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponHealthPercent(lua_State* L)
{
	// weapon:healthPercent(percent)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setHealthPercent(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponSoul(lua_State* L)
{
	// weapon:soul(soul)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setSoul(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponBreakChance(lua_State* L)
{
	// weapon:breakChance(percent)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setBreakChance(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponWandDamage(lua_State* L)
{
	// weapon:damage(damage[min, max]) only use this if the weapon is a wand!
	WeaponWand* weapon = tfs::lua::getUserdata<WeaponWand>(L, 1);
	if (weapon) {
		weapon->setMinChange(tfs::lua::getNumber<uint32_t>(L, 2));
		if (lua_gettop(L) > 2) {
			weapon->setMaxChange(tfs::lua::getNumber<uint32_t>(L, 3));
		} else {
			weapon->setMaxChange(tfs::lua::getNumber<uint32_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponElement(lua_State* L)
{
	// weapon:element(combatType)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		if (!tfs::lua::getNumber<CombatType_t>(L, 2)) {
			std::string element = tfs::lua::getString(L, 2);
			std::string tmpStrValue = boost::algorithm::to_lower_copy(element);
			if (tmpStrValue == "earth") {
				weapon->params.combatType = COMBAT_EARTHDAMAGE;
			} else if (tmpStrValue == "ice") {
				weapon->params.combatType = COMBAT_ICEDAMAGE;
			} else if (tmpStrValue == "energy") {
				weapon->params.combatType = COMBAT_ENERGYDAMAGE;
			} else if (tmpStrValue == "fire") {
				weapon->params.combatType = COMBAT_FIREDAMAGE;
			} else if (tmpStrValue == "death") {
				weapon->params.combatType = COMBAT_DEATHDAMAGE;
			} else if (tmpStrValue == "holy") {
				weapon->params.combatType = COMBAT_HOLYDAMAGE;
			} else {
				std::cout << "[Warning - weapon:element] Type \"" << element << "\" does not exist.\n";
			}
		} else {
			weapon->params.combatType = tfs::lua::getNumber<CombatType_t>(L, 2);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponPremium(lua_State* L)
{
	// weapon:premium(bool)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setNeedPremium(tfs::lua::getBoolean(L, 2));
		weapon->setWieldInfo(WIELDINFO_PREMIUM);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponVocation(lua_State* L)
{
	// weapon:vocation(vocName[, showInDescription = false, lastVoc = false])
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->addVocationWeaponSet(tfs::lua::getString(L, 2));
		weapon->setWieldInfo(WIELDINFO_VOCREQ);
		std::string tmp;
		bool showInDescription = tfs::lua::getBoolean(L, 3, false);
		bool lastVoc = tfs::lua::getBoolean(L, 4, false);

		if (showInDescription) {
			if (weapon->getVocationString().empty()) {
				tmp = boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
				tmp += "s";
				weapon->setVocationString(tmp);
			} else {
				tmp = weapon->getVocationString();
				if (lastVoc) {
					tmp += " and ";
				} else {
					tmp += ", ";
				}
				tmp += boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
				tmp += "s";
				weapon->setVocationString(tmp);
			}
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponId(lua_State* L)
{
	// weapon:id(id)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		weapon->setID(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponAttack(lua_State* L)
{
	// weapon:attack(atk)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.attack = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponDefense(lua_State* L)
{
	// weapon:defense(defense[, extraDefense])
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.defense = tfs::lua::getNumber<int32_t>(L, 2);
		if (lua_gettop(L) > 2) {
			it.extraDefense = tfs::lua::getNumber<int32_t>(L, 3);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponRange(lua_State* L)
{
	// weapon:range(range)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.shootRange = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponCharges(lua_State* L)
{
	// weapon:charges(charges[, showCharges = true])
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		bool showCharges = tfs::lua::getBoolean(L, 3, true);
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);

		it.charges = tfs::lua::getNumber<uint32_t>(L, 2);
		it.showCharges = showCharges;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponDuration(lua_State* L)
{
	// weapon:duration(duration[, showDuration = true])
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		bool showDuration = tfs::lua::getBoolean(L, 3, true);
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);

		if (lua_istable(L, 2)) {
			it.decayTimeMin = tfs::lua::getField<uint32_t>(L, 2, "min");
			it.decayTimeMax = tfs::lua::getField<uint32_t>(L, 2, "max");
		} else {
			it.decayTimeMin = tfs::lua::getNumber<uint32_t>(L, 2);
		}

		it.showDuration = showDuration;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponDecayTo(lua_State* L)
{
	// weapon:decayTo([itemid = 0])
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t itemid = tfs::lua::getNumber<uint16_t>(L, 2, 0);
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);

		it.decayTo = itemid;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponTransformEquipTo(lua_State* L)
{
	// weapon:transformEquipTo(itemid)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.transformEquipTo = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponTransformDeEquipTo(lua_State* L)
{
	// weapon:transformDeEquipTo(itemid)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.transformDeEquipTo = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponShootType(lua_State* L)
{
	// weapon:shootType(type)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.shootType = tfs::lua::getNumber<ShootType_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponSlotType(lua_State* L)
{
	// weapon:slotType(slot)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		std::string slot = tfs::lua::getString(L, 2);

		if (slot == "two-handed") {
			it.slotPosition |= SLOTP_TWO_HAND;
		} else {
			it.slotPosition |= SLOTP_HAND;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponAmmoType(lua_State* L)
{
	// weapon:ammoType(type)
	WeaponDistance* weapon = tfs::lua::getUserdata<WeaponDistance>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		std::string type = tfs::lua::getString(L, 2);

		if (type == "arrow") {
			it.ammoType = AMMO_ARROW;
		} else if (type == "bolt") {
			it.ammoType = AMMO_BOLT;
		} else {
			std::cout << "[Warning - weapon:ammoType] Type \"" << type << "\" does not exist.\n";
			lua_pushnil(L);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponHitChance(lua_State* L)
{
	// weapon:hitChance(chance)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.hitChance = tfs::lua::getNumber<int8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponMaxHitChance(lua_State* L)
{
	// weapon:maxHitChance(max)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.maxHitChance = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaWeaponExtraElement(lua_State* L)
{
	// weapon:extraElement(atk, combatType)
	Weapon* weapon = tfs::lua::getUserdata<Weapon>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.abilities.get()->elementDamage = tfs::lua::getNumber<uint16_t>(L, 2);

		if (!tfs::lua::getNumber<CombatType_t>(L, 3)) {
			std::string element = tfs::lua::getString(L, 3);
			std::string tmpStrValue = boost::algorithm::to_lower_copy(element);
			if (tmpStrValue == "earth") {
				it.abilities.get()->elementType = COMBAT_EARTHDAMAGE;
			} else if (tmpStrValue == "ice") {
				it.abilities.get()->elementType = COMBAT_ICEDAMAGE;
			} else if (tmpStrValue == "energy") {
				it.abilities.get()->elementType = COMBAT_ENERGYDAMAGE;
			} else if (tmpStrValue == "fire") {
				it.abilities.get()->elementType = COMBAT_FIREDAMAGE;
			} else if (tmpStrValue == "death") {
				it.abilities.get()->elementType = COMBAT_DEATHDAMAGE;
			} else if (tmpStrValue == "holy") {
				it.abilities.get()->elementType = COMBAT_HOLYDAMAGE;
			} else {
				std::cout << "[Warning - weapon:extraElement] Type \"" << element << "\" does not exist.\n";
			}
		} else {
			it.abilities.get()->elementType = tfs::lua::getNumber<CombatType_t>(L, 3);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// XML
int LuaScriptInterface::luaCreateXmlDocument(lua_State* L)
{
	// XMLDocument(filename)
	std::string filename = tfs::lua::getString(L, 2);
	if (filename.empty()) {
		lua_pushnil(L);
		return 1;
	}

	auto doc = std::make_unique<pugi::xml_document>();
	if (auto result = doc->load_file(filename.data())) {
		tfs::lua::pushUserdata(L, doc.release());
		tfs::lua::setMetatable(L, -1, "XMLDocument");
	} else {
		printXMLError("Error - LuaScriptInterface::luaCreateXmlDocument", filename, result);
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaDeleteXmlDocument(lua_State* L)
{
	// doc:delete() or doc:__gc()
	pugi::xml_document** document = tfs::lua::getRawUserdata<pugi::xml_document>(L, 1);
	if (document && *document) {
		delete *document;
		*document = nullptr;
	}
	return 1;
}

int LuaScriptInterface::luaXmlDocumentChild(lua_State* L)
{
	// doc:child(name)
	pugi::xml_document* document = tfs::lua::getUserdata<pugi::xml_document>(L, 1);
	if (!document) {
		lua_pushnil(L);
		return 1;
	}

	std::string name = tfs::lua::getString(L, 2);
	if (name.empty()) {
		lua_pushnil(L);
		return 1;
	}

	auto node = std::make_unique<pugi::xml_node>(document->child(name.data()));
	tfs::lua::pushUserdata(L, node.release());
	tfs::lua::setMetatable(L, -1, "XMLNode");
	return 1;
}

int LuaScriptInterface::luaDeleteXmlNode(lua_State* L)
{
	// node:delete() or node:__gc()
	pugi::xml_node** node = tfs::lua::getRawUserdata<pugi::xml_node>(L, 1);
	if (node && *node) {
		delete *node;
		*node = nullptr;
	}
	return 1;
}

int LuaScriptInterface::luaXmlNodeAttribute(lua_State* L)
{
	// node:attribute(name)
	pugi::xml_node* node = tfs::lua::getUserdata<pugi::xml_node>(L, 1);
	if (!node) {
		lua_pushnil(L);
		return 1;
	}

	std::string name = tfs::lua::getString(L, 2);
	if (name.empty()) {
		lua_pushnil(L);
		return 1;
	}

	pugi::xml_attribute attribute = node->attribute(name.data());
	if (attribute) {
		tfs::lua::pushString(L, attribute.value());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int LuaScriptInterface::luaXmlNodeName(lua_State* L)
{
	// node:name()
	pugi::xml_node* node = tfs::lua::getUserdata<pugi::xml_node>(L, 1);
	if (!node) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushString(L, node->name());
	return 1;
}

int LuaScriptInterface::luaXmlNodeFirstChild(lua_State* L)
{
	// node:firstChild()
	pugi::xml_node* node = tfs::lua::getUserdata<pugi::xml_node>(L, 1);
	if (!node) {
		lua_pushnil(L);
		return 1;
	}

	auto firstChild = node->first_child();
	if (!firstChild) {
		lua_pushnil(L);
		return 1;
	}

	auto newNode = std::make_unique<pugi::xml_node>(firstChild);
	tfs::lua::pushUserdata(L, newNode.release());
	tfs::lua::setMetatable(L, -1, "XMLNode");
	return 1;
}

int LuaScriptInterface::luaXmlNodeNextSibling(lua_State* L)
{
	// node:nextSibling()
	pugi::xml_node* node = tfs::lua::getUserdata<pugi::xml_node>(L, 1);
	if (!node) {
		lua_pushnil(L);
		return 1;
	}

	auto nextSibling = node->next_sibling();
	if (!nextSibling) {
		lua_pushnil(L);
		return 1;
	}

	auto newNode = std::make_unique<pugi::xml_node>(nextSibling);
	tfs::lua::pushUserdata(L, newNode.release());
	tfs::lua::setMetatable(L, -1, "XMLNode");
	return 1;
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
	L = luaL_newstate();
	if (!L) {
		return false;
	}

	luaL_openlibs(L);
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
	if (!L) {
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
			luaL_unref(L, LUA_REGISTRYINDEX, parameter);
		}
		luaL_unref(L, LUA_REGISTRYINDEX, timerEventDesc.function);
	}

	combatIdMap.clear();
	areaIdMap.clear();
	timerEvents.clear();
	cacheFiles.clear();

	lua_close(L);
	L = nullptr;
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
	lua_rawgeti(L, LUA_REGISTRYINDEX, timerEventDesc.function);

	// push parameters
	for (auto parameter : std::views::reverse(timerEventDesc.parameters)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, parameter);
	}

	// call the function
	if (tfs::lua::reserveScriptEnv()) {
		ScriptEnvironment* env = tfs::lua::getScriptEnv();
		env->setTimerEvent();
		env->setScriptId(timerEventDesc.scriptId, this);
		callFunction(timerEventDesc.parameters.size());
	} else {
		std::cout << "[Error - LuaScriptInterface::executeTimerEvent] Call stack overflow\n";
	}

	// free resources
	luaL_unref(L, LUA_REGISTRYINDEX, timerEventDesc.function);
	for (auto parameter : timerEventDesc.parameters) {
		luaL_unref(L, LUA_REGISTRYINDEX, parameter);
	}
}
