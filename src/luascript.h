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
	VARIANT_NONE = 0,
	VARIANT_NUMBER,
	VARIANT_POSITION,
	VARIANT_TARGETPOSITION,
	VARIANT_STRING,
};

enum LuaDataType {
	LuaData_Unknown = 0,
	LuaData_Item,
	LuaData_Container,
	LuaData_Player,
	LuaData_Monster,
	LuaData_Npc
};

struct LuaVariant {
	LuaVariant() {
		type = VARIANT_NONE;
		number = 0;
	}

	LuaVariantType_t type;
	std::string text;
	PositionEx pos;
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

		void resetEnv();

		void setScriptId(int32_t scriptId, LuaScriptInterface* scriptInterface) {
			m_scriptId = scriptId;
			m_interface = scriptInterface;
		}
		bool setCallbackId(int32_t callbackId, LuaScriptInterface* scriptInterface);
		void setEventDesc(const std::string& desc) {
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

		static void addTempItem(ScriptEnvironment* env, Item* item);
		static void removeTempItem(ScriptEnvironment* env, Item* item);
		static void removeTempItem(Item* item);
		static void addUniqueThing(Thing* thing);
		static void removeUniqueThing(Thing* thing);
		uint32_t addThing(Thing* thing);
		void insertThing(uint32_t uid, Thing* thing);

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
		typedef std::unordered_map<uint32_t, Thing*> ThingMap;
		typedef std::vector<const LuaVariant*> VariantVector;
		typedef std::map<uint32_t, int32_t> StorageMap;
		typedef std::map<uint32_t, DBResult_ptr> DBResultMap;
		typedef std::list<Item*> ItemList;

		//script file id
		int32_t m_scriptId;
		int32_t m_callbackId;
		bool m_timerEvent;
		LuaScriptInterface* m_interface;
		//script event desc
		std::string m_eventdesc;

		//unique id map
		static ThingMap m_globalMap;

		//item/creature map
		int32_t m_lastUID;
		ThingMap m_localMap;

		//temporary item list
		typedef std::map<ScriptEnvironment*, ItemList> TempItemListMap;
		static TempItemListMap m_tempItems;

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
	LUA_ERROR_SPELL_NOT_FOUND
};

class LuaScriptInterface
{
	public:
		LuaScriptInterface(const std::string& interfaceName);
		virtual ~LuaScriptInterface();

		virtual bool initState();
		bool reInitState();

		int32_t loadFile(const std::string& file, Npc* npc = nullptr);

		const std::string& getFileById(int32_t scriptId);
		int32_t getEvent(const std::string& eventName);
		int32_t getMetaEvent(const std::string& globalName, const std::string& eventName);

		static ScriptEnvironment* getScriptEnv() {
			assert(m_scriptEnvIndex >= 0 && m_scriptEnvIndex < 17);
			return &m_scriptEnv[m_scriptEnvIndex];
		}

		static bool reserveScriptEnv() {
			if (++m_scriptEnvIndex < 16) {
				return true;
			} else {
				--m_scriptEnvIndex;
				return false;
			}
		}

		static void resetScriptEnv() {
			if (m_scriptEnvIndex >= 0) {
				m_scriptEnv[m_scriptEnvIndex].resetEnv();
				--m_scriptEnvIndex;
			}
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

		static int32_t luaErrorHandler(lua_State* L);
		bool callFunction(int32_t params);
		void callVoidFunction(int32_t params);

		//push/pop common structures
		static void pushThing(lua_State* L, Thing* thing, uint32_t uid);
		static void pushVariant(lua_State* L, const LuaVariant& var);
		static void pushString(lua_State* L, const std::string& value);
		static void pushCallback(lua_State* L, int32_t callback);

		static LuaVariant popVariant(lua_State* L);
		static void popPosition(lua_State* L, PositionEx& position);
		static std::string popString(lua_State* L);
		static int32_t popCallback(lua_State* L);
		static bool popBoolean(lua_State* L);

		// Userdata
		template<class T>
		static void pushUserdata(lua_State* L, T* value)
		{
			T** userdata = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
			*userdata = value;
		}

		// Metatables
		static void setMetatable(lua_State* L, int32_t index, const std::string& string);
		static void setItemMetatable(lua_State* L, int32_t index, const Item* item);
		static void setCreatureMetatable(lua_State* L, int32_t index, const Creature* creature);

		// Get
		template<typename T>
		inline static T getNumber(lua_State* L, int32_t arg)
		{
			return static_cast<T>(lua_tonumber(L, arg));
		}
		template<typename T>
		static T getNumber(lua_State *L, int32_t arg, T defaultValue)
		{
			if (arg > lua_gettop(L)) {
				return defaultValue;
			}
			return static_cast<T>(lua_tonumber(L, arg));
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
			return static_cast<T**>(lua_touserdata(L, arg));
		}

		inline static bool getBoolean(lua_State* L, int32_t arg)
		{
			return lua_toboolean(L, arg) != 0;
		}
		inline static bool getBoolean(lua_State* L, int32_t arg, bool defaultValue)
		{
			if (arg > lua_gettop(L)) {
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

		// Pop
		template<typename T>
		static T popNumber(lua_State* L) {
			if (lua_gettop(L) == 0) {
				return static_cast<T>(0);
			}

			T ret = static_cast<T>(lua_tonumber(L, -1));
			lua_pop(L, 1);
			return ret;
		}
		static Outfit_t popOutfit(lua_State* L);

		// Push
		static void pushBoolean(lua_State* L, bool value);
		static void pushPosition(lua_State* L, const Position& position, int32_t stackpos = 0);
		static void pushOutfit(lua_State* L, const Outfit_t& outfit);

		//
		template<typename T>
		static T popField(lua_State* L, const std::string& key)
		{
			lua_getfield(L, -1, key.c_str());
			return popNumber<T>(L);
		}

		static std::string popFieldString(lua_State* L, const std::string& key);

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
		static const luaL_Reg luaBitReg[13];
#endif
		static const luaL_Reg luaConfigManagerTable[4];
		static const luaL_Reg luaDatabaseTable[7];
		static const luaL_Reg luaResultTable[7];

		static int32_t protectedCall(lua_State* L, int32_t nargs, int32_t nresults);

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
		static int32_t luaDoShowTextWindow(lua_State* L);
		static int32_t luaDoCreateItem(lua_State* L);
		static int32_t luaDoCreateItemEx(lua_State* L);
		static int32_t luaDoCreateTeleport(lua_State* L);
		static int32_t luaDoAddCondition(lua_State* L);
		static int32_t luaDoRemoveCondition(lua_State* L);
		static int32_t luaDoMoveCreature(lua_State* L);

		static int32_t luaDoPlayerAddItem(lua_State* L);
		static int32_t luaDoTileAddItemEx(lua_State* L);
		static int32_t luaDoRelocate(lua_State* L);
		static int32_t luaDoPlayerRemoveItem(lua_State* L);
		static int32_t luaDoSetCreatureLight(lua_State* L);

		//get item info
		static int32_t luaGetThingfromPos(lua_State* L);
		static int32_t luaGetThingPos(lua_State* L);
		static int32_t luaHasProperty(lua_State* L);
		static int32_t luaGetDepotId(lua_State* L);

		//houses
		static int32_t luaGetHouseByPlayerGUID(lua_State* L);

		//get creature info functions
		static int32_t luaGetSpectators(lua_State* L);

		static int32_t luaGetPlayerFlagValue(lua_State* L);
		static int32_t luaGetCreatureCondition(lua_State* L);

		static int32_t luaGetPlayerInstantSpellInfo(lua_State* L);
		static int32_t luaGetPlayerInstantSpellCount(lua_State* L);

		static int32_t luaGetWorldTime(lua_State* L);
		static int32_t luaGetWorldLight(lua_State* L);
		static int32_t luaGetWorldUpTime(lua_State* L);
		static int32_t luaBroadcastMessage(lua_State* L);

		//type validation
		static int32_t luaIsDepot(lua_State* L);
		static int32_t luaIsMoveable(lua_State* L);
		static int32_t luaIsValidUID(lua_State* L);

		//container
		static int32_t luaDoAddContainerItem(lua_State* L);

		//
		static int32_t luaCreateCombatObject(lua_State* L);
		static int32_t luaCreateCombatArea(lua_State* L);
		static int32_t luaSetCombatArea(lua_State* L);
		static int32_t luaSetCombatCondition(lua_State* L);
		static int32_t luaSetCombatParam(lua_State* L);
		static int32_t luaCreateConditionObject(lua_State* L);
		static int32_t luaSetConditionParam(lua_State* L);
		static int32_t luaAddDamageCondition(lua_State* L);
		static int32_t luaAddOutfitCondition(lua_State* L);

		static int32_t luaSetCombatCallBack(lua_State* L);
		static int32_t luaSetCombatFormula(lua_State* L);
		static int32_t luaSetConditionFormula(lua_State* L);
		static int32_t luaDoCombat(lua_State* L);

		static int32_t luaDoAreaCombatHealth(lua_State* L);
		static int32_t luaDoTargetCombatHealth(lua_State* L);

		//
		static int32_t luaDoAreaCombatMana(lua_State* L);
		static int32_t luaDoTargetCombatMana(lua_State* L);

		static int32_t luaDoAreaCombatCondition(lua_State* L);
		static int32_t luaDoTargetCombatCondition(lua_State* L);

		static int32_t luaDoAreaCombatDispel(lua_State* L);
		static int32_t luaDoTargetCombatDispel(lua_State* L);

		static int32_t luaDoChallengeCreature(lua_State* L);

		static int32_t luaNumberToVariant(lua_State* L);
		static int32_t luaStringToVariant(lua_State* L);
		static int32_t luaPositionToVariant(lua_State* L);
		static int32_t luaTargetPositionToVariant(lua_State* L);

		static int32_t luaVariantToNumber(lua_State* L);
		static int32_t luaVariantToString(lua_State* L);
		static int32_t luaVariantToPosition(lua_State* L);

		static int32_t luaDoChangeSpeed(lua_State* L);

		static int32_t luaSetCreatureOutfit(lua_State* L);
		static int32_t luaSetMonsterOutfit(lua_State* L);
		static int32_t luaSetItemOutfit(lua_State* L);

		static int32_t luaDebugPrint(lua_State* L);
		static int32_t luaIsInArray(lua_State* L);
		static int32_t luaAddEvent(lua_State* L);
		static int32_t luaStopEvent(lua_State* L);

		static int32_t luaMayNotMove(lua_State* L);

		static int32_t luaSaveServer(lua_State* L);
		static int32_t luaCleanMap(lua_State* L);

		static int32_t luaIsInWar(lua_State* L);
		static int32_t luaDoPlayerSetOfflineTrainingSkill(lua_State* L);

		static int32_t luaGetWaypointPositionByName(lua_State* L);

		static int32_t luaSendChannelMessage(lua_State* L);
		static int32_t luaSendGuildChannelMessage(lua_State* L);

#ifndef LUAJIT_VERSION
		static int32_t luaBitNot(lua_State* L);
		static int32_t luaBitAnd(lua_State* L);
		static int32_t luaBitOr(lua_State* L);
		static int32_t luaBitXor(lua_State* L);
		static int32_t luaBitLeftShift(lua_State* L);
		static int32_t luaBitRightShift(lua_State* L);
		static int32_t luaBitUNot(lua_State* L);
		static int32_t luaBitUAnd(lua_State* L);
		static int32_t luaBitUOr(lua_State* L);
		static int32_t luaBitUXor(lua_State* L);
		static int32_t luaBitULeftShift(lua_State* L);
		static int32_t luaBitURightShift(lua_State* L);
#endif

		static int32_t luaConfigManagerGetString(lua_State* L);
		static int32_t luaConfigManagerGetNumber(lua_State* L);
		static int32_t luaConfigManagerGetBoolean(lua_State* L);

		static int32_t luaDatabaseExecute(lua_State* L);
		static int32_t luaDatabaseStoreQuery(lua_State* L);
		static int32_t luaDatabaseEscapeString(lua_State* L);
		static int32_t luaDatabaseEscapeBlob(lua_State* L);
		static int32_t luaDatabaseLastInsertId(lua_State* L);
		static int32_t luaDatabaseConnected(lua_State* L);
		static int32_t luaDatabaseTableExists(lua_State* L);

		static int32_t luaResultGetDataInt(lua_State* L);
		static int32_t luaResultGetDataLong(lua_State* L);
		static int32_t luaResultGetDataString(lua_State* L);
		static int32_t luaResultGetDataStream(lua_State* L);
		static int32_t luaResultNext(lua_State* L);
		static int32_t luaResultFree(lua_State* L);

		// Userdata
		static int32_t luaUserdataCompare(lua_State* L);

		// _G
		static int32_t luaIsType(lua_State* L);

		// Game
		static int32_t luaGameGetSpectators(lua_State* L);
		static int32_t luaGameGetPlayers(lua_State* L);
		static int32_t luaGameLoadMap(lua_State* L);

		static int32_t luaGameGetExperienceStage(lua_State* L);
		static int32_t luaGameGetMonsterCount(lua_State* L);
		static int32_t luaGameGetPlayerCount(lua_State* L);
		static int32_t luaGameGetNpcCount(lua_State* L);

		static int32_t luaGameGetTowns(lua_State* L);
		static int32_t luaGameGetHouses(lua_State* L);

		static int32_t luaGameGetGameState(lua_State* L);
		static int32_t luaGameSetGameState(lua_State* L);

		static int32_t luaGameGetWorldType(lua_State* L);
		static int32_t luaGameSetWorldType(lua_State* L);

		static int32_t luaGameGetReturnMessage(lua_State* L);

		static int32_t luaGameCreateItem(lua_State* L);
		static int32_t luaGameCreateMonster(lua_State* L);
		static int32_t luaGameCreateNpc(lua_State* L);

		static int32_t luaGameStartRaid(lua_State* L);

		// Position
		static int32_t luaPositionCreate(lua_State* L);
		static int32_t luaPositionAdd(lua_State* L);
		static int32_t luaPositionSub(lua_State* L);
		static int32_t luaPositionCompare(lua_State* L);

		static int32_t luaPositionGetTile(lua_State* L);
		static int32_t luaPositionGetDistance(lua_State* L);
		static int32_t luaPositionIsSightClear(lua_State* L);

		static int32_t luaPositionSendMagicEffect(lua_State* L);
		static int32_t luaPositionSendDistanceEffect(lua_State* L);

		// Tile
		static int32_t luaTileCreate(lua_State* L);

		static int32_t luaTileGetPosition(lua_State* L);
		static int32_t luaTileGetGround(lua_State* L);
		static int32_t luaTileGetThing(lua_State* L);
		static int32_t luaTileGetThingCount(lua_State* L);
		static int32_t luaTileGetTopVisibleThing(lua_State* L);

		static int32_t luaTileGetTopTopItem(lua_State* L);
		static int32_t luaTileGetTopDownItem(lua_State* L);
		static int32_t luaTileGetFieldItem(lua_State* L);

		static int32_t luaTileGetItemById(lua_State* L);
		static int32_t luaTileGetItemByType(lua_State* L);
		static int32_t luaTileGetItemByTopOrder(lua_State* L);
		static int32_t luaTileGetItemCountById(lua_State* L);

		static int32_t luaTileGetBottomCreature(lua_State* L);
		static int32_t luaTileGetTopCreature(lua_State* L);
		static int32_t luaTileGetBottomVisibleCreature(lua_State* L);
		static int32_t luaTileGetTopVisibleCreature(lua_State* L);

		static int32_t luaTileGetItems(lua_State* L);
		static int32_t luaTileGetItemCount(lua_State* L);
		static int32_t luaTileGetDownItemCount(lua_State* L);
		static int32_t luaTileGetTopItemCount(lua_State* L);

		static int32_t luaTileGetCreatures(lua_State* L);
		static int32_t luaTileGetCreatureCount(lua_State* L);

		static int32_t luaTileHasProperty(lua_State* L);
		static int32_t luaTileHasFlag(lua_State* L);

		static int32_t luaTileQueryAdd(lua_State* L);

		static int32_t luaTileGetHouse(lua_State* L);

		// NetworkMessage
		static int32_t luaNetworkMessageCreate(lua_State* L);
		static int32_t luaNetworkMessageDelete(lua_State* L);

		static int32_t luaNetworkMessageGetByte(lua_State* L);
		static int32_t luaNetworkMessageGetU16(lua_State* L);
		static int32_t luaNetworkMessageGetU32(lua_State* L);
		static int32_t luaNetworkMessageGetU64(lua_State* L);
		static int32_t luaNetworkMessageGetString(lua_State* L);
		static int32_t luaNetworkMessageGetPosition(lua_State* L);

		static int32_t luaNetworkMessageAddByte(lua_State* L);
		static int32_t luaNetworkMessageAddU16(lua_State* L);
		static int32_t luaNetworkMessageAddU32(lua_State* L);
		static int32_t luaNetworkMessageAddU64(lua_State* L);
		static int32_t luaNetworkMessageAddString(lua_State* L);
		static int32_t luaNetworkMessageAddPosition(lua_State* L);
		static int32_t luaNetworkMessageAddDouble(lua_State* L);
		static int32_t luaNetworkMessageAddItem(lua_State* L);
		static int32_t luaNetworkMessageAddItemId(lua_State* L);

		static int32_t luaNetworkMessageReset(lua_State* L);
		static int32_t luaNetworkMessageSkipBytes(lua_State* L);
		static int32_t luaNetworkMessageSendToPlayer(lua_State* L);

		// ModalWindow
		static int32_t luaModalWindowCreate(lua_State* L);
		static int32_t luaModalWindowDelete(lua_State* L);

		static int32_t luaModalWindowGetId(lua_State* L);
		static int32_t luaModalWindowGetTitle(lua_State* L);
		static int32_t luaModalWindowGetMessage(lua_State* L);

		static int32_t luaModalWindowSetTitle(lua_State* L);
		static int32_t luaModalWindowSetMessage(lua_State* L);

		static int32_t luaModalWindowGetButtonCount(lua_State* L);
		static int32_t luaModalWindowGetChoiceCount(lua_State* L);

		static int32_t luaModalWindowAddButton(lua_State* L);
		static int32_t luaModalWindowAddChoice(lua_State* L);

		static int32_t luaModalWindowGetDefaultEnterButton(lua_State* L);
		static int32_t luaModalWindowSetDefaultEnterButton(lua_State* L);

		static int32_t luaModalWindowGetDefaultEscapeButton(lua_State* L);
		static int32_t luaModalWindowSetDefaultEscapeButton(lua_State* L);

		static int32_t luaModalWindowHasPriority(lua_State* L);
		static int32_t luaModalWindowSetPriority(lua_State* L);

		static int32_t luaModalWindowSendToPlayer(lua_State* L);

		// Item
		static int32_t luaItemCreate(lua_State* L);

		static int32_t luaItemIsCreature(lua_State* L);
		static int32_t luaItemIsItem(lua_State* L);
		static int32_t luaItemIsContainer(lua_State* L);

		static int32_t luaItemGetParent(lua_State* L);
		static int32_t luaItemGetTopParent(lua_State* L);

		static int32_t luaItemGetId(lua_State* L);
		static int32_t luaItemGetType(lua_State* L);

		static int32_t luaItemClone(lua_State* L);
		static int32_t luaItemSplit(lua_State* L);
		static int32_t luaItemRemove(lua_State* L);

		static int32_t luaItemGetUniqueId(lua_State* L);
		static int32_t luaItemGetActionId(lua_State* L);
		static int32_t luaItemSetActionId(lua_State* L);

		static int32_t luaItemGetCount(lua_State* L);
		static int32_t luaItemGetCharges(lua_State* L);
		static int32_t luaItemGetFluidType(lua_State* L);

		static int32_t luaItemGetSubType(lua_State* L);

		static int32_t luaItemGetName(lua_State* L);
		static int32_t luaItemGetPluralName(lua_State* L);
		static int32_t luaItemGetArticle(lua_State* L);

		static int32_t luaItemGetPosition(lua_State* L);
		static int32_t luaItemGetTile(lua_State* L);

		static int32_t luaItemGetAttribute(lua_State* L);
		static int32_t luaItemSetAttribute(lua_State* L);
		static int32_t luaItemRemoveAttribute(lua_State* L);

		static int32_t luaItemMoveTo(lua_State* L);
		static int32_t luaItemTransform(lua_State* L);
		static int32_t luaItemDecay(lua_State* L);

		static int32_t luaItemGetDescription(lua_State* L);

		// Container
		static int32_t luaContainerCreate(lua_State* L);

		static int32_t luaContainerIsContainer(lua_State* L);

		static int32_t luaContainerGetSize(lua_State* L);
		static int32_t luaContainerGetCapacity(lua_State* L);
		static int32_t luaContainerGetEmptySlots(lua_State* L);

		static int32_t luaContainerGetItemHoldingCount(lua_State* L);
		static int32_t luaContainerGetItemCountById(lua_State* L);

		static int32_t luaContainerGetItem(lua_State* L);
		static int32_t luaContainerHasItem(lua_State* L);
		static int32_t luaContainerAddItem(lua_State* L);
		static int32_t luaContainerAddItemEx(lua_State* L);

		// Creature
		static int32_t luaCreatureCreate(lua_State* L);

		static int32_t luaCreatureRegisterEvent(lua_State* L);
		static int32_t luaCreatureUnregisterEvent(lua_State* L);

		static int32_t luaCreatureIsRemoved(lua_State* L);
		static int32_t luaCreatureIsCreature(lua_State* L);
		static int32_t luaCreatureIsPlayer(lua_State* L);
		static int32_t luaCreatureIsMonster(lua_State* L);
		static int32_t luaCreatureIsNpc(lua_State* L);
		static int32_t luaCreatureIsItem(lua_State* L);
		static int32_t luaCreatureIsInGhostMode(lua_State* L);
		static int32_t luaCreatureIsHealthHidden(lua_State* L);

		static int32_t luaCreatureCanSee(lua_State* L);
		static int32_t luaCreatureCanSeeCreature(lua_State* L);

		static int32_t luaCreatureGetParent(lua_State* L);

		static int32_t luaCreatureGetId(lua_State* L);
		static int32_t luaCreatureGetName(lua_State* L);

		static int32_t luaCreatureGetTarget(lua_State* L);
		static int32_t luaCreatureSetTarget(lua_State* L);

		static int32_t luaCreatureGetFollowCreature(lua_State* L);
		static int32_t luaCreatureSetFollowCreature(lua_State* L);

		static int32_t luaCreatureGetMaster(lua_State* L);
		static int32_t luaCreatureSetMaster(lua_State* L);

		static int32_t luaCreatureGetLight(lua_State* L);
		static int32_t luaCreatureSetLight(lua_State* L);

		static int32_t luaCreatureGetSpeed(lua_State* L);
		static int32_t luaCreatureGetBaseSpeed(lua_State* L);

		static int32_t luaCreatureSetDropLoot(lua_State* L);

		static int32_t luaCreatureGetPosition(lua_State* L);
		static int32_t luaCreatureGetTile(lua_State* L);
		static int32_t luaCreatureGetDirection(lua_State* L);
		static int32_t luaCreatureSetDirection(lua_State* L);

		static int32_t luaCreatureGetHealth(lua_State* L);
		static int32_t luaCreatureAddHealth(lua_State* L);
		static int32_t luaCreatureGetMaxHealth(lua_State* L);
		static int32_t luaCreatureSetMaxHealth(lua_State* L);
		static int32_t luaCreatureSetHiddenHealth(lua_State* L);

		static int32_t luaCreatureGetMana(lua_State* L);
		static int32_t luaCreatureAddMana(lua_State* L);
		static int32_t luaCreatureGetMaxMana(lua_State* L);

		static int32_t luaCreatureGetOutfit(lua_State* L);
		static int32_t luaCreatureSetOutfit(lua_State* L);

		static int32_t luaCreatureGetCondition(lua_State* L);
		static int32_t luaCreatureAddCondition(lua_State* L);
		static int32_t luaCreatureRemoveCondition(lua_State* L);

		static int32_t luaCreatureRemove(lua_State* L);
		static int32_t luaCreatureTeleportTo(lua_State* L);
		static int32_t luaCreatureSay(lua_State* L);

		static int32_t luaCreatureGetDamageMap(lua_State* L);

		static int32_t luaCreatureGetSummons(lua_State* L);

		static int32_t luaCreatureGetDescription(lua_State* L);

		static int32_t luaCreatureGetPathTo(lua_State* L);

		// Player
		static int32_t luaPlayerCreate(lua_State* L);

		static int32_t luaPlayerIsPlayer(lua_State* L);

		static int32_t luaPlayerGetGuid(lua_State* L);
		static int32_t luaPlayerGetIp(lua_State* L);
		static int32_t luaPlayerGetAccountId(lua_State* L);
		static int32_t luaPlayerGetLastLoginSaved(lua_State* L);

		static int32_t luaPlayerGetAccountType(lua_State* L);
		static int32_t luaPlayerSetAccountType(lua_State* L);

		static int32_t luaPlayerGetCapacity(lua_State* L);
		static int32_t luaPlayerSetCapacity(lua_State* L);

		static int32_t luaPlayerGetFreeCapacity(lua_State* L);

		static int32_t luaPlayerGetDepotChest(lua_State* L);
		static int32_t luaPlayerGetInbox(lua_State* L);

		static int32_t luaPlayerGetSkull(lua_State* L);
		static int32_t luaPlayerSetSkull(lua_State* L);
		static int32_t luaPlayerGetSkullTime(lua_State* L);
		static int32_t luaPlayerSetSkullTime(lua_State* L);
		static int32_t luaPlayerGetDeathPenalty(lua_State* L);

		static int32_t luaPlayerGetExperience(lua_State* L);
		static int32_t luaPlayerAddExperience(lua_State* L);
		static int32_t luaPlayerGetLevel(lua_State* L);

		static int32_t luaPlayerGetMagicLevel(lua_State* L);
		static int32_t luaPlayerGetBaseMagicLevel(lua_State* L);
		static int32_t luaPlayerSetMaxMana(lua_State* L);
		static int32_t luaPlayerGetManaSpent(lua_State* L);
		static int32_t luaPlayerAddManaSpent(lua_State* L);

		static int32_t luaPlayerGetSkillLevel(lua_State* L);
		static int32_t luaPlayerGetEffectiveSkillLevel(lua_State* L);
		static int32_t luaPlayerGetSkillPercent(lua_State* L);
		static int32_t luaPlayerGetSkillTries(lua_State* L);
		static int32_t luaPlayerAddSkillTries(lua_State* L);

		static int32_t luaPlayerGetItemCount(lua_State* L);
		static int32_t luaPlayerGetItemById(lua_State* L);

		static int32_t luaPlayerGetVocation(lua_State* L);
		static int32_t luaPlayerSetVocation(lua_State* L);

		static int32_t luaPlayerGetSex(lua_State* L);
		static int32_t luaPlayerSetSex(lua_State* L);

		static int32_t luaPlayerGetTown(lua_State* L);
		static int32_t luaPlayerSetTown(lua_State* L);

		static int32_t luaPlayerGetGuild(lua_State* L);
		static int32_t luaPlayerSetGuild(lua_State* L);

		static int32_t luaPlayerGetGuildLevel(lua_State* L);
		static int32_t luaPlayerSetGuildLevel(lua_State* L);

		static int32_t luaPlayerGetGuildNick(lua_State* L);
		static int32_t luaPlayerSetGuildNick(lua_State* L);

		static int32_t luaPlayerGetGroup(lua_State* L);
		static int32_t luaPlayerSetGroup(lua_State* L);

		static int32_t luaPlayerGetStamina(lua_State* L);
		static int32_t luaPlayerSetStamina(lua_State* L);

		static int32_t luaPlayerGetSoul(lua_State* L);
		static int32_t luaPlayerAddSoul(lua_State* L);
		static int32_t luaPlayerGetMaxSoul(lua_State* L);

		static int32_t luaPlayerGetBankBalance(lua_State* L);
		static int32_t luaPlayerSetBankBalance(lua_State* L);

		static int32_t luaPlayerGetStorageValue(lua_State* L);
		static int32_t luaPlayerSetStorageValue(lua_State* L);

		static int32_t luaPlayerAddItem(lua_State* L);
		static int32_t luaPlayerAddItemEx(lua_State* L);
		static int32_t luaPlayerRemoveItem(lua_State* L);

		static int32_t luaPlayerGetMoney(lua_State* L);
		static int32_t luaPlayerAddMoney(lua_State* L);
		static int32_t luaPlayerRemoveMoney(lua_State* L);

		static int32_t luaPlayerShowTextDialog(lua_State* L);

		static int32_t luaPlayerSendTextMessage(lua_State* L);
		static int32_t luaPlayerSendChannelMessage(lua_State* L);
		static int32_t luaPlayerChannelSay(lua_State* L);
		static int32_t luaPlayerOpenChannel(lua_State* L);

		static int32_t luaPlayerGetSlotItem(lua_State* L);

		static int32_t luaPlayerGetParty(lua_State* L);

		static int32_t luaPlayerAddOutfit(lua_State* L);
		static int32_t luaPlayerAddOutfitAddon(lua_State* L);
		static int32_t luaPlayerRemoveOutfit(lua_State* L);
		static int32_t luaPlayerRemoveOutfitAddon(lua_State* L);
		static int32_t luaPlayerHasOutfit(lua_State* L);
		static int32_t luaPlayerSendOutfitWindow(lua_State* L);

		static int32_t luaPlayerAddMount(lua_State* L);
		static int32_t luaPlayerRemoveMount(lua_State* L);
		static int32_t luaPlayerHasMount(lua_State* L);

		static int32_t luaPlayerGetPremiumDays(lua_State* L);
		static int32_t luaPlayerAddPremiumDays(lua_State* L);
		static int32_t luaPlayerRemovePremiumDays(lua_State* L);

		static int32_t luaPlayerHasBlessing(lua_State* L);
		static int32_t luaPlayerAddBlessing(lua_State* L);
		static int32_t luaPlayerRemoveBlessing(lua_State* L);

		static int32_t luaPlayerCanLearnSpell(lua_State* L);
		static int32_t luaPlayerLearnSpell(lua_State* L);
		static int32_t luaPlayerForgetSpell(lua_State* L);
		static int32_t luaPlayerHasLearnedSpell(lua_State* L);

		static int32_t luaPlayerSendTutorial(lua_State* L);
		static int32_t luaPlayerAddMapMark(lua_State* L);

		static int32_t luaPlayerSave(lua_State* L);
		static int32_t luaPlayerPopupFYI(lua_State* L);

		static int32_t luaPlayerIsPzLocked(lua_State* L);

		static int32_t luaPlayerGetClient(lua_State* L);
		static int32_t luaPlayerGetHouse(lua_State* L);

		static int32_t luaPlayerSetGhostMode(lua_State* L);

		static int32_t luaPlayerGetContainerId(lua_State* L);
		static int32_t luaPlayerGetContainerById(lua_State* L);
		static int32_t luaPlayerGetContainerIndex(lua_State* L);

		// Monster
		static int32_t luaMonsterCreate(lua_State* L);

		static int32_t luaMonsterIsMonster(lua_State* L);

		static int32_t luaMonsterGetType(lua_State* L);

		static int32_t luaMonsterGetSpawnPosition(lua_State* L);
		static int32_t luaMonsterDespawn(lua_State* L);

		static int32_t luaMonsterIsIdle(lua_State* L);
		static int32_t luaMonsterSetIdle(lua_State* L);

		static int32_t luaMonsterIsTarget(lua_State* L);
		static int32_t luaMonsterIsOpponent(lua_State* L);
		static int32_t luaMonsterIsFriend(lua_State* L);

		static int32_t luaMonsterAddFriend(lua_State* L);
		static int32_t luaMonsterRemoveFriend(lua_State* L);
		static int32_t luaMonsterGetFriendList(lua_State* L);
		static int32_t luaMonsterGetFriendCount(lua_State* L);

		static int32_t luaMonsterAddTarget(lua_State* L);
		static int32_t luaMonsterRemoveTarget(lua_State* L);
		static int32_t luaMonsterGetTargetList(lua_State* L);
		static int32_t luaMonsterGetTargetCount(lua_State* L);

		static int32_t luaMonsterSelectTarget(lua_State* L);
		static int32_t luaMonsterSearchTarget(lua_State* L);

		// Npc
		static int32_t luaNpcCreate(lua_State* L);

		static int32_t luaNpcIsNpc(lua_State* L);

		static int32_t luaNpcSetMasterPos(lua_State* L);

		static int32_t luaNpcGetSpeechBubble(lua_State* L);
		static int32_t luaNpcSetSpeechBubble(lua_State* L);

		// Guild
		static int32_t luaGuildCreate(lua_State* L);

		static int32_t luaGuildGetId(lua_State* L);
		static int32_t luaGuildGetName(lua_State* L);
		static int32_t luaGuildGetMembersOnline(lua_State* L);

		static int32_t luaGuildAddMember(lua_State* L);
		static int32_t luaGuildRemoveMember(lua_State* L);

		static int32_t luaGuildAddRank(lua_State* L);
		static int32_t luaGuildGetRankById(lua_State* L);
		static int32_t luaGuildGetRankByLevel(lua_State* L);

		static int32_t luaGuildGetMotd(lua_State* L);
		static int32_t luaGuildSetMotd(lua_State* L);

		// Group
		static int32_t luaGroupCreate(lua_State* L);

		static int32_t luaGroupGetId(lua_State* L);
		static int32_t luaGroupGetName(lua_State* L);
		static int32_t luaGroupGetFlags(lua_State* L);
		static int32_t luaGroupGetAccess(lua_State* L);
		static int32_t luaGroupGetMaxDepotItems(lua_State* L);
		static int32_t luaGroupGetMaxVipEntries(lua_State* L);

		// Vocation
		static int32_t luaVocationCreate(lua_State* L);

		static int32_t luaVocationGetId(lua_State* L);
		static int32_t luaVocationGetClientId(lua_State* L);
		static int32_t luaVocationGetName(lua_State* L);
		static int32_t luaVocationGetDescription(lua_State* L);

		static int32_t luaVocationGetRequiredSkillTries(lua_State* L);
		static int32_t luaVocationGetRequiredManaSpent(lua_State* L);

		static int32_t luaVocationGetCapacityGain(lua_State* L);

		static int32_t luaVocationGetHealthGain(lua_State* L);
		static int32_t luaVocationGetHealthGainTicks(lua_State* L);
		static int32_t luaVocationGetHealthGainAmount(lua_State* L);

		static int32_t luaVocationGetManaGain(lua_State* L);
		static int32_t luaVocationGetManaGainTicks(lua_State* L);
		static int32_t luaVocationGetManaGainAmount(lua_State* L);

		static int32_t luaVocationGetMaxSoul(lua_State* L);
		static int32_t luaVocationGetSoulGainTicks(lua_State* L);

		static int32_t luaVocationGetAttackSpeed(lua_State* L);
		static int32_t luaVocationGetBaseSpeed(lua_State* L);

		static int32_t luaVocationGetDemotion(lua_State* L);
		static int32_t luaVocationGetPromotion(lua_State* L);

		// Town
		static int32_t luaTownCreate(lua_State* L);

		static int32_t luaTownGetId(lua_State* L);
		static int32_t luaTownGetName(lua_State* L);
		static int32_t luaTownGetTemplePosition(lua_State* L);

		// House
		static int32_t luaHouseCreate(lua_State* L);

		static int32_t luaHouseGetId(lua_State* L);
		static int32_t luaHouseGetName(lua_State* L);
		static int32_t luaHouseGetTown(lua_State* L);
		static int32_t luaHouseGetExitPosition(lua_State* L);
		static int32_t luaHouseGetRent(lua_State* L);

		static int32_t luaHouseGetOwnerGuid(lua_State* L);
		static int32_t luaHouseSetOwnerGuid(lua_State* L);

		static int32_t luaHouseGetBeds(lua_State* L);
		static int32_t luaHouseGetBedCount(lua_State* L);

		static int32_t luaHouseGetDoors(lua_State* L);
		static int32_t luaHouseGetDoorCount(lua_State* L);

		static int32_t luaHouseGetTiles(lua_State* L);
		static int32_t luaHouseGetTileCount(lua_State* L);

		static int32_t luaHouseGetAccessList(lua_State* L);
		static int32_t luaHouseSetAccessList(lua_State* L);

		// ItemType
		static int32_t luaItemTypeCreate(lua_State* L);

		static int32_t luaItemTypeIsCorpse(lua_State* L);
		static int32_t luaItemTypeIsDoor(lua_State* L);
		static int32_t luaItemTypeIsContainer(lua_State* L);
		static int32_t luaItemTypeIsFluidContainer(lua_State* L);
		static int32_t luaItemTypeIsMovable(lua_State* L);
		static int32_t luaItemTypeIsRune(lua_State* L);
		static int32_t luaItemTypeIsStackable(lua_State* L);
		static int32_t luaItemTypeIsReadable(lua_State* L);
		static int32_t luaItemTypeIsWritable(lua_State* L);

		static int32_t luaItemTypeGetType(lua_State* L);
		static int32_t luaItemTypeGetId(lua_State* L);
		static int32_t luaItemTypeGetName(lua_State* L);
		static int32_t luaItemTypeGetPluralName(lua_State* L);
		static int32_t luaItemTypeGetArticle(lua_State* L);
		static int32_t luaItemTypeGetDescription(lua_State* L);

		static int32_t luaItemTypeGetFluidSource(lua_State* L);
		static int32_t luaItemTypeGetCapacity(lua_State* L);
		static int32_t luaItemTypeGetWeight(lua_State* L);

		static int32_t luaItemTypeGetAttack(lua_State* L);
		static int32_t luaItemTypeGetDefense(lua_State* L);
		static int32_t luaItemTypeGetExtraDefense(lua_State* L);
		static int32_t luaItemTypeGetArmor(lua_State* L);
		static int32_t luaItemTypeGetWeaponType(lua_State* L);

		static int32_t luaItemTypeGetElementType(lua_State* L);
		static int32_t luaItemTypeGetElementDamage(lua_State* L);

		static int32_t luaItemTypeGetTransformEquipId(lua_State* L);
		static int32_t luaItemTypeGetTransformDeEquipId(lua_State* L);
		static int32_t luaItemTypeGetDecayId(lua_State* L);

		static int32_t luaItemTypeHasSubType(lua_State* L);

		// Combat
		static int32_t luaCombatCreate(lua_State* L);

		static int32_t luaCombatSetParameter(lua_State* L);
		static int32_t luaCombatSetFormula(lua_State* L);

		static int32_t luaCombatSetArea(lua_State* L);
		static int32_t luaCombatSetCondition(lua_State* L);
		static int32_t luaCombatSetCallback(lua_State* L);

		static int32_t luaCombatExecute(lua_State* L);

		// Condition
		static int32_t luaConditionCreate(lua_State* L);

		static int32_t luaConditionGetId(lua_State* L);
		static int32_t luaConditionGetSubId(lua_State* L);
		static int32_t luaConditionGetType(lua_State* L);
		static int32_t luaConditionGetIcons(lua_State* L);
		static int32_t luaConditionGetEndTime(lua_State* L);

		static int32_t luaConditionClone(lua_State* L);

		static int32_t luaConditionGetTicks(lua_State* L);
		static int32_t luaConditionSetTicks(lua_State* L);

		static int32_t luaConditionSetParameter(lua_State* L);
		static int32_t luaConditionSetFormula(lua_State* L);

		static int32_t luaConditionAddDamage(lua_State* L);
		static int32_t luaConditionAddOutfit(lua_State* L);

		// MonsterType
		static int32_t luaMonsterTypeCreate(lua_State* L);

		static int32_t luaMonsterTypeIsAttackable(lua_State* L);
		static int32_t luaMonsterTypeIsConvinceable(lua_State* L);
		static int32_t luaMonsterTypeIsSummonable(lua_State* L);
		static int32_t luaMonsterTypeIsIllusionable(lua_State* L);
		static int32_t luaMonsterTypeIsHostile(lua_State* L);
		static int32_t luaMonsterTypeIsPushable(lua_State* L);
		static int32_t luaMonsterTypeIsHealthShown(lua_State* L);

		static int32_t luaMonsterTypeCanPushItems(lua_State* L);
		static int32_t luaMonsterTypeCanPushCreatures(lua_State* L);

		static int32_t luaMonsterTypeGetName(lua_State* L);
		static int32_t luaMonsterTypeGetNameDescription(lua_State* L);

		static int32_t luaMonsterTypeGetHealth(lua_State* L);
		static int32_t luaMonsterTypeGetMaxHealth(lua_State* L);
		static int32_t luaMonsterTypeGetRunHealth(lua_State* L);
		static int32_t luaMonsterTypeGetExperience(lua_State* L);

		static int32_t luaMonsterTypeGetCombatImmunities(lua_State* L);
		static int32_t luaMonsterTypeGetConditionImmunities(lua_State* L);

		static int32_t luaMonsterTypeGetAttackList(lua_State* L);
		static int32_t luaMonsterTypeGetDefenseList(lua_State* L);
		static int32_t luaMonsterTypeGetElementList(lua_State* L);

		static int32_t luaMonsterTypeGetVoices(lua_State* L);
		static int32_t luaMonsterTypeGetLoot(lua_State* L);
		static int32_t luaMonsterTypeGetCreatureEvents(lua_State* L);

		static int32_t luaMonsterTypeGetSummonList(lua_State* L);
		static int32_t luaMonsterTypeGetMaxSummons(lua_State* L);

		static int32_t luaMonsterTypeGetArmor(lua_State* L);
		static int32_t luaMonsterTypeGetDefense(lua_State* L);
		static int32_t luaMonsterTypeGetOutfit(lua_State* L);
		static int32_t luaMonsterTypeGetRace(lua_State* L);
		static int32_t luaMonsterTypeGetCorpseId(lua_State* L);
		static int32_t luaMonsterTypeGetManaCost(lua_State* L);
		static int32_t luaMonsterTypeGetBaseSpeed(lua_State* L);
		static int32_t luaMonsterTypeGetLight(lua_State* L);

		static int32_t luaMonsterTypeGetStaticAttackChance(lua_State* L);
		static int32_t luaMonsterTypeGetTargetDistance(lua_State* L);
		static int32_t luaMonsterTypeGetYellChance(lua_State* L);
		static int32_t luaMonsterTypeGetYellSpeedTicks(lua_State* L);
		static int32_t luaMonsterTypeGetChangeTargetChance(lua_State* L);
		static int32_t luaMonsterTypeGetChangeTargetSpeed(lua_State* L);

		// Party
		static int32_t luaPartyDisband(lua_State* L);

		static int32_t luaPartyGetLeader(lua_State* L);
		static int32_t luaPartySetLeader(lua_State* L);
	
		static int32_t luaPartyGetMembers(lua_State* L);
		static int32_t luaPartyGetMemberCount(lua_State* L);

		static int32_t luaPartyGetInvitees(lua_State* L);
		static int32_t luaPartyGetInviteeCount(lua_State* L);

		static int32_t luaPartyAddInvite(lua_State* L);
		static int32_t luaPartyRemoveInvite(lua_State* L);

		static int32_t luaPartyAddMember(lua_State* L);
		static int32_t luaPartyRemoveMember(lua_State* L);

		static int32_t luaPartyIsSharedExperienceActive(lua_State* L);
		static int32_t luaPartyIsSharedExperienceEnabled(lua_State* L);
		static int32_t luaPartyShareExperience(lua_State* L);
		static int32_t luaPartySetSharedExperience(lua_State* L);

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

		bool initState();
		bool reInitState();
		bool closeState();

		LuaScriptInterface* getTestInterface();

		Combat* getCombatObject(uint32_t id) const;
		uint32_t createCombatObject(LuaScriptInterface* interface);
		void clearCombatObjects(LuaScriptInterface* interface);

		Condition* getConditionObject(uint32_t id) const;
		bool createConditionObject(LuaScriptInterface* interface, ConditionType_t conditionType, ConditionId_t conditionId, uint32_t& id);
		void clearConditionObjects(LuaScriptInterface* interface);

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
		std::unordered_map<LuaScriptInterface*, std::vector<uint32_t>> m_conditionIdMap;
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
