#include "../../otpch.h"

#include "../../script.h"
#include "../../weapons.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Scripts* g_scripts;
extern std::unique_ptr<Weapons> g_weapons;

namespace {

int luaCreateWeapon(lua_State* L)
{
	// Weapon(type)
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		tfs::lua::reportError(L, "Weapons can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	WeaponType_t type = tfs::lua::getNumber<WeaponType_t>(L, 2);
	switch (type) {
		case WEAPON_SWORD:
		case WEAPON_AXE:
		case WEAPON_CLUB: {
			auto weapon = std::make_shared<WeaponMelee>(tfs::lua::getScriptEnv()->getScriptInterface());
			weapon->weaponType = type;
			weapon->fromLua = true;
			tfs::lua::pushSharedPtr(L, weapon);
			tfs::lua::setMetatable(L, -1, "Weapon");
			break;
		}
		case WEAPON_DISTANCE:
		case WEAPON_AMMO: {
			auto weapon = std::make_shared<WeaponDistance>(tfs::lua::getScriptEnv()->getScriptInterface());
			weapon->weaponType = type;
			weapon->fromLua = true;
			tfs::lua::pushSharedPtr(L, weapon);
			tfs::lua::setMetatable(L, -1, "Weapon");
			break;
		}
		case WEAPON_WAND: {
			auto weapon = std::make_shared<WeaponWand>(tfs::lua::getScriptEnv()->getScriptInterface());
			weapon->weaponType = type;
			weapon->fromLua = true;
			tfs::lua::pushSharedPtr(L, weapon);
			tfs::lua::setMetatable(L, -1, "Weapon");
			break;
		}
		default: {
			lua_pushnil(L);
			break;
		}
	}
	return 1;
}

int luaWeaponAction(lua_State* L)
{
	// weapon:action(callback)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		std::string typeName = tfs::lua::getString(L, 2);
		std::string tmpStr = boost::algorithm::to_lower_copy(typeName);
		if (tmpStr == "removecount") {
			weapon->action = WEAPONACTION_REMOVECOUNT;
		} else if (tmpStr == "removecharge") {
			weapon->action = WEAPONACTION_REMOVECHARGE;
		} else if (tmpStr == "move") {
			weapon->action = WEAPONACTION_MOVE;
		} else {
			std::cout << "Error: [Weapon::action] No valid action " << typeName << '\n';
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponRegister(lua_State* L)
{
	// weapon:register()
	const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1);
	if (!weapon) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t id = weapon->getID();
	ItemType& it = Item::items.getItemType(id);
	it.weaponType = weapon->weaponType;

	if (weapon->getWieldInfo() != 0) {
		it.wieldInfo = weapon->getWieldInfo();
		it.vocationString = weapon->getVocationString();
		it.minReqLevel = weapon->getReqLevel();
		it.minReqMagicLevel = weapon->getReqMagLv();
	}

	weapon->configureWeapon(it);
	tfs::lua::pushBoolean(L, g_weapons->registerLuaEvent(weapon));
	return 1;
}

int luaWeaponOnUseWeapon(lua_State* L)
{
	// weapon:onUseWeapon(callback)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		tfs::lua::pushBoolean(L, weapon->loadCallback());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponUnproperly(lua_State* L)
{
	// weapon:wieldUnproperly(bool)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setWieldUnproperly(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponLevel(lua_State* L)
{
	// weapon:level(lvl)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setRequiredLevel(tfs::lua::getNumber<uint32_t>(L, 2));
		weapon->setWieldInfo(WIELDINFO_LEVEL);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponMagicLevel(lua_State* L)
{
	// weapon:magicLevel(lvl)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setRequiredMagLevel(tfs::lua::getNumber<uint32_t>(L, 2));
		weapon->setWieldInfo(WIELDINFO_MAGLV);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponMana(lua_State* L)
{
	// weapon:mana(mana)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setMana(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponManaPercent(lua_State* L)
{
	// weapon:manaPercent(percent)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setManaPercent(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponHealth(lua_State* L)
{
	// weapon:health(health)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setHealth(tfs::lua::getNumber<int32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponHealthPercent(lua_State* L)
{
	// weapon:healthPercent(percent)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setHealthPercent(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponSoul(lua_State* L)
{
	// weapon:soul(soul)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setSoul(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponBreakChance(lua_State* L)
{
	// weapon:breakChance(percent)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setBreakChance(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponWandDamage(lua_State* L)
{
	// weapon:damage(damage[min, max]) only use this if the weapon is a wand!
	WeaponWand* weapon = tfs::lua::getUserdata<WeaponWand>(L, 1);
	if (weapon) {
		weapon->setMinChange(tfs::lua::getNumber<uint32_t>(L, 2));
		if (lua_gettop(L) > 2) {
			weapon->setMaxChange(tfs::lua::getNumber<uint32_t>(L, 3));
		} else {
			weapon->setMaxChange(tfs::lua::getNumber<uint32_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponElement(lua_State* L)
{
	// weapon:element(combatType)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		if (!tfs::lua::getNumber<CombatType_t>(L, 2)) {
			std::string element = tfs::lua::getString(L, 2);
			std::string tmpStrValue = boost::algorithm::to_lower_copy(element);
			if (tmpStrValue == "earth") {
				weapon->params.combatType = COMBAT_EARTHDAMAGE;
			} else if (tmpStrValue == "ice") {
				weapon->params.combatType = COMBAT_ICEDAMAGE;
			} else if (tmpStrValue == "energy") {
				weapon->params.combatType = COMBAT_ENERGYDAMAGE;
			} else if (tmpStrValue == "fire") {
				weapon->params.combatType = COMBAT_FIREDAMAGE;
			} else if (tmpStrValue == "death") {
				weapon->params.combatType = COMBAT_DEATHDAMAGE;
			} else if (tmpStrValue == "holy") {
				weapon->params.combatType = COMBAT_HOLYDAMAGE;
			} else {
				std::cout << "[Warning - weapon:element] Type \"" << element << "\" does not exist.\n";
			}
		} else {
			weapon->params.combatType = tfs::lua::getNumber<CombatType_t>(L, 2);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponPremium(lua_State* L)
{
	// weapon:premium(bool)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setNeedPremium(tfs::lua::getBoolean(L, 2));
		weapon->setWieldInfo(WIELDINFO_PREMIUM);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponVocation(lua_State* L)
{
	// weapon:vocation(vocName[, showInDescription = false, lastVoc = false])
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->addVocationWeaponSet(tfs::lua::getString(L, 2));
		weapon->setWieldInfo(WIELDINFO_VOCREQ);
		std::string tmp;
		bool showInDescription = tfs::lua::getBoolean(L, 3, false);
		bool lastVoc = tfs::lua::getBoolean(L, 4, false);

		if (showInDescription) {
			if (weapon->getVocationString().empty()) {
				tmp = boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
				tmp += "s";
				weapon->setVocationString(tmp);
			} else {
				tmp = weapon->getVocationString();
				if (lastVoc) {
					tmp += " and ";
				} else {
					tmp += ", ";
				}
				tmp += boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
				tmp += "s";
				weapon->setVocationString(tmp);
			}
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponId(lua_State* L)
{
	// weapon:id(id)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		weapon->setID(tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponAttack(lua_State* L)
{
	// weapon:attack(atk)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.attack = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponDefense(lua_State* L)
{
	// weapon:defense(defense[, extraDefense])
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.defense = tfs::lua::getNumber<int32_t>(L, 2);
		if (lua_gettop(L) > 2) {
			it.extraDefense = tfs::lua::getNumber<int32_t>(L, 3);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponRange(lua_State* L)
{
	// weapon:range(range)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.shootRange = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponCharges(lua_State* L)
{
	// weapon:charges(charges[, showCharges = true])
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		bool showCharges = tfs::lua::getBoolean(L, 3, true);
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);

		it.charges = tfs::lua::getNumber<uint32_t>(L, 2);
		it.showCharges = showCharges;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponDuration(lua_State* L)
{
	// weapon:duration(duration[, showDuration = true])
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		bool showDuration = tfs::lua::getBoolean(L, 3, true);
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);

		if (lua_istable(L, 2)) {
			it.decayTimeMin = tfs::lua::getField<uint32_t>(L, 2, "min");
			it.decayTimeMax = tfs::lua::getField<uint32_t>(L, 2, "max");
		} else {
			it.decayTimeMin = tfs::lua::getNumber<uint32_t>(L, 2);
		}

		it.showDuration = showDuration;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponDecayTo(lua_State* L)
{
	// weapon:decayTo([itemid = 0])
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t itemid = tfs::lua::getNumber<uint16_t>(L, 2, 0);
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);

		it.decayTo = itemid;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponTransformEquipTo(lua_State* L)
{
	// weapon:transformEquipTo(itemid)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.transformEquipTo = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponTransformDeEquipTo(lua_State* L)
{
	// weapon:transformDeEquipTo(itemid)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.transformDeEquipTo = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponShootType(lua_State* L)
{
	// weapon:shootType(type)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.shootType = tfs::lua::getNumber<ShootType_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponSlotType(lua_State* L)
{
	// weapon:slotType(slot)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		std::string slot = tfs::lua::getString(L, 2);

		if (slot == "two-handed") {
			it.slotPosition |= SLOTP_TWO_HAND;
		} else {
			it.slotPosition |= SLOTP_HAND;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponAmmoType(lua_State* L)
{
	// weapon:ammoType(type)
	WeaponDistance* weapon = tfs::lua::getUserdata<WeaponDistance>(L, 1);
	if (weapon) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		std::string type = tfs::lua::getString(L, 2);

		if (type == "arrow") {
			it.ammoType = AMMO_ARROW;
		} else if (type == "bolt") {
			it.ammoType = AMMO_BOLT;
		} else {
			std::cout << "[Warning - weapon:ammoType] Type \"" << type << "\" does not exist.\n";
			lua_pushnil(L);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponHitChance(lua_State* L)
{
	// weapon:hitChance(chance)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.hitChance = tfs::lua::getNumber<int8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponMaxHitChance(lua_State* L)
{
	// weapon:maxHitChance(max)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.maxHitChance = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaWeaponExtraElement(lua_State* L)
{
	// weapon:extraElement(atk, combatType)
	if (const auto& weapon = tfs::lua::getSharedPtr<Weapon>(L, 1)) {
		uint16_t id = weapon->getID();
		ItemType& it = Item::items.getItemType(id);
		it.abilities.get()->elementDamage = tfs::lua::getNumber<uint16_t>(L, 2);

		if (!tfs::lua::getNumber<CombatType_t>(L, 3)) {
			std::string element = tfs::lua::getString(L, 3);
			std::string tmpStrValue = boost::algorithm::to_lower_copy(element);
			if (tmpStrValue == "earth") {
				it.abilities.get()->elementType = COMBAT_EARTHDAMAGE;
			} else if (tmpStrValue == "ice") {
				it.abilities.get()->elementType = COMBAT_ICEDAMAGE;
			} else if (tmpStrValue == "energy") {
				it.abilities.get()->elementType = COMBAT_ENERGYDAMAGE;
			} else if (tmpStrValue == "fire") {
				it.abilities.get()->elementType = COMBAT_FIREDAMAGE;
			} else if (tmpStrValue == "death") {
				it.abilities.get()->elementType = COMBAT_DEATHDAMAGE;
			} else if (tmpStrValue == "holy") {
				it.abilities.get()->elementType = COMBAT_HOLYDAMAGE;
			} else {
				std::cout << "[Warning - weapon:extraElement] Type \"" << element << "\" does not exist.\n";
			}
		} else {
			it.abilities.get()->elementType = tfs::lua::getNumber<CombatType_t>(L, 3);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerWeapon(LuaScriptInterface& lsi)
{
	registerEnum(lsi, WEAPON_NONE);
	registerEnum(lsi, WEAPON_SWORD);
	registerEnum(lsi, WEAPON_CLUB);
	registerEnum(lsi, WEAPON_AXE);
	registerEnum(lsi, WEAPON_SHIELD);
	registerEnum(lsi, WEAPON_DISTANCE);
	registerEnum(lsi, WEAPON_WAND);
	registerEnum(lsi, WEAPON_AMMO);
	registerEnum(lsi, WEAPON_QUIVER);

	registerEnum(lsi, AMMO_NONE);
	registerEnum(lsi, AMMO_BOLT);
	registerEnum(lsi, AMMO_ARROW);
	registerEnum(lsi, AMMO_SPEAR);
	registerEnum(lsi, AMMO_THROWINGSTAR);
	registerEnum(lsi, AMMO_THROWINGKNIFE);
	registerEnum(lsi, AMMO_STONE);
	registerEnum(lsi, AMMO_SNOWBALL);

	lsi.registerClass("Weapon", "", luaCreateWeapon);
	lsi.registerMethod("Weapon", "action", luaWeaponAction);
	lsi.registerMethod("Weapon", "register", luaWeaponRegister);
	lsi.registerMethod("Weapon", "id", luaWeaponId);
	lsi.registerMethod("Weapon", "level", luaWeaponLevel);
	lsi.registerMethod("Weapon", "magicLevel", luaWeaponMagicLevel);
	lsi.registerMethod("Weapon", "mana", luaWeaponMana);
	lsi.registerMethod("Weapon", "manaPercent", luaWeaponManaPercent);
	lsi.registerMethod("Weapon", "health", luaWeaponHealth);
	lsi.registerMethod("Weapon", "healthPercent", luaWeaponHealthPercent);
	lsi.registerMethod("Weapon", "soul", luaWeaponSoul);
	lsi.registerMethod("Weapon", "breakChance", luaWeaponBreakChance);
	lsi.registerMethod("Weapon", "premium", luaWeaponPremium);
	lsi.registerMethod("Weapon", "wieldUnproperly", luaWeaponUnproperly);
	lsi.registerMethod("Weapon", "vocation", luaWeaponVocation);
	lsi.registerMethod("Weapon", "onUseWeapon", luaWeaponOnUseWeapon);
	lsi.registerMethod("Weapon", "element", luaWeaponElement);
	lsi.registerMethod("Weapon", "attack", luaWeaponAttack);
	lsi.registerMethod("Weapon", "defense", luaWeaponDefense);
	lsi.registerMethod("Weapon", "range", luaWeaponRange);
	lsi.registerMethod("Weapon", "charges", luaWeaponCharges);
	lsi.registerMethod("Weapon", "duration", luaWeaponDuration);
	lsi.registerMethod("Weapon", "decayTo", luaWeaponDecayTo);
	lsi.registerMethod("Weapon", "transformEquipTo", luaWeaponTransformEquipTo);
	lsi.registerMethod("Weapon", "transformDeEquipTo", luaWeaponTransformDeEquipTo);
	lsi.registerMethod("Weapon", "slotType", luaWeaponSlotType);
	lsi.registerMethod("Weapon", "hitChance", luaWeaponHitChance);
	lsi.registerMethod("Weapon", "extraElement", luaWeaponExtraElement);

	// exclusively for distance weapons
	lsi.registerMethod("Weapon", "ammoType", luaWeaponAmmoType);
	lsi.registerMethod("Weapon", "maxHitChance", luaWeaponMaxHitChance);

	// exclusively for wands
	lsi.registerMethod("Weapon", "damage", luaWeaponWandDamage);

	// exclusively for wands & distance weapons
	lsi.registerMethod("Weapon", "shootType", luaWeaponShootType);
}
