// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "spells.h"

#include "combat.h"
#include "configmanager.h"
#include "game.h"
#include "luavariant.h"
#include "monsters.h"
#include "pugicast.h"

extern Game g_game;
extern Spells* g_spells;
extern Monsters g_monsters;
extern ConfigManager g_config;
extern LuaEnvironment g_luaEnvironment;

Spells::Spells() { scriptInterface.initState(); }

Spells::~Spells() { clear(); }

TalkActionResult_t Spells::playerSaySpell(Player* player, std::string& words)
{
	std::string str_words = words;

	// strip trailing spaces
	boost::algorithm::trim(str_words);

	InstantSpell_shared_ptr instantSpell = getInstantSpell(str_words);
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
				boost::algorithm::trim(paramText);
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
	instants.clear();
	runes.clear();

	scriptInterface.reInitState();
}

bool Spells::registerInstantLuaEvent(InstantSpell_shared_ptr instant)
{
	if (instant) {
		std::string words = instant->getWords();
		auto result = instants.emplace(instant->getWords(), instant);
		if (!result.second) {
			std::cout << "[Warning - Spells::registerInstantLuaEvent] Duplicate registered instant spell with words: "
			          << words << std::endl;
		}
		return result.second;
	}

	return false;
}

bool Spells::registerRuneLuaEvent(RuneSpell_shared_ptr rune)
{
	if (rune) {
		uint16_t id = rune->getRuneItemId();
		auto result = runes.emplace(rune->getRuneItemId(), rune);
		if (!result.second) {
			std::cout << "[Warning - Spells::registerRuneLuaEvent] Duplicate registered rune with id: " << id
			          << std::endl;
		}
		return result.second;
	}

	return false;
}

Spell_shared_ptr Spells::getSpellByName(const std::string& name)
{
	Spell_shared_ptr spell = getRuneSpellByName(name);
	if (!spell) {
		spell = getInstantSpellByName(name);
	}
	return spell;
}

RuneSpell_shared_ptr Spells::getRuneSpell(uint32_t id)
{
	auto it = runes.find(id);
	if (it == runes.end()) {
		for (auto& rune : runes) {
			if (rune.second->getId() == id) {
				return rune.second;
			}
		}
		return nullptr;
	}
	return it->second;
}

RuneSpell_shared_ptr Spells::getRuneSpellByName(const std::string& name)
{
	for (auto& it : runes) {
		if (caseInsensitiveEqual(it.second->getName(), name)) {
			return it.second;
		}
	}
	return nullptr;
}

InstantSpell_shared_ptr Spells::getInstantSpell(const std::string& words)
{
	InstantSpell_shared_ptr result = nullptr;

	for (auto& it : instants) {
		const std::string& instantSpellWords = it.second->getWords();
		size_t spellLen = instantSpellWords.length();
		if (caseInsensitiveStartsWith(words, instantSpellWords)) {
			if (!result || spellLen > result->getWords().size()) {
				result = it.second;
				if (words.length() == spellLen) {
					break;
				}
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

InstantSpell_shared_ptr Spells::getInstantSpellByName(const std::string& name)
{
	for (auto& it : instants) {
		if (caseInsensitiveEqual(it.second->getName(), name)) {
			return it.second;
		}
	}
	return nullptr;
}

Position Spells::getCasterPosition(Creature* creature, Direction dir)
{
	return getNextPosition(dir, creature->getPosition());
}

CombatSpell::CombatSpell(Combat_ptr combat, bool needTarget, bool needDirection) :
    Event(&g_spells->scriptInterface), combat(combat), needDirection(needDirection), needTarget(needTarget)
{}

bool CombatSpell::loadScriptCombat()
{
	combat = g_luaEnvironment.getCombatObject(g_luaEnvironment.lastCombatId);
	return combat != nullptr;
}

bool CombatSpell::castSpell(Creature* creature)
{
	if (scripted) {
		LuaVariant var;

		if (needDirection) {
			var.setPosition(Spells::getCasterPosition(creature, creature->getDirection()));
		} else {
			var.setPosition(creature->getPosition());
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
	if (scripted) {
		LuaVariant var;

		if (combat->hasArea()) {
			if (needTarget) {
				var.setPosition(target->getPosition());
			} else if (needDirection) {
				var.setPosition(Spells::getCasterPosition(creature, creature->getDirection()));
			} else {
				var.setPosition(creature->getPosition());
			}
		} else {
			var.setNumber(target->getID());
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
	// onCastSpell(creature, var)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CombatSpell::executeCastSpell] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);

	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);

	LuaScriptInterface::pushVariant(L, var);

	return scriptInterface->callFunction(2);
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

	if ((aggressive || pzLock) && (range < 1 || (range > 0 && !player->getAttackedCreature())) &&
	    player->getSkull() == SKULL_BLACK) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return false;
	}

	if ((aggressive || pzLock) && player->hasCondition(CONDITION_PACIFIED)) {
		player->sendCancelMessage(RETURNVALUE_YOUAREEXHAUSTED);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if ((aggressive || pzLock) && !player->hasFlag(PlayerFlag_IgnoreProtectionZone) &&
	    player->getZone() == ZONE_PROTECTION) {
		player->sendCancelMessage(RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE);
		return false;
	}

	if (player->hasCondition(CONDITION_SPELLGROUPCOOLDOWN, group) ||
	    player->hasCondition(CONDITION_SPELLCOOLDOWN, spellId) ||
	    (secondaryGroup != SPELLGROUP_NONE && player->hasCondition(CONDITION_SPELLGROUPCOOLDOWN, secondaryGroup))) {
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
	} else if (!hasVocationSpellMap(player->getVocationId())) {
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

	if (blockingCreature && tile->getBottomVisibleCreature(player)) {
		player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}

	if (blockingSolid && tile->hasFlag(TILESTATE_BLOCKSOLID)) {
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

	if (range != -1 && !g_game.canThrowObjectTo(playerPos, toPos, true, true, range, range)) {
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
	} else if (blockingSolid && tile->hasFlag(TILESTATE_BLOCKSOLID)) {
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
		if (targetPlayer && targetPlayer != player && player->getSkullClient(targetPlayer) == SKULL_NONE &&
		    !Combat::isInPvpZone(player, targetPlayer)) {
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
				Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLCOOLDOWN,
				                                                  cooldown, 0, false, spellId);
				player->addCondition(condition);
			}

			if (groupCooldown > 0) {
				Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN,
				                                                  groupCooldown, 0, false, group);
				player->addCondition(condition);
			}

			if (secondaryGroupCooldown > 0) {
				Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN,
				                                                  secondaryGroupCooldown, 0, false, secondaryGroup);
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

bool InstantSpell::playerCastInstant(Player* player, std::string& param)
{
	if (!playerSpellCheck(player)) {
		return false;
	}

	LuaVariant var;

	if (selfTarget) {
		var.setNumber(player->getID());
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
			if (!target || target->isRemoved() || target->isDead()) {
				if (!casterTargetOrDirection) {
					if (cooldown > 0) {
						Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLCOOLDOWN,
						                                                  cooldown, 0, false, spellId);
						player->addCondition(condition);
					}

					if (groupCooldown > 0) {
						Condition* condition = Condition::createCondition(
						    CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN, groupCooldown, 0, false, group);
						player->addCondition(condition);
					}

					if (secondaryGroupCooldown > 0) {
						Condition* condition =
						    Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN,
						                               secondaryGroupCooldown, 0, false, secondaryGroup);
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
			if (!target || target->isRemoved() || target->isDead()) {
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

			var.setNumber(target->getID());
		} else {
			var.setPosition(Spells::getCasterPosition(player, player->getDirection()));

			if (!playerInstantSpellCheck(player, var.getPosition())) {
				return false;
			}
		}
	} else if (hasParam) {
		if (getHasPlayerNameParam()) {
			Player* playerTarget = nullptr;
			ReturnValue ret = g_game.getPlayerByNameWildcard(param, playerTarget);

			if (ret != RETURNVALUE_NOERROR) {
				if (cooldown > 0) {
					Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLCOOLDOWN,
					                                                  cooldown, 0, false, spellId);
					player->addCondition(condition);
				}

				if (groupCooldown > 0) {
					Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN,
					                                                  groupCooldown, 0, false, group);
					player->addCondition(condition);
				}

				if (secondaryGroupCooldown > 0) {
					Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SPELLGROUPCOOLDOWN,
					                                                  secondaryGroupCooldown, 0, false, secondaryGroup);
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

		var.setString(param);
	} else {
		if (needDirection) {
			var.setPosition(Spells::getCasterPosition(player, player->getDirection()));
		} else {
			var.setPosition(player->getPosition());
		}

		if (!playerInstantSpellCheck(player, var.getPosition())) {
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
	    (range == -1 && !g_game.canThrowObjectTo(fromPos, toPos, checkLineOfSight, true, Map::maxClientViewportX - 1,
	                                             Map::maxClientViewportY - 1)) ||
	    (range != -1 && !g_game.canThrowObjectTo(fromPos, toPos, checkLineOfSight, true, range, range))) {
		return false;
	}
	return true;
}

bool InstantSpell::castSpell(Creature* creature)
{
	LuaVariant var;

	if (casterTargetOrDirection) {
		Creature* target = creature->getAttackedCreature();
		if (target && !target->isDead()) {
			if (!canThrowSpell(creature, target)) {
				return false;
			}

			var.setNumber(target->getID());
			return internalCastSpell(creature, var);
		}

		return false;
	} else if (needDirection) {
		var.setPosition(Spells::getCasterPosition(creature, creature->getDirection()));
	} else {
		var.setPosition(creature->getPosition());
	}

	return internalCastSpell(creature, var);
}

bool InstantSpell::castSpell(Creature* creature, Creature* target)
{
	if (needTarget) {
		LuaVariant var;
		var.setNumber(target->getID());
		return internalCastSpell(creature, var);
	}
	return castSpell(creature);
}

bool InstantSpell::internalCastSpell(Creature* creature, const LuaVariant& var)
{
	return executeCastSpell(creature, var);
}

bool InstantSpell::executeCastSpell(Creature* creature, const LuaVariant& var)
{
	// onCastSpell(creature, var)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - InstantSpell::executeCastSpell] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);

	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);

	LuaScriptInterface::pushVariant(L, var);

	return scriptInterface->callFunction(2);
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
		if (vocationSpellMap.empty() || hasVocationSpellMap(player->getVocationId())) {
			return true;
		}
	}

	return false;
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

bool RuneSpell::executeUse(Player* player, Item* item, const Position&, Thing* target, const Position& toPosition,
                           bool isHotkey)
{
	if (!playerRuneSpellCheck(player, toPosition)) {
		return false;
	}

	if (!scripted) {
		return false;
	}

	LuaVariant var;

	if (needTarget) {
		if (!target) {
			Tile* toTile = g_game.map.getTile(toPosition);
			if (toTile) {
				const Creature* visibleCreature = toTile->getBottomVisibleCreature(player);
				if (visibleCreature) {
					var.setNumber(visibleCreature->getID());
				}
			}
		} else {
			var.setNumber(target->getCreature()->getID());
		}
	} else {
		var.setPosition(toPosition);
	}

	if (!internalCastSpell(player, var, isHotkey)) {
		return false;
	}

	postCastSpell(player);

	if (var.isNumber()) {
		target = g_game.getCreatureByID(var.getNumber());
		if (getPzLock() && target) {
			player->onAttackedCreature(target->getCreature());
		}
	}

	if (hasCharges && item && g_config.getBoolean(ConfigManager::REMOVE_RUNE_CHARGES)) {
		int32_t newCount = std::max<int32_t>(0, item->getItemCount() - 1);
		player->sendSupplyUsed(item->getClientID());
		g_game.transformItem(item, item->getID(), newCount);
	}
	return true;
}

bool RuneSpell::castSpell(Creature* creature)
{
	LuaVariant var;
	var.setNumber(creature->getID());
	return internalCastSpell(creature, var, false);
}

bool RuneSpell::castSpell(Creature* creature, Creature* target)
{
	LuaVariant var;
	var.setNumber(target->getID());
	return internalCastSpell(creature, var, false);
}

bool RuneSpell::internalCastSpell(Creature* creature, const LuaVariant& var, bool isHotkey)
{
	bool result;
	if (scripted) {
		result = executeCastSpell(creature, var, isHotkey);
	} else {
		result = false;
	}
	return result;
}

bool RuneSpell::executeCastSpell(Creature* creature, const LuaVariant& var, bool isHotkey)
{
	// onCastSpell(creature, var, isHotkey)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - RuneSpell::executeCastSpell] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);

	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);

	LuaScriptInterface::pushVariant(L, var);

	LuaScriptInterface::pushBoolean(L, isHotkey);

	return scriptInterface->callFunction(3);
}
