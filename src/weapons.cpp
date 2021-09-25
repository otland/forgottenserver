/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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
#include "game.h"
#include "pugicast.h"
#include "weapons.h"

extern Game g_game;
extern Vocations g_vocations;
extern ConfigManager g_config;
extern Weapons* g_weapons;

Weapons::Weapons()
{
	scriptInterface.initState();
}

Weapons::~Weapons()
{
	clear(false);
}

const Weapon* Weapons::getWeapon(const Item* item) const
{
	if (!item) {
		return nullptr;
	}

	auto it = weapons.find(item->getID());
	if (it == weapons.end()) {
		return nullptr;
	}
	return it->second;
}

void Weapons::clear(bool fromLua)
{
	for (auto it = weapons.begin(); it != weapons.end(); ) {
		if (fromLua == it->second->fromLua) {
			it = weapons.erase(it);
		} else {
			++it;
		}
	}

	reInitState(fromLua);
}

LuaScriptInterface& Weapons::getScriptInterface()
{
	return scriptInterface;
}

std::string Weapons::getScriptBaseName() const
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

		switch (it.weaponType) {
			case WEAPON_AXE:
			case WEAPON_SWORD:
			case WEAPON_CLUB: {
				WeaponMelee* weapon = new WeaponMelee(&scriptInterface);
				weapon->configureWeapon(it);
				weapons[i] = weapon;
				break;
			}

			case WEAPON_AMMO:
			case WEAPON_DISTANCE: {
				if (it.weaponType == WEAPON_DISTANCE && it.ammoType != AMMO_NONE) {
					continue;
				}

				WeaponDistance* weapon = new WeaponDistance(&scriptInterface);
				weapon->configureWeapon(it);
				weapons[i] = weapon;
				break;
			}

			default:
				break;
		}
	}
}

Event_ptr Weapons::getEvent(const std::string& nodeName)
{
	if (strcasecmp(nodeName.c_str(), "melee") == 0) {
		return Event_ptr(new WeaponMelee(&scriptInterface));
	} else if (strcasecmp(nodeName.c_str(), "distance") == 0) {
		return Event_ptr(new WeaponDistance(&scriptInterface));
	} else if (strcasecmp(nodeName.c_str(), "wand") == 0) {
		return Event_ptr(new WeaponWand(&scriptInterface));
	}
	return nullptr;
}

bool Weapons::registerEvent(Event_ptr event, const pugi::xml_node&)
{
	Weapon* weapon = static_cast<Weapon*>(event.release()); //event is guaranteed to be a Weapon

	auto result = weapons.emplace(weapon->getID(), weapon);
	if (!result.second) {
		std::cout << "[Warning - Weapons::registerEvent] Duplicate registered item with id: " << weapon->getID() << std::endl;
	}
	return result.second;
}

bool Weapons::registerLuaEvent(Weapon* weapon)
{
	weapons[weapon->getID()] = weapon;
	return true;
}

//monsters
int32_t Weapons::getMaxMeleeDamage(int32_t attackSkill, int32_t attackValue)
{
	return static_cast<int32_t>(std::ceil((attackSkill * (attackValue * 0.05)) + (attackValue * 0.5)));
}

//players
int32_t Weapons::getMaxWeaponDamage(uint32_t level, int32_t attackSkill, int32_t attackValue, float attackFactor)
{
	return static_cast<int32_t>(std::round((level / 5) + (((((attackSkill / 4.) + 1) * (attackValue / 3.)) * 1.03) / attackFactor)));
}

bool Weapon::configureEvent(const pugi::xml_node& node)
{
	pugi::xml_attribute attr;
	if (!(attr = node.attribute("id"))) {
		std::cout << "[Error - Weapon::configureEvent] Weapon without id." << std::endl;
		return false;
	}
	id = pugi::cast<uint16_t>(attr.value());

	if ((attr = node.attribute("level"))) {
		level = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = node.attribute("maglv")) || (attr = node.attribute("maglevel"))) {
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

	if ((attr = node.attribute("prem"))) {
		premium = attr.as_bool();
	}

	if ((attr = node.attribute("breakchance"))) {
		breakChance = std::min<uint8_t>(100, pugi::cast<uint16_t>(attr.value()));
	}

	if ((attr = node.attribute("action"))) {
		action = getWeaponAction(asLowerCaseString(attr.as_string()));
		if (action == WEAPONACTION_NONE) {
			std::cout << "[Warning - Weapon::configureEvent] Unknown action " << attr.as_string() << std::endl;
		}
	}

	if ((attr = node.attribute("enabled"))) {
		enabled = attr.as_bool();
	}

	if ((attr = node.attribute("unproperly"))) {
		wieldUnproperly = attr.as_bool();
	}

	std::list<std::string> vocStringList;
	for (auto vocationNode : node.children()) {
		if (!(attr = vocationNode.attribute("name"))) {
			continue;
		}

		int32_t vocationId = g_vocations.getVocationId(attr.as_string());
		if (vocationId != -1) {
			vocWeaponMap[vocationId] = true;
			int32_t promotedVocation = g_vocations.getPromotedVocation(vocationId);
			if (promotedVocation != VOCATION_NONE) {
				vocWeaponMap[promotedVocation] = true;
			}

			if (vocationNode.attribute("showInDescription").as_bool(true)) {
				vocStringList.push_back(asLowerCaseString(attr.as_string()));
			}
		}
	}

	std::string vocationString;
	for (const std::string& str : vocStringList) {
		if (!vocationString.empty()) {
			if (str != vocStringList.back()) {
				vocationString.push_back(',');
				vocationString.push_back(' ');
			} else {
				vocationString += " and ";
			}
		}

		vocationString += str;
		vocationString.push_back('s');
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

	configureWeapon(Item::items[id]);
	return true;
}

void Weapon::configureWeapon(const ItemType& it)
{
	id = it.id;
}

std::string Weapon::getScriptEventName() const
{
	return "onUseWeapon";
}

int32_t Weapon::playerWeaponCheck(Player* player, Creature* target, uint8_t shootRange) const
{
	const Position& playerPos = player->getPosition();
	const Position& targetPos = target->getPosition();
	if (playerPos.z != targetPos.z) {
		return 0;
	}

	if (std::max<uint32_t>(Position::getDistanceX(playerPos, targetPos), Position::getDistanceY(playerPos, targetPos)) > shootRange) {
		return 0;
	}

	if (!player->hasFlag(PlayerFlag_IgnoreWeaponCheck)) {
		if (!enabled) {
			return 0;
		}

		if (player->getMana() < getManaCost(player)) {
			return 0;
		}

		if (player->getHealth() < getHealthCost(player)) {
			return 0;
		}

		if (player->getSoul() < soul) {
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
	int32_t damageModifier = playerWeaponCheck(player, target, item->getShootRange());
	if (damageModifier == 0) {
		return false;
	}

	internalUseWeapon(player, item, target, damageModifier);
	return true;
}

bool Weapon::useFist(Player* player, Creature* target)
{
	if (!Position::areInRange<1, 1>(player->getPosition(), target->getPosition())) {
		return false;
	}

	float attackFactor = player->getAttackFactor();
	int32_t attackSkill = player->getSkillLevel(SKILL_FIST);
	int32_t attackValue = 7;

	int32_t maxDamage = Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor);

	CombatParams params;
	params.combatType = COMBAT_PHYSICALDAMAGE;
	params.blockedByArmor = true;
	params.blockedByShield = true;

	CombatDamage damage;
	damage.origin = ORIGIN_MELEE;
	damage.primary.type = params.combatType;
	damage.primary.value = -normal_random(0, maxDamage);

	Combat::doTargetCombat(player, target, damage, params);
	if (!player->hasFlag(PlayerFlag_NotGainSkill) && player->getAddAttackSkill()) {
		player->addSkillAdvance(SKILL_FIST, 1);
	}

	return true;
}

void Weapon::internalUseWeapon(Player* player, Item* item, Creature* target, int32_t damageModifier) const
{
	if (scripted) {
		LuaVariant var;
		var.type = VARIANT_NUMBER;
		var.number = target->getID();
		executeUseWeapon(player, var);
	} else {
		CombatDamage damage;
		WeaponType_t weaponType = item->getWeaponType();
		if (weaponType == WEAPON_AMMO || weaponType == WEAPON_DISTANCE) {
			damage.origin = ORIGIN_RANGED;
		} else if (weaponType == WEAPON_WAND) {
			damage.origin = ORIGIN_WAND;
		} else {
			damage.origin = ORIGIN_MELEE;
		}
		damage.primary.type = params.combatType;
		damage.primary.value = (getWeaponDamage(player, target, item) * damageModifier) / 100;
		damage.secondary.type = getElementType();
		damage.secondary.value = getElementDamage(player, target, item);
		Combat::doTargetCombat(player, target, damage, params);
	}

	onUsedWeapon(player, item, target->getTile());
}

void Weapon::internalUseWeapon(Player* player, Item* item, Tile* tile) const
{
	if (scripted) {
		LuaVariant var;
		var.type = VARIANT_TARGETPOSITION;
		var.pos = tile->getPosition();
		executeUseWeapon(player, var);
	} else {
		Combat::postCombatEffects(player, tile->getPosition(), params);
		g_game.addMagicEffect(tile->getPosition(), CONST_ME_POFF);
	}

	onUsedWeapon(player, item, tile);
}

void Weapon::onUsedWeapon(Player* player, Item* item, Tile* destTile) const
{
	if (!player->hasFlag(PlayerFlag_NotGainSkill)) {
		skills_t skillType;
		uint32_t skillPoint;
		if (getSkillType(player, item, skillType, skillPoint)) {
			player->addSkillAdvance(skillType, skillPoint);
		}
	}

	uint32_t manaCost = getManaCost(player);
	if (manaCost != 0) {
		player->addManaSpent(manaCost);
		player->changeMana(-static_cast<int32_t>(manaCost));
	}

	uint32_t healthCost = getHealthCost(player);
	if (healthCost != 0) {
		player->changeHealth(-static_cast<int32_t>(healthCost));
	}

	if (!player->hasFlag(PlayerFlag_HasInfiniteSoul) && soul > 0) {
		player->changeSoul(-static_cast<int32_t>(soul));
	}

	if (breakChance != 0 && uniform_random(1, 100) <= breakChance) {
		Weapon::decrementItemCount(item);
		return;
	}

	switch (action) {
		case WEAPONACTION_REMOVECOUNT:
			if (g_config.getBoolean(ConfigManager::REMOVE_WEAPON_AMMO)) {
				Weapon::decrementItemCount(item);
			}
			break;

		case WEAPONACTION_REMOVECHARGE: {
			uint16_t charges = item->getCharges();
			if (charges != 0 && g_config.getBoolean(ConfigManager::REMOVE_WEAPON_CHARGES)) {
				g_game.transformItem(item, item->getID(), charges - 1);
			}
			break;
		}

		case WEAPONACTION_MOVE:
			g_game.internalMoveItem(item->getParent(), destTile, INDEX_WHEREEVER, item, 1, nullptr, FLAG_NOLIMIT);
			break;

		default:
			break;
	}
}

uint32_t Weapon::getManaCost(const Player* player) const
{
	if (mana != 0) {
		return mana;
	}

	if (manaPercent == 0) {
		return 0;
	}

	return (player->getMaxMana() * manaPercent) / 100;
}

int32_t Weapon::getHealthCost(const Player* player) const
{
	if (health != 0) {
		return health;
	}

	if (healthPercent == 0) {
		return 0;
	}

	return (player->getMaxHealth() * healthPercent) / 100;
}

bool Weapon::executeUseWeapon(Player* player, const LuaVariant& var) const
{
	//onUseWeapon(player, var)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - Weapon::executeUseWeapon] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);
	LuaScriptInterface::pushUserdata<Player>(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");
	scriptInterface->pushVariant(L, var);

	return scriptInterface->callFunction(2);
}

void Weapon::decrementItemCount(Item* item)
{
	uint16_t count = item->getItemCount();
	if (count > 1) {
		g_game.transformItem(item, item->getID(), count - 1);
	} else {
		g_game.internalRemoveItem(item);
	}
}

WeaponMelee::WeaponMelee(LuaScriptInterface* interface) :
	Weapon(interface)
{
	params.blockedByArmor = true;
	params.blockedByShield = true;
	params.combatType = COMBAT_PHYSICALDAMAGE;
}

void WeaponMelee::configureWeapon(const ItemType& it)
{
	if (it.abilities) {
		elementType = it.abilities->elementType;
		elementDamage = it.abilities->elementDamage;
		params.aggressive = true;
		params.useCharges = true;
	} else {
		elementType = COMBAT_NONE;
		elementDamage = 0;
	}
	Weapon::configureWeapon(it);
}

bool WeaponMelee::useWeapon(Player* player, Item* item, Creature* target) const
{
	int32_t damageModifier = playerWeaponCheck(player, target, item->getShootRange());
	if (damageModifier == 0) {
		return false;
	}

	internalUseWeapon(player, item, target, damageModifier);
	return true;
}

bool WeaponMelee::getSkillType(const Player* player, const Item* item,
                               skills_t& skill, uint32_t& skillpoint) const
{
	if (player->getAddAttackSkill() && player->getLastAttackBlockType() != BLOCK_IMMUNITY) {
		skillpoint = 1;
	} else {
		skillpoint = 0;
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

int32_t WeaponMelee::getElementDamage(const Player* player, const Creature*, const Item* item) const
{
	if (elementType == COMBAT_NONE) {
		return 0;
	}

	int32_t attackSkill = player->getWeaponSkill(item);
	int32_t attackValue = elementDamage;
	float attackFactor = player->getAttackFactor();

	int32_t maxValue = Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor);
	return -normal_random(0, static_cast<int32_t>(maxValue * player->getVocation()->meleeDamageMultiplier));
}

int32_t WeaponMelee::getWeaponDamage(const Player* player, const Creature*, const Item* item, bool maxDamage /*= false*/) const
{
	int32_t attackSkill = player->getWeaponSkill(item);
	int32_t attackValue = std::max<int32_t>(0, item->getAttack());
	float attackFactor = player->getAttackFactor();

	int32_t maxValue = static_cast<int32_t>(Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor) * player->getVocation()->meleeDamageMultiplier);
	if (maxDamage) {
		return -maxValue;
	}

	return -normal_random(0, maxValue);
}

WeaponDistance::WeaponDistance(LuaScriptInterface* interface) :
	Weapon(interface)
{
	params.blockedByArmor = true;
	params.combatType = COMBAT_PHYSICALDAMAGE;
}

void WeaponDistance::configureWeapon(const ItemType& it)
{
	params.distanceEffect = it.shootType;

	if (it.abilities) {
		elementType = it.abilities->elementType;
		elementDamage = it.abilities->elementDamage;
		params.aggressive = true;
		params.useCharges = true;
	} else {
		elementType = COMBAT_NONE;
		elementDamage = 0;
	}

	Weapon::configureWeapon(it);
}

bool WeaponDistance::useWeapon(Player* player, Item* item, Creature* target) const
{
	int32_t damageModifier = 0;
	const ItemType& it = Item::items[id];
	if (it.weaponType == WEAPON_AMMO) {
		Item* mainWeaponItem = player->getWeapon(true);
		const Weapon* mainWeapon = g_weapons->getWeapon(mainWeaponItem);
		if (mainWeapon) {
			damageModifier = mainWeapon->playerWeaponCheck(player, target, mainWeaponItem->getShootRange());
		} else if (mainWeaponItem) {
			damageModifier = playerWeaponCheck(player, target, mainWeaponItem->getShootRange());
		}
	} else {
		damageModifier = playerWeaponCheck(player, target, item->getShootRange());
	}

	if (damageModifier == 0) {
		return false;
	}

	int32_t chance;
	if (it.hitChance == 0) {
		//hit chance is based on distance to target and distance skill
		uint32_t skill = player->getSkillLevel(SKILL_DISTANCE);
		const Position& playerPos = player->getPosition();
		const Position& targetPos = target->getPosition();
		uint32_t distance = std::max<uint32_t>(Position::getDistanceX(playerPos, targetPos), Position::getDistanceY(playerPos, targetPos));

		uint32_t maxHitChance;
		if (it.maxHitChance != -1) {
			maxHitChance = it.maxHitChance;
		} else if (it.ammoType != AMMO_NONE) {
			//hit chance on two-handed weapons is limited to 90%
			maxHitChance = 90;
		} else {
			//one-handed is set to 75%
			maxHitChance = 75;
		}

		if (maxHitChance == 75) {
			//chance for one-handed weapons
			switch (distance) {
				case 1:
				case 5:
					chance = std::min<uint32_t>(skill, 74) + 1;
					break;
				case 2:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 28) * 2.40f) + 8;
					break;
				case 3:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 45) * 1.55f) + 6;
					break;
				case 4:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 58) * 1.25f) + 3;
					break;
				case 6:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 90) * 0.80f) + 3;
					break;
				case 7:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 104) * 0.70f) + 2;
					break;
				default:
					chance = it.hitChance;
					break;
			}
		} else if (maxHitChance == 90) {
			//formula for two-handed weapons
			switch (distance) {
				case 1:
				case 5:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 74) * 1.20f) + 1;
					break;
				case 2:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 28) * 3.20f);
					break;
				case 3:
					chance = std::min<uint32_t>(skill, 45) * 2;
					break;
				case 4:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 58) * 1.55f);
					break;
				case 6:
				case 7:
					chance = std::min<uint32_t>(skill, 90);
					break;
				default:
					chance = it.hitChance;
					break;
			}
		} else if (maxHitChance == 100) {
			switch (distance) {
				case 1:
				case 5:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 73) * 1.35f) + 1;
					break;
				case 2:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 30) * 3.20f) + 4;
					break;
				case 3:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 48) * 2.05f) + 2;
					break;
				case 4:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 65) * 1.50f) + 2;
					break;
				case 6:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 87) * 1.20f) - 4;
					break;
				case 7:
					chance = static_cast<uint32_t>(std::min<uint32_t>(skill, 90) * 1.10f) + 1;
					break;
				default:
					chance = it.hitChance;
					break;
			}
		} else {
			chance = maxHitChance;
		}
	} else {
		chance = it.hitChance;
	}

	if (item->getWeaponType() == WEAPON_AMMO) {
		Item* bow = player->getWeapon(true);
		if (bow && bow->getHitChance() != 0) {
			chance += bow->getHitChance();
		}
	}

	if (chance >= uniform_random(1, 100)) {
		Weapon::internalUseWeapon(player, item, target, damageModifier);
	} else {
		//miss target
		Tile* destTile = target->getTile();

		if (!Position::areInRange<1, 1, 0>(player->getPosition(), target->getPosition())) {
			static std::vector<std::pair<int32_t, int32_t>> destList {
				{-1, -1}, {0, -1}, {1, -1},
				{-1,  0}, {0,  0}, {1,  0},
				{-1,  1}, {0,  1}, {1,  1}
			};
			std::shuffle(destList.begin(), destList.end(), getRandomGenerator());

			Position destPos = target->getPosition();

			for (const auto& dir : destList) {
				// Blocking tiles or tiles without ground ain't valid targets for spears
				Tile* tmpTile = g_game.map.getTile(destPos.x + dir.first, destPos.y + dir.second, destPos.z);
				if (tmpTile && !tmpTile->hasFlag(TILESTATE_IMMOVABLEBLOCKSOLID) && tmpTile->getGround() != nullptr) {
					destTile = tmpTile;
					break;
				}
			}
		}

		Weapon::internalUseWeapon(player, item, destTile);
	}
	return true;
}

int32_t WeaponDistance::getElementDamage(const Player* player, const Creature* target, const Item* item) const
{
	if (elementType == COMBAT_NONE) {
		return 0;
	}

	int32_t attackValue = elementDamage;
	if (item->getWeaponType() == WEAPON_AMMO) {
		Item* weapon = player->getWeapon(true);
		if (weapon) {
			attackValue += weapon->getAttack();
		}
	}

	int32_t attackSkill = player->getSkillLevel(SKILL_DISTANCE);
	float attackFactor = player->getAttackFactor();

	int32_t minValue = 0;
	int32_t maxValue = Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor);
	if (target) {
		if (target->getPlayer()) {
			minValue = static_cast<int32_t>(std::ceil(player->getLevel() * 0.1));
		} else {
			minValue = static_cast<int32_t>(std::ceil(player->getLevel() * 0.2));
		}
	}

	return -normal_random(minValue, static_cast<int32_t>(maxValue * player->getVocation()->distDamageMultiplier));
}

int32_t WeaponDistance::getWeaponDamage(const Player* player, const Creature* target, const Item* item, bool maxDamage /*= false*/) const
{
	int32_t attackValue = item->getAttack();

	if (item->getWeaponType() == WEAPON_AMMO) {
		Item* weapon = player->getWeapon(true);
		if (weapon) {
			attackValue += weapon->getAttack();
		}
	}

	int32_t attackSkill = player->getSkillLevel(SKILL_DISTANCE);
	float attackFactor = player->getAttackFactor();

	int32_t maxValue = static_cast<int32_t>(Weapons::getMaxWeaponDamage(player->getLevel(), attackSkill, attackValue, attackFactor) * player->getVocation()->distDamageMultiplier);
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
	return -normal_random(minValue, maxValue);
}

bool WeaponDistance::getSkillType(const Player* player, const Item*, skills_t& skill, uint32_t& skillpoint) const
{
	skill = SKILL_DISTANCE;

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
	} else {
		skillpoint = 0;
	}
	return true;
}

bool WeaponWand::configureEvent(const pugi::xml_node& node)
{
	if (!Weapon::configureEvent(node)) {
		return false;
	}

	pugi::xml_attribute attr;
	if ((attr = node.attribute("min"))) {
		minChange = pugi::cast<int32_t>(attr.value());
	}

	if ((attr = node.attribute("max"))) {
		maxChange = pugi::cast<int32_t>(attr.value());
	}

	attr = node.attribute("type");
	if (!attr) {
		return true;
	}

	std::string tmpStrValue = asLowerCaseString(attr.as_string());
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
		std::cout << "[Warning - WeaponWand::configureEvent] Type \"" << attr.as_string() << "\" does not exist." << std::endl;
	}
	return true;
}

void WeaponWand::configureWeapon(const ItemType& it)
{
	params.distanceEffect = it.shootType;

	Weapon::configureWeapon(it);
}

int32_t WeaponWand::getWeaponDamage(const Player*, const Creature*, const Item*, bool maxDamage /*= false*/) const
{
	if (maxDamage) {
		return -maxChange;
	}
	return -normal_random(minChange, maxChange);
}
