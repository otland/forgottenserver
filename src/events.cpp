// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "events.h"

#include "item.h"
#include "player.h"

namespace {

LuaScriptInterface scriptInterface{"Event Interface"};

struct CreatureEvents
{
	int32_t onChangeOutfit = -1;
	int32_t onAreaCombat = -1;
	int32_t onTargetCombat = -1;
	int32_t onHear = -1;
	int32_t onChangeZone = -1;
	int32_t onUpdateStorage = -1;
};

struct PartyEvents
{
	int32_t onJoin = -1;
	int32_t onLeave = -1;
	int32_t onDisband = -1;
	int32_t onShareExperience = -1;
	int32_t onInvite = -1;
	int32_t onRevokeInvitation = -1;
	int32_t onPassLeadership = -1;
};

struct PlayerEvents
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
	int32_t onReportBug = -1;
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
};

struct MonsterEvents
{
	int32_t onDropLoot = -1;
	int32_t onSpawn = -1;
};

CreatureEvents creatureEvents;
PartyEvents partyEvents;
PlayerEvents playerEvents;
MonsterEvents monsterEvents;

bool load_from_xml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/events/events.xml");
	if (!result) {
		printXMLError("Error - tfs::events::load_from_xml", "data/events/events.xml", result);
		return false;
	}

	creatureEvents = {};
	partyEvents = {};
	playerEvents = {};
	monsterEvents = {};

	std::set<std::string> classes;
	for (auto eventNode : doc.child("events").children()) {
		if (!eventNode.attribute("enabled").as_bool()) {
			continue;
		}

		const std::string& className = eventNode.attribute("class").as_string();
		auto res = classes.insert(className);
		if (res.second) {
			const std::string& lowercase = boost::algorithm::to_lower_copy(className);
			if (scriptInterface.loadFile("data/events/scripts/" + lowercase + ".lua") != 0) {
				std::cout << "[Warning - tfs::events::load_from_xml] Can not load script: " << lowercase << ".lua"
				          << std::endl;
				std::cout << scriptInterface.getLastLuaError() << std::endl;
			}
		}

		const std::string& methodName = eventNode.attribute("method").as_string();
		const auto event = scriptInterface.getMetaEvent(className, methodName);
		if (className == "Creature") {
			if (methodName == "onChangeOutfit") {
				creatureEvents.onChangeOutfit = event;
			} else if (methodName == "onAreaCombat") {
				creatureEvents.onAreaCombat = event;
			} else if (methodName == "onTargetCombat") {
				creatureEvents.onTargetCombat = event;
			} else if (methodName == "onHear") {
				creatureEvents.onHear = event;
			} else if (methodName == "onChangeZone") {
				creatureEvents.onChangeZone = event;
			} else if (methodName == "onUpdateStorage") {
				creatureEvents.onUpdateStorage = event;
			} else {
				std::cout << "[Warning - tfs::events::load_from_xml] Unknown creature method: " << methodName
				          << std::endl;
			}
		} else if (className == "Party") {
			if (methodName == "onJoin") {
				partyEvents.onJoin = event;
			} else if (methodName == "onLeave") {
				partyEvents.onLeave = event;
			} else if (methodName == "onDisband") {
				partyEvents.onDisband = event;
			} else if (methodName == "onShareExperience") {
				partyEvents.onShareExperience = event;
			} else if (methodName == "onInvite") {
				partyEvents.onInvite = event;
			} else if (methodName == "onRevokeInvitation") {
				partyEvents.onRevokeInvitation = event;
			} else if (methodName == "onPassLeadership") {
				partyEvents.onPassLeadership = event;
			} else {
				std::cout << "[Warning - tfs::events::load_from_xml] Unknown party method: " << methodName << std::endl;
			}
		} else if (className == "Player") {
			if (methodName == "onBrowseField") {
				playerEvents.onBrowseField = event;
			} else if (methodName == "onLook") {
				playerEvents.onLook = event;
			} else if (methodName == "onLookInBattleList") {
				playerEvents.onLookInBattleList = event;
			} else if (methodName == "onLookInTrade") {
				playerEvents.onLookInTrade = event;
			} else if (methodName == "onLookInShop") {
				playerEvents.onLookInShop = event;
			} else if (methodName == "onLookInMarket") {
				playerEvents.onLookInMarket = event;
			} else if (methodName == "onTradeRequest") {
				playerEvents.onTradeRequest = event;
			} else if (methodName == "onTradeAccept") {
				playerEvents.onTradeAccept = event;
			} else if (methodName == "onTradeCompleted") {
				playerEvents.onTradeCompleted = event;
			} else if (methodName == "onPodiumRequest") {
				playerEvents.onPodiumRequest = event;
			} else if (methodName == "onPodiumEdit") {
				playerEvents.onPodiumEdit = event;
			} else if (methodName == "onMoveItem") {
				playerEvents.onMoveItem = event;
			} else if (methodName == "onItemMoved") {
				playerEvents.onItemMoved = event;
			} else if (methodName == "onMoveCreature") {
				playerEvents.onMoveCreature = event;
			} else if (methodName == "onReportRuleViolation") {
				playerEvents.onReportRuleViolation = event;
			} else if (methodName == "onReportBug") {
				playerEvents.onReportBug = event;
			} else if (methodName == "onRotateItem") {
				playerEvents.onRotateItem = event;
			} else if (methodName == "onTurn") {
				playerEvents.onTurn = event;
			} else if (methodName == "onGainExperience") {
				playerEvents.onGainExperience = event;
			} else if (methodName == "onLoseExperience") {
				playerEvents.onLoseExperience = event;
			} else if (methodName == "onGainSkillTries") {
				playerEvents.onGainSkillTries = event;
			} else if (methodName == "onWrapItem") {
				playerEvents.onWrapItem = event;
			} else if (methodName == "onInventoryUpdate") {
				playerEvents.onInventoryUpdate = event;
			} else if (methodName == "onNetworkMessage") {
				playerEvents.onNetworkMessage = event;
			} else if (methodName == "onSpellCheck") {
				playerEvents.onSpellCheck = event;
			} else {
				std::cout << "[Warning - tfs::events::load_from_xml] Unknown player method: " << methodName
				          << std::endl;
			}
		} else if (className == "Monster") {
			if (methodName == "onDropLoot") {
				monsterEvents.onDropLoot = event;
			} else if (methodName == "onSpawn") {
				monsterEvents.onSpawn = event;
			} else {
				std::cout << "[Warning - tfs::events::load_from_xml] Unknown monster method: " << methodName
				          << std::endl;
			}
		} else {
			std::cout << "[Warning - tfs::events::load_from_xml] Unknown class: " << className << std::endl;
		}
	}

	return true;
}

} // namespace

namespace tfs::events {

int32_t getScriptId(EventInfoId eventInfoId)
{
	switch (eventInfoId) {
		case EventInfoId::CREATURE_ONHEAR:
			return creatureEvents.onHear;
		case EventInfoId::MONSTER_ONSPAWN:
			return monsterEvents.onSpawn;
		default:
			return -1;
	}
}

bool load()
{
	scriptInterface.initState();
	return load_from_xml();
}

bool reload()
{
	scriptInterface.reInitState();
	return load_from_xml();
}

} // namespace tfs::events

namespace tfs::events::creature {

bool onChangeOutfit(Creature* creature, const Outfit_t& outfit)
{
	// Creature:onChangeOutfit(outfit) or Creature.onChangeOutfit(self, outfit)
	if (creatureEvents.onChangeOutfit == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onChangeOutfit] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureEvents.onChangeOutfit, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureEvents.onChangeOutfit);

	tfs::lua::pushUserdata(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	tfs::lua::pushOutfit(L, outfit);

	return scriptInterface.callFunction(2);
}

ReturnValue onAreaCombat(Creature* creature, Tile* tile, bool aggressive)
{
	// Creature:onAreaCombat(tile, aggressive) or Creature.onAreaCombat(self, tile, aggressive)
	if (creatureEvents.onAreaCombat == -1) {
		return RETURNVALUE_NOERROR;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onAreaCombat] Call stack overflow" << std::endl;
		return RETURNVALUE_NOTPOSSIBLE;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureEvents.onAreaCombat, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureEvents.onAreaCombat);

	if (creature) {
		tfs::lua::pushUserdata(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else {
		lua_pushnil(L);
	}

	tfs::lua::pushUserdata(L, tile);
	tfs::lua::setMetatable(L, -1, "Tile");

	tfs::lua::pushBoolean(L, aggressive);

	ReturnValue returnValue;
	if (tfs::lua::protectedCall(L, 3, 1) != 0) {
		returnValue = RETURNVALUE_NOTPOSSIBLE;
		reportErrorFunc(L, tfs::lua::popString(L));
	} else {
		returnValue = tfs::lua::getNumber<ReturnValue>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
	return returnValue;
}

ReturnValue onTargetCombat(Creature* creature, Creature* target)
{
	// Creature:onTargetCombat(target) or Creature.onTargetCombat(self, target)
	if (creatureEvents.onTargetCombat == -1) {
		return RETURNVALUE_NOERROR;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onTargetCombat] Call stack overflow" << std::endl;
		return RETURNVALUE_NOTPOSSIBLE;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureEvents.onTargetCombat, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureEvents.onTargetCombat);

	if (creature) {
		tfs::lua::pushUserdata(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else {
		lua_pushnil(L);
	}

	tfs::lua::pushUserdata(L, target);
	tfs::lua::setCreatureMetatable(L, -1, target);

	ReturnValue returnValue;
	if (tfs::lua::protectedCall(L, 2, 1) != 0) {
		returnValue = RETURNVALUE_NOTPOSSIBLE;
		reportErrorFunc(L, tfs::lua::popString(L));
	} else {
		returnValue = tfs::lua::getNumber<ReturnValue>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
	return returnValue;
}

void onHear(Creature* creature, Creature* speaker, const std::string& words, SpeakClasses type)
{
	// Creature:onHear(speaker, words, type)
	if (creatureEvents.onHear == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onHear] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureEvents.onHear, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureEvents.onHear);

	tfs::lua::pushUserdata(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	tfs::lua::pushUserdata(L, speaker);
	tfs::lua::setCreatureMetatable(L, -1, speaker);

	tfs::lua::pushString(L, words);
	lua_pushnumber(L, type);

	scriptInterface.callVoidFunction(4);
}

void onChangeZone(Creature* creature, ZoneType_t fromZone, ZoneType_t toZone)
{
	// Creature:onChangeZone(fromZone, toZone)
	if (creatureEvents.onChangeZone == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onChangeZone] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureEvents.onChangeZone, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureEvents.onChangeZone);

	tfs::lua::pushUserdata(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	lua_pushnumber(L, fromZone);
	lua_pushnumber(L, toZone);

	scriptInterface.callVoidFunction(3);
}

void onUpdateStorage(Creature* creature, uint32_t key, std::optional<int32_t> value, std::optional<int32_t> oldValue,
                     bool isSpawn)
{
	// Creature:onUpdateStorage(key, value, oldValue, isSpawn)
	if (creatureEvents.onUpdateStorage == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::creature::onUpdateStorage] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureEvents.onUpdateStorage, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(creatureEvents.onUpdateStorage);

	tfs::lua::pushUserdata(L, creature);
	tfs::lua::setMetatable(L, -1, "Creature");

	lua_pushnumber(L, key);

	if (value) {
		lua_pushnumber(L, value.value());
	} else {
		lua_pushnil(L);
	}

	if (oldValue) {
		lua_pushnumber(L, oldValue.value());
	} else {
		lua_pushnil(L);
	}

	tfs::lua::pushBoolean(L, isSpawn);

	scriptInterface.callVoidFunction(5);
}

} // namespace tfs::events::creature

namespace tfs::events::party {

bool onJoin(Party* party, Player* player)
{
	// Party:onJoin(player) or Party.onJoin(self, player)
	if (partyEvents.onJoin == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onJoin] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(partyEvents.onJoin, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyEvents.onJoin);

	tfs::lua::pushUserdata(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

bool onLeave(Party* party, Player* player)
{
	// Party:onLeave(player) or Party.onLeave(self, player)
	if (partyEvents.onLeave == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onLeave] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(partyEvents.onLeave, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyEvents.onLeave);

	tfs::lua::pushUserdata(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

bool onDisband(Party* party)
{
	// Party:onDisband() or Party.onDisband(self)
	if (partyEvents.onDisband == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onDisband] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(partyEvents.onDisband, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyEvents.onDisband);

	tfs::lua::pushUserdata(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	return scriptInterface.callFunction(1);
}

bool onInvite(Party* party, Player* player)
{
	// Party:onInvite(player) or Party.onInvite(self, player)
	if (partyEvents.onInvite == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onInvite] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(partyEvents.onInvite, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyEvents.onInvite);

	tfs::lua::pushUserdata(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

bool onRevokeInvitation(Party* party, Player* player)
{
	// Party:onRevokeInvitation(player) or Party.onRevokeInvitation(self, player)
	if (partyEvents.onRevokeInvitation == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onRevokeInvitation] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(partyEvents.onRevokeInvitation, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyEvents.onRevokeInvitation);

	tfs::lua::pushUserdata(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

bool onPassLeadership(Party* party, Player* player)
{
	// Party:onPassLeadership(player) or Party.onPassLeadership(self, player)
	if (partyEvents.onPassLeadership == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onPassLeadership] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(partyEvents.onPassLeadership, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyEvents.onPassLeadership);

	tfs::lua::pushUserdata(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface.callFunction(2);
}

void onShareExperience(Party* party, uint64_t& exp)
{
	// Party:onShareExperience(exp) or Party.onShareExperience(self, exp)
	if (partyEvents.onShareExperience == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::party::onShareExperience] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(partyEvents.onShareExperience, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(partyEvents.onShareExperience);

	tfs::lua::pushUserdata(L, party);
	tfs::lua::setMetatable(L, -1, "Party");

	lua_pushnumber(L, exp);

	if (tfs::lua::protectedCall(L, 2, 1) != 0) {
		reportErrorFunc(L, tfs::lua::popString(L));
	} else {
		exp = tfs::lua::getNumber<uint64_t>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
}

} // namespace tfs::events::party

namespace tfs::events::player {

bool onBrowseField(Player* player, const Position& position)
{
	// Player:onBrowseField(position) or Player.onBrowseField(self, position)
	if (playerEvents.onBrowseField == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onBrowseField] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onBrowseField, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onBrowseField);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushPosition(L, position);

	return scriptInterface.callFunction(2);
}

void onLook(Player* player, const Position& position, Thing* thing, uint8_t stackpos, int32_t lookDistance)
{
	// Player:onLook(thing, position, distance) or Player.onLook(self, thing, position, distance)
	if (playerEvents.onLook == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLook] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onLook, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onLook);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	if (Creature* creature = thing->getCreature()) {
		tfs::lua::pushUserdata(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else if (Item* item = thing->getItem()) {
		tfs::lua::pushUserdata(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}

	tfs::lua::pushPosition(L, position, stackpos);
	lua_pushnumber(L, lookDistance);

	scriptInterface.callVoidFunction(4);
}

void onLookInBattleList(Player* player, Creature* creature, int32_t lookDistance)
{
	// Player:onLookInBattleList(creature, position, distance) or Player.onLookInBattleList(self, creature, position,
	// distance)
	if (playerEvents.onLookInBattleList == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLookInBattleList] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onLookInBattleList, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onLookInBattleList);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	lua_pushnumber(L, lookDistance);

	scriptInterface.callVoidFunction(3);
}

void onLookInTrade(Player* player, Player* partner, Item* item, int32_t lookDistance)
{
	// Player:onLookInTrade(partner, item, distance) or Player.onLookInTrade(self, partner, item, distance)
	if (playerEvents.onLookInTrade == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLookInTrade] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onLookInTrade, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onLookInTrade);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, partner);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	lua_pushnumber(L, lookDistance);

	scriptInterface.callVoidFunction(4);
}

bool onLookInShop(Player* player, const ItemType* itemType, uint8_t count)
{
	// Player:onLookInShop(itemType, count) or Player.onLookInShop(self, itemType, count)
	if (playerEvents.onLookInShop == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLookInShop] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onLookInShop, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onLookInShop);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, itemType);
	tfs::lua::setMetatable(L, -1, "ItemType");

	lua_pushnumber(L, count);

	return scriptInterface.callFunction(3);
}

bool onLookInMarket(Player* player, const ItemType* itemType)
{
	// Player:onLookInMarket(itemType) or Player.onLookInMarket(self, itemType)
	if (playerEvents.onLookInMarket == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLookInMarket] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onLookInMarket, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onLookInMarket);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, itemType);
	tfs::lua::setMetatable(L, -1, "ItemType");

	return scriptInterface.callFunction(2);
}

ReturnValue onMoveItem(Player* player, Item* item, uint16_t count, const Position& fromPosition,
                       const Position& toPosition, Cylinder* fromCylinder, Cylinder* toCylinder)
{
	// Player:onMoveItem(item, count, fromPosition, toPosition) or Player.onMoveItem(self, item, count, fromPosition,
	// toPosition, fromCylinder, toCylinder)
	if (playerEvents.onMoveItem == -1) {
		return RETURNVALUE_NOERROR;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onMoveItem] Call stack overflow" << std::endl;
		return RETURNVALUE_NOTPOSSIBLE;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onMoveItem, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onMoveItem);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	lua_pushnumber(L, count);
	tfs::lua::pushPosition(L, fromPosition);
	tfs::lua::pushPosition(L, toPosition);

	tfs::lua::pushCylinder(L, fromCylinder);
	tfs::lua::pushCylinder(L, toCylinder);

	ReturnValue returnValue;
	if (tfs::lua::protectedCall(L, 7, 1) != 0) {
		returnValue = RETURNVALUE_NOTPOSSIBLE;
		reportErrorFunc(L, tfs::lua::popString(L));
	} else {
		returnValue = tfs::lua::getNumber<ReturnValue>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
	return returnValue;
}

void onItemMoved(Player* player, Item* item, uint16_t count, const Position& fromPosition, const Position& toPosition,
                 Cylinder* fromCylinder, Cylinder* toCylinder)
{
	// Player:onItemMoved(item, count, fromPosition, toPosition) or Player.onItemMoved(self, item, count, fromPosition,
	// toPosition, fromCylinder, toCylinder)
	if (playerEvents.onItemMoved == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onItemMoved] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onItemMoved, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onItemMoved);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	lua_pushnumber(L, count);
	tfs::lua::pushPosition(L, fromPosition);
	tfs::lua::pushPosition(L, toPosition);

	tfs::lua::pushCylinder(L, fromCylinder);
	tfs::lua::pushCylinder(L, toCylinder);

	scriptInterface.callVoidFunction(7);
}

bool onMoveCreature(Player* player, Creature* creature, const Position& fromPosition, const Position& toPosition)
{
	// Player:onMoveCreature(creature, fromPosition, toPosition) or Player.onMoveCreature(self, creature, fromPosition,
	// toPosition)
	if (playerEvents.onMoveCreature == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onMoveCreature] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onMoveCreature, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onMoveCreature);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);

	tfs::lua::pushPosition(L, fromPosition);
	tfs::lua::pushPosition(L, toPosition);

	return scriptInterface.callFunction(4);
}

void onReportRuleViolation(Player* player, const std::string& targetName, uint8_t reportType, uint8_t reportReason,
                           const std::string& comment, const std::string& translation)
{
	// Player:onReportRuleViolation(targetName, reportType, reportReason, comment, translation)
	if (playerEvents.onReportRuleViolation == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onReportRuleViolation] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onReportRuleViolation, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onReportRuleViolation);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushString(L, targetName);

	lua_pushnumber(L, reportType);
	lua_pushnumber(L, reportReason);

	tfs::lua::pushString(L, comment);
	tfs::lua::pushString(L, translation);

	scriptInterface.callVoidFunction(6);
}

bool onReportBug(Player* player, const std::string& message, const Position& position, uint8_t category)
{
	// Player:onReportBug(message, position, category)
	if (playerEvents.onReportBug == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onReportBug] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onReportBug, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onReportBug);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushString(L, message);
	tfs::lua::pushPosition(L, position);
	lua_pushnumber(L, category);

	return scriptInterface.callFunction(4);
}

void onRotateItem(Player* player, Item* item)
{
	// Player:onRotateItem(item)
	if (playerEvents.onRotateItem == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onRotateItem] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onRotateItem, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onRotateItem);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	scriptInterface.callFunction(2);
}

bool onTurn(Player* player, Direction direction)
{
	// Player:onTurn(direction) or Player.onTurn(self, direction)
	if (playerEvents.onTurn == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onTurn] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onTurn, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onTurn);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	lua_pushnumber(L, direction);

	return scriptInterface.callFunction(2);
}

bool onTradeRequest(Player* player, Player* target, Item* item)
{
	// Player:onTradeRequest(target, item)
	if (playerEvents.onTradeRequest == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onTradeRequest] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onTradeRequest, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onTradeRequest);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, target);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	return scriptInterface.callFunction(3);
}

bool onTradeAccept(Player* player, Player* target, Item* item, Item* targetItem)
{
	// Player:onTradeAccept(target, item, targetItem)
	if (playerEvents.onTradeAccept == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onTradeAccept] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onTradeAccept, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onTradeAccept);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, target);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushUserdata(L, targetItem);
	tfs::lua::setItemMetatable(L, -1, targetItem);

	return scriptInterface.callFunction(4);
}

void onTradeCompleted(Player* player, Player* target, Item* item, Item* targetItem, bool isSuccess)
{
	// Player:onTradeCompleted(target, item, targetItem, isSuccess)
	if (playerEvents.onTradeCompleted == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onTradeCompleted] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onTradeCompleted, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onTradeCompleted);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, target);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushUserdata(L, targetItem);
	tfs::lua::setItemMetatable(L, -1, targetItem);

	tfs::lua::pushBoolean(L, isSuccess);

	return scriptInterface.callVoidFunction(5);
}

void onPodiumRequest(Player* player, Item* item)
{
	// Player:onPodiumRequest(item) or Player.onPodiumRequest(self, item)
	if (playerEvents.onPodiumRequest == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onPodiumRequest] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onPodiumRequest, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onPodiumRequest);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	scriptInterface.callFunction(2);
}

void onPodiumEdit(Player* player, Item* item, const Outfit_t& outfit, bool podiumVisible, Direction direction)
{
	// Player:onPodiumEdit(item, outfit, direction, isVisible) or Player.onPodiumEdit(self, item, outfit, direction,
	// isVisible)
	if (playerEvents.onPodiumEdit == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onPodiumEdit] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onPodiumEdit, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onPodiumEdit);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	tfs::lua::pushOutfit(L, outfit);

	lua_pushnumber(L, direction);
	lua_pushboolean(L, podiumVisible);

	scriptInterface.callFunction(5);
}

void onGainExperience(Player* player, Creature* source, uint64_t& exp, uint64_t rawExp, bool sendText)
{
	// Player:onGainExperience(source, exp, rawExp, sendText) rawExp gives the original exp which is not multiplied
	if (playerEvents.onGainExperience == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onGainExperience] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onGainExperience, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onGainExperience);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	if (source) {
		tfs::lua::pushUserdata(L, source);
		tfs::lua::setCreatureMetatable(L, -1, source);
	} else {
		lua_pushnil(L);
	}

	lua_pushnumber(L, exp);
	lua_pushnumber(L, rawExp);
	tfs::lua::pushBoolean(L, sendText);

	if (tfs::lua::protectedCall(L, 5, 1) != 0) {
		reportErrorFunc(L, tfs::lua::popString(L));
	} else {
		exp = tfs::lua::getNumber<uint64_t>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
}

void onLoseExperience(Player* player, uint64_t& exp)
{
	// Player:onLoseExperience(exp)
	if (playerEvents.onLoseExperience == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onLoseExperience] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onLoseExperience, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onLoseExperience);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	lua_pushnumber(L, exp);

	if (tfs::lua::protectedCall(L, 2, 1) != 0) {
		reportErrorFunc(L, tfs::lua::popString(L));
	} else {
		exp = tfs::lua::getNumber<uint64_t>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
}

void onGainSkillTries(Player* player, skills_t skill, uint64_t& tries)
{
	// Player:onGainSkillTries(skill, tries)
	if (playerEvents.onGainSkillTries == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onGainSkillTries] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onGainSkillTries, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onGainSkillTries);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	lua_pushnumber(L, skill);
	lua_pushnumber(L, tries);

	if (tfs::lua::protectedCall(L, 3, 1) != 0) {
		reportErrorFunc(L, tfs::lua::popString(L));
	} else {
		tries = tfs::lua::getNumber<uint64_t>(L, -1);
		lua_pop(L, 1);
	}

	tfs::lua::resetScriptEnv();
}

void onWrapItem(Player* player, Item* item)
{
	// Player:onWrapItem(item)
	if (playerEvents.onWrapItem == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onWrapItem] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onWrapItem, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onWrapItem);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	scriptInterface.callVoidFunction(2);
}

void onInventoryUpdate(Player* player, Item* item, slots_t slot, bool equip)
{
	// Player:onInventoryUpdate(item, slot, equip)
	if (playerEvents.onInventoryUpdate == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onInventoryUpdate] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onInventoryUpdate, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onInventoryUpdate);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushUserdata(L, item);
	tfs::lua::setItemMetatable(L, -1, item);

	lua_pushnumber(L, slot);
	tfs::lua::pushBoolean(L, equip);

	scriptInterface.callVoidFunction(4);
}

void onNetworkMessage(Player* player, uint8_t recvByte, NetworkMessage* msg)
{
	// Player:onNetworkMessage(recvByte, msg)
	if (playerEvents.onNetworkMessage == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onNetworkMessage] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onNetworkMessage, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onNetworkMessage);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	lua_pushnumber(L, recvByte);

	tfs::lua::pushUserdata(L, msg);
	tfs::lua::setMetatable(L, -1, "NetworkMessage");

	scriptInterface.callVoidFunction(3);
}

bool onSpellCheck(Player* player, const Spell* spell)
{
	// Player:onSpellCheck(spell)
	if (playerEvents.onSpellCheck == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::player::onSpellCheck] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEvents.onSpellCheck, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(playerEvents.onSpellCheck);

	tfs::lua::pushUserdata(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushSpell(L, *spell);

	return scriptInterface.callFunction(2);
}

} // namespace tfs::events::player

namespace tfs::events::monster {

bool onSpawn(Monster* monster, const Position& position, bool startup, bool artificial)
{
	// Monster:onSpawn(position, startup, artificial)
	if (monsterEvents.onSpawn == -1) {
		return true;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::monster:onSpawn] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(monsterEvents.onSpawn, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(monsterEvents.onSpawn);

	tfs::lua::pushUserdata(L, monster);
	tfs::lua::setMetatable(L, -1, "Monster");
	tfs::lua::pushPosition(L, position);
	tfs::lua::pushBoolean(L, startup);
	tfs::lua::pushBoolean(L, artificial);

	return scriptInterface.callFunction(4);
}

void onDropLoot(Monster* monster, Container* corpse)
{
	// Monster:onDropLoot(corpse)
	if (monsterEvents.onDropLoot == -1) {
		return;
	}

	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - tfs::events::monsteronDropLoot] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(monsterEvents.onDropLoot, &scriptInterface);

	lua_State* L = scriptInterface.getLuaState();
	scriptInterface.pushFunction(monsterEvents.onDropLoot);

	tfs::lua::pushUserdata(L, monster);
	tfs::lua::setMetatable(L, -1, "Monster");

	tfs::lua::pushUserdata(L, corpse);
	tfs::lua::setMetatable(L, -1, "Container");

	return scriptInterface.callVoidFunction(2);
}

} // namespace tfs::events::monster
