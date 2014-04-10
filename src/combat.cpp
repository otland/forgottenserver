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

#include "combat.h"

#include "game.h"
#include "creature.h"
#include "player.h"
#include "const.h"
#include "tools.h"
#include "weapons.h"
#include "configmanager.h"

extern Game g_game;
extern Weapons* g_weapons;
extern ConfigManager g_config;

Combat::Combat() :
	formulaType(COMBAT_FORMULA_UNDEFINED),
	mina(0.0), minb(0.0), maxa(0.0), maxb(0.0),
	area(nullptr)
{
	//
}

Combat::~Combat()
{
	for (const Condition* condition : params.conditionList) {
		delete condition;
	}

	delete params.valueCallback;
	delete params.tileCallback;
	delete params.targetCallback;
	delete area;
}

CombatDamage Combat::getCombatDamage(Creature* creature, Creature* target) const
{
	CombatDamage damage;
	if (!creature) {
		return damage;
	}

	damage.primary.type = params.combatType;

	int32_t min, max;
	if (creature->getCombatValues(min, max)) {
		damage.primary.value = normal_random(min, max);
	} else if (Player* player = creature->getPlayer()) {
		if (params.valueCallback) {
			params.valueCallback->getMinMaxValues(player, damage, params.useCharges);
		} else {
			switch (formulaType) {
				case COMBAT_FORMULA_LEVELMAGIC: {
					int32_t levelFormula = player->getLevel() * 2 + player->getMagicLevel() * 3;
					damage.primary.value = normal_random(
						static_cast<int32_t>(levelFormula * mina + minb),
						static_cast<int32_t>(levelFormula * maxa + maxb)
					);
					break;
				}

				case COMBAT_FORMULA_SKILL: {
					Item* tool = player->getWeapon();
					const Weapon* weapon = g_weapons->getWeapon(tool);
					if (weapon) {
						damage.primary.value = normal_random(
							static_cast<int32_t>(minb),
							static_cast<int32_t>(weapon->getWeaponDamage(player, target, tool, true) * maxa + maxb)
						);

						damage.secondary.type = weapon->getElementType();
						damage.secondary.value = weapon->getElementDamage(player, target, tool);
						if (params.useCharges) {
							uint16_t charges = tool->getCharges();
							if (charges != 0) {
								g_game.transformItem(tool, tool->getID(), charges - 1);
							}
						}
					} else {
						damage.primary.value = normal_random(
							static_cast<int32_t>(minb),
							static_cast<int32_t>(maxb)
						);
					}
					break;
				}

				case COMBAT_FORMULA_DAMAGE: {
					damage.primary.value = normal_random(
						static_cast<int32_t>(mina),
						static_cast<int32_t>(maxa)
					);
					break;
				}
				default:
					break;
			}
		}
	} else if (formulaType == COMBAT_FORMULA_DAMAGE) {
		damage.primary.value = normal_random(
			static_cast<int32_t>(mina),
			static_cast<int32_t>(maxa)
		);
	}
	return damage;
}

void Combat::getCombatArea(const Position& centerPos, const Position& targetPos, const AreaCombat* area,
                           std::list<Tile*>& list)
{
	if (targetPos.z >= MAP_MAX_LAYERS) {
		return;
	}

	if (area) {
		area->getList(centerPos, targetPos, list);
	} else {
		Tile* tile = g_game.getTile(targetPos.x, targetPos.y, targetPos.z);
		if (!tile) {
			tile = new StaticTile(targetPos.x, targetPos.y, targetPos.z);
			g_game.setTile(tile);
		}
		list.push_back(tile);
	}
}

CombatType_t Combat::ConditionToDamageType(ConditionType_t type)
{
	switch (type) {
		case CONDITION_FIRE:
			return COMBAT_FIREDAMAGE;

		case CONDITION_ENERGY:
			return COMBAT_ENERGYDAMAGE;

		case CONDITION_BLEEDING:
			return COMBAT_PHYSICALDAMAGE;

		case CONDITION_DROWN:
			return COMBAT_DROWNDAMAGE;

		case CONDITION_POISON:
			return COMBAT_EARTHDAMAGE;

		case CONDITION_FREEZING:
			return COMBAT_ICEDAMAGE;

		case CONDITION_DAZZLED:
			return COMBAT_HOLYDAMAGE;

		case CONDITION_CURSED:
			return COMBAT_DEATHDAMAGE;

		default:
			break;
	}

	return COMBAT_NONE;
}

ConditionType_t Combat::DamageToConditionType(CombatType_t type)
{
	switch (type) {
		case COMBAT_FIREDAMAGE:
			return CONDITION_FIRE;

		case COMBAT_ENERGYDAMAGE:
			return CONDITION_ENERGY;

		case COMBAT_DROWNDAMAGE:
			return CONDITION_DROWN;

		case COMBAT_EARTHDAMAGE:
			return CONDITION_POISON;

		case COMBAT_ICEDAMAGE:
			return CONDITION_FREEZING;

		case COMBAT_HOLYDAMAGE:
			return CONDITION_DAZZLED;

		case COMBAT_DEATHDAMAGE:
			return CONDITION_CURSED;

		case COMBAT_PHYSICALDAMAGE:
			return CONDITION_BLEEDING;

		default:
			return CONDITION_NONE;
	}
}

bool Combat::isPlayerCombat(const Creature* target)
{
	if (target->getPlayer()) {
		return true;
	}

	if (target->isSummon() && target->getMaster()->getPlayer()) {
		return true;
	}

	return false;
}

ReturnValue Combat::canTargetCreature(const Player* player, const Creature* target)
{
	if (player == target) {
		return RET_YOUMAYNOTATTACKTHISPLAYER;
	}

	if (!player->hasFlag(PlayerFlag_IgnoreProtectionZone)) {
		//pz-zone
		if (player->getZone() == ZONE_PROTECTION) {
			return RET_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE;
		}

		if (target->getZone() == ZONE_PROTECTION) {
			return RET_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE;
		}

		//nopvp-zone
		if (isPlayerCombat(target)) {
			if (player->getZone() == ZONE_NOPVP) {
				return RET_ACTIONNOTPERMITTEDINANOPVPZONE;
			}

			if (target->getZone() == ZONE_NOPVP) {
				return RET_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE;
			}
		}
	}

	if (player->hasFlag(PlayerFlag_CannotUseCombat) || !target->isAttackable()) {
		if (target->getPlayer()) {
			return RET_YOUMAYNOTATTACKTHISPLAYER;
		} else {
			return RET_YOUMAYNOTATTACKTHISCREATURE;
		}
	}

	if (target->getPlayer()) {
		if (isProtected(player, target->getPlayer())) {
			return RET_YOUMAYNOTATTACKTHISPLAYER;
		}

		if (player->getSecureMode() == SECUREMODE_ON && !Combat::isInPvpZone(player, target) &&
		        player->getSkullClient(target->getPlayer()) == SKULL_NONE) {
			return RET_TURNSECUREMODETOATTACKUNMARKEDPLAYERS;
		}
	}

	return Combat::canDoCombat(player, target);
}

ReturnValue Combat::canDoCombat(const Creature* caster, const Tile* tile, bool isAggressive)
{
	if (tile->hasProperty(CONST_PROP_BLOCKPROJECTILE)) {
		return RET_NOTENOUGHROOM;
	}

	if (tile->floorChange()) {
		return RET_NOTENOUGHROOM;
	}

	if (tile->getTeleportItem()) {
		return RET_NOTENOUGHROOM;
	}

	if (caster) {
		const Position& casterPosition = caster->getPosition();
		const Position& tilePosition = tile->getPosition();

		if (casterPosition.z < tilePosition.z) {
			return RET_FIRSTGODOWNSTAIRS;
		} else if (casterPosition.z > tilePosition.z) {
			return RET_FIRSTGOUPSTAIRS;
		}

		if (const Player* player = caster->getPlayer()) {
			if (player->hasFlag(PlayerFlag_IgnoreProtectionZone)) {
				return RET_NOERROR;
			}
		}
	}

	//pz-zone
	if (isAggressive && tile->hasFlag(TILESTATE_PROTECTIONZONE)) {
		return RET_ACTIONNOTPERMITTEDINPROTECTIONZONE;
	}

	return RET_NOERROR;
}

bool Combat::isInPvpZone(const Creature* attacker, const Creature* target)
{
	if (attacker->getZone() != ZONE_PVP) {
		return false;
	}

	if (target->getZone() != ZONE_PVP) {
		return false;
	}

	return true;
}

bool Combat::isProtected(const Player* attacker, const Player* target)
{
	uint32_t protectionLevel = g_config.getNumber(ConfigManager::PROTECTION_LEVEL);

	if (target->getLevel() < protectionLevel || attacker->getLevel() < protectionLevel) {
		return true;
	}

	if (attacker->getVocationId() == VOCATION_NONE || target->getVocationId() == VOCATION_NONE) {
		return true;
	}

	if (attacker->getSkull() == SKULL_BLACK && attacker->getSkullClient(target) == SKULL_NONE) {
		return true;
	}

	return false;
}

ReturnValue Combat::canDoCombat(const Creature* attacker, const Creature* target)
{
	if (attacker) {
		if (const Player* targetPlayer = target->getPlayer()) {
			if (targetPlayer->hasFlag(PlayerFlag_CannotBeAttacked)) {
				return RET_YOUMAYNOTATTACKTHISPLAYER;
			}

			if (const Player* attackerPlayer = attacker->getPlayer()) {
				if (attackerPlayer->hasFlag(PlayerFlag_CannotAttackPlayer)) {
					return RET_YOUMAYNOTATTACKTHISPLAYER;
				}

				if (isProtected(attackerPlayer, targetPlayer)) {
					return RET_YOUMAYNOTATTACKTHISPLAYER;
				}

				//nopvp-zone
				const Tile* targetPlayerTile = targetPlayer->getTile();

				if (targetPlayerTile->hasFlag(TILESTATE_NOPVPZONE)) {
					return RET_ACTIONNOTPERMITTEDINANOPVPZONE;
				} else if (attackerPlayer->getTile()->hasFlag(TILESTATE_NOPVPZONE) && !targetPlayerTile->hasFlag(TILESTATE_NOPVPZONE) && !targetPlayerTile->hasFlag(TILESTATE_PROTECTIONZONE)) {
					return RET_ACTIONNOTPERMITTEDINANOPVPZONE;
				}
			}

			if (attacker->isSummon()) {
				if (const Player* masterAttackerPlayer = attacker->getMaster()->getPlayer()) {
					if (masterAttackerPlayer->hasFlag(PlayerFlag_CannotAttackPlayer)) {
						return RET_YOUMAYNOTATTACKTHISPLAYER;
					}

					if (targetPlayer->getTile()->hasFlag(TILESTATE_NOPVPZONE)) {
						return RET_ACTIONNOTPERMITTEDINANOPVPZONE;
					}

					if (isProtected(masterAttackerPlayer, targetPlayer)) {
						return RET_YOUMAYNOTATTACKTHISPLAYER;
					}
				}
			}
		} else if (target->getMonster()) {
			if (const Player* attackerPlayer = attacker->getPlayer()) {
				if (attackerPlayer->hasFlag(PlayerFlag_CannotAttackMonster)) {
					return RET_YOUMAYNOTATTACKTHISCREATURE;
				}

				if (target->isSummon() && target->getMaster()->getPlayer() && target->getZone() == ZONE_NOPVP) {
					return RET_ACTIONNOTPERMITTEDINANOPVPZONE;
				}
			} else if (attacker->getMonster()) {
				const Creature* targetMaster = target->getMaster();

				if (!targetMaster || !targetMaster->getPlayer()) {
					const Creature* attackerMaster = attacker->getMaster();

					if (!attackerMaster || !attackerMaster->getPlayer()) {
						return RET_YOUMAYNOTATTACKTHISCREATURE;
					}
				}
			}
		}

		if (g_game.getWorldType() == WORLD_TYPE_NO_PVP) {
			if (attacker->getPlayer() || (attacker->isSummon() && attacker->getMaster()->getPlayer())) {
				if (target->getPlayer()) {
					if (!isInPvpZone(attacker, target)) {
						return RET_YOUMAYNOTATTACKTHISPLAYER;
					}
				}

				if (target->isSummon() && target->getMaster()->getPlayer()) {
					if (!isInPvpZone(attacker, target)) {
						return RET_YOUMAYNOTATTACKTHISCREATURE;
					}
				}
			}
		}
	}

	return RET_NOERROR;
}

void Combat::setPlayerCombatValues(formulaType_t _type, double _mina, double _minb, double _maxa, double _maxb)
{
	formulaType = _type;
	mina = _mina;
	minb = _minb;
	maxa = _maxa;
	maxb = _maxb;
}

bool Combat::setParam(CombatParam_t param, uint32_t value)
{
	switch (param) {
		case COMBAT_PARAM_TYPE: {
			params.combatType = (CombatType_t)value;
			return true;
		}

		case COMBAT_PARAM_EFFECT: {
			params.impactEffect = (uint8_t)value;
			return true;
		}

		case COMBAT_PARAM_DISTANCEEFFECT: {
			params.distanceEffect = (uint8_t)value;
			return true;
		}

		case COMBAT_PARAM_BLOCKARMOR: {
			params.blockedByArmor = (value != 0);
			return true;
		}

		case COMBAT_PARAM_BLOCKSHIELD: {
			params.blockedByShield = (value != 0);
			return true;
		}

		case COMBAT_PARAM_TARGETCASTERORTOPMOST: {
			params.targetCasterOrTopMost = (value != 0);
			return true;
		}

		case COMBAT_PARAM_CREATEITEM: {
			params.itemId = value;
			return true;
		}

		case COMBAT_PARAM_AGGRESSIVE: {
			params.isAggressive = (value != 0);
			return true;
		}

		case COMBAT_PARAM_DISPEL: {
			params.dispelType = (ConditionType_t)value;
			return true;
		}

		case COMBAT_PARAM_USECHARGES: {
			params.useCharges = (value != 0);
			return true;
		}
	}
	return false;
}

bool Combat::setCallback(CallBackParam_t key)
{
	switch (key) {
		case CALLBACK_PARAM_LEVELMAGICVALUE: {
			delete params.valueCallback;
			params.valueCallback = new ValueCallback(COMBAT_FORMULA_LEVELMAGIC);
			return true;
		}

		case CALLBACK_PARAM_SKILLVALUE: {
			delete params.valueCallback;
			params.valueCallback = new ValueCallback(COMBAT_FORMULA_SKILL);
			return true;
		}

		case CALLBACK_PARAM_TARGETTILE: {
			delete params.tileCallback;
			params.tileCallback = new TileCallback();
			return true;
		}

		case CALLBACK_PARAM_TARGETCREATURE: {
			delete params.targetCallback;
			params.targetCallback = new TargetCallback();
			return true;
		}
	}
	return false;
}

CallBack* Combat::getCallback(CallBackParam_t key)
{
	switch (key) {
		case CALLBACK_PARAM_LEVELMAGICVALUE:
		case CALLBACK_PARAM_SKILLVALUE: {
			return params.valueCallback;
		}

		case CALLBACK_PARAM_TARGETTILE: {
			return params.tileCallback;
		}

		case CALLBACK_PARAM_TARGETCREATURE: {
			return params.targetCallback;
		}
	}
	return nullptr;
}

bool Combat::CombatHealthFunc(Creature* caster, Creature* target, const CombatParams& params, void* data)
{
	CombatDamage damage = *reinterpret_cast<CombatDamage*>(data);

	bool blockPrimary = g_game.combatBlockHit(damage.primary.type, caster, target, damage.primary.value, params.blockedByShield, params.blockedByArmor, params.itemId != 0);
	bool blockSecondary = g_game.combatBlockHit(damage.secondary.type, caster, target, damage.secondary.value, false, false, params.itemId != 0);
	if (blockPrimary) {
		if (blockSecondary) {
			return false;
		}
		damage.primary.value = 0;
	} else if (blockSecondary) {
		damage.secondary.value = 0;
	}

	if ((damage.primary.value < 0 || damage.secondary.value < 0) && caster) {
		Player* targetPlayer = target->getPlayer();
		if (targetPlayer && caster->getPlayer() && targetPlayer->getSkull() != SKULL_BLACK) {
			damage.primary.value /= 2;
			damage.secondary.value /= 2;
		}
	}

	bool result = g_game.combatChangeHealth(caster, target, damage);
	if (result) {
		CombatConditionFunc(caster, target, params, nullptr);
		CombatDispelFunc(caster, target, params, nullptr);
	}

	return result;
}

bool Combat::CombatManaFunc(Creature* caster, Creature* target, const CombatParams& params, void* data)
{
	CombatDamage damage = *reinterpret_cast<CombatDamage*>(data);
	if (damage.primary.value < 0) {
		if (caster && caster->getPlayer() && target->getPlayer()) {
			damage.primary.value /= 2;
		}
	}

	bool result = g_game.combatChangeMana(caster, target, damage.primary.value);
	if (result) {
		CombatConditionFunc(caster, target, params, nullptr);
		CombatDispelFunc(caster, target, params, nullptr);
	}

	return result;
}

bool Combat::CombatConditionFunc(Creature* caster, Creature* target, const CombatParams& params, void*)
{
	bool result = false;
	for (const Condition* condition : params.conditionList) {
		if (caster == target || !target->isImmune(condition->getType())) {
			Condition* conditionCopy = condition->clone();
			if (caster) {
				conditionCopy->setParam(CONDITION_PARAM_OWNER, caster->getID());
			}

			//TODO: infight condition until all aggressive conditions has ended
			result = target->addCombatCondition(conditionCopy);
		}
	}
	return result;
}

bool Combat::CombatDispelFunc(Creature*, Creature* target, const CombatParams& params, void*)
{
	if (!target->hasCondition(params.dispelType)) {
		return false;
	}

	target->removeCombatCondition(params.dispelType);
	return true;
}

bool Combat::CombatnullptrFunc(Creature* caster, Creature* target, const CombatParams& params, void*)
{
	CombatConditionFunc(caster, target, params, nullptr);
	CombatDispelFunc(caster, target, params, nullptr);
	return true;
}

void Combat::combatTileEffects(const SpectatorVec& list, Creature* caster, Tile* tile, const CombatParams& params)
{
	if (params.itemId != 0) {
		uint32_t itemId = params.itemId;

		Player* _caster;
		if (caster) {
			if (caster->getPlayer()) {
				_caster = caster->getPlayer();
			} else if (caster->isSummon()) {
				_caster = caster->getMaster()->getPlayer();
			} else {
				_caster = nullptr;
			}
		} else {
			_caster = nullptr;
		}

		switch (itemId) {
			case ITEM_FIREFIELD_PERSISTENT_FULL:
				itemId = ITEM_FIREFIELD_PVP_FULL;
				break;

			case ITEM_FIREFIELD_PERSISTENT_MEDIUM:
				itemId = ITEM_FIREFIELD_PVP_MEDIUM;
				break;

			case ITEM_FIREFIELD_PERSISTENT_SMALL:
				itemId = ITEM_FIREFIELD_PVP_SMALL;
				break;

			case ITEM_ENERGYFIELD_PERSISTENT:
				itemId = ITEM_ENERGYFIELD_PVP;
				break;

			case ITEM_POISONFIELD_PERSISTENT:
				itemId = ITEM_POISONFIELD_PVP;
				break;

			case ITEM_MAGICWALL_PERSISTENT:
				itemId = ITEM_MAGICWALL;
				break;

			case ITEM_WILDGROWTH_PERSISTENT:
				itemId = ITEM_WILDGROWTH;
				break;

			default:
				break;
		}

		if (_caster) {
			if (g_game.getWorldType() == WORLD_TYPE_NO_PVP || tile->hasFlag(TILESTATE_NOPVPZONE)) {
				if (itemId == ITEM_FIREFIELD_PVP_FULL) {
					itemId = ITEM_FIREFIELD_NOPVP;
				} else if (itemId == ITEM_POISONFIELD_PVP) {
					itemId = ITEM_POISONFIELD_NOPVP;
				} else if (itemId == ITEM_ENERGYFIELD_PVP) {
					itemId = ITEM_ENERGYFIELD_NOPVP;
				}
			} else if (itemId == ITEM_FIREFIELD_PVP_FULL || itemId == ITEM_POISONFIELD_PVP || itemId == ITEM_ENERGYFIELD_PVP) {
				_caster->addInFightTicks(true);
			}
		}

		Item* item = Item::CreateItem(itemId);

		if (caster) {
			item->setOwner(caster->getID());
		}

		ReturnValue ret = g_game.internalAddItem(tile, item);
		if (ret == RET_NOERROR) {
			g_game.startDecay(item);
		} else {
			delete item;
		}
	}

	if (params.tileCallback) {
		params.tileCallback->onTileCombat(caster, tile);
	}

	if (params.impactEffect != CONST_ME_NONE) {
		Game::addMagicEffect(list, tile->getPosition(), params.impactEffect);
	}
}

void Combat::postCombatEffects(Creature* caster, const Position& pos, const CombatParams& params)
{
	if (caster && params.distanceEffect != CONST_ME_NONE) {
		addDistanceEffect(caster, caster->getPosition(), pos, params.distanceEffect);
	}
}

void Combat::addDistanceEffect(Creature* caster, const Position& fromPos, const Position& toPos, uint8_t effect)
{
	if (caster && effect == CONST_ANI_WEAPONTYPE) {
		switch (caster->getWeaponType()) {
			case WEAPON_AXE:
				effect = CONST_ANI_WHIRLWINDAXE;
				break;
			case WEAPON_SWORD:
				effect = CONST_ANI_WHIRLWINDSWORD;
				break;
			case WEAPON_CLUB:
				effect = CONST_ANI_WHIRLWINDCLUB;
				break;
			default:
				effect = CONST_ANI_NONE;
				break;
		}
	}

	if (effect != CONST_ANI_NONE) {
		g_game.addDistanceEffect(fromPos, toPos, effect);
	}
}

void Combat::CombatFunc(Creature* caster, const Position& pos, const AreaCombat* area, const CombatParams& params, COMBATFUNC func, void* data)
{
	std::list<Tile*> tileList;

	if (caster) {
		getCombatArea(caster->getPosition(), pos, area, tileList);
	} else {
		getCombatArea(pos, pos, area, tileList);
	}

	SpectatorVec list;
	uint32_t maxX = 0;
	uint32_t maxY = 0;
	uint32_t diff;

	//calculate the max viewable range
	for (Tile* tile : tileList) {
		const Position& tilePos = tile->getPosition();

		diff = Position::getDistanceX(tilePos, pos);
		if (diff > maxX) {
			maxX = diff;
		}

		diff = Position::getDistanceY(tilePos, pos);
		if (diff > maxY) {
			maxY = diff;
		}
	}

	const int32_t rangeX = maxX + Map::maxViewportX;
	const int32_t rangeY = maxY + Map::maxViewportY;
	g_game.getSpectators(list, pos, true, true, rangeX, rangeX, rangeY, rangeY);

	for (Tile* tile : tileList) {
		if (canDoCombat(caster, tile, params.isAggressive) != RET_NOERROR) {
			continue;
		}

		if (CreatureVector* creatures = tile->getCreatures()) {
			const Creature* topCreature = tile->getTopCreature();
			for (Creature* creature : *creatures) {
				if (params.targetCasterOrTopMost) {
					if (caster && caster->getTile() == tile) {
						if (creature != caster) {
							continue;
						}
					} else if (creature != topCreature) {
						continue;
					}
				}

				if (!params.isAggressive || (caster != creature && Combat::canDoCombat(caster, creature) == RET_NOERROR)) {
					func(caster, creature, params, data);
					if (params.targetCallback) {
						params.targetCallback->onTargetCombat(caster, creature);
					}

					if (params.targetCasterOrTopMost) {
						break;
					}
				}
			}
		}
		combatTileEffects(list, caster, tile, params);
	}
	postCombatEffects(caster, pos, params);
}

void Combat::doCombat(Creature* caster, Creature* target) const
{
	//target combat callback function
	if (params.combatType != COMBAT_NONE) {
		CombatDamage damage = getCombatDamage(caster, target);
		if (damage.primary.type != COMBAT_MANADRAIN) {
			doCombatHealth(caster, target, damage, params);
		} else {
			doCombatMana(caster, target, damage, params);
		}
	} else {
		doCombatDefault(caster, target, params);
	}
}

void Combat::doCombat(Creature* caster, const Position& position) const
{
	//area combat callback function
	if (params.combatType != COMBAT_NONE) {
		CombatDamage damage = getCombatDamage(caster, nullptr);
		if (damage.primary.type != COMBAT_MANADRAIN) {
			doCombatHealth(caster, position, area, damage, params);
		} else {
			doCombatMana(caster, position, area, damage, params);
		}
	} else {
		CombatFunc(caster, position, area, params, CombatnullptrFunc, nullptr);
	}
}

void Combat::doCombatHealth(Creature* caster, Creature* target, CombatDamage& damage, const CombatParams& params)
{
	bool canCombat = !params.isAggressive || (caster != target && Combat::canDoCombat(caster, target) == RET_NOERROR);
	if ((caster == target || canCombat) && params.impactEffect != CONST_ME_NONE) {
		g_game.addMagicEffect(target->getPosition(), params.impactEffect);
	}

	if (canCombat) {
		CombatHealthFunc(caster, target, params, &damage);
		if (params.targetCallback) {
			params.targetCallback->onTargetCombat(caster, target);
		}

		if (caster && params.distanceEffect != CONST_ANI_NONE) {
			addDistanceEffect(caster, caster->getPosition(), target->getPosition(), params.distanceEffect);
		}
	}
}

void Combat::doCombatHealth(Creature* caster, const Position& position, const AreaCombat* area, CombatDamage& damage, const CombatParams& params)
{
	CombatFunc(caster, position, area, params, CombatHealthFunc, &damage);
}

void Combat::doCombatMana(Creature* caster, Creature* target, CombatDamage& damage, const CombatParams& params)
{
	bool canCombat = !params.isAggressive || (caster != target && Combat::canDoCombat(caster, target) == RET_NOERROR);
	if ((caster == target || canCombat) && params.impactEffect != CONST_ME_NONE) {
		g_game.addMagicEffect(target->getPosition(), params.impactEffect);
	}

	if (canCombat) {
		CombatManaFunc(caster, target, params, &damage);
		if (params.targetCallback) {
			params.targetCallback->onTargetCombat(caster, target);
		}

		if (caster && params.distanceEffect != CONST_ANI_NONE) {
			addDistanceEffect(caster, caster->getPosition(), target->getPosition(), params.distanceEffect);
		}
	}
}

void Combat::doCombatMana(Creature* caster, const Position& position, const AreaCombat* area, CombatDamage& damage, const CombatParams& params)
{
	CombatFunc(caster, position, area, params, CombatManaFunc, &damage);
}

void Combat::doCombatCondition(Creature* caster, const Position& position, const AreaCombat* area, const CombatParams& params)
{
	CombatFunc(caster, position, area, params, CombatConditionFunc, nullptr);
}

void Combat::doCombatCondition(Creature* caster, Creature* target, const CombatParams& params)
{
	bool canCombat = !params.isAggressive || (caster != target && Combat::canDoCombat(caster, target) == RET_NOERROR);
	if ((caster == target || canCombat) && params.impactEffect != CONST_ME_NONE) {
		g_game.addMagicEffect(target->getPosition(), params.impactEffect);
	}

	if (canCombat) {
		CombatConditionFunc(caster, target, params, nullptr);
		if (params.targetCallback) {
			params.targetCallback->onTargetCombat(caster, target);
		}

		if (caster && params.distanceEffect != CONST_ME_NONE) {
			addDistanceEffect(caster, caster->getPosition(), target->getPosition(), params.distanceEffect);
		}
	}
}

void Combat::doCombatDispel(Creature* caster, const Position& position, const AreaCombat* area, const CombatParams& params)
{
	CombatFunc(caster, position, area, params, CombatDispelFunc, nullptr);
}

void Combat::doCombatDispel(Creature* caster, Creature* target, const CombatParams& params)
{
	bool canCombat = !params.isAggressive || (caster != target && Combat::canDoCombat(caster, target) == RET_NOERROR);
	if ((caster == target || canCombat) && params.impactEffect != CONST_ME_NONE) {
		g_game.addMagicEffect(target->getPosition(), params.impactEffect);
	}

	if (canCombat) {
		CombatDispelFunc(caster, target, params, nullptr);
		if (params.targetCallback) {
			params.targetCallback->onTargetCombat(caster, target);
		}

		if (caster && params.distanceEffect != CONST_ME_NONE) {
			addDistanceEffect(caster, caster->getPosition(), target->getPosition(), params.distanceEffect);
		}
	}
}

void Combat::doCombatDefault(Creature* caster, Creature* target, const CombatParams& params)
{
	if (!params.isAggressive || (caster != target && Combat::canDoCombat(caster, target) == RET_NOERROR)) {
		SpectatorVec list;
		g_game.getSpectators(list, target->getPosition(), true, true);

		CombatnullptrFunc(caster, target, params, nullptr);
		combatTileEffects(list, caster, target->getTile(), params);

		if (params.targetCallback) {
			params.targetCallback->onTargetCombat(caster, target);
		}

		/*
		if (params.impactEffect != CONST_ME_NONE) {
			g_game.addMagicEffect(target->getPosition(), params.impactEffect);
		}
		*/

		if (caster && params.distanceEffect != CONST_ME_NONE) {
			addDistanceEffect(caster, caster->getPosition(), target->getPosition(), params.distanceEffect);
		}
	}
}

//**********************************************************//

void ValueCallback::getMinMaxValues(Player* player, CombatDamage& damage, bool useCharges) const
{
	//"onGetPlayerMinMaxValues"(...)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - ValueCallback::getMinMaxValues] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	if (!env->setCallbackId(m_scriptId, m_scriptInterface)) {
		m_scriptInterface->resetScriptEnv();
		return;
	}

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	lua_pushnumber(L, player->getID());

	int32_t parameters = 1;
	switch (type) {
		case COMBAT_FORMULA_LEVELMAGIC: {
			//"onGetPlayerMinMaxValues"(cid, level, maglevel)
			lua_pushnumber(L, player->getLevel());
			lua_pushnumber(L, player->getMagicLevel());
			parameters += 2;
			break;
		}

		case COMBAT_FORMULA_SKILL: {
			//"onGetPlayerMinMaxValues"(cid, attackSkill, attackValue, attackFactor)
			Item* tool = player->getWeapon();
			const Weapon* weapon = g_weapons->getWeapon(tool);

			int32_t attackValue = 7;
			if (weapon) {
				attackValue = tool->getAttack();

				damage.secondary.type = weapon->getElementType();
				damage.secondary.value = weapon->getElementDamage(player, nullptr, tool);
				if (useCharges) {
					uint16_t charges = tool->getCharges();
					if (charges != 0) {
						g_game.transformItem(tool, tool->getID(), charges - 1);
					}
				}
			}

			lua_pushnumber(L, player->getWeaponSkill(tool));
			lua_pushnumber(L, attackValue);
			lua_pushnumber(L, player->getAttackFactor());
			parameters += 3;
			break;
		}

		default: {
			std::cout << "ValueCallback::getMinMaxValues - unknown callback type" << std::endl;
			m_scriptInterface->resetScriptEnv();
			return;
		}
	}

	int32_t size0 = lua_gettop(L);
	if (lua_pcall(L, parameters, 2, 0) != 0) {
		LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
	} else {
		damage.primary.value = normal_random(
			LuaScriptInterface::getNumber<int32_t>(L, -2),
			LuaScriptInterface::getNumber<int32_t>(L, -1)
		);
		lua_pop(L, 2);
	}

	if ((lua_gettop(L) + parameters + 1) != size0) {
		LuaScriptInterface::reportError(nullptr, "Stack size changed!");
	}

	m_scriptInterface->resetScriptEnv();
}

//**********************************************************//

void TileCallback::onTileCombat(Creature* creature, Tile* tile) const
{
	//"onTileCombat"(cid, pos)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - TileCallback::onTileCombat] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	if (!env->setCallbackId(m_scriptId, m_scriptInterface)) {
		m_scriptInterface->resetScriptEnv();
		return;
	}

	lua_State* L = m_scriptInterface->getLuaState();

	uint32_t cid;
	if (creature) {
		cid = creature->getID();
	} else {
		cid = 0;
	}

	m_scriptInterface->pushFunction(m_scriptId);
	lua_pushnumber(L, cid);
	LuaScriptInterface::pushPosition(L, tile->getPosition());

	m_scriptInterface->callFunction(2);
}

//**********************************************************//

void TargetCallback::onTargetCombat(Creature* creature, Creature* target) const
{
	//"onTargetCombat"(cid, target)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - TargetCallback::onTargetCombat] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	if (!env->setCallbackId(m_scriptId, m_scriptInterface)) {
		m_scriptInterface->resetScriptEnv();
		return;
	}

	lua_State* L = m_scriptInterface->getLuaState();

	uint32_t cid;
	if (creature) {
		cid = creature->getID();
	} else {
		cid = 0;
	}

	uint32_t targetCid;
	if (target) {
		targetCid = target->getID();
	} else {
		targetCid = 0;
	}

	m_scriptInterface->pushFunction(m_scriptId);
	lua_pushnumber(L, cid);
	lua_pushnumber(L, targetCid);

	int32_t size0 = lua_gettop(L);

	if (lua_pcall(L, 2, 0 /*nReturnValues*/, 0) != 0) {
		LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
	}

	if ((lua_gettop(L) + 2 /*nParams*/ + 1) != size0) {
		LuaScriptInterface::reportError(nullptr, "Stack size changed!");
	}

	m_scriptInterface->resetScriptEnv();
}

//**********************************************************//

void AreaCombat::clear()
{
	for (const auto& it : areas) {
		delete it.second;
	}
	areas.clear();
}

AreaCombat::AreaCombat(const AreaCombat& rhs)
{
	hasExtArea = rhs.hasExtArea;
	for (const auto& it : rhs.areas) {
		areas[it.first] = new MatrixArea(*it.second);
	}
}

void AreaCombat::getList(const Position& centerPos, const Position& targetPos, std::list<Tile*>& list) const
{
	const MatrixArea* area = getArea(centerPos, targetPos);
	if (!area) {
		return;
	}

	uint32_t centerY, centerX;
	area->getCenter(centerY, centerX);

	Position tmpPos(targetPos.x - centerX, targetPos.y - centerY, targetPos.z);
	uint32_t cols = area->getCols();
	for (uint32_t y = 0, rows = area->getRows(); y < rows; ++y) {
		for (uint32_t x = 0; x < cols; ++x) {
			if (area->getValue(y, x) != 0) {
				if (g_game.isSightClear(targetPos, tmpPos, true)) {
					Tile* tile = g_game.getTile(tmpPos);
					if (!tile) {
						tile = new StaticTile(tmpPos.x, tmpPos.y, tmpPos.z);
						g_game.setTile(tile);
					}
					list.push_back(tile);
				}
			}
			tmpPos.x++;
		}
		tmpPos.x -= cols;
		tmpPos.y++;
	}
}

void AreaCombat::copyArea(const MatrixArea* input, MatrixArea* output, MatrixOperation_t op) const
{
	uint32_t centerY, centerX;
	input->getCenter(centerY, centerX);

	if (op == MATRIXOPERATION_COPY) {
		for (uint32_t y = 0; y < input->getRows(); ++y) {
			for (uint32_t x = 0; x < input->getCols(); ++x) {
				(*output)[y][x] = (*input)[y][x];
			}
		}

		output->setCenter(centerY, centerX);
	} else if (op == MATRIXOPERATION_MIRROR) {
		for (uint32_t y = 0; y < input->getRows(); ++y) {
			uint32_t rx = 0;
			for (int32_t x = input->getCols(); --x >= 0;) {
				(*output)[y][rx++] = (*input)[y][x];
			}
		}

		output->setCenter(centerY, (input->getRows() - 1) - centerX);
	} else if (op == MATRIXOPERATION_FLIP) {
		for (uint32_t x = 0; x < input->getCols(); ++x) {
			uint32_t ry = 0;
			for (int32_t y = input->getRows(); --y >= 0;) {
				(*output)[ry++][x] = (*input)[y][x];
			}
		}

		output->setCenter((input->getCols() - 1) - centerY, centerX);
	} else {
		// rotation
		int32_t rotateCenterX = (output->getCols() / 2) - 1;
		int32_t rotateCenterY = (output->getRows() / 2) - 1;
		int32_t angle;

		switch (op) {
			case MATRIXOPERATION_ROTATE90:
				angle = 90;
				break;

			case MATRIXOPERATION_ROTATE180:
				angle = 180;
				break;

			case MATRIXOPERATION_ROTATE270:
				angle = 270;
				break;

			default:
				angle = 0;
				break;
		}

		double angleRad = M_PI * angle / 180.0;

		double a = std::cos(angleRad);
		double b = -std::sin(angleRad);
		double c = std::sin(angleRad);
		double d = std::cos(angleRad);

		const uint32_t rows = input->getRows();
		for (uint32_t x = 0, cols = input->getCols(); x < cols; ++x) {
			for (uint32_t y = 0; y < rows; ++y) {
				//calculate new coordinates using rotation center
				int32_t newX = x - centerX;
				int32_t newY = y - centerY;

				//perform rotation
				int32_t rotatedX = static_cast<int32_t>(round(newX * a + newY * b));
				int32_t rotatedY = static_cast<int32_t>(round(newX * c + newY * d));

				//write in the output matrix using rotated coordinates
				(*output)[rotatedY + rotateCenterY][rotatedX + rotateCenterX] = (*input)[y][x];
			}
		}

		output->setCenter(rotateCenterY, rotateCenterX);
	}
}

MatrixArea* AreaCombat::createArea(const std::list<uint32_t>& list, uint32_t rows)
{
	uint32_t cols;
	if (rows == 0) {
		cols = 0;
	} else {
		cols = list.size() / rows;
	}

	MatrixArea* area = new MatrixArea(rows, cols);

	uint32_t x = 0;
	uint32_t y = 0;

	for (uint32_t value : list) {
		if (value == 1 || value == 3) {
			area->setValue(y, x, true);
		}

		if (value == 2 || value == 3) {
			area->setCenter(y, x);
		}

		++x;

		if (cols == x) {
			x = 0;
			++y;
		}
	}
	return area;
}

void AreaCombat::setupArea(const std::list<uint32_t>& list, uint32_t rows)
{
	MatrixArea* area = createArea(list, rows);

	//NORTH
	areas[NORTH] = area;

	uint32_t maxOutput = std::max<uint32_t>(area->getCols(), area->getRows()) * 2;

	//SOUTH
	MatrixArea* southArea = new MatrixArea(maxOutput, maxOutput);
	copyArea(area, southArea, MATRIXOPERATION_ROTATE180);
	areas[SOUTH] = southArea;

	//EAST
	MatrixArea* eastArea = new MatrixArea(maxOutput, maxOutput);
	copyArea(area, eastArea, MATRIXOPERATION_ROTATE90);
	areas[EAST] = eastArea;

	//WEST
	MatrixArea* westArea = new MatrixArea(maxOutput, maxOutput);
	copyArea(area, westArea, MATRIXOPERATION_ROTATE270);
	areas[WEST] = westArea;
}

void AreaCombat::setupArea(int32_t length, int32_t spread)
{
	std::list<uint32_t> list;

	uint32_t rows = length;
	int32_t cols = 1;

	if (spread != 0) {
		cols = ((length - (length % spread)) / spread) * 2 + 1;
	}

	int32_t colSpread = cols;

	for (uint32_t y = 1; y <= rows; ++y) {
		int32_t mincol = cols - colSpread + 1;
		int32_t maxcol = cols - (cols - colSpread);

		for (int32_t x = 1; x <= cols; ++x) {
			if (y == rows && x == ((cols - (cols % 2)) / 2) + 1) {
				list.push_back(3);
			} else if (x >= mincol && x <= maxcol) {
				list.push_back(1);
			} else {
				list.push_back(0);
			}
		}

		if (spread > 0 && y % spread == 0) {
			--colSpread;
		}
	}

	setupArea(list, rows);
}

void AreaCombat::setupArea(int32_t radius)
{
	int32_t area[13][13] = {
		{0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 8, 8, 7, 8, 8, 0, 0, 0, 0},
		{0, 0, 0, 8, 7, 6, 6, 6, 7, 8, 0, 0, 0},
		{0, 0, 8, 7, 6, 5, 5, 5, 6, 7, 8, 0, 0},
		{0, 8, 7, 6, 5, 4, 4, 4, 5, 6, 7, 8, 0},
		{0, 8, 6, 5, 4, 3, 2, 3, 4, 5, 6, 8, 0},
		{8, 7, 6, 5, 4, 2, 1, 2, 4, 5, 6, 7, 8},
		{0, 8, 6, 5, 4, 3, 2, 3, 4, 5, 6, 8, 0},
		{0, 8, 7, 6, 5, 4, 4, 4, 5, 6, 7, 8, 0},
		{0, 0, 8, 7, 6, 5, 5, 5, 6, 7, 8, 0, 0},
		{0, 0, 0, 8, 7, 6, 6, 6, 7, 8, 0, 0, 0},
		{0, 0, 0, 0, 8, 8, 7, 8, 8, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0}
	};

	std::list<uint32_t> list;

	for (int32_t y = 0; y < 13; ++y) {
		for (int32_t x = 0; x < 13; ++x) {
			if (area[y][x] == 1) {
				list.push_back(3);
			} else if (area[y][x] > 0 && area[y][x] <= radius) {
				list.push_back(1);
			} else {
				list.push_back(0);
			}
		}
	}

	setupArea(list, 13);
}

void AreaCombat::setupExtArea(const std::list<uint32_t>& list, uint32_t rows)
{
	if (list.empty()) {
		return;
	}

	hasExtArea = true;
	MatrixArea* area = createArea(list, rows);

	//NORTH-WEST
	areas[NORTHWEST] = area;

	uint32_t maxOutput = std::max<uint32_t>(area->getCols(), area->getRows()) * 2;

	//NORTH-EAST
	MatrixArea* neArea = new MatrixArea(maxOutput, maxOutput);
	copyArea(area, neArea, MATRIXOPERATION_MIRROR);
	areas[NORTHEAST] = neArea;

	//SOUTH-WEST
	MatrixArea* swArea = new MatrixArea(maxOutput, maxOutput);
	copyArea(area, swArea, MATRIXOPERATION_FLIP);
	areas[SOUTHWEST] = swArea;

	//SOUTH-EAST
	MatrixArea* seArea = new MatrixArea(maxOutput, maxOutput);
	copyArea(swArea, seArea, MATRIXOPERATION_MIRROR);
	areas[SOUTHEAST] = seArea;
}

//**********************************************************//

void MagicField::onStepInField(Creature* creature)
{
	//remove magic walls/wild growth
	if (id == ITEM_MAGICWALL || id == ITEM_WILDGROWTH || id == ITEM_MAGICWALL_SAFE || id == ITEM_WILDGROWTH_SAFE || isBlocking()) {
		if (!creature->isInGhostMode()) {
			g_game.internalRemoveItem(this, 1);
		}

		return;
	}

	const ItemType& it = items[getID()];
	if (it.condition) {
		Condition* conditionCopy = it.condition->clone();
		uint32_t ownerId = getOwner();
		if (ownerId) {
			bool harmfulField = true;

			if (g_game.getWorldType() == WORLD_TYPE_NO_PVP || getTile()->hasFlag(TILESTATE_NOPVPZONE)) {
				Creature* owner = g_game.getCreatureByID(ownerId);
				if (owner) {
					if (owner->getPlayer() || (owner->isSummon() && owner->getMaster()->getPlayer())) {
						harmfulField = false;
					}
				}
			}

			Player* targetPlayer = creature->getPlayer();
			if (targetPlayer) {
				Player* attackerPlayer = g_game.getPlayerByID(ownerId);
				if (attackerPlayer) {
					if (Combat::isProtected(attackerPlayer, targetPlayer)) {
						harmfulField = false;
					}
				}
			}

			if (!harmfulField || (OTSYS_TIME() - createTime <= 5000) || creature->hasBeenAttacked(ownerId)) {
				conditionCopy->setParam(CONDITION_PARAM_OWNER, ownerId);
			}
		}

		creature->addCondition(conditionCopy);
	}
}
