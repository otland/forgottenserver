// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "events.h"

#include "item.h"
#include "lua/env.h"
#include "lua/error.h"
#include "lua/meta.h"
#include "player.h"

namespace {

LuaScriptInterface scriptInterface{"Event Interface"};

struct CreatureHandlers
{
	int32_t onChangeOutfit = -1;
	int32_t onAreaCombat = -1;
	int32_t onTargetCombat = -1;
	int32_t onHear = -1;
	int32_t onChangeZone = -1;
	int32_t onUpdateStorage = -1;
} creatureHandlers;

void load_creature_from_scripts()
{
	creatureHandlers = {};

	if (scriptInterface.loadFile("data/scripts/events/creature.lua") != 0) {
		std::cout << "[Warning - tfs::events::load_creature_from_scripts] Cannot load creature events." << std::endl;
		std::cout << scriptInterface.getLastLuaError() << std::endl;
		return;
	}

	creatureHandlers.onChangeOutfit = scriptInterface.getMetaEvent("Creature", "onChangeOutfit");
	creatureHandlers.onAreaCombat = scriptInterface.getMetaEvent("Creature", "onAreaCombat");
	creatureHandlers.onTargetCombat = scriptInterface.getMetaEvent("Creature", "onTargetCombat");
	creatureHandlers.onHear = scriptInterface.getMetaEvent("Creature", "onHear");
	creatureHandlers.onChangeZone = scriptInterface.getMetaEvent("Creature", "onChangeZone");
	creatureHandlers.onUpdateStorage = scriptInterface.getMetaEvent("Creature", "onUpdateStorage");
}

struct PartyHandlers
{
	int32_t onJoin = -1;
	int32_t onLeave = -1;
	int32_t onDisband = -1;
	int32_t onShareExperience = -1;
	int32_t onInvite = -1;
	int32_t onRevokeInvitation = -1;
	int32_t onPassLeadership = -1;
} partyHandlers;

void load_party_from_scripts()
{
	partyHandlers = {};

	if (scriptInterface.loadFile("data/scripts/events/party.lua") != 0) {
		std::cout << "[Warning - tfs::events::load_party_from_scripts] Cannot load party events." << std::endl;
		std::cout << scriptInterface.getLastLuaError() << std::endl;
		return;
	}

	partyHandlers.onJoin = scriptInterface.getMetaEvent("Party", "onJoin");
	partyHandlers.onLeave = scriptInterface.getMetaEvent("Party", "onLeave");
	partyHandlers.onDisband = scriptInterface.getMetaEvent("Party", "onDisband");
	partyHandlers.onShareExperience = scriptInterface.getMetaEvent("Party", "onShareExperience");
	partyHandlers.onInvite = scriptInterface.getMetaEvent("Party", "onInvite");
	partyHandlers.onRevokeInvitation = scriptInterface.getMetaEvent("Party", "onRevokeInvitation");
	partyHandlers.onPassLeadership = scriptInterface.getMetaEvent("Party", "onPassLeadership");
}

struct PlayerHandlers
{
	int32_t onBrowseField = -1;
	int32_t onLook = -1;
	int32_t onLookInBattleList = -1;
	int32_t onLookInTrade = -1;
	int32_t onLookInShop = -1;
	int32_t onLookInMarket = -1;
	int32_t onMoveItem = -1;
	int32_t onItemMoved = -1;
	int32_t onMoveCreature = -1;
	int32_t onReportRuleViolation = -1;
	int32_t onRotateItem = -1;
	int32_t onTurn = -1;
	int32_t onTradeRequest = -1;
	int32_t onTradeAccept = -1;
	int32_t onTradeCompleted = -1;
	int32_t onPodiumRequest = -1;
	int32_t onPodiumEdit = -1;
	int32_t onGainExperience = -1;
	int32_t onLoseExperience = -1;
	int32_t onGainSkillTries = -1;
	int32_t onWrapItem = -1;
	int32_t onInventoryUpdate = -1;
	int32_t onNetworkMessage = -1;
	int32_t onSpellCheck = -1;
} playerHandlers;

void load_player_from_scripts()
{
	playerHandlers = {};

	if (scriptInterface.loadFile("data/scripts/events/player.lua") != 0) {
		std::cout << "[Warning - tfs::events::load_player_from_scripts] Cannot load player events." << std::endl;
		std::cout << scriptInterface.getLastLuaError() << std::endl;
		return;
	}

	playerHandlers.onBrowseField = scriptInterface.getMetaEvent("Player", "onBrowseField");
	playerHandlers.onLook = scriptInterface.getMetaEvent("Player", "onLook");
	playerHandlers.onLookInBattleList = scriptInterface.getMetaEvent("Player", "onLookInBattleList");
	playerHandlers.onLookInTrade = scriptInterface.getMetaEvent("Player", "onLookInTrade");
	playerHandlers.onLookInShop = scriptInterface.getMetaEvent("Player", "onLookInShop");
	playerHandlers.onLookInMarket = scriptInterface.getMetaEvent("Player", "onLookInMarket");
	playerHandlers.onMoveItem = scriptInterface.getMetaEvent("Player", "onMoveItem");
	playerHandlers.onItemMoved = scriptInterface.getMetaEvent("Player", "onItemMoved");
	playerHandlers.onMoveCreature = scriptInterface.getMetaEvent("Player", "onMoveCreature");
	playerHandlers.onReportRuleViolation = scriptInterface.getMetaEvent("Player", "onReportRuleViolation");
	playerHandlers.onRotateItem = scriptInterface.getMetaEvent("Player", "onRotateItem");
	playerHandlers.onTurn = scriptInterface.getMetaEvent("Player", "onTurn");
	playerHandlers.onTradeRequest = scriptInterface.getMetaEvent("Player", "onTradeRequest");
	playerHandlers.onTradeAccept = scriptInterface.getMetaEvent("Player", "onTradeAccept");
	playerHandlers.onTradeCompleted = scriptInterface.getMetaEvent("Player", "onTradeCompleted");
	playerHandlers.onPodiumRequest = scriptInterface.getMetaEvent("Player", "onPodiumRequest");
	playerHandlers.onPodiumEdit = scriptInterface.getMetaEvent("Player", "onPodiumEdit");
	playerHandlers.onGainExperience = scriptInterface.getMetaEvent("Player", "onGainExperience");
	playerHandlers.onLoseExperience = scriptInterface.getMetaEvent("Player", "onLoseExperience");
	playerHandlers.onGainSkillTries = scriptInterface.getMetaEvent("Player", "onGainSkillTries");
	playerHandlers.onWrapItem = scriptInterface.getMetaEvent("Player", "onWrapItem");
	playerHandlers.onInventoryUpdate = scriptInterface.getMetaEvent("Player", "onInventoryUpdate");
	playerHandlers.onNetworkMessage = scriptInterface.getMetaEvent("Player", "onNetworkMessage");
	playerHandlers.onSpellCheck = scriptInterface.getMetaEvent("Player", "onSpellCheck");
}

struct MonsterHandlers
{
	int32_t onDropLoot = -1;
	int32_t onSpawn = -1;
} monsterHandlers;

void load_monster_from_scripts()
{
	monsterHandlers = {};

	if (scriptInterface.loadFile("data/scripts/events/monster.lua") != 0) {
		std::cout << "[Warning - tfs::events::load_monster_from_scripts] Cannot load monster events." << std::endl;
		std::cout << scriptInterface.getLastLuaError() << std::endl;
		return;
	}

	monsterHandlers.onDropLoot = scriptInterface.getMetaEvent("Monster", "onDropLoot");
	monsterHandlers.onSpawn = scriptInterface.getMetaEvent("Monster", "onSpawn");
}

} // namespace

namespace tfs::events {

int32_t getScriptId(EventInfoId eventInfoId)
{
	switch (eventInfoId) {
		case EventInfoId::CREATURE_ONHEAR:
			return creatureHandlers.onHear;
		case EventInfoId::MONSTER_ONSPAWN:
			return monsterHandlers.onSpawn;
		default:
			return -1;
	}
}

void load()
{
	scriptInterface.initState();

	load_creature_from_scripts();
	load_party_from_scripts();
	load_player_from_scripts();
	load_monster_from_scripts();
}

void reload()
{
	scriptInterface.reInitState();

	load_creature_from_scripts();
	load_party_from_scripts();
	load_player_from_scripts();
	load_monster_from_scripts();
}

} // namespace tfs::events

namespace tfs::events::creature {

bool onChangeOutfit(const std::shared_ptr<Creature>& creature, const Outfit_t& outfit)
{
	// Creature:onChangeOutfit(outfit) or Creature.onChangeOutfit(self, outfit)
	if (creatureHandlers.onChangeOutfit == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onChangeOutfit] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureHandlers.onChangeOutfit, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureHandlers.onChangeOutfit);

	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	tfs::lua::pushOutfit(L, outfit);

	return scriptInterface.callFunction(2);
}

ReturnValue onAreaCombat(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Tile>& tile, bool aggressive)
{
	// Creature:onAreaCombat(tile, aggressive) or Creature.onAreaCombat(self, tile, aggressive)
	if (creatureHandlers.onAreaCombat == -1) {
		return RETURNVALUE_NOERROR;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onAreaCombat] Call stack overflow" << std::endl;
		return RETURNVALUE_NOTPOSSIBLE;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureHandlers.onAreaCombat, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureHandlers.onAreaCombat);

	if (creature) {
		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else {
		lua_pushnil(L);
	}

	tfs::lua::pushSharedPtr(L, tile);
	tfs::lua::setMetatable(L, -1, "Tile");

	tfs::lua::pushBoolean(L, aggressive);

	ReturnValue returnValue;
	if (tfs::lua::protectedCall(L, 3, 1) != 0) {
		returnValue = RETURNVALUE_NOTPOSSIBLE;
		tfs::lua::reportError(L, tfs::lua::popString(L));
	} else {
		returnValue = tfs::lua::getNumber<ReturnValue>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
	return returnValue;
}

ReturnValue onTargetCombat(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Creature>& target)
{
	// Creature:onTargetCombat(target) or Creature.onTargetCombat(self, target)
	if (creatureHandlers.onTargetCombat == -1) {
		return RETURNVALUE_NOERROR;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onTargetCombat] Call stack overflow" << std::endl;
		return RETURNVALUE_NOTPOSSIBLE;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureHandlers.onTargetCombat, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureHandlers.onTargetCombat);

	if (creature) {
		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else {
		lua_pushnil(L);
	}

	tfs::lua::pushSharedPtr(L, target);
	tfs::lua::setCreatureMetatable(L, -1, target);

	ReturnValue returnValue;
	if (tfs::lua::protectedCall(L, 2, 1) != 0) {
		returnValue = RETURNVALUE_NOTPOSSIBLE;
		tfs::lua::reportError(L, tfs::lua::popString(L));
	} else {
		returnValue = tfs::lua::getNumber<ReturnValue>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
	return returnValue;
}

void onHear(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Creature>& speaker,
            const std::string& words, SpeakClasses type)
{
	// Creature:onHear(speaker, words, type)
	if (creatureHandlers.onHear == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onHear] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureHandlers.onHear, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureHandlers.onHear);

	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	tfs::lua::pushSharedPtr(L, speaker);
	tfs::lua::setCreatureMetatable(L, -1, speaker);

	tfs::lua::pushString(L, words);
	tfs::lua::pushNumber(L, type);

	scriptInterface.callVoidFunction(4);
}

void onChangeZone(const std::shared_ptr<Creature>& creature, ZoneType_t fromZone, ZoneType_t toZone)
{
	// Creature:onChangeZone(fromZone, toZone)
	if (creatureHandlers.onChangeZone == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onChangeZone] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureHandlers.onChangeZone, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureHandlers.onChangeZone);

	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	tfs::lua::pushNumber(L, fromZone);
	tfs::lua::pushNumber(L, toZone);

	scriptInterface.callVoidFunction(3);
}

void onUpdateStorage(const std::shared_ptr<Creature>& creature, uint32_t key, std::optional<int32_t> value,
                     std::optional<int32_t> oldValue, bool isSpawn)
{
	// Creature:onUpdateStorage(key, value, oldValue, isSpawn)
	if (creatureHandlers.onUpdateStorage == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onUpdateStorage] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureHandlers.onUpdateStorage, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureHandlers.onUpdateStorage);

	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setMetatable(L, -1, "Creature");

	tfs::lua::pushNumber(L, key);

	if (value) {
		tfs::lua::pushNumber(L, value.value());
	} else {
		lua_pushnil(L);
	}

	if (oldValue) {
		tfs::lua::pushNumber(L, oldValue.value());
	} else {
		lua_pushnil(L);
	}

	tfs::lua::pushBoolean(L, isSpawn);

	scriptInterface.callVoidFunction(5);
}

} // namespace tfs::events::creature

namespace tfs::events::party {

bool onJoin(const std::shared_ptr<Party>& party, const std::shared_ptr<Player>& player)
{
	// Party:onJoin(player) or Party.onJoin(self, player)
	if (partyHandlers.onJoin == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onJoin] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(partyHandlers.onJoin, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyHandlers.onJoin);

	tfs::lua::pushSharedPtr(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

bool onLeave(const std::shared_ptr<Party>& party, const std::shared_ptr<Player>& player)
{
	// Party:onLeave(player) or Party.onLeave(self, player)
	if (partyHandlers.onLeave == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onLeave] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(partyHandlers.onLeave, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyHandlers.onLeave);

	tfs::lua::pushSharedPtr(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

bool onDisband(const std::shared_ptr<Party>& party)
{
	// Party:onDisband() or Party.onDisband(self)
	if (partyHandlers.onDisband == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onDisband] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(partyHandlers.onDisband, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyHandlers.onDisband);

	tfs::lua::pushSharedPtr(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	return scriptInterface.callFunction(1);
}

bool onInvite(const std::shared_ptr<Party>& party, const std::shared_ptr<Player>& player)
{
	// Party:onInvite(player) or Party.onInvite(self, player)
	if (partyHandlers.onInvite == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onInvite] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(partyHandlers.onInvite, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyHandlers.onInvite);

	tfs::lua::pushSharedPtr(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

bool onRevokeInvitation(const std::shared_ptr<Party>& party, const std::shared_ptr<Player>& player)
{
	// Party:onRevokeInvitation(player) or Party.onRevokeInvitation(self, player)
	if (partyHandlers.onRevokeInvitation == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onRevokeInvitation] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(partyHandlers.onRevokeInvitation, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyHandlers.onRevokeInvitation);

	tfs::lua::pushSharedPtr(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

bool onPassLeadership(const std::shared_ptr<Party>& party, const std::shared_ptr<Player>& player)
{
	// Party:onPassLeadership(player) or Party.onPassLeadership(self, player)
	if (partyHandlers.onPassLeadership == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onPassLeadership] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(partyHandlers.onPassLeadership, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyHandlers.onPassLeadership);

	tfs::lua::pushSharedPtr(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

void onShareExperience(const std::shared_ptr<Party>& party, uint64_t& exp)
{
	// Party:onShareExperience(exp) or Party.onShareExperience(self, exp)
	if (partyHandlers.onShareExperience == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onShareExperience] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(partyHandlers.onShareExperience, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyHandlers.onShareExperience);

	tfs::lua::pushSharedPtr(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushNumber(L, exp);

	if (tfs::lua::protectedCall(L, 2, 1) != 0) {
		tfs::lua::reportError(L, tfs::lua::popString(L));
	} else {
		exp = tfs::lua::getNumber<uint64_t>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
}

} // namespace tfs::events::party

namespace tfs::events::player {

bool onBrowseField(const std::shared_ptr<Player>& player, const Position& position)
{
	// Player:onBrowseField(position) or Player.onBrowseField(self, position)
	if (playerHandlers.onBrowseField == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onBrowseField] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onBrowseField, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onBrowseField);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushPosition(L, position);

	return scriptInterface.callFunction(2);
}

void onLook(const std::shared_ptr<Player>& player, const Position& position, const std::shared_ptr<Thing>& thing,
            uint8_t stackpos, int32_t lookDistance)
{
	// Player:onLook(thing, position, distance) or Player.onLook(self, thing, position, distance)
	if (playerHandlers.onLook == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLook] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onLook, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onLook);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	if (const auto& creature = thing->asCreature()) {
		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else if (const auto& item = thing->asItem()) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}

	tfs::lua::pushPosition(L, position, stackpos);
	tfs::lua::pushNumber(L, lookDistance);

	scriptInterface.callVoidFunction(4);
}

void onLookInBattleList(const std::shared_ptr<Player>& player, const std::shared_ptr<Creature>& creature,
                        int32_t lookDistance)
{
	// Player:onLookInBattleList(creature, position, distance) or Player.onLookInBattleList(self, creature, position,
	// distance)
	if (playerHandlers.onLookInBattleList == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLookInBattleList] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onLookInBattleList, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onLookInBattleList);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	tfs::lua::pushNumber(L, lookDistance);

	scriptInterface.callVoidFunction(3);
}

void onLookInTrade(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& partner,
                   const std::shared_ptr<Item>& item, int32_t lookDistance)
{
	// Player:onLookInTrade(partner, item, distance) or Player.onLookInTrade(self, partner, item, distance)
	if (playerHandlers.onLookInTrade == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLookInTrade] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onLookInTrade, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onLookInTrade);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, partner);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushNumber(L, lookDistance);

	scriptInterface.callVoidFunction(4);
}

void onLookInShop(const std::shared_ptr<Player>& player, const ItemType* itemType, uint8_t count)
{
	// Player:onLookInShop(itemType, count) or Player.onLookInShop(self, itemType, count)
	if (playerHandlers.onLookInShop == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLookInShop] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onLookInShop, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onLookInShop);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, itemType);
	tfs::lua::setMetatable(L, -1, "ItemType");

	tfs::lua::pushNumber(L, count);

	scriptInterface.callVoidFunction(3);
}

void onLookInMarket(const std::shared_ptr<Player>& player, const ItemType* itemType)
{
	// Player:onLookInMarket(itemType) or Player.onLookInMarket(self, itemType)
	if (playerHandlers.onLookInMarket == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLookInMarket] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onLookInMarket, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onLookInMarket);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, itemType);
	tfs::lua::setMetatable(L, -1, "ItemType");

	scriptInterface.callVoidFunction(2);
}

ReturnValue onMoveItem(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, uint16_t count,
                       const Position& fromPosition, const Position& toPosition,
                       const std::shared_ptr<Thing>& fromThing, const std::shared_ptr<Thing>& toThing)
{
	// Player:onMoveItem(item, count, fromPosition, toPosition) or Player.onMoveItem(self, item, count, fromPosition,
	// toPosition, fromThing, toThing)
	if (playerHandlers.onMoveItem == -1) {
		return RETURNVALUE_NOERROR;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onMoveItem] Call stack overflow" << std::endl;
		return RETURNVALUE_NOTPOSSIBLE;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onMoveItem, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onMoveItem);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushNumber(L, count);
	tfs::lua::pushPosition(L, fromPosition);
	tfs::lua::pushPosition(L, toPosition);

	tfs::lua::pushThing(L, fromThing);
	tfs::lua::pushThing(L, toThing);

	ReturnValue returnValue;
	if (tfs::lua::protectedCall(L, 7, 1) != 0) {
		returnValue = RETURNVALUE_NOTPOSSIBLE;
		tfs::lua::reportError(L, tfs::lua::popString(L));
	} else {
		returnValue = tfs::lua::getNumber<ReturnValue>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
	return returnValue;
}

void onItemMoved(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, uint16_t count,
                 const Position& fromPosition, const Position& toPosition, const std::shared_ptr<Thing>& fromThing,
                 const std::shared_ptr<Thing>& toThing)
{
	// Player:onItemMoved(item, count, fromPosition, toPosition) or Player.onItemMoved(self, item, count, fromPosition,
	// toPosition, fromThing, toThing)
	if (playerHandlers.onItemMoved == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onItemMoved] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onItemMoved, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onItemMoved);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushNumber(L, count);
	tfs::lua::pushPosition(L, fromPosition);
	tfs::lua::pushPosition(L, toPosition);

	tfs::lua::pushThing(L, fromThing);
	tfs::lua::pushThing(L, toThing);

	scriptInterface.callVoidFunction(7);
}

bool onMoveCreature(const std::shared_ptr<Player>& player, const std::shared_ptr<Creature>& creature,
                    const Position& fromPosition, const Position& toPosition)
{
	// Player:onMoveCreature(creature, fromPosition, toPosition) or Player.onMoveCreature(self, creature, fromPosition,
	// toPosition)
	if (playerHandlers.onMoveCreature == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onMoveCreature] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onMoveCreature, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onMoveCreature);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	tfs::lua::pushPosition(L, fromPosition);
	tfs::lua::pushPosition(L, toPosition);

	return scriptInterface.callFunction(4);
}

void onReportRuleViolation(const std::shared_ptr<Player>& player, const std::string& targetName, uint8_t reportType,
                           uint8_t reportReason, const std::string& comment, const std::string& translation)
{
	// Player:onReportRuleViolation(targetName, reportType, reportReason, comment, translation)
	if (playerHandlers.onReportRuleViolation == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onReportRuleViolation] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onReportRuleViolation, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onReportRuleViolation);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushString(L, targetName);

	tfs::lua::pushNumber(L, reportType);
	tfs::lua::pushNumber(L, reportReason);

	tfs::lua::pushString(L, comment);
	tfs::lua::pushString(L, translation);

	scriptInterface.callVoidFunction(6);
}

void onRotateItem(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item)
{
	// Player:onRotateItem(item)
	if (playerHandlers.onRotateItem == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onRotateItem] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onRotateItem, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onRotateItem);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	scriptInterface.callFunction(2);
}

bool onTurn(const std::shared_ptr<Player>& player, Direction direction)
{
	// Player:onTurn(direction) or Player.onTurn(self, direction)
	if (playerHandlers.onTurn == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onTurn] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onTurn, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onTurn);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushNumber(L, direction);

	return scriptInterface.callFunction(2);
}

bool onTradeRequest(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& target,
                    const std::shared_ptr<Item>& item)
{
	// Player:onTradeRequest(target, item)
	if (playerHandlers.onTradeRequest == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onTradeRequest] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onTradeRequest, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onTradeRequest);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, target);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	return scriptInterface.callFunction(3);
}

bool onTradeAccept(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& target,
                   const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& targetItem)
{
	// Player:onTradeAccept(target, item, targetItem)
	if (playerHandlers.onTradeAccept == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onTradeAccept] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onTradeAccept, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onTradeAccept);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, target);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushSharedPtr(L, targetItem);
	tfs::lua::setItemMetatable(L, -1, targetItem);

	return scriptInterface.callFunction(4);
}

void onTradeCompleted(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& target,
                      const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& targetItem, bool isSuccess)
{
	// Player:onTradeCompleted(target, item, targetItem, isSuccess)
	if (playerHandlers.onTradeCompleted == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onTradeCompleted] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onTradeCompleted, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onTradeCompleted);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, target);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushSharedPtr(L, targetItem);
	tfs::lua::setItemMetatable(L, -1, targetItem);

	tfs::lua::pushBoolean(L, isSuccess);

	scriptInterface.callVoidFunction(5);
}

void onPodiumRequest(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item)
{
	// Player:onPodiumRequest(item) or Player.onPodiumRequest(self, item)
	if (playerHandlers.onPodiumRequest == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onPodiumRequest] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onPodiumRequest, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onPodiumRequest);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	scriptInterface.callVoidFunction(2);
}

void onPodiumEdit(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, const Outfit_t& outfit,
                  bool podiumVisible, Direction direction)
{
	// Player:onPodiumEdit(item, outfit, direction, isVisible) or Player.onPodiumEdit(self, item, outfit, direction,
	// isVisible)
	if (playerHandlers.onPodiumEdit == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onPodiumEdit] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onPodiumEdit, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onPodiumEdit);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushOutfit(L, outfit);

	tfs::lua::pushNumber(L, direction);
	tfs::lua::pushBoolean(L, podiumVisible);

	scriptInterface.callVoidFunction(5);
}

void onGainExperience(const std::shared_ptr<Player>& player, const std::shared_ptr<Creature>& source, uint64_t& exp,
                      uint64_t rawExp, bool sendText)
{
	// Player:onGainExperience(source, exp, rawExp, sendText) rawExp gives the original exp which is not multiplied
	if (playerHandlers.onGainExperience == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onGainExperience] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onGainExperience, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onGainExperience);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	if (source) {
		tfs::lua::pushSharedPtr(L, source);
		tfs::lua::setCreatureMetatable(L, -1, source);
	} else {
		lua_pushnil(L);
	}

	tfs::lua::pushNumber(L, exp);
	tfs::lua::pushNumber(L, rawExp);
	tfs::lua::pushBoolean(L, sendText);

	if (tfs::lua::protectedCall(L, 5, 1) != 0) {
		tfs::lua::reportError(L, tfs::lua::popString(L));
	} else {
		exp = tfs::lua::getNumber<uint64_t>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
}

void onLoseExperience(const std::shared_ptr<Player>& player, uint64_t& exp)
{
	// Player:onLoseExperience(exp)
	if (playerHandlers.onLoseExperience == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLoseExperience] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onLoseExperience, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onLoseExperience);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushNumber(L, exp);

	if (tfs::lua::protectedCall(L, 2, 1) != 0) {
		tfs::lua::reportError(L, tfs::lua::popString(L));
	} else {
		exp = tfs::lua::getNumber<uint64_t>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
}

void onGainSkillTries(const std::shared_ptr<Player>& player, skills_t skill, uint64_t& tries)
{
	// Player:onGainSkillTries(skill, tries)
	if (playerHandlers.onGainSkillTries == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onGainSkillTries] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onGainSkillTries, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onGainSkillTries);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushNumber(L, skill);
	tfs::lua::pushNumber(L, tries);

	if (tfs::lua::protectedCall(L, 3, 1) != 0) {
		tfs::lua::reportError(L, tfs::lua::popString(L));
	} else {
		tries = tfs::lua::getNumber<uint64_t>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
}

void onWrapItem(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item)
{
	// Player:onWrapItem(item)
	if (playerHandlers.onWrapItem == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onWrapItem] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onWrapItem, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onWrapItem);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	scriptInterface.callVoidFunction(2);
}

void onInventoryUpdate(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, slots_t slot,
                       bool equip)
{
	// Player:onInventoryUpdate(item, slot, equip)
	if (playerHandlers.onInventoryUpdate == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onInventoryUpdate] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onInventoryUpdate, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onInventoryUpdate);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushNumber(L, slot);
	tfs::lua::pushBoolean(L, equip);

	scriptInterface.callVoidFunction(4);
}

void onNetworkMessage(const std::shared_ptr<Player>& player, uint8_t recvByte, NetworkMessage_ptr& msg)
{
	// Player:onNetworkMessage(recvByte, msg)
	if (playerHandlers.onNetworkMessage == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onNetworkMessage] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onNetworkMessage, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onNetworkMessage);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushNumber(L, recvByte);

	tfs::lua::pushUserdata(L, msg.release());
	tfs::lua::setMetatable(L, -1, "NetworkMessage");

	scriptInterface.callVoidFunction(3);
}

bool onSpellCheck(const std::shared_ptr<Player>& player, const Spell* spell)
{
	// Player:onSpellCheck(spell)
	if (playerHandlers.onSpellCheck == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onSpellCheck] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(playerHandlers.onSpellCheck, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerHandlers.onSpellCheck);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSpell(L, *spell);

	return scriptInterface.callFunction(2);
}

} // namespace tfs::events::player

namespace tfs::events::monster {

bool onSpawn(const std::shared_ptr<Monster>& monster, const Position& position, bool startup, bool artificial)
{
	// Monster:onSpawn(position, startup, artificial)
	if (monsterHandlers.onSpawn == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::monster:onSpawn] Call stack overflow" << std::endl;
		return false;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(monsterHandlers.onSpawn, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(monsterHandlers.onSpawn);

	tfs::lua::pushSharedPtr(L, monster);
	tfs::lua::setMetatable(L, -1, "Monster");
	tfs::lua::pushPosition(L, position);
	tfs::lua::pushBoolean(L, startup);
	tfs::lua::pushBoolean(L, artificial);

	return scriptInterface.callFunction(4);
}

void onDropLoot(const std::shared_ptr<Monster>& monster, const std::shared_ptr<Container>& corpse)
{
	// Monster:onDropLoot(corpse)
	if (monsterHandlers.onDropLoot == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::monsteronDropLoot] Call stack overflow" << std::endl;
		return;
	}

	const auto env = tfs::lua::getScriptEnv();
	env->setScriptId(monsterHandlers.onDropLoot, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(monsterHandlers.onDropLoot);

	tfs::lua::pushSharedPtr(L, monster);
	tfs::lua::setMetatable(L, -1, "Monster");

	tfs::lua::pushSharedPtr(L, corpse);
	tfs::lua::setMetatable(L, -1, "Container");

	return scriptInterface.callVoidFunction(2);
}

} // namespace tfs::events::monster
