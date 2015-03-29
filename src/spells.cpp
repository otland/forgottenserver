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

#include "otpch.h"

#include "combat.h"
#include "configmanager.h"
#include "const.h"
#include "game.h"
#include "house.h"
#include "housetile.h"
#include "monster.h"
#include "monsters.h"
#include "pugicast.h"
#include "spells.h"
#include "tools.h"

extern Game g_game;
extern Spells* g_spells;
extern Monsters g_monsters;
extern Vocations g_vocations;
extern ConfigManager g_config;
extern LuaEnvironment g_luaEnvironment;

Spells::Spells():
	m_scriptInterface("Spell Interface")
{
	m_scriptInterface.initState();
}

Spells::~Spells()
{
	clear();
}

TalkActionResult_t Spells::playerSaySpell(Player* player, std::string& words)
{
	std::string str_words = words;

	//strip trailing spaces
	trimString(str_words);

	InstantSpell* instantSpell = getInstantSpell(str_words);
	if (!instantSpell) {
		return TALKACTION_CONTINUE;
	}

	std::string param;

	if (instantSpell->getHasParam()) {
		size_t spellLen = instantSpell->getWords().length();
		size_t paramLen = str_words.length() - spellLen;
		std::string paramText = str_words.substr(spellLen, paramLen);
		if (!paramText.empty() && paramText.front() == ' ') {
			size_t loc1 = paramText.find('"', 1);
			if (loc1 != std::string::npos) {
				size_t loc2 = paramText.find('"', loc1 + 1);
				if (loc2 == std::string::npos) {
					loc2 = paramText.length();
				} else if (paramText.find_last_not_of(' ') != loc2) {
					return TALKACTION_CONTINUE;
				}

				param = paramText.substr(loc1 + 1, loc2 - loc1 - 1);
			} else {
				trimString(paramText);
				loc1 = paramText.find(' ', 0);
				if (loc1 == std::string::npos) {
					param = paramText;
				} else {
					return TALKACTION_CONTINUE;
				}
			}
		}
	}

	if (instantSpell->playerCastInstant(player, param)) {
		words = instantSpell->getWords();

		if (instantSpell->getHasParam() && !param.empty()) {
			words += " \"" + param + "\"";
		}

		return TALKACTION_BREAK;
	}

	return TALKACTION_FAILED;
}

void Spells::clear()
{
	for (const auto& it : runes) {
		delete it.second;
	}
	runes.clear();

	for (const auto& it : instants) {
		delete it.second;
	}
	instants.clear();

	m_scriptInterface.reInitState();
}

LuaScriptInterface& Spells::getScriptInterface()
{
	return m_scriptInterface;
}

std::string Spells::getScriptBaseName() const
{
	return "spells";
}

Event* Spells::getEvent(const std::string& nodeName)
{
	if (strcasecmp(nodeName.c_str(), "rune") == 0) {
		return new RuneSpell(&m_scriptInterface);
	} else if (strcasecmp(nodeName.c_str(), "instant") == 0) {
		return new InstantSpell(&m_scriptInterface);
	} else if (strcasecmp(nodeName.c_str(), "conjure") == 0) {
		return new ConjureSpell(&m_scriptInterface);
	}
	return nullptr;
}

bool Spells::registerEvent(Event* event, const pugi::xml_node&)
{
	InstantSpell* instant = dynamic_cast<InstantSpell*>(event);
	if (instant) {
		if (instants.find(instant->getWords()) != instants.end()) {
			std::cout << "[Warning - Spells::registerEvent] Duplicate registered instant spell with words: " << instant->getWords() << std::endl;
			return false;
		}

		instants[instant->getWords()] = instant;
		return true;
	}

	RuneSpell* rune = dynamic_cast<RuneSpell*>(event);
	if (rune) {
		uint16_t runeId = rune->getRuneItemId();
		if (runes.find(runeId) != runes.end()) {
			std::cout << "[Warning - Spells::registerEvent] Duplicate registered rune with id: " << runeId << std::endl;
			return false;
		}

		runes[runeId] = rune;
		return true;
	}
	return false;
}

Spell* Spells::getSpellByName(const std::string& name)
{
	Spell* spell = getRuneSpellByName(name);
	if (!spell) {
		spell = getInstantSpellByName(name);
	}
	return spell;
}

RuneSpell* Spells::getRuneSpell(uint32_t id)
{
	auto it = runes.find(id);
	if (it == runes.end()) {
		return nullptr;
	}
	return it->second;
}

RuneSpell* Spells::getRuneSpellByName(const std::string& name)
{
	for (const auto& it : runes) {
		if (strcasecmp(it.second->getName().c_str(), name.c_str()) == 0) {
			return it.second;
		}
	}
	return nullptr;
}

InstantSpell* Spells::getInstantSpell(const std::string& words)
{
	InstantSpell* result = nullptr;

	for (const auto& it : instants) {
		InstantSpell* instantSpell = it.second;

		const std::string& instantSpellWords = instantSpell->getWords();
		size_t spellLen = instantSpellWords.length();
		if (strncasecmp(instantSpellWords.c_str(), words.c_str(), spellLen) == 0) {
			if (!result || spellLen > result->getWords().length()) {
				result = instantSpell;
			}
		}
	}

	if (result) {
		const std::string& resultWords = result->getWords();
		if (words.length() > resultWords.length()) {
			if (!result->getHasParam()) {
				return nullptr;
			}

			size_t spellLen = resultWords.length();
			size_t paramLen = words.length() - spellLen;
			if (paramLen < 2 || words[spellLen] != ' ') {
				return nullptr;
			}
		}
		return result;
	}
	return nullptr;
}

uint32_t Spells::getInstantSpellCount(const Player* player) const
{
	uint32_t count = 0;
	for (const auto& it : instants) {
		InstantSpell* instantSpell = it.second;
		if (instantSpell->canCast(player)) {
			++count;
		}
	}
	return count;
}

InstantSpell* Spells::getInstantSpellByIndex(const Player* player, uint32_t index)
{
	uint32_t count = 0;
	for (const auto& it : instants) {
		InstantSpell* instantSpell = it.second;
		if (instantSpell->canCast(player)) {
			if (count == index) {
				return instantSpell;
			}
			++count;
		}
	}
	return nullptr;
}

InstantSpell* Spells::getInstantSpellByName(const std::string& name)
{
	for (const auto& it : instants) {
		if (strcasecmp(it.second->getName().c_str(), name.c_str()) == 0) {
			return it.second;
		}
	}
	return nullptr;
}

Position Spells::getCasterPosition(Creature* creature, Direction dir)
{
	return getNextPosition(dir, creature->getPosition());
}

CombatSpell::CombatSpell(Combat* _combat, bool _needTarget, bool _needDirection) :
	Event(&g_spells->getScriptInterface())
{
	combat = _combat;
	needTarget = _needTarget;
	needDirection = _needDirection;
}

CombatSpell::~CombatSpell()
{
	if (!m_scripted) {
		delete combat;
	}
}

bool CombatSpell::loadScriptCombat()
{
	combat = g_luaEnvironment.getCombatObject(g_luaEnvironment.m_lastCombatId);
	return combat != nullptr;
}

bool CombatSpell::castSpell(Creature* creature)
{
	if (m_scripted) {
		LuaVariant var;
		var.type = VARIANT_POSITION;

		if (needDirection) {
			var.pos = Spells::getCasterPosition(creature, creature->getDirection());
		} else {
			var.pos = creature->getPosition();
		}

		return executeCastSpell(creature, var);
	}

	Position pos;
	if (needDirection) {
		pos = Spells::getCasterPosition(creature, creature->getDirection());
	} else {
		pos = creature->getPosition();
	}

	combat->doCombat(creature, pos);
	return true;
}

bool CombatSpell::castSpell(Creature* creature, Creature* target)
{
	if (m_scripted) {
		LuaVariant var;

		if (combat->hasArea()) {
			var.type = VARIANT_POSITION;

			if (needTarget) {
				var.pos = target->getPosition();
			} else if (needDirection) {
				var.pos = Spells::getCasterPosition(creature, creature->getDirection());
			} else {
				var.pos = creature->getPosition();
			}
		} else {
			var.type = VARIANT_NUMBER;
			var.number = target->getID();
		}
		return executeCastSpell(creature, var);
	}

	if (combat->hasArea()) {
		if (needTarget) {
			combat->doCombat(creature, target->getPosition());
		} else {
			return castSpell(creature);
		}
	} else {
		combat->doCombat(creature, target);
	}
	return true;
}

bool CombatSpell::executeCastSpell(Creature* creature, const LuaVariant& var)
{
	//onCastSpell(creature, var)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CombatSpell::executeCastSpell] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);

	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);

	LuaScriptInterface::pushVariant(L, var);

	return m_scriptInterface->callFunction(2);
}

Spell::Spell()
{
	spellId = 0;
	level = 0;
	magLevel = 0;
	mana = 0;
	manaPercent = 0;
	soul = 0;
	range = -1;
	cooldown = 1000;
	needTarget = false;
	needWeapon = false;
	selfTarget = false;
	blockingSolid = false;
	blockingCreature = false;
	premium = false;
	enabled = true;
	aggressive = true;
	learnable = false;
	group = SPELLGROUP_NONE;
	groupCooldown = 1000;
	secondaryGroup = SPELLGROUP_NONE;
	secondaryGroupCooldown = 0;
}

bool Spell::configureSpell(const pugi::xml_node& node)
{
	pugi::xml_attribute nameAttribute = node.attribute("name");
	if (!nameAttribute) {
		std::cout << "[Error - Spell::configureSpell] Spell without name" << std::endl;
		return false;
	}

	name = nameAttribute.as_string();

	static const char* reservedList[] = {
		"melee",
		"physical",
		"poison",
		"fire",
		"energy",
		"drown",
		"lifedrain",
		"manadrain",
		"healing",
		"speed",
		"outfit",
		"invisible",
		"drunk",
		"firefield",
		"poisonfield",
		"energyfield",
		"firecondition",
		"poisoncondition",
		"energycondition",
		"drowncondition",
		"freezecondition",
		"cursecondition",
		"dazzlecondition"
	};

	//static size_t size = sizeof(reservedList) / sizeof(const char*);
	//for (size_t i = 0; i < size; ++i) {
	for (const char* reserved : reservedList) {
		if (strcasecmp(reserved, name.c_str()) == 0) {
			std::cout << "[Error - Spell::configureSpell] Spell is using a reserved name: " << reserved << std::endl;
			return false;
		}
	}

	pugi::xml_attribute attr;
	if ((attr = node.attribute("spellid"))) {
		spellId = pugi::cast<uint16_t>(attr.value());
	}

	if ((attr = node.attribute("group"))) {
		std::string tmpStr = asLowerCaseString(attr.as_string());
		if (tmpStr == "none" || tmpStr == "0") {
			group = SPELLGROUP_NONE;
		} else if (tmpStr == "attack" || tmpStr == "1") {
			group = SPELLGROUP_ATTACK;
		} else if (tmpStr == "healing" || tmpStr == "2") {
			group = SPELLGROUP_HEALING;
		} else if (tmpStr == "support" || tmpStr == "3") {
			group = SPELLGROUP_SUPPORT;
		} else if (tmpStr == "special" || tmpStr == "4") {
			group = SPELLGROUP_SPECIAL;
		} else {
			std::cout << "[Warning - Spell::configureSpell] Unknown group: " << attr.as_string() << std::endl;
		}
	}

	if ((attr = node.attribute("groupcooldown"))) {
		groupCooldown = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("secondarygroup"))) {
		std::string tmpStr = asLowerCaseString(attr.as_string());
		if (tmpStr == "none" || tmpStr == "0") {
			secondaryGroup = SPELLGROUP_NONE;
		} else if (tmpStr == "attack" || tmpStr == "1") {
			secondaryGroup = SPELLGROUP_ATTACK;
		} else if (tmpStr == "healing" || tmpStr == "2") {
			secondaryGroup = SPELLGROUP_HEALING;
		} else if (tmpStr == "support" || tmpStr == "3") {
			secondaryGroup = SPELLGROUP_SUPPORT;
		} else if (tmpStr == "special" || tmpStr == "4") {
			secondaryGroup = SPELLGROUP_SPECIAL;
		} else {
			std::cout << "[Warning - Spell::configureSpell] Unknown secondarygroup: " << attr.as_string() << std::endl;
		}
	}

	if ((attr = node.attribute("secondarygroupcooldown"))) {
		secondaryGroupCooldown = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("lvl"))) {
		level = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("maglv"))) {
		magLevel = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("mana"))) {
		mana = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("manapercent"))) {
		manaPercent = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("soul"))) {
		soul = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("range"))) {
		range = pugi::cast<int32_t>(attr.value());
	}

	if ((attr = node.attribute("exhaustion")) || (attr = node.attribute("cooldown"))) {
		cooldown = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("prem"))) {
		premium = attr.as_bool();
	}

	if ((attr = node.attribute("enabled"))) {
		enabled = attr.as_bool();
	}

	if ((attr = node.attribute("needtarget"))) {
		needTarget = attr.as_bool();
	}

	if ((attr = node.attribute("needweapon"))) {
		needWeapon = attr.as_bool();
	}

	if ((attr = node.attribute("selftarget"))) {
		selfTarget = attr.as_bool();
	}

	if ((attr = node.attribute("needlearn"))) {
		learnable = attr.as_bool();
	}

	if ((attr = node.attribute("blocking"))) {
		blockingSolid = attr.as_bool();
		blockingCreature = blockingSolid;
	}

	if ((attr = node.attribute("blocktype"))) {
		std::string tmpStrValue = asLowerCaseString(attr.as_string());
		if (tmpStrValue == "all") {
			blockingSolid = true;
			blockingCreature = true;
		} else if (tmpStrValue == "solid") {
			blockingSolid = true;
		} else if (tmpStrValue == "creature") {
			blockingCreature = true;
		} else {
			std::cout << "[Warning - Spell::configureSpell] Blocktype \"" << attr.as_string() << "\" does not exist." << std::endl;
		}
	}

	if ((attr = node.attribute("aggressive"))) {
		aggressive = booleanString(attr.as_string());
	}

	if (group == SPELLGROUP_NONE) {
		group = (aggressive ? SPELLGROUP_ATTACK : SPELLGROUP_HEALING);
	}

	for (pugi::xml_node vocationNode = node.first_child(); vocationNode; vocationNode = vocationNode.next_sibling()) {
		if (!(attr = vocationNode.attribute("name"))) {
			continue;
		}

		int32_t vocationId = g_vocations.getVocationId(attr.as_string());
		if (vocationId != -1) {
			vocSpellMap[vocationId] = true;
			int32_t promotedVocation = g_vocations.getPromotedVocation(vocationId);
			if (promotedVocation != 0) {
				vocSpellMap[promotedVocation] = true;
			}
		} else {
			std::cout << "[Warning - Spell::configureSpell] Wrong vocation name: " << attr.as_string() << std::endl;
		}
	}
	return true;
}

bool Spell::playerSpellCheck(Player* player) const
{
	if (player->hasFlag(PlayerFlag_CannotUseSpells)) {
		return false;
	}

	if (player->hasFlag(PlayerFlag_IgnoreSpellCheck)) {
		return true;
	}

	if (!enabled) {
		return false;
	}

	if (aggressive && !player->hasFlag(PlayerFlag_IgnoreProtectionZone) && player->getZone() == ZONE_PROTECTION) {
		player->sendCancelMessage(RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE);
		return false;
	}

	if (player->hasCondition(CONDITION_SPELLGROUPCOOLDOWN, group) || player->hasCondition(CONDITION_SPELLCOOLDOWN, spellId) || (secondaryGroup != SPELLGROUP_NONE && player->hasCondition(CONDITION_SPELLGROUPCOOLDOWN, secondaryGroup))) {
		player->sendCancelMessage(RETURNVALUE_YOUAREEXHAUSTED);

		if (isInstant()) {
			g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		}

		return false;
	}

	if (player->getLevel() < level) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHLEVEL);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (player->getMagicLevel() < magLevel) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHMAGICLEVEL);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (player->getMana() < getManaCost(player) && !player->hasFlag(PlayerFlag_HasInfiniteMana)) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHMANA);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (player->getSoul() < soul && !player->hasFlag(PlayerFlag_HasInfiniteSoul)) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHSOUL);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (isInstant() && isLearnable()) {
		if (!player->hasLearnedInstantSpell(getName())) {
			player->sendCancelMessage(RETURNVALUE_YOUNEEDTOLEARNTHISSPELL);
			g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
			return false;
		}
	} else if (!vocSpellMap.empty() && vocSpellMap.find(player->getVocationId()) == vocSpellMap.end()) {
		player->sendCancelMessage(RETURNVALUE_YOURVOCATIONCANNOTUSETHISSPELL);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (needWeapon) {
		switch (player->getWeaponType()) {
			case WEAPON_SWORD:
			case WEAPON_CLUB:
			case WEAPON_AXE:
				break;

			default: {
				player->sendCancelMessage(RETURNVALUE_YOUNEEDAWEAPONTOUSETHISSPELL);
				g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
				return false;
			}
		}
	}

	if (isPremium() && !player->isPremium()) {
		player->sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	return true;
}

bool Spell::playerInstantSpellCheck(Player* player, const Position& toPos)
{
	if (toPos.x == 0xFFFF) {
		return true;
	}

	const Position& playerPos = player->getPosition();
	if (playerPos.z > toPos.z) {
		player->sendCancelMessage(RETURNVALUE_FIRSTGOUPSTAIRS);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	} else if (playerPos.z < toPos.z) {
		player->sendCancelMessage(RETURNVALUE_FIRSTGODOWNSTAIRS);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	Tile* tile = g_game.map.getTile(toPos);
	if (!tile) {
		tile = new StaticTile(toPos.x, toPos.y, toPos.z);
		g_game.map.setTile(toPos, tile);
	}

	ReturnValue ret = Combat::canDoCombat(player, tile, aggressive);
	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (blockingCreature && tile->getBottomVisibleCreature(player) != nullptr) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (blockingSolid && tile->hasProperty(CONST_PROP_BLOCKSOLID)) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	return true;
}

bool Spell::playerRuneSpellCheck(Player* player, const Position& toPos)
{
	if (!playerSpellCheck(player)) {
		return false;
	}

	if (toPos.x == 0xFFFF) {
		return true;
	}

	const Position& playerPos = player->getPosition();
	if (playerPos.z > toPos.z) {
		player->sendCancelMessage(RETURNVALUE_FIRSTGOUPSTAIRS);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	} else if (playerPos.z < toPos.z) {
		player->sendCancelMessage(RETURNVALUE_FIRSTGODOWNSTAIRS);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	Tile* tile = g_game.map.getTile(toPos);
	if (!tile) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (range != -1 && !g_game.canThrowObjectTo(playerPos, toPos, true, range, range)) {
		player->sendCancelMessage(RETURNVALUE_DESTINATIONOUTOFREACH);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	ReturnValue ret = Combat::canDoCombat(player, tile, aggressive);
	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	const Creature* topVisibleCreature = tile->getBottomVisibleCreature(player);
	if (blockingCreature && topVisibleCreature) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	} else if (blockingSolid && tile->hasProperty(CONST_PROP_BLOCKSOLID)) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (needTarget && !topVisibleCreature) {
		player->sendCancelMessage(RETURNVALUE_CANONLYUSETHISRUNEONCREATURES);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (aggressive && needTarget && topVisibleCreature && player->hasSecureMode()) {
		const Player* targetPlayer = topVisibleCreature->getPlayer();
		if (targetPlayer && targetPlayer != player && player->getSkullClient(targetPlayer) == SKULL_NONE && !Combat::isInPvpZone(player, targetPlayer)) {
			player->sendCancelMessage(RETURNVALUE_TURNSECUREMODETOATTACKUNMARKEDPLAYERS);
			g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
			return false;
		}
	}
	return true;
}

void Spell::postCastSpell(Player* player, bool finishedCast /*= true*/, bool payCost /*= true*/) const
{
	if (finishedCast) {
		if (!player->hasFlag(PlayerFlag_HasNoExhaustion)) {
			if (cooldown > 0) {
				Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLCOOLDOWN, cooldown, 0, false, spellId);
				player->addCondition(condition);
			}

			if (groupCooldown > 0) {
				Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN, groupCooldown, 0, false, group);
				player->addCondition(condition);
			}

			if (secondaryGroupCooldown > 0) {
				Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN, secondaryGroupCooldown, 0, false, secondaryGroup);
				player->addCondition(condition);
			}
		}

		if (aggressive) {
			player->addInFightTicks();
		}
	}

	if (payCost) {
		Spell::postCastSpell(player, getManaCost(player), getSoulCost());
	}
}

void Spell::postCastSpell(Player* player, uint32_t manaCost, uint32_t soulCost)
{
	if (manaCost > 0) {
		player->addManaSpent(manaCost);
		player->changeMana(-static_cast<int32_t>(manaCost));
	}

	if (!player->hasFlag(PlayerFlag_HasInfiniteSoul)) {
		if (soulCost > 0) {
			player->changeSoul(-static_cast<int32_t>(soulCost));
		}
	}
}

uint32_t Spell::getManaCost(const Player* player) const
{
	if (mana != 0) {
		return mana;
	}

	if (manaPercent != 0) {
		uint32_t maxMana = player->getMaxMana();
		uint32_t manaCost = (maxMana * manaPercent) / 100;
		return manaCost;
	}

	return 0;
}

ReturnValue Spell::CreateIllusion(Creature* creature, const Outfit_t& outfit, int32_t time)
{
	ConditionOutfit* outfitCondition = new ConditionOutfit(CONDITIONID_COMBAT, CONDITION_OUTFIT, time);
	outfitCondition->setOutfit(outfit);
	creature->addCondition(outfitCondition);
	return RETURNVALUE_NOERROR;
}

ReturnValue Spell::CreateIllusion(Creature* creature, const std::string& name, int32_t time)
{
	uint32_t mId = g_monsters.getIdByName(name);
	if (mId == 0) {
		return RETURNVALUE_CREATUREDOESNOTEXIST;
	}

	const MonsterType* mType = g_monsters.getMonsterType(mId);
	if (mType == nullptr) {
		return RETURNVALUE_CREATUREDOESNOTEXIST;
	}

	Player* player = creature->getPlayer();
	if (player && !player->hasFlag(PlayerFlag_CanIllusionAll)) {
		if (!mType->isIllusionable) {
			return RETURNVALUE_NOTPOSSIBLE;
		}
	}

	return CreateIllusion(creature, mType->outfit, time);
}

ReturnValue Spell::CreateIllusion(Creature* creature, uint32_t itemId, int32_t time)
{
	const ItemType& it = Item::items[itemId];
	if (it.id == 0) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	Outfit_t outfit;
	outfit.lookTypeEx = itemId;

	return CreateIllusion(creature, outfit, time);
}

InstantSpell::InstantSpell(LuaScriptInterface* _interface) :
	TalkAction(_interface)
{
	needDirection = false;
	hasParam = false;
	hasPlayerNameParam = false;
	checkLineOfSight = true;
	casterTargetOrDirection = false;
	function = nullptr;
}

std::string InstantSpell::getScriptEventName() const
{
	return "onCastSpell";
}

bool InstantSpell::configureEvent(const pugi::xml_node& node)
{
	if (!Spell::configureSpell(node)) {
		return false;
	}

	if (!TalkAction::configureEvent(node)) {
		return false;
	}

	pugi::xml_attribute attr;
	if ((attr = node.attribute("params"))) {
		hasParam = attr.as_bool();
	}

	if ((attr = node.attribute("playernameparam"))) {
		hasPlayerNameParam = attr.as_bool();
	}

	if ((attr = node.attribute("direction"))) {
		needDirection = attr.as_bool();
	} else if ((attr = node.attribute("casterTargetOrDirection"))) {
		casterTargetOrDirection = attr.as_bool();
	}

	if ((attr = node.attribute("blockwalls"))) {
		checkLineOfSight = attr.as_bool();
	}
	return true;
}

bool InstantSpell::loadFunction(const pugi::xml_attribute& attr)
{
	const char* functionName = attr.as_string();
	if (strcasecmp(functionName, "edithouseguest") == 0) {
		function = HouseGuestList;
	} else if (strcasecmp(functionName, "edithousesubowner") == 0) {
		function = HouseSubOwnerList;
	} else if (strcasecmp(functionName, "edithousedoor") == 0) {
		function = HouseDoorList;
	} else if (strcasecmp(functionName, "housekick") == 0) {
		function = HouseKick;
	} else if (strcasecmp(functionName, "searchplayer") == 0) {
		function = SearchPlayer;
	} else if (strcasecmp(functionName, "levitate") == 0) {
		function = Levitate;
	} else if (strcasecmp(functionName, "illusion") == 0) {
		function = Illusion;
	} else if (strcasecmp(functionName, "summonmonster") == 0) {
		function = SummonMonster;
	} else {
		std::cout << "[Warning - InstantSpell::loadFunction] Function \"" << functionName << "\" does not exist." << std::endl;
		return false;
	}

	m_scripted = false;
	return true;
}

bool InstantSpell::playerCastInstant(Player* player, std::string& param)
{
	if (!playerSpellCheck(player)) {
		return false;
	}

	LuaVariant var;

	if (selfTarget) {
		var.type = VARIANT_NUMBER;
		var.number = player->getID();
	} else if (needTarget || casterTargetOrDirection) {
		Creature* target = nullptr;
		bool useDirection = false;

		if (hasParam) {
			Player* playerTarget = nullptr;
			ReturnValue ret = g_game.getPlayerByNameWildcard(param, playerTarget);

			if (playerTarget && playerTarget->isAccessPlayer() && !player->isAccessPlayer()) {
				playerTarget = nullptr;
			}

			target = playerTarget;
			if (!target || target->getHealth() <= 0) {
				if (!casterTargetOrDirection) {
					if (cooldown > 0) {
						Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLCOOLDOWN, cooldown, 0, false, spellId);
						player->addCondition(condition);
					}

					if (groupCooldown > 0) {
						Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN, groupCooldown, 0, false, group);
						player->addCondition(condition);
					}

					if (secondaryGroupCooldown > 0) {
						Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN, secondaryGroupCooldown, 0, false, secondaryGroup);
						player->addCondition(condition);
					}

					player->sendCancelMessage(ret);
					g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
					return false;
				}

				useDirection = true;
			}

			if (playerTarget) {
				param = playerTarget->getName();
			}
		} else {
			target = player->getAttackedCreature();
			if (!target || target->getHealth() <= 0) {
				if (!casterTargetOrDirection) {
					player->sendCancelMessage(RETURNVALUE_YOUCANONLYUSEITONCREATURES);
					g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
					return false;
				}

				useDirection = true;
			}
		}

		if (!useDirection) {
			if (!canThrowSpell(player, target)) {
				player->sendCancelMessage(RETURNVALUE_CREATUREISNOTREACHABLE);
				g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
				return false;
			}

			var.type = VARIANT_NUMBER;
			var.number = target->getID();
		} else {
			var.type = VARIANT_POSITION;
			var.pos = Spells::getCasterPosition(player, player->getDirection());

			if (!playerInstantSpellCheck(player, var.pos)) {
				return false;
			}
		}
	} else if (hasParam) {
		var.type = VARIANT_STRING;

		if (getHasPlayerNameParam()) {
			Player* playerTarget = nullptr;
			ReturnValue ret = g_game.getPlayerByNameWildcard(param, playerTarget);

			if (ret != RETURNVALUE_NOERROR) {
				if (cooldown > 0) {
					Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLCOOLDOWN, cooldown, 0, false, spellId);
					player->addCondition(condition);
				}

				if (groupCooldown > 0) {
					Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN, groupCooldown, 0, false, group);
					player->addCondition(condition);
				}

				if (secondaryGroupCooldown > 0) {
					Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN, secondaryGroupCooldown, 0, false, secondaryGroup);
					player->addCondition(condition);
				}

				player->sendCancelMessage(ret);
				g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
				return false;
			}

			if (playerTarget && (!playerTarget->isAccessPlayer() || player->isAccessPlayer())) {
				param = playerTarget->getName();
			}
		}

		var.text = param;
	} else {
		var.type = VARIANT_POSITION;

		if (needDirection) {
			var.pos = Spells::getCasterPosition(player, player->getDirection());
		} else {
			var.pos = player->getPosition();
		}

		if (!playerInstantSpellCheck(player, var.pos)) {
			return false;
		}
	}

	bool result = internalCastSpell(player, var);
	if (result) {
		postCastSpell(player);
	}

	return result;
}

bool InstantSpell::canThrowSpell(const Creature* creature, const Creature* target) const
{
	const Position& fromPos = creature->getPosition();
	const Position& toPos = target->getPosition();
	if (fromPos.z != toPos.z ||
	        (range == -1 && !g_game.canThrowObjectTo(fromPos, toPos, checkLineOfSight)) ||
	        (range != -1 && !g_game.canThrowObjectTo(fromPos, toPos, checkLineOfSight, range, range))) {
		return false;
	}
	return true;
}

bool InstantSpell::castSpell(Creature* creature)
{
	LuaVariant var;

	if (casterTargetOrDirection) {
		Creature* target = creature->getAttackedCreature();
		if (target && target->getHealth() > 0) {
			if (!canThrowSpell(creature, target)) {
				return false;
			}

			var.type = VARIANT_NUMBER;
			var.number = target->getID();
			return internalCastSpell(creature, var);
		}

		return false;
	} else if (needDirection) {
		var.type = VARIANT_POSITION;
		var.pos = Spells::getCasterPosition(creature, creature->getDirection());
	} else {
		var.type = VARIANT_POSITION;
		var.pos = creature->getPosition();
	}

	return internalCastSpell(creature, var);
}

bool InstantSpell::castSpell(Creature* creature, Creature* target)
{
	if (needTarget) {
		LuaVariant var;
		var.type = VARIANT_NUMBER;
		var.number = target->getID();
		return internalCastSpell(creature, var);
	} else {
		return castSpell(creature);
	}
}

bool InstantSpell::internalCastSpell(Creature* creature, const LuaVariant& var)
{
	if (m_scripted) {
		return executeCastSpell(creature, var);
	} else if (function) {
		return function(this, creature, var.text);
	}

	return false;
}

bool InstantSpell::executeCastSpell(Creature* creature, const LuaVariant& var)
{
	//onCastSpell(creature, var)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - InstantSpell::executeCastSpell] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);

	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);

	LuaScriptInterface::pushVariant(L, var);

	return m_scriptInterface->callFunction(2);
}

House* InstantSpell::getHouseFromPos(Creature* creature)
{
	if (!creature) {
		return nullptr;
	}

	Player* player = creature->getPlayer();
	if (!player) {
		return nullptr;
	}

	HouseTile* houseTile = dynamic_cast<HouseTile*>(player->getTile());
	if (!houseTile) {
		return nullptr;
	}

	House* house = houseTile->getHouse();
	if (!house) {
		return nullptr;
	}

	return house;
}

bool InstantSpell::HouseGuestList(const InstantSpell*, Creature* creature, const std::string&)
{
	House* house = getHouseFromPos(creature);
	if (!house) {
		return false;
	}

	Player* player = creature->getPlayer();
	if (house->canEditAccessList(GUEST_LIST, player)) {
		player->setEditHouse(house, GUEST_LIST);
		player->sendHouseWindow(house, GUEST_LIST);
	} else {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
	}
	return true;
}

bool InstantSpell::HouseSubOwnerList(const InstantSpell*, Creature* creature, const std::string&)
{
	House* house = getHouseFromPos(creature);
	if (!house) {
		return false;
	}

	Player* player = creature->getPlayer();
	if (house->canEditAccessList(SUBOWNER_LIST, player)) {
		player->setEditHouse(house, SUBOWNER_LIST);
		player->sendHouseWindow(house, SUBOWNER_LIST);
	} else {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
	}
	return true;
}

bool InstantSpell::HouseDoorList(const InstantSpell*, Creature* creature, const std::string&)
{
	House* house = getHouseFromPos(creature);
	if (!house) {
		return false;
	}

	Player* player = creature->getPlayer();
	Position pos = Spells::getCasterPosition(player, player->getDirection());
	Door* door = house->getDoorByPosition(pos);
	if (door && house->canEditAccessList(door->getDoorId(), player)) {
		player->setEditHouse(house, door->getDoorId());
		player->sendHouseWindow(house, door->getDoorId());
	} else {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
	}
	return true;
}

bool InstantSpell::HouseKick(const InstantSpell*, Creature* creature, const std::string& param)
{
	Player* player = creature->getPlayer();

	Player* targetPlayer = g_game.getPlayerByName(param);
	if (!targetPlayer) {
		targetPlayer = player;
	}

	House* house = getHouseFromPos(targetPlayer);
	if (!house) {
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return false;
	}

	if (!house->kickPlayer(player, targetPlayer)) {
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return false;
	}
	return true;
}

bool InstantSpell::SearchPlayer(const InstantSpell*, Creature* creature, const std::string& param)
{
	//a. From 1 to 4 sq's [Person] is standing next to you.
	//b. From 5 to 100 sq's [Person] is to the south, north, east, west.
	//c. From 101 to 274 sq's [Person] is far to the south, north, east, west.
	//d. From 275 to infinite sq's [Person] is very far to the south, north, east, west.
	//e. South-west, s-e, n-w, n-e (corner coordinates): this phrase appears if the player you're looking for has moved five squares in any direction from the south, north, east or west.
	//f. Lower level to the (direction): this phrase applies if the person you're looking for is from 1-25 squares up/down the actual floor you're in.
	//g. Higher level to the (direction): this phrase applies if the person you're looking for is from 1-25 squares up/down the actual floor you're in.

	Player* player = creature->getPlayer();
	if (!player) {
		return false;
	}

	enum distance_t {
		DISTANCE_BESIDE,
		DISTANCE_CLOSE,
		DISTANCE_FAR,
		DISTANCE_VERYFAR,
	};

	enum direction_t {
		DIR_N, DIR_S, DIR_E, DIR_W,
		DIR_NE, DIR_NW, DIR_SE, DIR_SW,
	};

	enum level_t {
		LEVEL_HIGHER,
		LEVEL_LOWER,
		LEVEL_SAME,
	};

	Player* playerExiva = g_game.getPlayerByName(param);
	if (!playerExiva) {
		return false;
	}

	if (playerExiva->isAccessPlayer() && !player->isAccessPlayer()) {
		player->sendCancelMessage(RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	const Position& lookPos = player->getPosition();
	const Position& searchPos = playerExiva->getPosition();

	int32_t dx = Position::getOffsetX(lookPos, searchPos);
	int32_t dy = Position::getOffsetY(lookPos, searchPos);
	int32_t dz = Position::getOffsetZ(lookPos, searchPos);

	distance_t distance;

	direction_t direction;

	level_t level;

	//getting floor
	if (dz > 0) {
		level = LEVEL_HIGHER;
	} else if (dz < 0) {
		level = LEVEL_LOWER;
	} else {
		level = LEVEL_SAME;
	}

	//getting distance
	if (std::abs(dx) < 4 && std::abs(dy) < 4) {
		distance = DISTANCE_BESIDE;
	} else {
		int32_t distance2 = dx * dx + dy * dy;
		if (distance2 < 10000) {
			distance = DISTANCE_CLOSE;
		} else if (distance2 < 75076) {
			distance = DISTANCE_FAR;
		} else {
			distance = DISTANCE_VERYFAR;
		}
	}

	//getting direction
	float tan;
	if (dx != 0) {
		tan = static_cast<float>(dy) / dx;
	} else {
		tan = 10.;
	}

	if (std::abs(tan) < 0.4142) {
		if (dx > 0) {
			direction = DIR_W;
		} else {
			direction = DIR_E;
		}
	} else if (std::abs(tan) < 2.4142) {
		if (tan > 0) {
			if (dy > 0) {
				direction = DIR_NW;
			} else {
				direction = DIR_SE;
			}
		} else {
			if (dx > 0) {
				direction = DIR_SW;
			} else {
				direction = DIR_NE;
			}
		}
	} else {
		if (dy > 0) {
			direction = DIR_N;
		} else {
			direction = DIR_S;
		}
	}

	std::ostringstream ss;
	ss << playerExiva->getName();

	if (distance == DISTANCE_BESIDE) {
		if (level == LEVEL_SAME) {
			ss << " is standing next to you.";
		} else if (level == LEVEL_HIGHER) {
			ss << " is above you.";
		} else if (level == LEVEL_LOWER) {
			ss << " is below you.";
		}
	} else {
		switch (distance) {
			case DISTANCE_CLOSE:
				if (level == LEVEL_SAME) {
					ss << " is to the ";
				} else if (level == LEVEL_HIGHER) {
					ss << " is on a higher level to the ";
				} else if (level == LEVEL_LOWER) {
					ss << " is on a lower level to the ";
				}
				break;
			case DISTANCE_FAR:
				ss << " is far to the ";
				break;
			case DISTANCE_VERYFAR:
				ss << " is very far to the ";
				break;
			default:
				break;
		}

		switch (direction) {
			case DIR_N:
				ss << "north.";
				break;
			case DIR_S:
				ss << "south.";
				break;
			case DIR_E:
				ss << "east.";
				break;
			case DIR_W:
				ss << "west.";
				break;
			case DIR_NE:
				ss << "north-east.";
				break;
			case DIR_NW:
				ss << "north-west.";
				break;
			case DIR_SE:
				ss << "south-east.";
				break;
			case DIR_SW:
				ss << "south-west.";
				break;
		}
	}
	player->sendTextMessage(MESSAGE_INFO_DESCR, ss.str());
	g_game.addMagicEffect(player->getPosition(), CONST_ME_MAGIC_BLUE);
	return true;
}

bool InstantSpell::SummonMonster(const InstantSpell* spell, Creature* creature, const std::string& param)
{
	Player* player = creature->getPlayer();
	if (!player) {
		return false;
	}

	MonsterType* mType = g_monsters.getMonsterType(param);
	if (!mType) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (!player->hasFlag(PlayerFlag_CanSummonAll)) {
		if (!mType->isSummonable) {
			player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
			g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
			return false;
		}

		if (player->getMana() < mType->manaCost) {
			player->sendCancelMessage(RETURNVALUE_NOTENOUGHMANA);
			g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
			return false;
		}

		if (player->getSummonCount() >= 2) {
			player->sendCancelMessage("You cannot summon more creatures.");
			g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
			return false;
		}
	}

	Monster* monster = Monster::createMonster(param);
	if (!monster) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	// Place the monster
	creature->addSummon(monster);

	if (!g_game.placeCreature(monster, creature->getPosition(), true)) {
		creature->removeSummon(monster);
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	Spell::postCastSpell(player, mType->manaCost, spell->getSoulCost());
	g_game.addMagicEffect(player->getPosition(), CONST_ME_MAGIC_BLUE);
	g_game.addMagicEffect(monster->getPosition(), CONST_ME_TELEPORT);
	return true;
}

bool InstantSpell::Levitate(const InstantSpell*, Creature* creature, const std::string& param)
{
	Player* player = creature->getPlayer();
	if (!player) {
		return false;
	}

	const Position& currentPos = creature->getPosition();
	const Position& destPos = Spells::getCasterPosition(creature, creature->getDirection());

	ReturnValue ret = RETURNVALUE_NOTPOSSIBLE;

	if (strcasecmp(param.c_str(), "up") == 0) {
		if (currentPos.z != 8) {
			Tile* tmpTile = g_game.map.getTile(currentPos.x, currentPos.y, currentPos.getZ() - 1);
			if (tmpTile == nullptr || (tmpTile->ground == nullptr && !tmpTile->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID))) {
				tmpTile = g_game.map.getTile(destPos.x, destPos.y, destPos.getZ() - 1);
				if (tmpTile && tmpTile->ground && !tmpTile->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) && !tmpTile->floorChange()) {
					ret = g_game.internalMoveCreature(*player, *tmpTile, FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE);
				}
			}
		}
	} else if (strcasecmp(param.c_str(), "down") == 0) {
		if (currentPos.z != 7) {
			Tile* tmpTile = g_game.map.getTile(destPos);
			if (tmpTile == nullptr || (tmpTile->ground == nullptr && !tmpTile->hasProperty(CONST_PROP_BLOCKSOLID))) {
				tmpTile = g_game.map.getTile(destPos.x, destPos.y, destPos.z + 1);
				if (tmpTile && tmpTile->ground && !tmpTile->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) && !tmpTile->floorChange()) {
					ret = g_game.internalMoveCreature(*player, *tmpTile, FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE);
				}
			}
		}
	}

	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	g_game.addMagicEffect(player->getPosition(), CONST_ME_TELEPORT);
	return true;
}

bool InstantSpell::Illusion(const InstantSpell*, Creature* creature, const std::string& param)
{
	Player* player = creature->getPlayer();
	if (!player) {
		return false;
	}

	ReturnValue ret = CreateIllusion(creature, param, 180000);
	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	g_game.addMagicEffect(player->getPosition(), CONST_ME_MAGIC_RED);
	return true;
}

bool InstantSpell::canCast(const Player* player) const
{
	if (player->hasFlag(PlayerFlag_CannotUseSpells)) {
		return false;
	}

	if (player->hasFlag(PlayerFlag_IgnoreSpellCheck)) {
		return true;
	}

	if (isLearnable()) {
		if (player->hasLearnedInstantSpell(getName())) {
			return true;
		}
	} else {
		if (vocSpellMap.empty() || vocSpellMap.find(player->getVocationId()) != vocSpellMap.end()) {
			return true;
		}
	}

	return false;
}


ConjureSpell::ConjureSpell(LuaScriptInterface* _interface) :
	InstantSpell(_interface)
{
	aggressive = false;
	conjureId = 0;
	conjureCount = 1;
	conjureReagentId = 0;
}

std::string ConjureSpell::getScriptEventName() const
{
	return "onCastSpell";
}

bool ConjureSpell::configureEvent(const pugi::xml_node& node)
{
	if (!InstantSpell::configureEvent(node)) {
		return false;
	}

	pugi::xml_attribute attr;
	if ((attr = node.attribute("conjureId"))) {
		conjureId = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("conjureCount"))) {
		conjureCount = pugi::cast<uint32_t>(attr.value());
	} else if (conjureId != 0) {
		// load default charges from items.xml
		const ItemType& it = Item::items[conjureId];
		if (it.charges != 0) {
			conjureCount = it.charges;
		}
	}

	if ((attr = node.attribute("reagentId"))) {
		conjureReagentId = pugi::cast<uint32_t>(attr.value());
	}

	return true;
}

bool ConjureSpell::loadFunction(const pugi::xml_attribute&)
{
	m_scripted = false;
	return true;
}

ReturnValue ConjureSpell::internalConjureItem(Player* player, uint32_t conjureId, uint32_t conjureCount)
{
	Item* newItem = Item::CreateItem(conjureId, conjureCount);
	if (!newItem) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	ReturnValue result = g_game.internalPlayerAddItem(player, newItem);
	if (result != RETURNVALUE_NOERROR) {
		delete newItem;
	}
	return result;
}

bool ConjureSpell::conjureItem(Creature* creature) const
{
	Player* player = creature->getPlayer();
	if (!player) {
		return false;
	}

	if (getReagentId() != 0) {
		if (!player->removeItemOfType(getReagentId(), 1, -1)) {
			player->sendCancelMessage(RETURNVALUE_YOUNEEDAMAGICITEMTOCASTSPELL);
			g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
			return false;
		}

		Item* newItem = Item::CreateItem(getConjureId(), getConjureCount());
		if (!newItem) {
			return false;
		}

		ReturnValue ret = g_game.internalPlayerAddItem(player, newItem);
		if (ret != RETURNVALUE_NOERROR) {
			delete newItem;
			return false;
		}
	} else if (internalConjureItem(player, getConjureId(), getConjureCount()) != RETURNVALUE_NOERROR) {
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	postCastSpell(player);
	g_game.addMagicEffect(player->getPosition(), CONST_ME_MAGIC_RED);
	return true;
}

bool ConjureSpell::playerCastInstant(Player* player, std::string& param)
{
	if (!playerSpellCheck(player)) {
		return false;
	}

	if (m_scripted) {
		LuaVariant var;
		var.type = VARIANT_STRING;
		var.text = param;
		return executeCastSpell(player, var);
	}
	return conjureItem(player);
}

RuneSpell::RuneSpell(LuaScriptInterface* _interface) :
	Action(_interface)
{
	hasCharges = true;
	runeId = 0;
	runeFunction = nullptr;

	allowFarUse = true;
}

std::string RuneSpell::getScriptEventName() const
{
	return "onCastSpell";
}

bool RuneSpell::configureEvent(const pugi::xml_node& node)
{
	if (!Spell::configureSpell(node)) {
		return false;
	}

	if (!Action::configureEvent(node)) {
		return false;
	}

	pugi::xml_attribute attr;
	if (!(attr = node.attribute("id"))) {
		std::cout << "[Error - RuneSpell::configureSpell] Rune spell without id." << std::endl;
		return false;
	}
	runeId = pugi::cast<uint16_t>(attr.value());

	uint32_t charges;
	if ((attr = node.attribute("charges"))) {
		charges = pugi::cast<uint32_t>(attr.value());
	} else {
		charges = 0;
	}

	hasCharges = (charges > 0);
	if (magLevel != 0 || level != 0) {
		//Change information in the ItemType to get accurate description
		ItemType& iType = Item::items.getItemType(runeId);
		iType.runeMagLevel = magLevel;
		iType.runeLevel = level;
		iType.charges = charges;
	}

	return true;
}

bool RuneSpell::loadFunction(const pugi::xml_attribute& attr)
{
	const char* functionName = attr.as_string();
	if (strcasecmp(functionName, "chameleon") == 0) {
		runeFunction = Illusion;
	} else if (strcasecmp(functionName, "convince") == 0) {
		runeFunction = Convince;
	} else {
		std::cout << "[Warning - RuneSpell::loadFunction] Function \"" << functionName << "\" does not exist." << std::endl;
		return false;
	}

	m_scripted = false;
	return true;
}

bool RuneSpell::Illusion(const RuneSpell*, Player* player, const Position& posTo)
{
	Thing* thing = g_game.internalGetThing(player, posTo, 0, 0, STACKPOS_MOVE);
	if (!thing) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	Item* illusionItem = thing->getItem();
	if (!illusionItem || !illusionItem->isMoveable()) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	ReturnValue ret = CreateIllusion(player, illusionItem->getID(), 200000);
	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	g_game.addMagicEffect(player->getPosition(), CONST_ME_MAGIC_RED);
	return true;
}

bool RuneSpell::Convince(const RuneSpell* spell, Player* player, const Position& posTo)
{
	if (!player->hasFlag(PlayerFlag_CanConvinceAll)) {
		if (player->getSummonCount() >= 2) {
			player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
			g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
			return false;
		}
	}

	Thing* thing = g_game.internalGetThing(player, posTo, 0, 0, STACKPOS_LOOK);
	if (!thing) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	Creature* convinceCreature = thing->getCreature();
	if (!convinceCreature) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	uint32_t manaCost = 0;
	if (convinceCreature->getMonster()) {
		manaCost = convinceCreature->getMonster()->getManaCost();
	}

	if (!player->hasFlag(PlayerFlag_HasInfiniteMana) && player->getMana() < manaCost) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHMANA);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (!convinceCreature->convinceCreature(player)) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	Spell::postCastSpell(player, manaCost, spell->getSoulCost());
	g_game.updateCreatureType(convinceCreature);
	g_game.addMagicEffect(player->getPosition(), CONST_ME_MAGIC_RED);
	return true;
}

ReturnValue RuneSpell::canExecuteAction(const Player* player, const Position& toPos)
{
	if (player->hasFlag(PlayerFlag_CannotUseSpells)) {
		return RETURNVALUE_CANNOTUSETHISOBJECT;
	}

	ReturnValue ret = Action::canExecuteAction(player, toPos);
	if (ret != RETURNVALUE_NOERROR) {
		return ret;
	}

	if (toPos.x == 0xFFFF) {
		if (needTarget) {
			return RETURNVALUE_CANONLYUSETHISRUNEONCREATURES;
		} else if (!selfTarget) {
			return RETURNVALUE_NOTENOUGHROOM;
		}
	}

	return RETURNVALUE_NOERROR;
}

bool RuneSpell::executeUse(Player* player, Item* item, const Position&, Thing* target, const Position& toPosition, bool isHotkey)
{
	if (!playerRuneSpellCheck(player, toPosition)) {
		return false;
	}

	bool result = false;
	if (m_scripted) {
		LuaVariant var;

		if (needTarget) {
			var.type = VARIANT_NUMBER;

			if (target == nullptr) {
				Tile* toTile = g_game.map.getTile(toPosition);
				if (toTile) {
					const Creature* visibleCreature = toTile->getBottomVisibleCreature(player);
					if (visibleCreature) {
						var.number = visibleCreature->getID();
					}
				}
			} else {
				var.number = target->getCreature()->getID();
			}
		} else {
			var.type = VARIANT_POSITION;
			var.pos = toPosition;
		}

		result = internalCastSpell(player, var, isHotkey);
	} else if (runeFunction) {
		result = runeFunction(this, player, toPosition);
	}

	if (!result) {
		return false;
	}

	postCastSpell(player);
	if (hasCharges && item && g_config.getBoolean(ConfigManager::REMOVE_RUNE_CHARGES)) {
		int32_t newCount = std::max<int32_t>(0, item->getItemCount() - 1);
		g_game.transformItem(item, item->getID(), newCount);
	}
	return true;
}

bool RuneSpell::castSpell(Creature* creature)
{
	LuaVariant var;
	var.type = VARIANT_NUMBER;
	var.number = creature->getID();
	return internalCastSpell(creature, var, false);
}

bool RuneSpell::castSpell(Creature* creature, Creature* target)
{
	LuaVariant var;
	var.type = VARIANT_NUMBER;
	var.number = target->getID();
	return internalCastSpell(creature, var, false);
}

bool RuneSpell::internalCastSpell(Creature* creature, const LuaVariant& var, bool isHotkey)
{
	bool result;
	if (m_scripted) {
		result = executeCastSpell(creature, var, isHotkey);
	} else {
		result = false;
	}
	return result;
}

bool RuneSpell::executeCastSpell(Creature* creature, const LuaVariant& var, bool isHotkey)
{
	//onCastSpell(creature, var, isHotkey)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - RuneSpell::executeCastSpell] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);

	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);

	LuaScriptInterface::pushVariant(L, var);

	LuaScriptInterface::pushBoolean(L, isHotkey);

	return m_scriptInterface->callFunction(3);
}
