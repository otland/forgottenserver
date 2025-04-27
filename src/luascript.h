// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_LUASCRIPT_H
#define FS_LUASCRIPT_H

#include "database.h"
#include "enums.h"
#include "position.h"

#if LUA_VERSION_NUM >= 502
#ifndef LUA_COMPAT_ALL
#ifndef LUA_COMPAT_MODULE
#define luaL_register(L, libname, l) (luaL_newlib(L, l), lua_pushvalue(L, -1), lua_setglobal(L, libname))
#endif
#undef lua_equal
#define lua_equal(L, i1, i2) lua_compare(L, (i1), (i2), LUA_OPEQ)
#endif
#endif

class AreaCombat;
class Combat;
class Container;
class Creature;
class Cylinder;
class Spell;
class Item;
class LuaScriptInterface;
class LuaVariant;
class Npc;
class Player;
class Thing;
struct Outfit;

using Combat_ptr = std::shared_ptr<Combat>;

inline constexpr int32_t EVENT_ID_USER = 1000;

struct LuaTimerEventDesc
{
	int32_t scriptId = -1;
	int32_t function = -1;
	std::vector<int32_t> parameters;
	uint32_t eventId = 0;

	LuaTimerEventDesc() = default;
	LuaTimerEventDesc(LuaTimerEventDesc&& other) = default;
};

class ScriptEnvironment
{
public:
	ScriptEnvironment();
	~ScriptEnvironment();

	// non-copyable
	ScriptEnvironment(const ScriptEnvironment&) = delete;
	ScriptEnvironment& operator=(const ScriptEnvironment&) = delete;

	void resetEnv();

	void setScriptId(int32_t scriptId, LuaScriptInterface* scriptInterface)
	{
		this->scriptId = scriptId;
		interface = scriptInterface;
	}
	bool setCallbackId(int32_t callbackId, LuaScriptInterface* scriptInterface);

	int32_t getScriptId() const { return scriptId; }
	LuaScriptInterface* getScriptInterface() { return interface; }

	void setTimerEvent() { timerEvent = true; }

	auto getEventInfo() const { return std::make_tuple(scriptId, interface, callbackId, timerEvent); }

	uint32_t addThing(Thing* thing);
	void insertItem(uint32_t uid, Item* item);

	void setNpc(Npc* npc) { curNpc = npc; }
	Npc* getNpc() const { return curNpc; }

	Thing* getThingByUID(uint32_t uid);
	Item* getItemByUID(uint32_t uid);
	Container* getContainerByUID(uint32_t uid);
	void removeItemByUID(uint32_t uid);

private:
	LuaScriptInterface* interface;

	// for npc scripts
	Npc* curNpc = nullptr;

	// local item map
	std::unordered_map<uint32_t, Item*> localMap;
	uint32_t lastUID = std::numeric_limits<uint16_t>::max();

	// script file id
	int32_t scriptId;
	int32_t callbackId;
	bool timerEvent;
};

enum ErrorCode_t
{
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
	int32_t getEvent(std::string_view eventName);
	int32_t getEvent();
	int32_t getMetaEvent(const std::string& globalName, const std::string& eventName);

	const std::string& getInterfaceName() const { return interfaceName; }
	const std::string& getLastLuaError() const { return lastLuaError; }

	lua_State* getLuaState() const { return L; }

	bool pushFunction(int32_t functionId);

	bool callFunction(int params);
	void callVoidFunction(int params);

#ifndef LUAJIT_VERSION
	static const luaL_Reg luaBitReg[7];
#endif
	static const luaL_Reg luaConfigManagerTable[4];
	static const luaL_Reg luaDatabaseTable[9];
	static const luaL_Reg luaResultTable[6];

protected:
	virtual bool closeState();

	void registerFunctions();

	lua_State* L = nullptr;

	int32_t eventTableRef = -1;
	int32_t runningEventId = EVENT_ID_USER;

	// script file cache
	std::map<int32_t, std::string> cacheFiles;

private:
	// lua functions
	static int luaDoPlayerAddItem(lua_State* L);

	// get item info
	static int luaGetDepotId(lua_State* L);

	// get world info
	static int luaGetWorldUpTime(lua_State* L);

	// get subtype name
	static int luaGetSubTypeName(lua_State* L);

	// type validation
	static int luaIsDepot(lua_State* L);
	static int luaIsMoveable(lua_State* L);
	static int luaIsValidUID(lua_State* L);

	//
	static int luaCreateCombatArea(lua_State* L);

	static int luaDoAreaCombat(lua_State* L);
	static int luaDoTargetCombat(lua_State* L);

	static int luaDoChallengeCreature(lua_State* L);

	static int luaDebugPrint(lua_State* L);
	static int luaAddEvent(lua_State* L);
	static int luaStopEvent(lua_State* L);

	static int luaSaveServer(lua_State* L);
	static int luaCleanMap(lua_State* L);

	static int luaIsInWar(lua_State* L);

	static int luaGetWaypointPositionByName(lua_State* L);

	static int luaSendChannelMessage(lua_State* L);
	static int luaSendGuildChannelMessage(lua_State* L);

	static int luaIsScriptsInterface(lua_State* L);

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
	static int luaTablePack(lua_State* L);

	// DB Insert
	static int luaDBInsertCreate(lua_State* L);
	static int luaDBInsertAddRow(lua_State* L);
	static int luaDBInsertExecute(lua_State* L);
	static int luaDBInsertDelete(lua_State* L);

	// DB Transaction
	static int luaDBTransactionCreate(lua_State* L);
	static int luaDBTransactionDelete(lua_State* L);
	static int luaDBTransactionBegin(lua_State* L);
	static int luaDBTransactionCommit(lua_State* L);

	// Game
	static int luaGameGetSpectators(lua_State* L);
	static int luaGameGetPlayers(lua_State* L);
	static int luaGameGetNpcs(lua_State* L);
	static int luaGameGetMonsters(lua_State* L);
	static int luaGameLoadMap(lua_State* L);

	static int luaGameGetExperienceStage(lua_State* L);
	static int luaGameGetExperienceForLevel(lua_State* L);
	static int luaGameGetMonsterCount(lua_State* L);
	static int luaGameGetPlayerCount(lua_State* L);
	static int luaGameGetNpcCount(lua_State* L);
	static int luaGameGetMonsterTypes(lua_State* L);
	static int luaGameGetBestiary(lua_State* L);
	static int luaGameGetCurrencyItems(lua_State* L);
	static int luaGameGetItemTypeByClientId(lua_State* L);
	static int luaGameGetMountIdByLookType(lua_State* L);

	static int luaGameGetTowns(lua_State* L);
	static int luaGameGetHouses(lua_State* L);
	static int luaGameGetOutfits(lua_State* L);
	static int luaGameGetMounts(lua_State* L);
	static int luaGameGetVocations(lua_State* L);

	static int luaGameGetGameState(lua_State* L);
	static int luaGameSetGameState(lua_State* L);

	static int luaGameGetWorldType(lua_State* L);
	static int luaGameSetWorldType(lua_State* L);

	static int luaGameGetItemAttributeByName(lua_State* L);
	static int luaGameGetReturnMessage(lua_State* L);

	static int luaGameCreateItem(lua_State* L);
	static int luaGameCreateContainer(lua_State* L);
	static int luaGameCreateMonster(lua_State* L);
	static int luaGameCreateNpc(lua_State* L);
	static int luaGameCreateTile(lua_State* L);
	static int luaGameCreateMonsterType(lua_State* L);

	static int luaGameStartEvent(lua_State* L);

	static int luaGameGetClientVersion(lua_State* L);

	static int luaGameReload(lua_State* L);

	// Variant
	static int luaVariantCreate(lua_State* L);

	static int luaVariantGetNumber(lua_State* L);
	static int luaVariantGetString(lua_State* L);
	static int luaVariantGetPosition(lua_State* L);

	// Position
	static int luaPositionCreate(lua_State* L);

	static int luaPositionIsSightClear(lua_State* L);

	static int luaPositionSendMagicEffect(lua_State* L);
	static int luaPositionSendDistanceEffect(lua_State* L);

	// Tile
	static int luaTileCreate(lua_State* L);

	static int luaTileRemove(lua_State* L);

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
	static int luaTileAddItem(lua_State* L);
	static int luaTileAddItemEx(lua_State* L);

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
	static int luaNetworkMessageSeek(lua_State* L);
	static int luaNetworkMessageTell(lua_State* L);
	static int luaNetworkMessageLength(lua_State* L);
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

	static int luaItemHasParent(lua_State* L);
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
	static int luaItemGetWorth(lua_State* L);

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
	static int luaItemGetCustomAttribute(lua_State* L);
	static int luaItemSetCustomAttribute(lua_State* L);
	static int luaItemRemoveCustomAttribute(lua_State* L);

	static int luaItemMoveTo(lua_State* L);
	static int luaItemTransform(lua_State* L);
	static int luaItemDecay(lua_State* L);

	static int luaItemGetSpecialDescription(lua_State* L);

	static int luaItemHasProperty(lua_State* L);
	static int luaItemIsLoadedFromMap(lua_State* L);

	static int luaItemSetStoreItem(lua_State* L);
	static int luaItemIsStoreItem(lua_State* L);

	static int luaItemSetReflect(lua_State* L);
	static int luaItemGetReflect(lua_State* L);

	static int luaItemSetBoostPercent(lua_State* L);
	static int luaItemGetBoostPercent(lua_State* L);

	// Container
	static int luaContainerCreate(lua_State* L);

	static int luaContainerGetSize(lua_State* L);
	static int luaContainerGetCapacity(lua_State* L);
	static int luaContainerGetEmptySlots(lua_State* L);
	static int luaContainerGetItems(lua_State* L);
	static int luaContainerGetItemHoldingCount(lua_State* L);
	static int luaContainerGetItemCountById(lua_State* L);

	static int luaContainerGetItem(lua_State* L);
	static int luaContainerHasItem(lua_State* L);
	static int luaContainerAddItem(lua_State* L);
	static int luaContainerAddItemEx(lua_State* L);
	static int luaContainerGetCorpseOwner(lua_State* L);

	// Teleport
	static int luaTeleportCreate(lua_State* L);

	static int luaTeleportGetDestination(lua_State* L);
	static int luaTeleportSetDestination(lua_State* L);

	// Podium
	static int luaPodiumCreate(lua_State* L);

	static int luaPodiumGetOutfit(lua_State* L);
	static int luaPodiumSetOutfit(lua_State* L);
	static int luaPodiumHasFlag(lua_State* L);
	static int luaPodiumSetFlag(lua_State* L);
	static int luaPodiumGetDirection(lua_State* L);
	static int luaPodiumSetDirection(lua_State* L);

	// Creature
	static int luaCreatureCreate(lua_State* L);

	static int luaCreatureGetEvents(lua_State* L);
	static int luaCreatureRegisterEvent(lua_State* L);
	static int luaCreatureUnregisterEvent(lua_State* L);

	static int luaCreatureIsRemoved(lua_State* L);
	static int luaCreatureIsCreature(lua_State* L);
	static int luaCreatureIsInGhostMode(lua_State* L);
	static int luaCreatureIsHealthHidden(lua_State* L);
	static int luaCreatureIsMovementBlocked(lua_State* L);
	static int luaCreatureIsImmune(lua_State* L);

	static int luaCreatureCanSee(lua_State* L);
	static int luaCreatureCanSeeCreature(lua_State* L);
	static int luaCreatureCanSeeGhostMode(lua_State* L);
	static int luaCreatureCanSeeInvisibility(lua_State* L);

	static int luaCreatureHasParent(lua_State* L);
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
	static int luaCreatureSetSkillLoss(lua_State* L);

	static int luaCreatureGetPosition(lua_State* L);
	static int luaCreatureGetTile(lua_State* L);
	static int luaCreatureGetDirection(lua_State* L);
	static int luaCreatureSetDirection(lua_State* L);

	static int luaCreatureGetHealth(lua_State* L);
	static int luaCreatureSetHealth(lua_State* L);
	static int luaCreatureAddHealth(lua_State* L);
	static int luaCreatureGetMaxHealth(lua_State* L);
	static int luaCreatureSetMaxHealth(lua_State* L);
	static int luaCreatureSetHiddenHealth(lua_State* L);
	static int luaCreatureSetMovementBlocked(lua_State* L);

	static int luaCreatureGetSkull(lua_State* L);
	static int luaCreatureSetSkull(lua_State* L);

	static int luaCreatureGetOutfit(lua_State* L);
	static int luaCreatureSetOutfit(lua_State* L);

	static int luaCreatureGetCondition(lua_State* L);
	static int luaCreatureAddCondition(lua_State* L);
	static int luaCreatureRemoveCondition(lua_State* L);
	static int luaCreatureHasCondition(lua_State* L);

	static int luaCreatureRemove(lua_State* L);
	static int luaCreatureTeleportTo(lua_State* L);
	static int luaCreatureSay(lua_State* L);

	static int luaCreatureGetDamageMap(lua_State* L);

	static int luaCreatureGetSummons(lua_State* L);

	static int luaCreatureGetDescription(lua_State* L);

	static int luaCreatureGetPathTo(lua_State* L);
	static int luaCreatureMove(lua_State* L);

	static int luaCreatureGetZone(lua_State* L);

	static int luaCreatureHasIcon(lua_State* L);
	static int luaCreatureSetIcon(lua_State* L);
	static int luaCreatureGetIcon(lua_State* L);
	static int luaCreatureRemoveIcon(lua_State* L);

	static int luaCreatureGetStorageValue(lua_State* L);
	static int luaCreatureSetStorageValue(lua_State* L);

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
	static int luaPlayerGetLevelPercent(lua_State* L);

	static int luaPlayerGetMagicLevel(lua_State* L);
	static int luaPlayerGetMagicLevelPercent(lua_State* L);
	static int luaPlayerGetBaseMagicLevel(lua_State* L);
	static int luaPlayerGetMana(lua_State* L);
	static int luaPlayerAddMana(lua_State* L);
	static int luaPlayerGetMaxMana(lua_State* L);
	static int luaPlayerSetMaxMana(lua_State* L);
	static int luaPlayerSetManaShieldBar(lua_State* L);
	static int luaPlayerGetManaSpent(lua_State* L);
	static int luaPlayerAddManaSpent(lua_State* L);
	static int luaPlayerRemoveManaSpent(lua_State* L);

	static int luaPlayerGetBaseMaxHealth(lua_State* L);
	static int luaPlayerGetBaseMaxMana(lua_State* L);

	static int luaPlayerGetSkillLevel(lua_State* L);
	static int luaPlayerGetEffectiveSkillLevel(lua_State* L);
	static int luaPlayerGetSkillPercent(lua_State* L);
	static int luaPlayerGetSkillTries(lua_State* L);
	static int luaPlayerAddSkillTries(lua_State* L);
	static int luaPlayerRemoveSkillTries(lua_State* L);
	static int luaPlayerGetSpecialSkill(lua_State* L);
	static int luaPlayerAddSpecialSkill(lua_State* L);

	static int luaPlayerAddOfflineTrainingTime(lua_State* L);
	static int luaPlayerGetOfflineTrainingTime(lua_State* L);
	static int luaPlayerRemoveOfflineTrainingTime(lua_State* L);

	static int luaPlayerAddOfflineTrainingTries(lua_State* L);

	static int luaPlayerGetOfflineTrainingSkill(lua_State* L);
	static int luaPlayerSetOfflineTrainingSkill(lua_State* L);

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

	static int luaPlayerAddItem(lua_State* L);
	static int luaPlayerAddItemEx(lua_State* L);
	static int luaPlayerRemoveItem(lua_State* L);
	static int luaPlayerSendSupplyUsed(lua_State* L);

	static int luaPlayerGetMoney(lua_State* L);
	static int luaPlayerAddMoney(lua_State* L);
	static int luaPlayerRemoveMoney(lua_State* L);

	static int luaPlayerShowTextDialog(lua_State* L);

	static int luaPlayerSendTextMessage(lua_State* L);
	static int luaPlayerSendChannelMessage(lua_State* L);
	static int luaPlayerSendPrivateMessage(lua_State* L);

	static int luaPlayerChannelSay(lua_State* L);
	static int luaPlayerOpenChannel(lua_State* L);
	static int luaPlayerLeaveChannel(lua_State* L);

	static int luaPlayerGetSlotItem(lua_State* L);

	static int luaPlayerGetParty(lua_State* L);

	static int luaPlayerAddOutfit(lua_State* L);
	static int luaPlayerAddOutfitAddon(lua_State* L);
	static int luaPlayerRemoveOutfit(lua_State* L);
	static int luaPlayerRemoveOutfitAddon(lua_State* L);
	static int luaPlayerHasOutfit(lua_State* L);
	static int luaPlayerCanWearOutfit(lua_State* L);
	static int luaPlayerSendOutfitWindow(lua_State* L);

	static int luaPlayerSendEditPodium(lua_State* L);

	static int luaPlayerAddMount(lua_State* L);
	static int luaPlayerRemoveMount(lua_State* L);
	static int luaPlayerHasMount(lua_State* L);
	static int luaPlayerToggleMount(lua_State* L);

	static int luaPlayerGetPremiumEndsAt(lua_State* L);
	static int luaPlayerSetPremiumEndsAt(lua_State* L);

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
	static int luaPlayerSendHouseWindow(lua_State* L);
	static int luaPlayerSetEditHouse(lua_State* L);

	static int luaPlayerSetGhostMode(lua_State* L);

	static int luaPlayerGetContainerId(lua_State* L);
	static int luaPlayerGetContainerById(lua_State* L);
	static int luaPlayerGetContainerIndex(lua_State* L);

	static int luaPlayerGetInstantSpells(lua_State* L);
	static int luaPlayerCanCast(lua_State* L);

	static int luaPlayerHasChaseMode(lua_State* L);
	static int luaPlayerHasSecureMode(lua_State* L);
	static int luaPlayerGetFightMode(lua_State* L);

	static int luaPlayerGetStoreInbox(lua_State* L);

	static int luaPlayerIsNearDepotBox(lua_State* L);

	static int luaPlayerGetIdleTime(lua_State* L);
	static int luaPlayerResetIdleTime(lua_State* L);

	static int luaPlayerSendCreatureSquare(lua_State* L);

	static int luaPlayerGetClientExpDisplay(lua_State* L);
	static int luaPlayerSetClientExpDisplay(lua_State* L);

	static int luaPlayerGetClientStaminaBonusDisplay(lua_State* L);
	static int luaPlayerSetClientStaminaBonusDisplay(lua_State* L);

	static int luaPlayerGetClientLowLevelBonusDisplay(lua_State* L);
	static int luaPlayerSetClientLowLevelBonusDisplay(lua_State* L);

	static int luaPlayerSendResourceBalance(lua_State* L);
	static int luaPlayerSendEnterMarket(lua_State* L);
	
	static int luaPlayerGetStashItemCount(lua_State* L);
	static int luaPlayerGetStashCounter(lua_State* L);
	static int luaPlayerOpenStash(lua_State* L);
	static int luaPlayerAddItemStash(lua_State* L);
	static int luaPlayerRemoveStashItem(lua_State* L);

	// Monster
	static int luaMonsterCreate(lua_State* L);

	static int luaMonsterIsMonster(lua_State* L);

	static int luaMonsterGetId(lua_State* L);
	static int luaMonsterGetType(lua_State* L);

	static int luaMonsterRename(lua_State* L);

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

	static int luaMonsterIsWalkingToSpawn(lua_State* L);
	static int luaMonsterWalkToSpawn(lua_State* L);

	static int luaMonsterHasIcon(lua_State* L);
	static int luaMonsterSetIcon(lua_State* L);
	static int luaMonsterGetIcon(lua_State* L);
	static int luaMonsterRemoveIcon(lua_State* L);

	// Npc
	static int luaNpcCreate(lua_State* L);

	static int luaNpcIsNpc(lua_State* L);

	static int luaNpcSetMasterPos(lua_State* L);

	static int luaNpcGetSpeechBubble(lua_State* L);
	static int luaNpcSetSpeechBubble(lua_State* L);

	static int luaNpcGetSpectators(lua_State* L);

	// Guild
	static int luaGuildCreate(lua_State* L);

	static int luaGuildGetId(lua_State* L);
	static int luaGuildGetName(lua_State* L);
	static int luaGuildGetMembersOnline(lua_State* L);

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
	static int luaGroupHasFlag(lua_State* L);

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

	static int luaVocationAllowsPvp(lua_State* L);

	// House
	static int luaHouseCreate(lua_State* L);

	static int luaHouseGetId(lua_State* L);
	static int luaHouseGetName(lua_State* L);
	static int luaHouseGetTown(lua_State* L);
	static int luaHouseGetExitPosition(lua_State* L);

	static int luaHouseGetRent(lua_State* L);
	static int luaHouseSetRent(lua_State* L);

	static int luaHouseGetPaidUntil(lua_State* L);
	static int luaHouseSetPaidUntil(lua_State* L);

	static int luaHouseGetPayRentWarnings(lua_State* L);
	static int luaHouseSetPayRentWarnings(lua_State* L);

	static int luaHouseGetOwnerName(lua_State* L);
	static int luaHouseGetOwnerGuid(lua_State* L);
	static int luaHouseSetOwnerGuid(lua_State* L);
	static int luaHouseStartTrade(lua_State* L);

	static int luaHouseGetBeds(lua_State* L);
	static int luaHouseGetBedCount(lua_State* L);

	static int luaHouseGetDoors(lua_State* L);
	static int luaHouseGetDoorCount(lua_State* L);
	static int luaHouseGetDoorIdByPosition(lua_State* L);

	static int luaHouseGetTiles(lua_State* L);
	static int luaHouseGetItems(lua_State* L);
	static int luaHouseGetTileCount(lua_State* L);

	static int luaHouseCanEditAccessList(lua_State* L);
	static int luaHouseGetAccessList(lua_State* L);
	static int luaHouseSetAccessList(lua_State* L);

	static int luaHouseKickPlayer(lua_State* L);

	static int luaHouseSave(lua_State* L);

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
	static int luaItemTypeIsBlocking(lua_State* L);
	static int luaItemTypeIsGroundTile(lua_State* L);
	static int luaItemTypeIsMagicField(lua_State* L);
	static int luaItemTypeIsUseable(lua_State* L);
	static int luaItemTypeIsPickupable(lua_State* L);
	static int luaItemTypeIsRotatable(lua_State* L);

	static int luaItemTypeGetType(lua_State* L);
	static int luaItemTypeGetGroup(lua_State* L);
	static int luaItemTypeGetId(lua_State* L);
	static int luaItemTypeGetClientId(lua_State* L);
	static int luaItemTypeGetName(lua_State* L);
	static int luaItemTypeGetPluralName(lua_State* L);
	static int luaItemTypeGetRotateTo(lua_State* L);
	static int luaItemTypeGetArticle(lua_State* L);
	static int luaItemTypeGetDescription(lua_State* L);
	static int luaItemTypeGetSlotPosition(lua_State* L);

	static int luaItemTypeGetCharges(lua_State* L);
	static int luaItemTypeGetFluidSource(lua_State* L);
	static int luaItemTypeGetCapacity(lua_State* L);
	static int luaItemTypeGetWeight(lua_State* L);
	static int luaItemTypeGetWorth(lua_State* L);

	static int luaItemTypeGetHitChance(lua_State* L);
	static int luaItemTypeGetShootRange(lua_State* L);
	static int luaItemTypeGetAttack(lua_State* L);
	static int luaItemTypeGetAttackSpeed(lua_State* L);
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
	static int luaItemTypeGetAmmoType(lua_State* L);
	static int luaItemTypeGetCorpseType(lua_State* L);
	static int luaItemTypeGetClassification(lua_State* L);
	static int luaItemTypeHasShowCount(lua_State* L);
	static int luaItemTypeGetAbilities(lua_State* L);
	static int luaItemTypeHasShowAttributes(lua_State* L);
	static int luaItemTypeHasShowCharges(lua_State* L);
	static int luaItemTypeHasShowDuration(lua_State* L);
	static int luaItemTypeHasAllowDistRead(lua_State* L);
	static int luaItemTypeGetWieldInfo(lua_State* L);
	static int luaItemTypeGetDurationMin(lua_State* L);
	static int luaItemTypeGetDurationMax(lua_State* L);
	static int luaItemTypeGetLevelDoor(lua_State* L);
	static int luaItemTypeGetRuneSpellName(lua_State* L);
	static int luaItemTypeGetVocationString(lua_State* L);
	static int luaItemTypeGetMinReqLevel(lua_State* L);
	static int luaItemTypeGetMinReqMagicLevel(lua_State* L);

	static int luaItemTypeGetMarketBuyStatistics(lua_State* L);
	static int luaItemTypeGetMarketSellStatistics(lua_State* L);

	static int luaItemTypeHasSubType(lua_State* L);

	static int luaItemTypeIsStoreItem(lua_State* L);

	// Combat
	static int luaCombatCreate(lua_State* L);
	static int luaCombatDelete(lua_State* L);

	static int luaCombatSetParameter(lua_State* L);
	static int luaCombatGetParameter(lua_State* L);

	static int luaCombatSetFormula(lua_State* L);

	static int luaCombatSetArea(lua_State* L);
	static int luaCombatAddCondition(lua_State* L);
	static int luaCombatClearConditions(lua_State* L);
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
	static int luaConditionGetParameter(lua_State* L);

	static int luaConditionSetFormula(lua_State* L);
	static int luaConditionSetOutfit(lua_State* L);

	static int luaConditionAddDamage(lua_State* L);

	// Outfit
	static int luaOutfitCreate(lua_State* L);
	static int luaOutfitCompare(lua_State* L);

	// MonsterType
	static int luaMonsterTypeCreate(lua_State* L);

	static int luaMonsterTypeIsAttackable(lua_State* L);
	static int luaMonsterTypeIsChallengeable(lua_State* L);
	static int luaMonsterTypeIsConvinceable(lua_State* L);
	static int luaMonsterTypeIsSummonable(lua_State* L);
	static int luaMonsterTypeIsIgnoringSpawnBlock(lua_State* L);
	static int luaMonsterTypeIsIllusionable(lua_State* L);
	static int luaMonsterTypeIsHostile(lua_State* L);
	static int luaMonsterTypeIsPushable(lua_State* L);
	static int luaMonsterTypeIsHealthHidden(lua_State* L);
	static int luaMonsterTypeIsBoss(lua_State* L);

	static int luaMonsterTypeCanPushItems(lua_State* L);
	static int luaMonsterTypeCanPushCreatures(lua_State* L);

	static int luaMonsterTypeCanWalkOnEnergy(lua_State* L);
	static int luaMonsterTypeCanWalkOnFire(lua_State* L);
	static int luaMonsterTypeCanWalkOnPoison(lua_State* L);

	static int luaMonsterTypeName(lua_State* L);
	static int luaMonsterTypeNameDescription(lua_State* L);

	static int luaMonsterTypeHealth(lua_State* L);
	static int luaMonsterTypeMaxHealth(lua_State* L);
	static int luaMonsterTypeRunHealth(lua_State* L);
	static int luaMonsterTypeExperience(lua_State* L);
	static int luaMonsterTypeSkull(lua_State* L);

	static int luaMonsterTypeCombatImmunities(lua_State* L);
	static int luaMonsterTypeConditionImmunities(lua_State* L);

	static int luaMonsterTypeGetAttackList(lua_State* L);
	static int luaMonsterTypeAddAttack(lua_State* L);

	static int luaMonsterTypeGetDefenseList(lua_State* L);
	static int luaMonsterTypeAddDefense(lua_State* L);

	static int luaMonsterTypeGetElementList(lua_State* L);
	static int luaMonsterTypeAddElement(lua_State* L);

	static int luaMonsterTypeGetVoices(lua_State* L);
	static int luaMonsterTypeAddVoice(lua_State* L);

	static int luaMonsterTypeGetLoot(lua_State* L);
	static int luaMonsterTypeAddLoot(lua_State* L);

	static int luaMonsterTypeGetCreatureEvents(lua_State* L);
	static int luaMonsterTypeRegisterEvent(lua_State* L);

	static int luaMonsterTypeEventOnCallback(lua_State* L);
	static int luaMonsterTypeEventType(lua_State* L);

	static int luaMonsterTypeGetSummonList(lua_State* L);
	static int luaMonsterTypeAddSummon(lua_State* L);

	static int luaMonsterTypeMaxSummons(lua_State* L);

	static int luaMonsterTypeArmor(lua_State* L);
	static int luaMonsterTypeDefense(lua_State* L);
	static int luaMonsterTypeOutfit(lua_State* L);
	static int luaMonsterTypeRace(lua_State* L);
	static int luaMonsterTypeCorpseId(lua_State* L);
	static int luaMonsterTypeManaCost(lua_State* L);
	static int luaMonsterTypeBaseSpeed(lua_State* L);
	static int luaMonsterTypeLight(lua_State* L);

	static int luaMonsterTypeStaticAttackChance(lua_State* L);
	static int luaMonsterTypeTargetDistance(lua_State* L);
	static int luaMonsterTypeYellChance(lua_State* L);
	static int luaMonsterTypeYellSpeedTicks(lua_State* L);
	static int luaMonsterTypeChangeTargetChance(lua_State* L);
	static int luaMonsterTypeChangeTargetSpeed(lua_State* L);

	static int luaMonsterTypeBestiaryInfo(lua_State* L);

	// Loot
	static int luaCreateLoot(lua_State* L);
	static int luaDeleteLoot(lua_State* L);
	static int luaLootSetId(lua_State* L);
	static int luaLootSetMaxCount(lua_State* L);
	static int luaLootSetSubType(lua_State* L);
	static int luaLootSetChance(lua_State* L);
	static int luaLootSetActionId(lua_State* L);
	static int luaLootSetDescription(lua_State* L);
	static int luaLootAddChildLoot(lua_State* L);

	// MonsterSpell
	static int luaCreateMonsterSpell(lua_State* L);
	static int luaDeleteMonsterSpell(lua_State* L);
	static int luaMonsterSpellSetType(lua_State* L);
	static int luaMonsterSpellSetScriptName(lua_State* L);
	static int luaMonsterSpellSetChance(lua_State* L);
	static int luaMonsterSpellSetInterval(lua_State* L);
	static int luaMonsterSpellSetRange(lua_State* L);
	static int luaMonsterSpellSetCombatValue(lua_State* L);
	static int luaMonsterSpellSetCombatType(lua_State* L);
	static int luaMonsterSpellSetAttackValue(lua_State* L);
	static int luaMonsterSpellSetNeedTarget(lua_State* L);
	static int luaMonsterSpellSetNeedDirection(lua_State* L);
	static int luaMonsterSpellSetCombatLength(lua_State* L);
	static int luaMonsterSpellSetCombatSpread(lua_State* L);
	static int luaMonsterSpellSetCombatRadius(lua_State* L);
	static int luaMonsterSpellSetCombatRing(lua_State* L);
	static int luaMonsterSpellSetConditionType(lua_State* L);
	static int luaMonsterSpellSetConditionDamage(lua_State* L);
	static int luaMonsterSpellSetConditionSpeedChange(lua_State* L);
	static int luaMonsterSpellSetConditionDuration(lua_State* L);
	static int luaMonsterSpellSetConditionDrunkenness(lua_State* L);
	static int luaMonsterSpellSetConditionTickInterval(lua_State* L);
	static int luaMonsterSpellSetCombatShootEffect(lua_State* L);
	static int luaMonsterSpellSetCombatEffect(lua_State* L);
	static int luaMonsterSpellSetOutfit(lua_State* L);

	// Party
	static int luaPartyCreate(lua_State* L);
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
	static int luaPartyIsMemberSharingExp(lua_State* L);
	static int luaPartyShareExperience(lua_State* L);
	static int luaPartySetSharedExperience(lua_State* L);

	// Spells
	static int luaSpellCreate(lua_State* L);

	static int luaSpellOnCastSpell(lua_State* L);
	static int luaSpellRegister(lua_State* L);
	static int luaSpellName(lua_State* L);
	static int luaSpellId(lua_State* L);
	static int luaSpellGroup(lua_State* L);
	static int luaSpellCooldown(lua_State* L);
	static int luaSpellGroupCooldown(lua_State* L);
	static int luaSpellLevel(lua_State* L);
	static int luaSpellMagicLevel(lua_State* L);
	static int luaSpellMana(lua_State* L);
	static int luaSpellManaPercent(lua_State* L);
	static int luaSpellSoul(lua_State* L);
	static int luaSpellRange(lua_State* L);
	static int luaSpellPremium(lua_State* L);
	static int luaSpellEnabled(lua_State* L);
	static int luaSpellNeedTarget(lua_State* L);
	static int luaSpellNeedWeapon(lua_State* L);
	static int luaSpellNeedLearn(lua_State* L);
	static int luaSpellSelfTarget(lua_State* L);
	static int luaSpellBlocking(lua_State* L);
	static int luaSpellAggressive(lua_State* L);
	static int luaSpellPzLock(lua_State* L);
	static int luaSpellVocation(lua_State* L);

	// only for InstantSpells
	static int luaSpellWords(lua_State* L);
	static int luaSpellNeedDirection(lua_State* L);
	static int luaSpellHasParams(lua_State* L);
	static int luaSpellHasPlayerNameParam(lua_State* L);
	static int luaSpellNeedCasterTargetOrDirection(lua_State* L);
	static int luaSpellIsBlockingWalls(lua_State* L);

	// only for RuneSpells
	static int luaSpellRuneLevel(lua_State* L);
	static int luaSpellRuneMagicLevel(lua_State* L);
	static int luaSpellRuneId(lua_State* L);
	static int luaSpellCharges(lua_State* L);
	static int luaSpellAllowFarUse(lua_State* L);
	static int luaSpellBlockWalls(lua_State* L);
	static int luaSpellCheckFloor(lua_State* L);

	// Actions
	static int luaCreateAction(lua_State* L);
	static int luaActionOnUse(lua_State* L);
	static int luaActionRegister(lua_State* L);
	static int luaActionItemId(lua_State* L);
	static int luaActionActionId(lua_State* L);
	static int luaActionUniqueId(lua_State* L);
	static int luaActionAllowFarUse(lua_State* L);
	static int luaActionBlockWalls(lua_State* L);
	static int luaActionCheckFloor(lua_State* L);

	// Talkactions
	static int luaCreateTalkaction(lua_State* L);
	static int luaTalkactionOnSay(lua_State* L);
	static int luaTalkactionRegister(lua_State* L);
	static int luaTalkactionSeparator(lua_State* L);
	static int luaTalkactionAccess(lua_State* L);
	static int luaTalkactionAccountType(lua_State* L);

	// CreatureEvents
	static int luaCreateCreatureEvent(lua_State* L);
	static int luaCreatureEventType(lua_State* L);
	static int luaCreatureEventRegister(lua_State* L);
	static int luaCreatureEventOnCallback(lua_State* L);

	// MoveEvents
	static int luaCreateMoveEvent(lua_State* L);
	static int luaMoveEventType(lua_State* L);
	static int luaMoveEventRegister(lua_State* L);
	static int luaMoveEventOnCallback(lua_State* L);
	static int luaMoveEventLevel(lua_State* L);
	static int luaMoveEventSlot(lua_State* L);
	static int luaMoveEventMagLevel(lua_State* L);
	static int luaMoveEventPremium(lua_State* L);
	static int luaMoveEventVocation(lua_State* L);
	static int luaMoveEventTileItem(lua_State* L);
	static int luaMoveEventItemId(lua_State* L);
	static int luaMoveEventActionId(lua_State* L);
	static int luaMoveEventUniqueId(lua_State* L);
	static int luaMoveEventPosition(lua_State* L);

	// GlobalEvents
	static int luaCreateGlobalEvent(lua_State* L);
	static int luaGlobalEventType(lua_State* L);
	static int luaGlobalEventRegister(lua_State* L);
	static int luaGlobalEventOnCallback(lua_State* L);
	static int luaGlobalEventTime(lua_State* L);
	static int luaGlobalEventInterval(lua_State* L);

	// Weapon
	static int luaCreateWeapon(lua_State* L);
	static int luaWeaponId(lua_State* L);
	static int luaWeaponLevel(lua_State* L);
	static int luaWeaponMagicLevel(lua_State* L);
	static int luaWeaponMana(lua_State* L);
	static int luaWeaponManaPercent(lua_State* L);
	static int luaWeaponHealth(lua_State* L);
	static int luaWeaponHealthPercent(lua_State* L);
	static int luaWeaponSoul(lua_State* L);
	static int luaWeaponPremium(lua_State* L);
	static int luaWeaponBreakChance(lua_State* L);
	static int luaWeaponAction(lua_State* L);
	static int luaWeaponUnproperly(lua_State* L);
	static int luaWeaponVocation(lua_State* L);
	static int luaWeaponOnUseWeapon(lua_State* L);
	static int luaWeaponRegister(lua_State* L);
	static int luaWeaponElement(lua_State* L);
	static int luaWeaponAttack(lua_State* L);
	static int luaWeaponDefense(lua_State* L);
	static int luaWeaponRange(lua_State* L);
	static int luaWeaponCharges(lua_State* L);
	static int luaWeaponDuration(lua_State* L);
	static int luaWeaponDecayTo(lua_State* L);
	static int luaWeaponTransformEquipTo(lua_State* L);
	static int luaWeaponTransformDeEquipTo(lua_State* L);
	static int luaWeaponSlotType(lua_State* L);
	static int luaWeaponHitChance(lua_State* L);
	static int luaWeaponExtraElement(lua_State* L);

	// exclusively for distance weapons
	static int luaWeaponMaxHitChance(lua_State* L);
	static int luaWeaponAmmoType(lua_State* L);

	// exclusively for wands
	static int luaWeaponWandDamage(lua_State* L);

	// exclusively for wands & distance weapons
	static int luaWeaponShootType(lua_State* L);

	// XML
	static int luaCreateXmlDocument(lua_State* L);
	static int luaDeleteXmlDocument(lua_State* L);
	static int luaXmlDocumentChild(lua_State* L);

	static int luaDeleteXmlNode(lua_State* L);
	static int luaXmlNodeAttribute(lua_State* L);
	static int luaXmlNodeName(lua_State* L);
	static int luaXmlNodeFirstChild(lua_State* L);
	static int luaXmlNodeNextSibling(lua_State* L);

	//
	std::string lastLuaError;
	std::string interfaceName;
	std::string loadingFile;
};

class LuaEnvironment : public LuaScriptInterface
{
public:
	LuaEnvironment();
	~LuaEnvironment();

	// non-copyable
	LuaEnvironment(const LuaEnvironment&) = delete;
	LuaEnvironment& operator=(const LuaEnvironment&) = delete;

	bool initState() override;
	bool reInitState();
	bool closeState() override;

	LuaScriptInterface* getTestInterface();

	Combat_ptr getCombatObject(uint32_t id) const;
	Combat_ptr createCombatObject(LuaScriptInterface* interface);
	void clearCombatObjects(LuaScriptInterface* interface);

	AreaCombat* getAreaObject(uint32_t id) const;
	uint32_t createAreaObject(LuaScriptInterface* interface);
	void clearAreaObjects(LuaScriptInterface* interface);

private:
	void executeTimerEvent(uint32_t eventIndex);

	std::unordered_map<uint32_t, LuaTimerEventDesc> timerEvents;
	std::unordered_map<uint32_t, Combat_ptr> combatMap;
	std::unordered_map<uint32_t, AreaCombat*> areaMap;

	std::unordered_map<LuaScriptInterface*, std::vector<uint32_t>> combatIdMap;
	std::unordered_map<LuaScriptInterface*, std::vector<uint32_t>> areaIdMap;

	LuaScriptInterface* testInterface = nullptr;

	uint32_t lastEventTimerId = 1;
	uint32_t lastCombatId = 0;
	uint32_t lastAreaId = 0;

	friend class LuaScriptInterface;
	friend class CombatSpell;
};

namespace tfs::lua {

void removeTempItem(Item* item);

ScriptEnvironment* getScriptEnv();
bool reserveScriptEnv();
void resetScriptEnv();

void reportError(std::string_view function, std::string_view error_desc, lua_State* L = nullptr,
                 bool stack_trace = false);
#define reportErrorFunc(L, a) tfs::lua::reportError(__FUNCTION__, a, L, true)

// push/pop common structures
void pushThing(lua_State* L, Thing* thing);
void pushVariant(lua_State* L, const LuaVariant& var);
void pushString(lua_State* L, std::string_view value);
void pushCallback(lua_State* L, int32_t callback);
void pushCylinder(lua_State* L, Cylinder* cylinder);

std::string popString(lua_State* L);
int32_t popCallback(lua_State* L);

// Userdata
template <class T>
void pushUserdata(lua_State* L, T* value)
{
	T** userdata = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
	*userdata = value;
}

// Metatables
void setMetatable(lua_State* L, int32_t index, std::string_view name);
void setItemMetatable(lua_State* L, int32_t index, const Item* item);
void setCreatureMetatable(lua_State* L, int32_t index, const Creature* creature);

// Get
template <typename T>
typename std::enable_if_t<std::is_enum_v<T>, T> getNumber(lua_State* L, int32_t arg)
{
	return static_cast<T>(static_cast<int64_t>(lua_tonumber(L, arg)));
}

template <typename T>
typename std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, T> getNumber(lua_State* L, int32_t arg)
{
	double num = lua_tonumber(L, arg);
	if (num < static_cast<double>(std::numeric_limits<T>::lowest()) ||
	    num > static_cast<double>(std::numeric_limits<T>::max())) {
		reportErrorFunc(L, fmt::format("Argument {} has out-of-range value for {}: {}", arg, typeid(T).name(), num));
	}

	return static_cast<T>(num);
}

template <typename T>
typename std::enable_if_t<(std::is_integral_v<T> && std::is_signed_v<T>) || std::is_floating_point_v<T>, T> getNumber(
    lua_State* L, int32_t arg)
{
	double num = lua_tonumber(L, arg);
	if (num < static_cast<double>(std::numeric_limits<T>::lowest()) ||
	    num > static_cast<double>(std::numeric_limits<T>::max())) {
		reportErrorFunc(L, fmt::format("Argument {} has out-of-range value for {}: {}", arg, typeid(T).name(), num));
	}

	return static_cast<T>(num);
}

template <typename T>
T getNumber(lua_State* L, int32_t arg, T defaultValue)
{
	if (lua_isnumber(L, arg) == 0) {
		return defaultValue;
	}
	return getNumber<T>(L, arg);
}

template <class T>
T** getRawUserdata(lua_State* L, int32_t arg)
{
	return static_cast<T**>(lua_touserdata(L, arg));
}

template <class T>
T* getUserdata(lua_State* L, int32_t arg)
{
	T** userdata = getRawUserdata<T>(L, arg);
	if (!userdata) {
		return nullptr;
	}
	return *userdata;
}

bool getBoolean(lua_State* L, int32_t arg);
bool getBoolean(lua_State* L, int32_t arg, bool defaultValue);
std::string getString(lua_State* L, int32_t arg);
Position getPosition(lua_State* L, int32_t arg);
Position getPosition(lua_State* L, int32_t arg, int32_t& stackpos);
Thing* getThing(lua_State* L, int32_t arg);
Creature* getCreature(lua_State* L, int32_t arg);
Player* getPlayer(lua_State* L, int32_t arg);

template <typename T>
T getField(lua_State* L, int32_t arg, std::string_view key)
{
	lua_getfield(L, arg, key.data());
	return getNumber<T>(L, -1);
}

template <typename T, typename... Args>
T getField(lua_State* L, int32_t arg, std::string_view key, T&& defaultValue)
{
	lua_getfield(L, arg, key.data());
	return getNumber<T>(L, -1, std::forward<T>(defaultValue));
}

std::string getFieldString(lua_State* L, int32_t arg, std::string_view key);

// Push
void pushBoolean(lua_State* L, bool value);
void pushSpell(lua_State* L, const Spell& spell);
void pushPosition(lua_State* L, const Position& position, int32_t stackpos = 0);
void pushOutfit(lua_State* L, const Outfit_t& outfit);
void pushOutfit(lua_State* L, const Outfit* outfit);

//
int protectedCall(lua_State* L, int nargs, int nresults);
void registerMethod(lua_State* L, std::string_view globalName, std::string_view methodName, lua_CFunction func);
std::string getErrorDesc(ErrorCode_t code);

} // namespace tfs::lua

#endif // FS_LUASCRIPT_H
