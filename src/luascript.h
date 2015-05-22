/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_LUASCRIPT_H_5344B2BC907E46E3943EA78574A212D8
#define FS_LUASCRIPT_H_5344B2BC907E46E3943EA78574A212D8

#include <lua.hpp>

#if LUA_VERSION_NUM >= 502
// NOTE: Define LUA_COMPAT_ALL as a workaround if this doesn't work
#ifndef LUA_COMPAT_ALL
#ifndef LUA_COMPAT_MODULE
#define luaL_register(L, libname, l) (luaL_newlib(L, l), lua_pushvalue(L, -1), lua_setglobal(L, libname))
#endif
#define lua_equal(L, i1, i2) lua_compare(L, (i1), (i2), LUA_OPEQ)
#endif
#endif

#include "database.h"
#include "enums.h"
#include "position.h"

class Thing;
class Creature;
class Player;
class Item;
class Container;
class AreaCombat;
class Combat;
class Condition;
class Npc;
class Monster;

enum LuaVariantType_t {
	VARIANT_NONE,

	VARIANT_NUMBER,
	VARIANT_POSITION,
	VARIANT_TARGETPOSITION,
	VARIANT_STRING,
};

enum LuaDataType {
	LuaData_Unknown,

	LuaData_Item,
	LuaData_Container,
	LuaData_Teleport,
	LuaData_Player,
	LuaData_Monster,
	LuaData_Npc,
	LuaData_Tile,
};

struct LuaVariant {
	LuaVariant() {
		type = VARIANT_NONE;
		number = 0;
	}

	LuaVariantType_t type;
	std::string text;
	Position pos;
	uint32_t number;
};

struct LuaTimerEventDesc {
	int32_t scriptId;
	int32_t function;
	std::list<int32_t> parameters;
	uint32_t eventId;

	LuaTimerEventDesc() :
		scriptId(-1), function(-1), eventId(0) {}

	LuaTimerEventDesc(LuaTimerEventDesc&& other) :
		scriptId(other.scriptId), function(other.function),
		parameters(std::move(other.parameters)), eventId(other.eventId) {}
};

class LuaScriptInterface;
class Game;
class Npc;

class ScriptEnvironment
{
	public:
		ScriptEnvironment();
		~ScriptEnvironment();

		// non-copyable
		ScriptEnvironment(const ScriptEnvironment&) = delete;
		ScriptEnvironment& operator=(const ScriptEnvironment&) = delete;

		void resetEnv();

		void setScriptId(int32_t scriptId, LuaScriptInterface* scriptInterface) {
			m_scriptId = scriptId;
			m_interface = scriptInterface;
		}
		bool setCallbackId(int32_t callbackId, LuaScriptInterface* scriptInterface);
		void setEventDesc(std::string desc) {
			m_eventdesc = desc;
		}

		std::string getEventDesc() const {
			return m_eventdesc;
		}
		int32_t getScriptId() const {
			return m_scriptId;
		}
		int32_t getCallbackId() const {
			return m_callbackId;
		}
		LuaScriptInterface* getScriptInterface() {
			return m_interface;
		}

		void setTimerEvent() {
			m_timerEvent = true;
		}
		void resetTimerEvent() {
			m_timerEvent = false;
		}

		void getEventInfo(int32_t& scriptId, std::string& desc, LuaScriptInterface*& scriptInterface, int32_t& callbackId, bool& timerEvent) const;

		void addTempItem(Item* item);
		static void removeTempItem(Item* item);
		uint32_t addThing(Thing* thing);
		void insertItem(uint32_t uid, Item* item);

		static DBResult_ptr getResultByID(uint32_t id);
		static uint32_t addResult(DBResult_ptr res);
		static bool removeResult(uint32_t id);

		void setNpc(Npc* npc) {
			m_curNpc = npc;
		}
		Npc* getNpc() const {
			return m_curNpc;
		}

		Thing* getThingByUID(uint32_t uid);
		Item* getItemByUID(uint32_t uid);
		Container* getContainerByUID(uint32_t uid);
		void removeItemByUID(uint32_t uid);

	private:
		typedef std::vector<const LuaVariant*> VariantVector;
		typedef std::map<uint32_t, int32_t> StorageMap;
		typedef std::map<uint32_t, DBResult_ptr> DBResultMap;

		//script file id
		int32_t m_scriptId;
		int32_t m_callbackId;
		bool m_timerEvent;
		LuaScriptInterface* m_interface;
		//script event desc
		std::string m_eventdesc;

		//local item map
		uint32_t m_lastUID;
		std::unordered_map<uint32_t, Item*> localMap;

		//temporary item list
		static std::multimap<ScriptEnvironment*, Item*> tempItems;

		//result map
		static uint32_t m_lastResultId;
		static DBResultMap m_tempResults;

		//for npc scripts
		Npc* m_curNpc;
};

#define reportErrorFunc(a)  reportError(__FUNCTION__, a, true)

enum ErrorCode_t {
	LUA_ERROR_PLAYER_NOT_FOUND,
	LUA_ERROR_CREATURE_NOT_FOUND,
	LUA_ERROR_ITEM_NOT_FOUND,
	LUA_ERROR_THING_NOT_FOUND,
	LUA_ERROR_TILE_NOT_FOUND,
	LUA_ERROR_HOUSE_NOT_FOUND,
	LUA_ERROR_COMBAT_NOT_FOUND,
	LUA_ERROR_CONDITION_NOT_FOUND,
	LUA_ERROR_AREA_NOT_FOUND,
	LUA_ERROR_CONTAINER_NOT_FOUND,
	LUA_ERROR_VARIANT_NOT_FOUND,
	LUA_ERROR_VARIANT_UNKNOWN,
	LUA_ERROR_SPELL_NOT_FOUND,
};

class LuaScriptInterface
{
	public:
		explicit LuaScriptInterface(std::string interfaceName);
		virtual ~LuaScriptInterface();

		// non-copyable
		LuaScriptInterface(const LuaScriptInterface&) = delete;
		LuaScriptInterface& operator=(const LuaScriptInterface&) = delete;

		virtual bool initState();
		bool reInitState();

		int32_t loadFile(const std::string& file, Npc* npc = nullptr);

		const std::string& getFileById(int32_t scriptId);
		int32_t getEvent(const std::string& eventName);
		int32_t getMetaEvent(const std::string& globalName, const std::string& eventName);

		static ScriptEnvironment* getScriptEnv() {
			assert(m_scriptEnvIndex >= 0 && m_scriptEnvIndex < 16);
			return &m_scriptEnv[m_scriptEnvIndex];
		}

		static bool reserveScriptEnv() {
			return ++m_scriptEnvIndex < 16;
		}

		static void resetScriptEnv() {
			assert(m_scriptEnvIndex >= 0);
			m_scriptEnv[m_scriptEnvIndex--].resetEnv();
		}

		static void reportError(const char* function, const std::string& error_desc, bool stack_trace = false);

		const std::string& getInterfaceName() const {
			return m_interfaceName;
		}
		const std::string& getLastLuaError() const {
			return m_lastLuaError;
		}

		lua_State* getLuaState() const {
			return m_luaState;
		}

		bool pushFunction(int32_t functionId);

		static int luaErrorHandler(lua_State* L);
		bool callFunction(int params);
		void callVoidFunction(int params);

		//push/pop common structures
		static void pushThing(lua_State* L, Thing* thing);
		static void pushVariant(lua_State* L, const LuaVariant& var);
		static void pushString(lua_State* L, const std::string& value);
		static void pushCallback(lua_State* L, int32_t callback);

		static std::string popString(lua_State* L);
		static int32_t popCallback(lua_State* L);

		// Userdata
		template<class T>
		static void pushUserdata(lua_State* L, T* value)
		{
			T** userdata = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
			*userdata = value;
		}

		// Metatables
		static void setMetatable(lua_State* L, int32_t index, const std::string& name);
		static void setWeakMetatable(lua_State* L, int32_t index, const std::string& name);

		static void setItemMetatable(lua_State* L, int32_t index, const Item* item);
		static void setCreatureMetatable(lua_State* L, int32_t index, const Creature* creature);

		// Get
		template<typename T>
		inline static typename std::enable_if<std::is_enum<T>::value, T>::type
			getNumber(lua_State* L, int32_t arg)
		{
			return static_cast<T>(static_cast<int64_t>(lua_tonumber(L, arg)));
		}
		template<typename T>
		inline static typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, T>::type
			getNumber(lua_State* L, int32_t arg)
		{
			return static_cast<T>(lua_tonumber(L, arg));
		}
		template<typename T>
		static T getNumber(lua_State *L, int32_t arg, T defaultValue)
		{
			const auto parameters = lua_gettop(L);
			if (parameters == 0 || arg > parameters) {
				return defaultValue;
			}
			return getNumber<T>(L, arg);
		}
		template<class T>
		static T* getUserdata(lua_State* L, int32_t arg)
		{
			T** userdata = getRawUserdata<T>(L, arg);
			if (!userdata) {
				return nullptr;
			}
			return *userdata;
		}
		template<class T>
		inline static T** getRawUserdata(lua_State* L, int32_t arg)
		{
			return reinterpret_cast<T**>(lua_touserdata(L, arg));
		}

		inline static bool getBoolean(lua_State* L, int32_t arg)
		{
			return lua_toboolean(L, arg) != 0;
		}
		inline static bool getBoolean(lua_State* L, int32_t arg, bool defaultValue)
		{
			const auto parameters = lua_gettop(L);
			if (parameters == 0 || arg > parameters) {
				return defaultValue;
			}
			return lua_toboolean(L, arg) != 0;
		}

		static std::string getString(lua_State* L, int32_t arg);
		static Position getPosition(lua_State* L, int32_t arg, int32_t& stackpos);
		static Position getPosition(lua_State* L, int32_t arg);
		static Outfit_t getOutfit(lua_State* L, int32_t arg);
		static LuaVariant getVariant(lua_State* L, int32_t arg);

		static Thing* getThing(lua_State* L, int32_t arg);
		static Creature* getCreature(lua_State* L, int32_t arg);
		static Player* getPlayer(lua_State* L, int32_t arg);

		template<typename T>
		static T getField(lua_State* L, int32_t arg, const std::string& key)
		{
			lua_getfield(L, arg, key.c_str());
			return getNumber<T>(L, -1);
		}

		static std::string getFieldString(lua_State* L, int32_t arg, const std::string& key);

		static LuaDataType getUserdataType(lua_State* L, int32_t arg);

		// Is
		inline static bool isNumber(lua_State* L, int32_t arg)
		{
			return lua_isnumber(L, arg) != 0;
		}
		inline static bool isString(lua_State* L, int32_t arg)
		{
			return lua_isstring(L, arg) != 0;
		}
		inline static bool isBoolean(lua_State* L, int32_t arg)
		{
			return lua_isboolean(L, arg);
		}
		inline static bool isTable(lua_State* L, int32_t arg)
		{
			return lua_istable(L, arg);
		}
		inline static bool isFunction(lua_State* L, int32_t arg)
		{
			return lua_isfunction(L, arg);
		}
		inline static bool isUserdata(lua_State* L, int32_t arg)
		{
			return lua_isuserdata(L, arg) != 0;
		}

		// Push
		static void pushBoolean(lua_State* L, bool value);
		static void pushPosition(lua_State* L, const Position& position, int32_t stackpos = 0);
		static void pushOutfit(lua_State* L, const Outfit_t& outfit);

		//
		inline static void setField(lua_State* L, const char* index, lua_Number value)
		{
			lua_pushnumber(L, value);
			lua_setfield(L, -2, index);
		}

		inline static void setField(lua_State* L, const char* index, const std::string& value)
		{
			pushString(L, value);
			lua_setfield(L, -2, index);
		}

		static std::string escapeString(const std::string& string);

#ifndef LUAJIT_VERSION
		static const luaL_Reg luaBitReg[7];
#endif
		static const luaL_Reg luaConfigManagerTable[4];
		static const luaL_Reg luaDatabaseTable[9];
		static const luaL_Reg luaResultTable[6];

		static int protectedCall(lua_State* L, int nargs, int nresults);

	protected:
		virtual bool closeState();

		void registerFunctions();

		void registerClass(const std::string& className, const std::string& baseClass, lua_CFunction newFunction = nullptr);
		void registerTable(const std::string& tableName);
		void registerMethod(const std::string& className, const std::string& methodName, lua_CFunction func);
		void registerMetaMethod(const std::string& className, const std::string& methodName, lua_CFunction func);
		void registerGlobalMethod(const std::string& functionName, lua_CFunction func);
		void registerVariable(const std::string& tableName, const std::string& name, lua_Number value);
		void registerGlobalVariable(const std::string& name, lua_Number value);
		void registerGlobalBoolean(const std::string& name, bool value);

		std::string getStackTrace(const std::string& error_desc);

		static std::string getErrorDesc(ErrorCode_t code);
		static bool getArea(lua_State* L, std::list<uint32_t>& list, uint32_t& rows);

		//lua functions
		static int luaDoCreateItem(lua_State* L);
		static int luaDoCreateItemEx(lua_State* L);
		static int luaDoMoveCreature(lua_State* L);

		static int luaDoPlayerAddItem(lua_State* L);
		static int luaDoTileAddItemEx(lua_State* L);
		static int luaDoSetCreatureLight(lua_State* L);

		//get item info
		static int luaGetDepotId(lua_State* L);

		//get creature info functions
		static int luaGetPlayerFlagValue(lua_State* L);
		static int luaGetCreatureCondition(lua_State* L);

		static int luaGetPlayerInstantSpellInfo(lua_State* L);
		static int luaGetPlayerInstantSpellCount(lua_State* L);

		static int luaGetWorldTime(lua_State* L);
		static int luaGetWorldLight(lua_State* L);
		static int luaGetWorldUpTime(lua_State* L);

		//type validation
		static int luaIsDepot(lua_State* L);
		static int luaIsMoveable(lua_State* L);
		static int luaIsValidUID(lua_State* L);

		//container
		static int luaDoAddContainerItem(lua_State* L);

		//
		static int luaCreateCombatObject(lua_State* L);
		static int luaCreateCombatArea(lua_State* L);
		static int luaSetCombatArea(lua_State* L);
		static int luaSetCombatCondition(lua_State* L);
		static int luaSetCombatParam(lua_State* L);
		static int luaCreateConditionObject(lua_State* L);
		static int luaSetConditionParam(lua_State* L);
		static int luaAddDamageCondition(lua_State* L);
		static int luaAddOutfitCondition(lua_State* L);

		static int luaSetCombatCallBack(lua_State* L);
		static int luaSetCombatFormula(lua_State* L);
		static int luaSetConditionFormula(lua_State* L);
		static int luaDoCombat(lua_State* L);

		static int luaDoAreaCombatHealth(lua_State* L);
		static int luaDoTargetCombatHealth(lua_State* L);

		//
		static int luaDoAreaCombatMana(lua_State* L);
		static int luaDoTargetCombatMana(lua_State* L);

		static int luaDoAreaCombatCondition(lua_State* L);
		static int luaDoTargetCombatCondition(lua_State* L);

		static int luaDoAreaCombatDispel(lua_State* L);
		static int luaDoTargetCombatDispel(lua_State* L);

		static int luaDoChallengeCreature(lua_State* L);

		static int luaSetCreatureOutfit(lua_State* L);
		static int luaSetMonsterOutfit(lua_State* L);
		static int luaSetItemOutfit(lua_State* L);

		static int luaDebugPrint(lua_State* L);
		static int luaIsInArray(lua_State* L);
		static int luaAddEvent(lua_State* L);
		static int luaStopEvent(lua_State* L);

		static int luaSaveServer(lua_State* L);
		static int luaCleanMap(lua_State* L);

		static int luaIsInWar(lua_State* L);
		static int luaDoPlayerSetOfflineTrainingSkill(lua_State* L);

		static int luaGetWaypointPositionByName(lua_State* L);

		static int luaSendChannelMessage(lua_State* L);
		static int luaSendGuildChannelMessage(lua_State* L);

#ifndef LUAJIT_VERSION
		static int luaBitNot(lua_State* L);
		static int luaBitAnd(lua_State* L);
		static int luaBitOr(lua_State* L);
		static int luaBitXor(lua_State* L);
		static int luaBitLeftShift(lua_State* L);
		static int luaBitRightShift(lua_State* L);
#endif

		static int luaConfigManagerGetString(lua_State* L);
		static int luaConfigManagerGetNumber(lua_State* L);
		static int luaConfigManagerGetBoolean(lua_State* L);

		static int luaDatabaseExecute(lua_State* L);
		static int luaDatabaseAsyncExecute(lua_State* L);
		static int luaDatabaseStoreQuery(lua_State* L);
		static int luaDatabaseAsyncStoreQuery(lua_State* L);
		static int luaDatabaseEscapeString(lua_State* L);
		static int luaDatabaseEscapeBlob(lua_State* L);
		static int luaDatabaseLastInsertId(lua_State* L);
		static int luaDatabaseConnected(lua_State* L);
		static int luaDatabaseTableExists(lua_State* L);

		static int luaResultGetNumber(lua_State* L);
		static int luaResultGetString(lua_State* L);
		static int luaResultGetStream(lua_State* L);
		static int luaResultNext(lua_State* L);
		static int luaResultFree(lua_State* L);

		// Userdata
		static int luaUserdataCompare(lua_State* L);

		// _G
		static int luaIsType(lua_State* L);
		static int luaRawGetMetatable(lua_State* L);

		// os
		static int luaSystemTime(lua_State* L);

		// table
		static int luaTableCreate(lua_State* L);

		// Game
		static int luaGameGetSpectators(lua_State* L);
		static int luaGameGetPlayers(lua_State* L);
		static int luaGameLoadMap(lua_State* L);

		static int luaGameGetExperienceStage(lua_State* L);
		static int luaGameGetMonsterCount(lua_State* L);
		static int luaGameGetPlayerCount(lua_State* L);
		static int luaGameGetNpcCount(lua_State* L);

		static int luaGameGetTowns(lua_State* L);
		static int luaGameGetHouses(lua_State* L);

		static int luaGameGetGameState(lua_State* L);
		static int luaGameSetGameState(lua_State* L);

		static int luaGameGetWorldType(lua_State* L);
		static int luaGameSetWorldType(lua_State* L);

		static int luaGameGetReturnMessage(lua_State* L);

		static int luaGameCreateItem(lua_State* L);
		static int luaGameCreateContainer(lua_State* L);
		static int luaGameCreateMonster(lua_State* L);
		static int luaGameCreateNpc(lua_State* L);
		static int luaGameCreateTile(lua_State* L);

		static int luaGameStartRaid(lua_State* L);

		// Variant
		static int luaVariantCreate(lua_State* L);

		static int luaVariantGetNumber(lua_State* L);
		static int luaVariantGetString(lua_State* L);
		static int luaVariantGetPosition(lua_State* L);

		// Position
		static int luaPositionCreate(lua_State* L);
		static int luaPositionAdd(lua_State* L);
		static int luaPositionSub(lua_State* L);
		static int luaPositionCompare(lua_State* L);

		static int luaPositionGetDistance(lua_State* L);
		static int luaPositionIsSightClear(lua_State* L);

		static int luaPositionSendMagicEffect(lua_State* L);
		static int luaPositionSendDistanceEffect(lua_State* L);

		// Tile
		static int luaTileCreate(lua_State* L);

		static int luaTileGetPosition(lua_State* L);
		static int luaTileGetGround(lua_State* L);
		static int luaTileGetThing(lua_State* L);
		static int luaTileGetThingCount(lua_State* L);
		static int luaTileGetTopVisibleThing(lua_State* L);

		static int luaTileGetTopTopItem(lua_State* L);
		static int luaTileGetTopDownItem(lua_State* L);
		static int luaTileGetFieldItem(lua_State* L);

		static int luaTileGetItemById(lua_State* L);
		static int luaTileGetItemByType(lua_State* L);
		static int luaTileGetItemByTopOrder(lua_State* L);
		static int luaTileGetItemCountById(lua_State* L);

		static int luaTileGetBottomCreature(lua_State* L);
		static int luaTileGetTopCreature(lua_State* L);
		static int luaTileGetBottomVisibleCreature(lua_State* L);
		static int luaTileGetTopVisibleCreature(lua_State* L);

		static int luaTileGetItems(lua_State* L);
		static int luaTileGetItemCount(lua_State* L);
		static int luaTileGetDownItemCount(lua_State* L);
		static int luaTileGetTopItemCount(lua_State* L);

		static int luaTileGetCreatures(lua_State* L);
		static int luaTileGetCreatureCount(lua_State* L);

		static int luaTileHasProperty(lua_State* L);
		static int luaTileHasFlag(lua_State* L);

		static int luaTileGetThingIndex(lua_State* L);

		static int luaTileQueryAdd(lua_State* L);

		static int luaTileGetHouse(lua_State* L);

		// NetworkMessage
		static int luaNetworkMessageCreate(lua_State* L);
		static int luaNetworkMessageDelete(lua_State* L);

		static int luaNetworkMessageGetByte(lua_State* L);
		static int luaNetworkMessageGetU16(lua_State* L);
		static int luaNetworkMessageGetU32(lua_State* L);
		static int luaNetworkMessageGetU64(lua_State* L);
		static int luaNetworkMessageGetString(lua_State* L);
		static int luaNetworkMessageGetPosition(lua_State* L);

		static int luaNetworkMessageAddByte(lua_State* L);
		static int luaNetworkMessageAddU16(lua_State* L);
		static int luaNetworkMessageAddU32(lua_State* L);
		static int luaNetworkMessageAddU64(lua_State* L);
		static int luaNetworkMessageAddString(lua_State* L);
		static int luaNetworkMessageAddPosition(lua_State* L);
		static int luaNetworkMessageAddDouble(lua_State* L);
		static int luaNetworkMessageAddItem(lua_State* L);
		static int luaNetworkMessageAddItemId(lua_State* L);

		static int luaNetworkMessageReset(lua_State* L);
		static int luaNetworkMessageSkipBytes(lua_State* L);
		static int luaNetworkMessageSendToPlayer(lua_State* L);

		// ModalWindow
		static int luaModalWindowCreate(lua_State* L);
		static int luaModalWindowDelete(lua_State* L);

		static int luaModalWindowGetId(lua_State* L);
		static int luaModalWindowGetTitle(lua_State* L);
		static int luaModalWindowGetMessage(lua_State* L);

		static int luaModalWindowSetTitle(lua_State* L);
		static int luaModalWindowSetMessage(lua_State* L);

		static int luaModalWindowGetButtonCount(lua_State* L);
		static int luaModalWindowGetChoiceCount(lua_State* L);

		static int luaModalWindowAddButton(lua_State* L);
		static int luaModalWindowAddChoice(lua_State* L);

		static int luaModalWindowGetDefaultEnterButton(lua_State* L);
		static int luaModalWindowSetDefaultEnterButton(lua_State* L);

		static int luaModalWindowGetDefaultEscapeButton(lua_State* L);
		static int luaModalWindowSetDefaultEscapeButton(lua_State* L);

		static int luaModalWindowHasPriority(lua_State* L);
		static int luaModalWindowSetPriority(lua_State* L);

		static int luaModalWindowSendToPlayer(lua_State* L);

		// Item
		static int luaItemCreate(lua_State* L);

		static int luaItemIsItem(lua_State* L);

		static int luaItemGetParent(lua_State* L);
		static int luaItemGetTopParent(lua_State* L);

		static int luaItemGetId(lua_State* L);

		static int luaItemClone(lua_State* L);
		static int luaItemSplit(lua_State* L);
		static int luaItemRemove(lua_State* L);

		static int luaItemGetUniqueId(lua_State* L);
		static int luaItemGetActionId(lua_State* L);
		static int luaItemSetActionId(lua_State* L);

		static int luaItemGetCount(lua_State* L);
		static int luaItemGetCharges(lua_State* L);
		static int luaItemGetFluidType(lua_State* L);
		static int luaItemGetWeight(lua_State* L);

		static int luaItemGetSubType(lua_State* L);

		static int luaItemGetName(lua_State* L);
		static int luaItemGetPluralName(lua_State* L);
		static int luaItemGetArticle(lua_State* L);

		static int luaItemGetPosition(lua_State* L);
		static int luaItemGetTile(lua_State* L);

		static int luaItemHasAttribute(lua_State* L);
		static int luaItemGetAttribute(lua_State* L);
		static int luaItemSetAttribute(lua_State* L);
		static int luaItemRemoveAttribute(lua_State* L);

		static int luaItemMoveTo(lua_State* L);
		static int luaItemTransform(lua_State* L);
		static int luaItemDecay(lua_State* L);

		static int luaItemGetDescription(lua_State* L);

		static int luaItemHasProperty(lua_State* L);

		// Container
		static int luaContainerCreate(lua_State* L);

		static int luaContainerGetSize(lua_State* L);
		static int luaContainerGetCapacity(lua_State* L);
		static int luaContainerGetEmptySlots(lua_State* L);

		static int luaContainerGetItemHoldingCount(lua_State* L);
		static int luaContainerGetItemCountById(lua_State* L);

		static int luaContainerGetItem(lua_State* L);
		static int luaContainerHasItem(lua_State* L);
		static int luaContainerAddItem(lua_State* L);
		static int luaContainerAddItemEx(lua_State* L);

		// Teleport
		static int luaTeleportCreate(lua_State* L);

		static int luaTeleportGetDestination(lua_State* L);
		static int luaTeleportSetDestination(lua_State* L);

		// Creature
		static int luaCreatureCreate(lua_State* L);

		static int luaCreatureRegisterEvent(lua_State* L);
		static int luaCreatureUnregisterEvent(lua_State* L);

		static int luaCreatureIsRemoved(lua_State* L);
		static int luaCreatureIsCreature(lua_State* L);
		static int luaCreatureIsInGhostMode(lua_State* L);
		static int luaCreatureIsHealthHidden(lua_State* L);

		static int luaCreatureCanSee(lua_State* L);
		static int luaCreatureCanSeeCreature(lua_State* L);

		static int luaCreatureGetParent(lua_State* L);

		static int luaCreatureGetId(lua_State* L);
		static int luaCreatureGetName(lua_State* L);

		static int luaCreatureGetTarget(lua_State* L);
		static int luaCreatureSetTarget(lua_State* L);

		static int luaCreatureGetFollowCreature(lua_State* L);
		static int luaCreatureSetFollowCreature(lua_State* L);

		static int luaCreatureGetMaster(lua_State* L);
		static int luaCreatureSetMaster(lua_State* L);

		static int luaCreatureGetLight(lua_State* L);
		static int luaCreatureSetLight(lua_State* L);

		static int luaCreatureGetSpeed(lua_State* L);
		static int luaCreatureGetBaseSpeed(lua_State* L);
		static int luaCreatureChangeSpeed(lua_State* L);

		static int luaCreatureSetDropLoot(lua_State* L);

		static int luaCreatureGetPosition(lua_State* L);
		static int luaCreatureGetTile(lua_State* L);
		static int luaCreatureGetDirection(lua_State* L);
		static int luaCreatureSetDirection(lua_State* L);

		static int luaCreatureGetHealth(lua_State* L);
		static int luaCreatureAddHealth(lua_State* L);
		static int luaCreatureGetMaxHealth(lua_State* L);
		static int luaCreatureSetMaxHealth(lua_State* L);
		static int luaCreatureSetHiddenHealth(lua_State* L);

		static int luaCreatureGetMana(lua_State* L);
		static int luaCreatureAddMana(lua_State* L);
		static int luaCreatureGetMaxMana(lua_State* L);

		static int luaCreatureGetSkull(lua_State* L);
		static int luaCreatureSetSkull(lua_State* L);

		static int luaCreatureGetOutfit(lua_State* L);
		static int luaCreatureSetOutfit(lua_State* L);

		static int luaCreatureGetCondition(lua_State* L);
		static int luaCreatureAddCondition(lua_State* L);
		static int luaCreatureRemoveCondition(lua_State* L);

		static int luaCreatureRemove(lua_State* L);
		static int luaCreatureTeleportTo(lua_State* L);
		static int luaCreatureSay(lua_State* L);

		static int luaCreatureGetDamageMap(lua_State* L);

		static int luaCreatureGetSummons(lua_State* L);

		static int luaCreatureGetDescription(lua_State* L);

		static int luaCreatureGetPathTo(lua_State* L);

		// Player
		static int luaPlayerCreate(lua_State* L);

		static int luaPlayerIsPlayer(lua_State* L);

		static int luaPlayerGetGuid(lua_State* L);
		static int luaPlayerGetIp(lua_State* L);
		static int luaPlayerGetAccountId(lua_State* L);
		static int luaPlayerGetLastLoginSaved(lua_State* L);
		static int luaPlayerGetLastLogout(lua_State* L);

		static int luaPlayerGetAccountType(lua_State* L);
		static int luaPlayerSetAccountType(lua_State* L);

		static int luaPlayerGetCapacity(lua_State* L);
		static int luaPlayerSetCapacity(lua_State* L);

		static int luaPlayerGetFreeCapacity(lua_State* L);

		static int luaPlayerGetDepotChest(lua_State* L);
		static int luaPlayerGetInbox(lua_State* L);

		static int luaPlayerGetSkullTime(lua_State* L);
		static int luaPlayerSetSkullTime(lua_State* L);
		static int luaPlayerGetDeathPenalty(lua_State* L);

		static int luaPlayerGetExperience(lua_State* L);
		static int luaPlayerAddExperience(lua_State* L);
		static int luaPlayerRemoveExperience(lua_State* L);
		static int luaPlayerGetLevel(lua_State* L);

		static int luaPlayerGetMagicLevel(lua_State* L);
		static int luaPlayerGetBaseMagicLevel(lua_State* L);
		static int luaPlayerSetMaxMana(lua_State* L);
		static int luaPlayerGetManaSpent(lua_State* L);
		static int luaPlayerAddManaSpent(lua_State* L);

		static int luaPlayerGetSkillLevel(lua_State* L);
		static int luaPlayerGetEffectiveSkillLevel(lua_State* L);
		static int luaPlayerGetSkillPercent(lua_State* L);
		static int luaPlayerGetSkillTries(lua_State* L);
		static int luaPlayerAddSkillTries(lua_State* L);

		static int luaPlayerGetItemCount(lua_State* L);
		static int luaPlayerGetItemById(lua_State* L);

		static int luaPlayerGetVocation(lua_State* L);
		static int luaPlayerSetVocation(lua_State* L);

		static int luaPlayerGetSex(lua_State* L);
		static int luaPlayerSetSex(lua_State* L);

		static int luaPlayerGetTown(lua_State* L);
		static int luaPlayerSetTown(lua_State* L);

		static int luaPlayerGetGuild(lua_State* L);
		static int luaPlayerSetGuild(lua_State* L);

		static int luaPlayerGetGuildLevel(lua_State* L);
		static int luaPlayerSetGuildLevel(lua_State* L);

		static int luaPlayerGetGuildNick(lua_State* L);
		static int luaPlayerSetGuildNick(lua_State* L);

		static int luaPlayerGetGroup(lua_State* L);
		static int luaPlayerSetGroup(lua_State* L);

		static int luaPlayerGetStamina(lua_State* L);
		static int luaPlayerSetStamina(lua_State* L);

		static int luaPlayerGetSoul(lua_State* L);
		static int luaPlayerAddSoul(lua_State* L);
		static int luaPlayerGetMaxSoul(lua_State* L);

		static int luaPlayerGetBankBalance(lua_State* L);
		static int luaPlayerSetBankBalance(lua_State* L);

		static int luaPlayerGetStorageValue(lua_State* L);
		static int luaPlayerSetStorageValue(lua_State* L);

		static int luaPlayerAddItem(lua_State* L);
		static int luaPlayerAddItemEx(lua_State* L);
		static int luaPlayerRemoveItem(lua_State* L);

		static int luaPlayerGetMoney(lua_State* L);
		static int luaPlayerAddMoney(lua_State* L);
		static int luaPlayerRemoveMoney(lua_State* L);

		static int luaPlayerShowTextDialog(lua_State* L);

		static int luaPlayerSendTextMessage(lua_State* L);
		static int luaPlayerSendChannelMessage(lua_State* L);
		static int luaPlayerSendPrivateMessage(lua_State* L);

		static int luaPlayerChannelSay(lua_State* L);
		static int luaPlayerOpenChannel(lua_State* L);

		static int luaPlayerGetSlotItem(lua_State* L);

		static int luaPlayerCreateParty(lua_State* L);
		static int luaPlayerGetParty(lua_State* L);

		static int luaPlayerAddOutfit(lua_State* L);
		static int luaPlayerAddOutfitAddon(lua_State* L);
		static int luaPlayerRemoveOutfit(lua_State* L);
		static int luaPlayerRemoveOutfitAddon(lua_State* L);
		static int luaPlayerHasOutfit(lua_State* L);
		static int luaPlayerSendOutfitWindow(lua_State* L);

		static int luaPlayerAddMount(lua_State* L);
		static int luaPlayerRemoveMount(lua_State* L);
		static int luaPlayerHasMount(lua_State* L);

		static int luaPlayerGetPremiumDays(lua_State* L);
		static int luaPlayerAddPremiumDays(lua_State* L);
		static int luaPlayerRemovePremiumDays(lua_State* L);

		static int luaPlayerHasBlessing(lua_State* L);
		static int luaPlayerAddBlessing(lua_State* L);
		static int luaPlayerRemoveBlessing(lua_State* L);

		static int luaPlayerCanLearnSpell(lua_State* L);
		static int luaPlayerLearnSpell(lua_State* L);
		static int luaPlayerForgetSpell(lua_State* L);
		static int luaPlayerHasLearnedSpell(lua_State* L);

		static int luaPlayerSendTutorial(lua_State* L);
		static int luaPlayerAddMapMark(lua_State* L);

		static int luaPlayerSave(lua_State* L);
		static int luaPlayerPopupFYI(lua_State* L);

		static int luaPlayerIsPzLocked(lua_State* L);

		static int luaPlayerGetClient(lua_State* L);
		static int luaPlayerGetHouse(lua_State* L);

		static int luaPlayerSetGhostMode(lua_State* L);

		static int luaPlayerGetContainerId(lua_State* L);
		static int luaPlayerGetContainerById(lua_State* L);
		static int luaPlayerGetContainerIndex(lua_State* L);

		// Monster
		static int luaMonsterCreate(lua_State* L);

		static int luaMonsterIsMonster(lua_State* L);

		static int luaMonsterGetType(lua_State* L);

		static int luaMonsterGetSpawnPosition(lua_State* L);
		static int luaMonsterIsInSpawnRange(lua_State* L);

		static int luaMonsterIsIdle(lua_State* L);
		static int luaMonsterSetIdle(lua_State* L);

		static int luaMonsterIsTarget(lua_State* L);
		static int luaMonsterIsOpponent(lua_State* L);
		static int luaMonsterIsFriend(lua_State* L);

		static int luaMonsterAddFriend(lua_State* L);
		static int luaMonsterRemoveFriend(lua_State* L);
		static int luaMonsterGetFriendList(lua_State* L);
		static int luaMonsterGetFriendCount(lua_State* L);

		static int luaMonsterAddTarget(lua_State* L);
		static int luaMonsterRemoveTarget(lua_State* L);
		static int luaMonsterGetTargetList(lua_State* L);
		static int luaMonsterGetTargetCount(lua_State* L);

		static int luaMonsterSelectTarget(lua_State* L);
		static int luaMonsterSearchTarget(lua_State* L);

		// Npc
		static int luaNpcCreate(lua_State* L);

		static int luaNpcIsNpc(lua_State* L);

		static int luaNpcSetMasterPos(lua_State* L);

		static int luaNpcGetSpeechBubble(lua_State* L);
		static int luaNpcSetSpeechBubble(lua_State* L);

		// Guild
		static int luaGuildCreate(lua_State* L);

		static int luaGuildGetId(lua_State* L);
		static int luaGuildGetName(lua_State* L);
		static int luaGuildGetMembersOnline(lua_State* L);

		static int luaGuildAddMember(lua_State* L);
		static int luaGuildRemoveMember(lua_State* L);

		static int luaGuildAddRank(lua_State* L);
		static int luaGuildGetRankById(lua_State* L);
		static int luaGuildGetRankByLevel(lua_State* L);

		static int luaGuildGetMotd(lua_State* L);
		static int luaGuildSetMotd(lua_State* L);

		// Group
		static int luaGroupCreate(lua_State* L);

		static int luaGroupGetId(lua_State* L);
		static int luaGroupGetName(lua_State* L);
		static int luaGroupGetFlags(lua_State* L);
		static int luaGroupGetAccess(lua_State* L);
		static int luaGroupGetMaxDepotItems(lua_State* L);
		static int luaGroupGetMaxVipEntries(lua_State* L);

		// Vocation
		static int luaVocationCreate(lua_State* L);

		static int luaVocationGetId(lua_State* L);
		static int luaVocationGetClientId(lua_State* L);
		static int luaVocationGetName(lua_State* L);
		static int luaVocationGetDescription(lua_State* L);

		static int luaVocationGetRequiredSkillTries(lua_State* L);
		static int luaVocationGetRequiredManaSpent(lua_State* L);

		static int luaVocationGetCapacityGain(lua_State* L);

		static int luaVocationGetHealthGain(lua_State* L);
		static int luaVocationGetHealthGainTicks(lua_State* L);
		static int luaVocationGetHealthGainAmount(lua_State* L);

		static int luaVocationGetManaGain(lua_State* L);
		static int luaVocationGetManaGainTicks(lua_State* L);
		static int luaVocationGetManaGainAmount(lua_State* L);

		static int luaVocationGetMaxSoul(lua_State* L);
		static int luaVocationGetSoulGainTicks(lua_State* L);

		static int luaVocationGetAttackSpeed(lua_State* L);
		static int luaVocationGetBaseSpeed(lua_State* L);

		static int luaVocationGetDemotion(lua_State* L);
		static int luaVocationGetPromotion(lua_State* L);

		// Town
		static int luaTownCreate(lua_State* L);

		static int luaTownGetId(lua_State* L);
		static int luaTownGetName(lua_State* L);
		static int luaTownGetTemplePosition(lua_State* L);

		// House
		static int luaHouseCreate(lua_State* L);

		static int luaHouseGetId(lua_State* L);
		static int luaHouseGetName(lua_State* L);
		static int luaHouseGetTown(lua_State* L);
		static int luaHouseGetExitPosition(lua_State* L);
		static int luaHouseGetRent(lua_State* L);

		static int luaHouseGetOwnerGuid(lua_State* L);
		static int luaHouseSetOwnerGuid(lua_State* L);

		static int luaHouseGetBeds(lua_State* L);
		static int luaHouseGetBedCount(lua_State* L);

		static int luaHouseGetDoors(lua_State* L);
		static int luaHouseGetDoorCount(lua_State* L);

		static int luaHouseGetTiles(lua_State* L);
		static int luaHouseGetTileCount(lua_State* L);

		static int luaHouseGetAccessList(lua_State* L);
		static int luaHouseSetAccessList(lua_State* L);

		// ItemType
		static int luaItemTypeCreate(lua_State* L);

		static int luaItemTypeIsCorpse(lua_State* L);
		static int luaItemTypeIsDoor(lua_State* L);
		static int luaItemTypeIsContainer(lua_State* L);
		static int luaItemTypeIsFluidContainer(lua_State* L);
		static int luaItemTypeIsMovable(lua_State* L);
		static int luaItemTypeIsRune(lua_State* L);
		static int luaItemTypeIsStackable(lua_State* L);
		static int luaItemTypeIsReadable(lua_State* L);
		static int luaItemTypeIsWritable(lua_State* L);

		static int luaItemTypeGetType(lua_State* L);
		static int luaItemTypeGetId(lua_State* L);
		static int luaItemTypeGetClientId(lua_State* L);
		static int luaItemTypeGetName(lua_State* L);
		static int luaItemTypeGetPluralName(lua_State* L);
		static int luaItemTypeGetArticle(lua_State* L);
		static int luaItemTypeGetDescription(lua_State* L);
		static int luaItemTypeGetSlotPosition(lua_State *L);

		static int luaItemTypeGetCharges(lua_State* L);
		static int luaItemTypeGetFluidSource(lua_State* L);
		static int luaItemTypeGetCapacity(lua_State* L);
		static int luaItemTypeGetWeight(lua_State* L);

		static int luaItemTypeGetHitChance(lua_State* L);
		static int luaItemTypeGetShootRange(lua_State* L);
		static int luaItemTypeGetAttack(lua_State* L);
		static int luaItemTypeGetDefense(lua_State* L);
		static int luaItemTypeGetExtraDefense(lua_State* L);
		static int luaItemTypeGetArmor(lua_State* L);
		static int luaItemTypeGetWeaponType(lua_State* L);

		static int luaItemTypeGetElementType(lua_State* L);
		static int luaItemTypeGetElementDamage(lua_State* L);

		static int luaItemTypeGetTransformEquipId(lua_State* L);
		static int luaItemTypeGetTransformDeEquipId(lua_State* L);
		static int luaItemTypeGetDestroyId(lua_State* L);
		static int luaItemTypeGetDecayId(lua_State* L);
		static int luaItemTypeGetRequiredLevel(lua_State* L);

		static int luaItemTypeHasSubType(lua_State* L);

		// Combat
		static int luaCombatCreate(lua_State* L);

		static int luaCombatSetParameter(lua_State* L);
		static int luaCombatSetFormula(lua_State* L);

		static int luaCombatSetArea(lua_State* L);
		static int luaCombatSetCondition(lua_State* L);
		static int luaCombatSetCallback(lua_State* L);
		static int luaCombatSetOrigin(lua_State* L);

		static int luaCombatExecute(lua_State* L);

		// Condition
		static int luaConditionCreate(lua_State* L);
		static int luaConditionDelete(lua_State* L);

		static int luaConditionGetId(lua_State* L);
		static int luaConditionGetSubId(lua_State* L);
		static int luaConditionGetType(lua_State* L);
		static int luaConditionGetIcons(lua_State* L);
		static int luaConditionGetEndTime(lua_State* L);

		static int luaConditionClone(lua_State* L);

		static int luaConditionGetTicks(lua_State* L);
		static int luaConditionSetTicks(lua_State* L);

		static int luaConditionSetParameter(lua_State* L);
		static int luaConditionSetFormula(lua_State* L);
		static int luaConditionSetOutfit(lua_State* L);

		static int luaConditionAddDamage(lua_State* L);

		// MonsterType
		static int luaMonsterTypeCreate(lua_State* L);

		static int luaMonsterTypeIsAttackable(lua_State* L);
		static int luaMonsterTypeIsConvinceable(lua_State* L);
		static int luaMonsterTypeIsSummonable(lua_State* L);
		static int luaMonsterTypeIsIllusionable(lua_State* L);
		static int luaMonsterTypeIsHostile(lua_State* L);
		static int luaMonsterTypeIsPushable(lua_State* L);
		static int luaMonsterTypeIsHealthShown(lua_State* L);

		static int luaMonsterTypeCanPushItems(lua_State* L);
		static int luaMonsterTypeCanPushCreatures(lua_State* L);

		static int luaMonsterTypeGetName(lua_State* L);
		static int luaMonsterTypeGetNameDescription(lua_State* L);

		static int luaMonsterTypeGetHealth(lua_State* L);
		static int luaMonsterTypeGetMaxHealth(lua_State* L);
		static int luaMonsterTypeGetRunHealth(lua_State* L);
		static int luaMonsterTypeGetExperience(lua_State* L);

		static int luaMonsterTypeGetCombatImmunities(lua_State* L);
		static int luaMonsterTypeGetConditionImmunities(lua_State* L);

		static int luaMonsterTypeGetAttackList(lua_State* L);
		static int luaMonsterTypeGetDefenseList(lua_State* L);
		static int luaMonsterTypeGetElementList(lua_State* L);

		static int luaMonsterTypeGetVoices(lua_State* L);
		static int luaMonsterTypeGetLoot(lua_State* L);
		static int luaMonsterTypeGetCreatureEvents(lua_State* L);

		static int luaMonsterTypeGetSummonList(lua_State* L);
		static int luaMonsterTypeGetMaxSummons(lua_State* L);

		static int luaMonsterTypeGetArmor(lua_State* L);
		static int luaMonsterTypeGetDefense(lua_State* L);
		static int luaMonsterTypeGetOutfit(lua_State* L);
		static int luaMonsterTypeGetRace(lua_State* L);
		static int luaMonsterTypeGetCorpseId(lua_State* L);
		static int luaMonsterTypeGetManaCost(lua_State* L);
		static int luaMonsterTypeGetBaseSpeed(lua_State* L);
		static int luaMonsterTypeGetLight(lua_State* L);

		static int luaMonsterTypeGetStaticAttackChance(lua_State* L);
		static int luaMonsterTypeGetTargetDistance(lua_State* L);
		static int luaMonsterTypeGetYellChance(lua_State* L);
		static int luaMonsterTypeGetYellSpeedTicks(lua_State* L);
		static int luaMonsterTypeGetChangeTargetChance(lua_State* L);
		static int luaMonsterTypeGetChangeTargetSpeed(lua_State* L);

		// Party
		static int luaPartyDisband(lua_State* L);

		static int luaPartyGetLeader(lua_State* L);
		static int luaPartySetLeader(lua_State* L);

		static int luaPartyGetMembers(lua_State* L);
		static int luaPartyGetMemberCount(lua_State* L);

		static int luaPartyGetInvitees(lua_State* L);
		static int luaPartyGetInviteeCount(lua_State* L);

		static int luaPartyAddInvite(lua_State* L);
		static int luaPartyRemoveInvite(lua_State* L);

		static int luaPartyAddMember(lua_State* L);
		static int luaPartyRemoveMember(lua_State* L);

		static int luaPartyIsSharedExperienceActive(lua_State* L);
		static int luaPartyIsSharedExperienceEnabled(lua_State* L);
		static int luaPartyShareExperience(lua_State* L);
		static int luaPartySetSharedExperience(lua_State* L);

		//
		lua_State* m_luaState;
		std::string m_lastLuaError;

		std::string m_interfaceName;
		int32_t m_eventTableRef;

		static ScriptEnvironment m_scriptEnv[16];
		static int32_t m_scriptEnvIndex;

		int32_t m_runningEventId;
		std::string m_loadingFile;

		//script file cache
		std::map<int32_t, std::string> m_cacheFiles;
};

class LuaEnvironment : public LuaScriptInterface
{
	public:
		LuaEnvironment();
		~LuaEnvironment();

		// non-copyable
		LuaEnvironment(const LuaEnvironment&) = delete;
		LuaEnvironment& operator=(const LuaEnvironment&) = delete;

		bool initState();
		bool reInitState();
		bool closeState();

		LuaScriptInterface* getTestInterface();

		Combat* getCombatObject(uint32_t id) const;
		uint32_t createCombatObject(LuaScriptInterface* interface);
		void clearCombatObjects(LuaScriptInterface* interface);

		Condition* getConditionObject(uint32_t id) const;
		bool createConditionObject(ConditionType_t conditionType, ConditionId_t conditionId, uint32_t& id);

		AreaCombat* getAreaObject(uint32_t id) const;
		uint32_t createAreaObject(LuaScriptInterface* interface);
		void clearAreaObjects(LuaScriptInterface* interface);

	private:
		void executeTimerEvent(uint32_t eventIndex);

		//
		std::unordered_map<uint32_t, LuaTimerEventDesc> m_timerEvents;
		std::unordered_map<uint32_t, Combat*> m_combatMap;
		std::unordered_map<uint32_t, Condition*> m_conditionMap;
		std::unordered_map<uint32_t, AreaCombat*> m_areaMap;

		std::unordered_map<LuaScriptInterface*, std::vector<uint32_t>> m_combatIdMap;
		std::unordered_map<LuaScriptInterface*, std::vector<uint32_t>> m_areaIdMap;

		LuaScriptInterface* m_testInterface;

		uint32_t m_lastEventTimerId;
		uint32_t m_lastCombatId;
		uint32_t m_lastConditionId;
		uint32_t m_lastAreaId;

		//
		friend class LuaScriptInterface;
		friend class CombatSpell;
};

#endif
