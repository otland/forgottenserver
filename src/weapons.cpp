/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#include "definitions.h"
#include "weapons.h"
#include "combat.h"
#include "tools.h"
#include "configmanager.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

extern Game g_game;
extern Vocations g_vocations;
extern ConfigManager g_config;
extern Weapons* g_weapons;

Weapons::Weapons():
	m_scriptInterface("Weapon Interface")
{
	m_scriptInterface.initState();
}

Weapons::~Weapons()
{
	clear();
}

const Weapon* Weapons::getWeapon(const Item* item) const
{
	if (!item) {
		return NULL;
	}

	WeaponMap::const_iterator it = weapons.find(item->getID());

	if (it != weapons.end()) {
		return it->second;
	}

	return NULL;
}

void Weapons::clear()
{
	WeaponMap::iterator it;

	for (it = weapons.begin(); it != weapons.end(); ++it) {
		delete it->second;
	}

	weapons.clear();
}

LuaScriptInterface& Weapons::getScriptInterface()
{
	return m_scriptInterface;
}

std::string Weapons::getScriptBaseName()
{
	return "weapons";
}

void Weapons::loadDefaults()
{
	for (size_t i = 100, size = Item::items.size(); i < size; ++i) {
		const ItemType& it = Item::items.getItemType(i);
		if (it.id == 0 || weapons.find(i) != weapons.end()) {
			continue;
		}

		if (it.weaponType != WEAPON_NONE) {
			switch (it.weaponType) {
				case WEAPON_AXE:
				case WEAPON_SWORD:
				case WEAPON_CLUB: {
					WeaponMelee* weapon = new WeaponMelee(&m_scriptInterface);
					weapon->configureWeapon(it);
					weapons[i] = weapon;
					break;
				}

				case WEAPON_AMMO:
				case WEAPON_DIST: {
					if (it.weaponType == WEAPON_DIST && it.ammoType != AMMO_NONE) {
						continue;
					}

					WeaponDistance* weapon = new WeaponDistance(&m_scriptInterface);
					weapon->configureWeapon(it);
					weapons[i] = weapon;
					break;
				}

				default:
					break;
			}
		}
	}
}

Event* Weapons::getEvent(const std::string& nodeName)
{
	std::string tmpNodeName = asLowerCaseString(nodeName);

	if (tmpNodeName == "melee") {
		return new WeaponMelee(&m_scriptInterface);
	} else if (tmpNodeName == "distance") {
		return new WeaponDistance(&m_scriptInterface);
	} else if (tmpNodeName == "wand" || tmpNodeName == "rod") {
		return new WeaponWand(&m_scriptInterface);
	}

	return NULL;
}

bool Weapons::registerEvent(Event* event, xmlNodePtr p)
{
	Weapon* weapon = dynamic_cast<Weapon*>(event);

	if (weapon) {
		if (weapons.find(weapon->getID()) != weapons.end()) {
			std::cout << "[Warning - Weapons::registerEvent] Duplicate registered item with id: " << weapon->getID() << std::endl;
			return false;
		}

		weapons[weapon->getID()] = weapon;
		return true;
	}

	return false;
}

//monsters
int32_t Weapons::getMaxMeleeDamage(int32_t attackSkill, int32_t attackValue)
{
	return ((int32_t)ceil((attackSkill * (attackValue * 0.05)) + (attackValue * 0.5)));
}

//players
int32_t Weapons::getMaxWeaponDamage(uint32_t level, int32_t attackSkill, int32_t attackValue, float attackFactor)
{
	return (int32_t)ceil((2 * (attackValue * (attackSkill + 5.8) / 25 + (level - 1) / 10.)) / attackFactor);
}

Weapon::Weapon(LuaScriptInterface* _interface) :
	Event(_interface)
{
	m_scripted = false;
	id = 0;
	level = 0;
	magLevel = 0;
	mana = 0;
	manaPercent = 0;
	soul = 0;
	exhaustion = 0;
	premium = false;
	enabled = true;
	wieldUnproperly = false;
	range = 1;
	ammoAction = AMMOACTION_NONE;
}

Weapon::~Weapon()
{
	//
}

void Weapon::setCombatParam(const CombatParams& _params)
{
	params = _params;
}

bool Weapon::configureEvent(xmlNodePtr p)
{
	int32_t intValue;
	std::string strValue;

	if (readXMLInteger(p, "id", intValue)) {
		id = intValue;
	} else {
		std::cout << "Error: [Weapon::configureEvent] Weapon without id." << std::endl;
		return false;
	}

	if (readXMLInteger(p, "lvl", intValue) || readXMLInteger(p, "level", intValue)) {
		level = intValue;
	}

	if (readXMLInteger(p, "maglv", intValue) || readXMLInteger(p, "maglevel", intValue)) {
		magLevel = intValue;
	}

	if (readXMLInteger(p, "mana", intValue)) {
		mana = intValue;
	}

	if (readXMLInteger(p, "manapercent", intValue)) {
		manaPercent = intValue;
	}

	if (readXMLInteger(p, "soul", intValue)) {
		soul = intValue;
	}

	if (readXMLInteger(p, "exhaustion", intValue)) {
		exhaustion = intValue;
	}

	if (readXMLInteger(p, "prem", intValue)) {
		premium = (intValue == 1);
	}

	if (readXMLInteger(p, "enabled", intValue)) {
		enabled = (intValue == 1);
	}

	if (readXMLInteger(p, "unproperly", intValue)) {
		wieldUnproperly = (intValue == 1);
	}

	if (readXMLString(p, "ammo", strValue)) {
		std::cout << "Warning: ammo is not longer used in weapons.xml." << std::endl;
	}

	typedef std::list<std::string> STRING_LIST;
	STRING_LIST vocStringList;
	xmlNodePtr vocationNode = p->children;

	while (vocationNode) {
		if (xmlStrcmp(vocationNode->name, (const xmlChar*)"vocation") == 0) {
			if (readXMLString(vocationNode, "name", strValue)) {
				int32_t vocationId = g_vocations.getVocationId(strValue);

				if (vocationId != -1) {
					vocWeaponMap[vocationId] = true;
					int32_t promotedVocation = g_vocations.getPromotedVocation(vocationId);

					if (promotedVocation != 0) {
						vocWeaponMap[promotedVocation] = true;
					}

					readXMLInteger(vocationNode, "showInDescription", intValue);

					if (intValue != 0) {
						toLowerCaseString(strValue);
						vocStringList.push_back(strValue);
					}
				}
			}
		}

		vocationNode = vocationNode->next;
	}

	range = Item::items[id].shootRange;

	std::string vocationString;

	if (!vocStringList.empty()) {
		for (STRING_LIST::iterator it = vocStringList.begin(); it != vocStringList.end(); ++it) {
			if (*it != vocStringList.front()) {
				if (*it != vocStringList.back()) {
					vocationString += ", ";
				} else {
					vocationString += " and ";
				}
			}

			vocationString += *it;
			vocationString += "s";
		}
	}

	uint32_t wieldInfo = 0;

	if (getReqLevel() > 0) {
		wieldInfo |= WIELDINFO_LEVEL;
	}

	if (getReqMagLv() > 0) {
		wieldInfo |= WIELDINFO_MAGLV;
	}

	if (!vocationString.empty()) {
		wieldInfo |= WIELDINFO_VOCREQ;
	}

	if (isPremium()) {
		wieldInfo |= WIELDINFO_PREMIUM;
	}

	if (wieldInfo != 0) {
		ItemType& it = Item::items.getItemType(id);
		it.wieldInfo = wieldInfo;
		it.vocationString = vocationString;
		it.minReqLevel = getReqLevel();
		it.minReqMagicLevel = getReqMagLv();
	}

	if (configureWeapon(Item::items[getID()])) {
		return true;
	}

	return false;
}

bool Weapon::loadFunction(const std::string& functionName)
{
	std::string tmpFunctionName = asLowerCaseString(functionName);

	if (tmpFunctionName == "internalloadweapon" || tmpFunctionName == "default") {
		if (configureWeapon(Item::items[getID()])) {
			return true;
		}
	} else if (tmpFunctionName == "script") {
		m_scripted = true;
	}

	return false;
}

bool Weapon::configureWeapon(const ItemType& it)
{
	id = it.id;
	return true;
}

std::string Weapon::getScriptEventName()
{
	return "onUseWeapon";
}

int32_t Weapon::playerWeaponCheck(Player* player, Creature* target) const
{
	const Position& playerPos = player->getPosition();
	const Position& targetPos = target->getPosition();

	if (playerPos.z != targetPos.z) {
		return 0;
	}

	int32_t trueRange;
	const ItemType& it = Item::items[getID()];

	if (it.weaponType == WEAPON_AMMO) {
		trueRange = player->getShootRange();
	} else {
		trueRange = range;
	}

	if (std::max<int32_t>(Position::getDistanceX(playerPos, targetPos), Position::getDistanceY(playerPos, targetPos)) > trueRange) {
		return 0;
	}

	if (!player->hasFlag(PlayerFlag_IgnoreWeaponCheck)) {
		if (!enabled) {
			return 0;
		}

		if (player->getMana() < getManaCost(player)) {
			return 0;
		}

		if (player->getPlayerInfo(PLAYERINFO_SOUL) < soul) {
			return 0;
		}

		if (isPremium() && !player->isPremium()) {
			return 0;
		}

		if (!vocWeaponMap.empty()) {
			if (vocWeaponMap.find(player->getVocationId()) == vocWeaponMap.end()) {
				return 0;
			}
		}

		int32_t damageModifier = 100;

		if (player->getLevel() < getReqLevel()) {
			damageModifier = (isWieldedUnproperly() ? damageModifier / 2 : 0);
		}

		if (player->getMagicLevel() < getReqMagLv()) {
			damageModifier = (isWieldedUnproperly() ? damageModifier / 2 : 0);
		}

		return damageModifier;
	}

	return 100;
}

bool Weapon::useWeapon(Player* player, Item* item, Creature* target) const
{
	int32_t damageModifier = playerWeaponCheck(player, target);

	if (damageModifier == 0) {
		return false;
	}

	return internalUseWeapon(player, item, target, damageModifier);
}

bool Weapon::useFist(Player* player, Creature* target)
{
	const Position& playerPos = player->getPosition();
	const Position& targetPos = target->getPosition();

	if (Position::areInRange<1, 1>(playerPos, targetPos)) {
		float attackFactor = player->getAttackFactor();
		int32_t attackSkill = player->getSkill(SKILL_FIST, SKILL_LEVEL);
		int32_t attackValue = 7;

		int32_t maxDamage = Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor);

		if (random_range(1, 100) <= g_config.getNumber(ConfigManager::CRITICAL_HIT_CHANCE)) {
			maxDamage <<= 1;
		}

		int32_t damage = -random_range(0, maxDamage, DISTRO_NORMAL);

		CombatParams params;
		params.combatType = COMBAT_PHYSICALDAMAGE;
		params.blockedByArmor = true;
		params.blockedByShield = true;
		Combat::doCombatHealth(player, target, damage, damage, params);

		if (!player->hasFlag(PlayerFlag_NotGainSkill) && player->getAddAttackSkill()) {
			player->addSkillAdvance(SKILL_FIST, 1);
		}

		return true;
	}

	return false;
}

bool Weapon::internalUseWeapon(Player* player, Item* item, Creature* target, int32_t damageModifier) const
{
	if (m_scripted) {
		LuaVariant var;
		var.type = VARIANT_NUMBER;
		var.number = target->getID();
		executeUseWeapon(player, var);
	} else {
		int32_t damage = (getWeaponDamage(player, target, item) * damageModifier) / 100;
		Combat::doCombatHealth(player, target, damage, damage, params);
	}

	onUsedAmmo(player, item, target->getTile());
	onUsedWeapon(player, item, target->getTile());
	return true;
}

bool Weapon::internalUseWeapon(Player* player, Item* item, Tile* tile) const
{
	if (m_scripted) {
		LuaVariant var;
		var.type = VARIANT_TARGETPOSITION;
		var.pos = tile->getPosition();
		executeUseWeapon(player, var);
	} else {
		Combat::postCombatEffects(player, tile->getPosition(), params);
		g_game.addMagicEffect(tile->getPosition(), NM_ME_POFF);
	}

	onUsedAmmo(player, item, tile);
	onUsedWeapon(player, item, tile);
	return true;
}

void Weapon::onUsedWeapon(Player* player, Item* item, Tile* destTile) const
{
	if (!player->hasFlag(PlayerFlag_NotGainSkill)) {
		skills_t skillType;
		uint32_t skillPoint = 0;

		if (getSkillType(player, item, skillType, skillPoint)) {
			player->addSkillAdvance(skillType, skillPoint);
		}
	}

	if (!player->hasFlag(PlayerFlag_HasNoExhaustion) && exhaustion > 0) {
		player->addWeaponExhaust(exhaustion);
	}

	int32_t manaCost = getManaCost(player);

	if (manaCost > 0) {
		player->addManaSpent(manaCost);
		player->changeMana(-manaCost);
	}

	if (!player->hasFlag(PlayerFlag_HasInfiniteSoul) && soul > 0) {
		player->changeSoul(-soul);
	}
}

void Weapon::onUsedAmmo(Player* player, Item* item, Tile* destTile) const
{
	if (!g_config.getBoolean(ConfigManager::REMOVE_AMMO)) {
		return;
	}

	if (ammoAction == AMMOACTION_REMOVECOUNT) {
		uint16_t newCount = item->getItemCount();

		if (newCount > 0) {
			newCount--;
		}

		g_game.transformItem(item, item->getID(), newCount);
	} else if (ammoAction == AMMOACTION_REMOVECHARGE) {
		uint16_t newCharge = item->getCharges();

		if (newCharge > 0) {
			newCharge--;
		}

		g_game.transformItem(item, item->getID(), newCharge);
	} else if (ammoAction == AMMOACTION_MOVE) {
		g_game.internalMoveItem(item->getParent(), destTile, INDEX_WHEREEVER, item, 1, NULL, FLAG_NOLIMIT);
	} else if (ammoAction == AMMOACTION_MOVEBACK) {
		/* do nothing */
	} else if (item->hasCharges()) {
		uint16_t newCharge = item->getCharges();

		if (newCharge > 0) {
			newCharge--;
		}

		g_game.transformItem(item, item->getID(), newCharge);
	}
}

int32_t Weapon::getManaCost(const Player* player) const
{
	if (mana != 0) {
		return mana;
	}

	if (manaPercent == 0) {
		return 0;
	}

	return (player->getMaxMana() * manaPercent) / 100;
}

bool Weapon::executeUseWeapon(Player* player, const LuaVariant& var) const
{
	//onUseWeapon(cid, var)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - Weapon::executeUseWeapon] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	lua_pushnumber(L, player->getID());
	m_scriptInterface->pushVariant(L, var);

	return m_scriptInterface->callFunction(2);
}

WeaponMelee::WeaponMelee(LuaScriptInterface* _interface) :
	Weapon(_interface)
{
	params.blockedByArmor = true;
	params.blockedByShield = true;
	params.combatType = COMBAT_PHYSICALDAMAGE;
	elementType = COMBAT_NONE;
	elementDamage = 0;
}

bool WeaponMelee::configureEvent(xmlNodePtr p)
{
	if (!Weapon::configureEvent(p)) {
		return false;
	}

	return true;
}

bool WeaponMelee::configureWeapon(const ItemType& it)
{
	if (it.abilities) {
		elementType = it.abilities->elementType;
		elementDamage = it.abilities->elementDamage;
	}

	return Weapon::configureWeapon(it);
}

bool WeaponMelee::useWeapon(Player* player, Item* item, Creature* target) const
{
	int32_t damageModifier = playerWeaponCheck(player, target);

	if (damageModifier == 0) {
		return false;
	}

	if (elementDamage != 0) {
		int32_t damage = getElementDamage(player, item);
		CombatParams eParams;
		eParams.combatType = elementType;
		eParams.isAggressive = true;
		eParams.useCharges = true;
		Combat::doCombatHealth(player, target, damage, damage, eParams);
	}

	return internalUseWeapon(player, item, target, damageModifier);
}

void WeaponMelee::onUsedWeapon(Player* player, Item* item, Tile* destTile) const
{
	Weapon::onUsedWeapon(player, item, destTile);
}

void WeaponMelee::onUsedAmmo(Player* player, Item* item, Tile* destTile) const
{
	Weapon::onUsedAmmo(player, item, destTile);
}

bool WeaponMelee::getSkillType(const Player* player, const Item* item,
                               skills_t& skill, uint32_t& skillpoint) const
{
	skillpoint = 0;

	if (player->getAddAttackSkill() && player->getLastAttackBlockType() != BLOCK_IMMUNITY) {
		skillpoint = 1;
	}

	WeaponType_t weaponType = item->getWeaponType();

	switch (weaponType) {
		case WEAPON_SWORD: {
			skill = SKILL_SWORD;
			return true;
		}

		case WEAPON_CLUB: {
			skill = SKILL_CLUB;
			return true;
		}

		case WEAPON_AXE: {
			skill = SKILL_AXE;
			return true;
		}

		default:
			break;
	}

	return false;
}

int32_t WeaponMelee::getElementDamage(const Player* player, const Item* item) const
{
	int32_t attackSkill = player->getWeaponSkill(item);
	int32_t attackValue = std::max<int32_t>(0, elementDamage);
	float attackFactor = player->getAttackFactor();

	int32_t maxValue = Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor);

	if (random_range(1, 100) <= g_config.getNumber(ConfigManager::CRITICAL_HIT_CHANCE)) {
		maxValue <<= 1;
	}

	maxValue = int32_t(maxValue * player->getVocation()->meleeDamageMultiplier);
	return -random_range(0, maxValue, DISTRO_NORMAL);
}

int32_t WeaponMelee::getWeaponDamage(const Player* player, const Creature* target, const Item* item, bool maxDamage /*= false*/) const
{
	int32_t attackSkill = player->getWeaponSkill(item);
	int32_t attackValue = std::max<int32_t>(0, item->getAttack());
	float attackFactor = player->getAttackFactor();

	int32_t maxValue = Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor);

	if (random_range(1, 100) <= g_config.getNumber(ConfigManager::CRITICAL_HIT_CHANCE)) {
		maxValue <<= 1;
	}

	maxValue = int32_t(maxValue * player->getVocation()->meleeDamageMultiplier);

	if (maxDamage) {
		return -maxValue;
	}

	return -random_range(0, maxValue, DISTRO_NORMAL);
}

WeaponDistance::WeaponDistance(LuaScriptInterface* _interface) :
	Weapon(_interface)
{
	hitChance = 0;
	maxHitChance = 0;
	breakChance = 0;
	ammuAttackValue = 0;
	params.blockedByArmor = true;
	params.combatType = COMBAT_PHYSICALDAMAGE;
	elementType = COMBAT_NONE;
	elementDamage = 0;
}

bool WeaponDistance::configureEvent(xmlNodePtr p)
{
	if (!Weapon::configureEvent(p)) {
		return false;
	}

	const ItemType& it = Item::items[id];

	//default values
	if (it.ammoType != AMMO_NONE) {
		//hit chance on two-handed weapons is limited to 90%
		maxHitChance = 90;
	} else {
		//one-handed is set to 75%
		maxHitChance = 75;
	}

	if (it.hitChance != 0) {
		hitChance = it.hitChance;
	}

	if (it.maxHitChance != -1) {
		maxHitChance = it.maxHitChance;
	}

	if (it.breakChance != -1) {
		breakChance = it.breakChance;
	}

	if (it.ammoAction != AMMOACTION_NONE) {
		ammoAction = it.ammoAction;
	}

	int32_t intValue;

	if (readXMLInteger(p, "hitChance", intValue)) {
		std::cout << "Warning: hitChance is not longer used in weapons.xml." << std::endl;
	}

	if (readXMLInteger(p, "breakChance", intValue)) {
		std::cout << "Warning: breakChance is not longer used in weapons.xml." << std::endl;
	}

	return true;
}

bool WeaponDistance::configureWeapon(const ItemType& it)
{
	//default values
	if (it.ammoType != AMMO_NONE) {
		//hit chance on two-handed weapons is limited to 90%
		maxHitChance = 90;
	} else {
		//one-handed is set to 75%
		maxHitChance = 75;
	}

	params.distanceEffect = it.shootType;
	range = it.shootRange;
	ammuAttackValue = it.attack;

	if (it.hitChance != 0) {
		hitChance = it.hitChance;
	}

	if (it.maxHitChance > 0) {
		maxHitChance = it.maxHitChance;
	}

	if (it.breakChance > 0) {
		breakChance = it.breakChance;
	}

	if (it.ammoAction != AMMOACTION_NONE) {
		ammoAction = it.ammoAction;
	}

	if (it.abilities) {
		elementType = it.abilities->elementType;
		elementDamage = it.abilities->elementDamage;
	}

	return Weapon::configureWeapon(it);
}

int32_t WeaponDistance::playerWeaponCheck(Player* player, Creature* target) const
{
	Item* bow = player->getWeapon(true);

	if (bow && bow->getWeaponType() == WEAPON_DIST && bow->getID() != id) {
		const Weapon* weap = g_weapons->getWeapon(bow);

		if (weap) {
			return weap->playerWeaponCheck(player, target);
		}
	}

	return Weapon::playerWeaponCheck(player, target);
}

bool WeaponDistance::useWeapon(Player* player, Item* item, Creature* target) const
{
	int32_t damageModifier = playerWeaponCheck(player, target);

	if (damageModifier == 0) {
		return false;
	}

	int32_t chance;

	if (hitChance == 0) {
		//hit chance is based on distance to target and distance skill
		uint32_t skill = player->getSkill(SKILL_DIST, SKILL_LEVEL);
		const Position& playerPos = player->getPosition();
		const Position& targetPos = target->getPosition();
		uint32_t distance = std::max<uint32_t>(Position::getDistanceX(playerPos, targetPos), Position::getDistanceY(playerPos, targetPos));

		if (maxHitChance == 75) {
			//chance for one-handed weapons
			switch (distance) {
				case 1:
					chance = std::min<uint32_t>(skill, 74) + 1;
					break;
				case 2:
					chance = (uint32_t)((float)2.4 * std::min<uint32_t>(skill, 28)) + 8;
					break;
				case 3:
					chance = (uint32_t)((float)1.55 * std::min<uint32_t>(skill, 45)) + 6;
					break;
				case 4:
					chance = (uint32_t)((float)1.25 * std::min<uint32_t>(skill, 58)) + 3;
					break;
				case 5:
					chance = (uint32_t)((float)std::min<uint32_t>(skill, 74)) + 1;
					break;
				case 6:
					chance = (uint32_t)((float)0.8 * std::min<uint32_t>(skill, 90)) + 3;
					break;
				case 7:
					chance = (uint32_t)((float)0.7 * std::min<uint32_t>(skill, 104)) + 2;
					break;
				default:
					chance = hitChance;
					break;
			}
		} else if (maxHitChance == 90) {
			//formula for two-handed weapons
			switch (distance) {
				case 1:
					chance = (uint32_t)((float)1.2 * std::min<uint32_t>(skill, 74)) + 1;
					break;
				case 2:
					chance = (uint32_t)((float)3.2 * std::min<uint32_t>(skill, 28));
					break;
				case 3:
					chance = (uint32_t)((float)2.0 * std::min<uint32_t>(skill, 45));
					break;
				case 4:
					chance = (uint32_t)((float)1.55 * std::min<uint32_t>(skill, 58));
					break;
				case 5:
					chance = (uint32_t)((float)1.2 * std::min<uint32_t>(skill, 74)) + 1;
					break;
				case 6:
					chance = (uint32_t)((float)1.0 * std::min<uint32_t>(skill, 90));
					break;
				case 7:
					chance = (uint32_t)((float)1.0 * std::min<uint32_t>(skill, 90));
					break;
				default:
					chance = hitChance;
					break;
			}
		} else if (maxHitChance == 100) {
			switch (distance) {
				case 1:
					chance = (uint32_t)((float)1.35 * std::min<uint32_t>(skill, 73)) + 1;
					break;
				case 2:
					chance = (uint32_t)((float)3.2 * std::min<uint32_t>(skill, 30)) + 4;
					break;
				case 3:
					chance = (uint32_t)((float)2.05 * std::min<uint32_t>(skill, 48)) + 2;
					break;
				case 4:
					chance = (uint32_t)((float)1.5 * std::min<uint32_t>(skill, 65)) + 2;
					break;
				case 5:
					chance = (uint32_t)((float)1.35 * std::min<uint32_t>(skill, 73)) + 1;
					break;
				case 6:
					chance = (uint32_t)((float)1.2 * std::min<uint32_t>(skill, 87)) - 4;
					break;
				case 7:
					chance = (uint32_t)((float)1.1 * std::min<uint32_t>(skill, 90)) + 1;
					break;
				default:
					chance = hitChance;
					break;
			}
		} else {
			chance = maxHitChance;
		}
	} else {
		chance = hitChance;
	}

	if (item->getWeaponType() == WEAPON_AMMO) {
		Item* bow = player->getWeapon(true);
		if (bow && bow->getHitChance() != 0) {
			chance += bow->getHitChance();
		}
	}

	if (chance >= random_range(1, 100)) {
		if (elementDamage != 0) {
			int32_t damage = getElementDamage(player, target, item);
			CombatParams eParams;
			eParams.combatType = elementType;
			eParams.isAggressive = true;
			eParams.useCharges = true;
			Combat::doCombatHealth(player, target, damage, damage, eParams);
		}

		Weapon::internalUseWeapon(player, item, target, damageModifier);
	} else {
		//miss target
		Tile* destTile = target->getTile();

		if (!Position::areInRange<1, 1, 0>(player->getPosition(), target->getPosition())) {
			std::vector<std::pair<int32_t, int32_t>> destList;
			destList.push_back(std::make_pair(-1, -1));
			destList.push_back(std::make_pair(-1, 0));
			destList.push_back(std::make_pair(-1, 1));
			destList.push_back(std::make_pair(0, -1));
			destList.push_back(std::make_pair(0, 0));
			destList.push_back(std::make_pair(0, 1));
			destList.push_back(std::make_pair(1, -1));
			destList.push_back(std::make_pair(1, 0));
			destList.push_back(std::make_pair(1, 1));

			std::random_shuffle(destList.begin(), destList.end());

			Position destPos = target->getPosition();

			for (const auto& dir : destList) {
				Tile* tmpTile = g_game.getTile(destPos.x + dir.first, destPos.y + dir.second, destPos.z);

				// Blocking tiles or tiles without ground ain't valid targets for spears
				if (tmpTile && !tmpTile->hasProperty(IMMOVABLEBLOCKSOLID) && tmpTile->ground != NULL) {
					destTile = tmpTile;
					break;
				}
			}
		}

		Weapon::internalUseWeapon(player, item, destTile);
	}

	return true;
}

void WeaponDistance::onUsedWeapon(Player* player, Item* item, Tile* destTile) const
{
	Weapon::onUsedWeapon(player, item, destTile);
}

void WeaponDistance::onUsedAmmo(Player* player, Item* item, Tile* destTile) const
{
	if (ammoAction == AMMOACTION_MOVEBACK && breakChance > 0 && random_range(1, 100) <= breakChance) {
		uint16_t newCount = item->getItemCount();

		if (newCount > 0) {
			newCount--;
		}

		g_game.transformItem(item, item->getID(), newCount);
	} else {
		Weapon::onUsedAmmo(player, item, destTile);
	}
}

int32_t WeaponDistance::getElementDamage(const Player* player, const Creature* target, const Item* item) const
{
	int32_t attackValue = elementDamage;

	if (item->getWeaponType() == WEAPON_AMMO) {
		Item* bow = const_cast<Player*>(player)->getWeapon(true);

		if (bow) {
			attackValue += bow->getAttack();
		}
	}

	int32_t attackSkill = player->getSkill(SKILL_DIST, SKILL_LEVEL);
	float attackFactor = player->getAttackFactor();

	int32_t maxValue = Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor);

	if (random_range(1, 100) <= g_config.getNumber(ConfigManager::CRITICAL_HIT_CHANCE)) {
		maxValue <<= 1;
	}

	maxValue = int32_t(maxValue * player->getVocation()->distDamageMultiplier);

	int32_t minValue = 0;

	if (target) {
		if (target->getPlayer()) {
			minValue = (int32_t)std::ceil(player->getLevel() * 0.1);
		} else {
			minValue = (int32_t)std::ceil(player->getLevel() * 0.2);
		}
	}

	return -random_range(minValue, maxValue, DISTRO_NORMAL);
}

int32_t WeaponDistance::getWeaponDamage(const Player* player, const Creature* target, const Item* item, bool maxDamage /*= false*/) const
{
	int32_t attackValue = ammuAttackValue;

	if (item->getWeaponType() == WEAPON_AMMO) {
		Item* bow = const_cast<Player*>(player)->getWeapon(true);
		if (bow) {
			attackValue += bow->getAttack();
		}
	}

	int32_t attackSkill = player->getSkill(SKILL_DIST, SKILL_LEVEL);
	float attackFactor = player->getAttackFactor();

	int32_t maxValue = Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor);

	if (random_range(1, 100) <= g_config.getNumber(ConfigManager::CRITICAL_HIT_CHANCE)) {
		maxValue <<= 1;
	}

	maxValue = int32_t(maxValue * player->getVocation()->distDamageMultiplier);
	if (maxDamage) {
		return -maxValue;
	}

	int32_t minValue;
	if (target) {
		if (target->getPlayer()) {
			minValue = static_cast<int32_t>(std::ceil(player->getLevel() * 0.1));
		} else {
			minValue = static_cast<int32_t>(std::ceil(player->getLevel() * 0.2));
		}
	} else {
		minValue = 0;
	}
	return -random_range(minValue, maxValue, DISTRO_NORMAL);
}

bool WeaponDistance::getSkillType(const Player* player, const Item* item,
                                  skills_t& skill, uint32_t& skillpoint) const
{
	skill = SKILL_DIST;
	skillpoint = 0;

	if (player->getAddAttackSkill()) {
		switch (player->getLastAttackBlockType()) {
			case BLOCK_NONE: {
				skillpoint = 2;
				break;
			}

			case BLOCK_DEFENSE:
			case BLOCK_ARMOR: {
				skillpoint = 1;
				break;
			}

			default:
				skillpoint = 0;
				break;
		}
	}

	return true;
}

WeaponWand::WeaponWand(LuaScriptInterface* _interface) :
	Weapon(_interface)
{
	minChange = 0;
	maxChange = 0;
}

bool WeaponWand::configureEvent(xmlNodePtr p)
{
	if (!Weapon::configureEvent(p)) {
		return false;
	}

	int32_t intValue;
	std::string strValue;

	if (readXMLInteger(p, "min", intValue)) {
		minChange = intValue;
	}

	if (readXMLInteger(p, "max", intValue)) {
		maxChange = intValue;
	}

	if (readXMLString(p, "type", strValue)) {
		std::string tmpStrValue = asLowerCaseString(strValue);

		if (tmpStrValue == "earth") {
			params.combatType = COMBAT_EARTHDAMAGE;
		} else if (tmpStrValue == "ice") {
			params.combatType = COMBAT_ICEDAMAGE;
		} else if (tmpStrValue == "energy") {
			params.combatType = COMBAT_ENERGYDAMAGE;
		} else if (tmpStrValue == "fire") {
			params.combatType = COMBAT_FIREDAMAGE;
		} else if (tmpStrValue == "death") {
			params.combatType = COMBAT_DEATHDAMAGE;
		} else if (tmpStrValue == "holy") {
			params.combatType = COMBAT_HOLYDAMAGE;
		} else {
			std::cout << "[Warning - WeaponWand::configureEvent] Type \"" << strValue << "\" does not exist." << std::endl;
		}
	}

	return true;
}

bool WeaponWand::configureWeapon(const ItemType& it)
{
	range = it.shootRange;
	params.distanceEffect = it.shootType;

	return Weapon::configureWeapon(it);
}

int32_t WeaponWand::getWeaponDamage(const Player* player, const Creature* target, const Item* item, bool maxDamage /*= false*/) const
{
	if (maxDamage) {
		return -maxChange;
	}

	return random_range(-minChange, -maxChange, DISTRO_NORMAL);
}
