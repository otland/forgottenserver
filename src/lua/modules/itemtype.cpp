#include "../../otpch.h"

#include "../../iomarket.h"
#include "../../item.h"
#include "../../items.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

namespace {

int luaItemTypeCreate(lua_State* L)
{
	// ItemType(id or name)
	uint32_t id;
	if (tfs::lua::isNumber(L, 2)) {
		id = tfs::lua::getNumber<uint32_t>(L, 2);
	} else if (lua_isstring(L, 2)) {
		id = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
	} else {
		lua_pushnil(L);
		return 1;
	}

	const ItemType& itemType = Item::items[id];
	tfs::lua::pushUserdata(L, &itemType);
	tfs::lua::setMetatable(L, -1, "ItemType");
	return 1;
}

int luaItemTypeIsCorpse(lua_State* L)
{
	// itemType:isCorpse()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->corpseType != RACE_NONE);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsDoor(lua_State* L)
{
	// itemType:isDoor()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isDoor());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsContainer(lua_State* L)
{
	// itemType:isContainer()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isContainer());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsFluidContainer(lua_State* L)
{
	// itemType:isFluidContainer()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isFluidContainer());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsMovable(lua_State* L)
{
	// itemType:isMovable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->moveable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsRune(lua_State* L)
{
	// itemType:isRune()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isRune());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsStackable(lua_State* L)
{
	// itemType:isStackable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->stackable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsReadable(lua_State* L)
{
	// itemType:isReadable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->canReadText);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsWritable(lua_State* L)
{
	// itemType:isWritable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->canWriteText);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsBlocking(lua_State* L)
{
	// itemType:isBlocking()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->blockProjectile || itemType->blockSolid);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsGroundTile(lua_State* L)
{
	// itemType:isGroundTile()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isGroundTile());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsMagicField(lua_State* L)
{
	// itemType:isMagicField()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isMagicField());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsUseable(lua_State* L)
{
	// itemType:isUseable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isUseable());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsPickupable(lua_State* L)
{
	// itemType:isPickupable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->isPickupable());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsRotatable(lua_State* L)
{
	// itemType:isRotatable()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->rotatable);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetType(lua_State* L)
{
	// itemType:getType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->type);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetGroup(lua_State* L)
{
	// itemType:getGroup()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->group);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetId(lua_State* L)
{
	// itemType:getId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->id);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetClientId(lua_State* L)
{
	// itemType:getClientId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->clientId);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetName(lua_State* L)
{
	// itemType:getName()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->name);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetPluralName(lua_State* L)
{
	// itemType:getPluralName()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->getPluralName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetRotateTo(lua_State* L)
{
	// itemType:getRotateTo()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->rotateTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetArticle(lua_State* L)
{
	// itemType:getArticle()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->article);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetDescription(lua_State* L)
{
	// itemType:getDescription()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->description);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetSlotPosition(lua_State* L)
{
	// itemType:getSlotPosition()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->slotPosition);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetCharges(lua_State* L)
{
	// itemType:getCharges()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->charges);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetFluidSource(lua_State* L)
{
	// itemType:getFluidSource()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->fluidSource);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetCapacity(lua_State* L)
{
	// itemType:getCapacity()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->maxItems);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetWeight(lua_State* L)
{
	// itemType:getWeight([count = 1])
	uint16_t count = tfs::lua::getNumber<uint16_t>(L, 2, 1);

	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	uint64_t weight = static_cast<uint64_t>(itemType->weight) * std::max<int32_t>(1, count);
	tfs::lua::pushNumber(L, weight);
	return 1;
}

int luaItemTypeGetWorth(lua_State* L)
{
	// itemType:getWorth()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushNumber(L, itemType->worth);
	return 1;
}

int luaItemTypeGetHitChance(lua_State* L)
{
	// itemType:getHitChance()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->hitChance);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetShootRange(lua_State* L)
{
	// itemType:getShootRange()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->shootRange);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetAttack(lua_State* L)
{
	// itemType:getAttack()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->attack);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetAttackSpeed(lua_State* L)
{
	// itemType:getAttackSpeed()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->attackSpeed);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetDefense(lua_State* L)
{
	// itemType:getDefense()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->defense);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetExtraDefense(lua_State* L)
{
	// itemType:getExtraDefense()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->extraDefense);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetArmor(lua_State* L)
{
	// itemType:getArmor()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->armor);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetWeaponType(lua_State* L)
{
	// itemType:getWeaponType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->weaponType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetAmmoType(lua_State* L)
{
	// itemType:getAmmoType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->ammoType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetCorpseType(lua_State* L)
{
	// itemType:getCorpseType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->corpseType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetClassification(lua_State* L)
{
	// itemType:getClassification()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->classification);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetAbilities(lua_State* L)
{
	// itemType:getAbilities()
	ItemType* itemType = tfs::lua::getUserdata<ItemType>(L, 1);
	if (itemType) {
		Abilities& abilities = itemType->getAbilities();
		lua_createtable(L, 10, 12);
		tfs::lua::setField(L, "healthGain", abilities.healthGain);
		tfs::lua::setField(L, "healthTicks", abilities.healthTicks);
		tfs::lua::setField(L, "manaGain", abilities.manaGain);
		tfs::lua::setField(L, "manaTicks", abilities.manaTicks);
		tfs::lua::setField(L, "conditionImmunities", abilities.conditionImmunities);
		tfs::lua::setField(L, "conditionSuppressions", abilities.conditionSuppressions);
		tfs::lua::setField(L, "speed", abilities.speed);
		tfs::lua::setField(L, "elementDamage", abilities.elementDamage);
		tfs::lua::setField(L, "elementType", abilities.elementType);

		lua_pushboolean(L, abilities.manaShield);
		lua_setfield(L, -2, "manaShield");
		lua_pushboolean(L, abilities.invisible);
		lua_setfield(L, -2, "invisible");
		lua_pushboolean(L, abilities.regeneration);
		lua_setfield(L, -2, "regeneration");

		// Stats
		lua_createtable(L, 0, STAT_LAST + 1);
		for (int32_t i = STAT_FIRST; i <= STAT_LAST; i++) {
			tfs::lua::pushNumber(L, abilities.stats[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "stats");

		// Stats percent
		lua_createtable(L, 0, STAT_LAST + 1);
		for (int32_t i = STAT_FIRST; i <= STAT_LAST; i++) {
			tfs::lua::pushNumber(L, abilities.statsPercent[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "statsPercent");

		// Skills
		lua_createtable(L, 0, SKILL_LAST + 1);
		for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; i++) {
			tfs::lua::pushNumber(L, abilities.skills[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "skills");

		// Special skills
		lua_createtable(L, 0, SPECIALSKILL_LAST + 1);
		for (int32_t i = SPECIALSKILL_FIRST; i <= SPECIALSKILL_LAST; i++) {
			tfs::lua::pushNumber(L, abilities.specialSkills[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "specialSkills");

		// Field absorb percent
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			tfs::lua::pushNumber(L, abilities.fieldAbsorbPercent[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "fieldAbsorbPercent");

		// Absorb percent
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			tfs::lua::pushNumber(L, abilities.absorbPercent[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "absorbPercent");

		// special magic level
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			tfs::lua::pushNumber(L, abilities.specialMagicLevelSkill[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "specialMagicLevel");

		// Damage boost percent
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			tfs::lua::pushNumber(L, abilities.boostPercent[i]);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "boostPercent");

		// Reflect chance
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			tfs::lua::pushNumber(L, abilities.reflect[i].chance);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "reflectChance");

		// Reflect percent
		lua_createtable(L, 0, COMBAT_COUNT);
		for (int32_t i = 0; i < COMBAT_COUNT; i++) {
			tfs::lua::pushNumber(L, abilities.reflect[i].percent);
			lua_rawseti(L, -2, i + 1);
		}
		lua_setfield(L, -2, "reflectPercent");
	}
	return 1;
}

int luaItemTypeHasShowAttributes(lua_State* L)
{
	// itemType:hasShowAttributes()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->showAttributes);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeHasShowCount(lua_State* L)
{
	// itemType:hasShowCount()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->showCount);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeHasShowCharges(lua_State* L)
{
	// itemType:hasShowCharges()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->showCharges);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeHasShowDuration(lua_State* L)
{
	// itemType:hasShowDuration()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->showDuration);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeHasAllowDistRead(lua_State* L)
{
	// itemType:hasAllowDistRead()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->allowDistRead);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetWieldInfo(lua_State* L)
{
	// itemType:getWieldInfo()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->wieldInfo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetDurationMin(lua_State* L)
{
	// itemType:getDurationMin()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->decayTimeMin);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetDurationMax(lua_State* L)
{
	// itemType:getDurationMax()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->decayTimeMax);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetLevelDoor(lua_State* L)
{
	// itemType:getLevelDoor()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->levelDoor);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetRuneSpellName(lua_State* L)
{
	// itemType:getRuneSpellName()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType && itemType->isRune()) {
		tfs::lua::pushString(L, itemType->runeSpellName);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetVocationString(lua_State* L)
{
	// itemType:getVocationString()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushString(L, itemType->vocationString);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetMinReqLevel(lua_State* L)
{
	// itemType:getMinReqLevel()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->minReqLevel);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetMinReqMagicLevel(lua_State* L)
{
	// itemType:getMinReqMagicLevel()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->minReqMagicLevel);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetMarketBuyStatistics(lua_State* L)
{
	// itemType:getMarketBuyStatistics()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		MarketStatistics* statistics = tfs::iomarket::getPurchaseStatistics(itemType->id);
		if (statistics) {
			lua_createtable(L, 4, 0);
			tfs::lua::setField(L, "numTransactions", statistics->numTransactions);
			tfs::lua::setField(L, "totalPrice", statistics->totalPrice);
			tfs::lua::setField(L, "highestPrice", statistics->highestPrice);
			tfs::lua::setField(L, "lowestPrice", statistics->lowestPrice);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetMarketSellStatistics(lua_State* L)
{
	// itemType:getMarketSellStatistics()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		MarketStatistics* statistics = tfs::iomarket::getSaleStatistics(itemType->id);
		if (statistics) {
			lua_createtable(L, 4, 0);
			tfs::lua::setField(L, "numTransactions", statistics->numTransactions);
			tfs::lua::setField(L, "totalPrice", statistics->totalPrice);
			tfs::lua::setField(L, "highestPrice", statistics->highestPrice);
			tfs::lua::setField(L, "lowestPrice", statistics->lowestPrice);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetElementType(lua_State* L)
{
	// itemType:getElementType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	auto& abilities = itemType->abilities;
	if (abilities) {
		tfs::lua::pushNumber(L, abilities->elementType);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetElementDamage(lua_State* L)
{
	// itemType:getElementDamage()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (!itemType) {
		lua_pushnil(L);
		return 1;
	}

	auto& abilities = itemType->abilities;
	if (abilities) {
		tfs::lua::pushNumber(L, abilities->elementDamage);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetTransformEquipId(lua_State* L)
{
	// itemType:getTransformEquipId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->transformEquipTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetTransformDeEquipId(lua_State* L)
{
	// itemType:getTransformDeEquipId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->transformDeEquipTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetDestroyId(lua_State* L)
{
	// itemType:getDestroyId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->destroyTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetDecayId(lua_State* L)
{
	// itemType:getDecayId()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->decayTo);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeGetRequiredLevel(lua_State* L)
{
	// itemType:getRequiredLevel()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushNumber(L, itemType->minReqLevel);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeHasSubType(lua_State* L)
{
	// itemType:hasSubType()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->hasSubType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaItemTypeIsStoreItem(lua_State* L)
{
	// itemType:isStoreItem()
	const ItemType* itemType = tfs::lua::getUserdata<const ItemType>(L, 1);
	if (itemType) {
		tfs::lua::pushBoolean(L, itemType->storeItem);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerItemType(LuaScriptInterface& lsi)
{
	registerEnum(lsi, ITEM_ATTRIBUTE_NONE);
	registerEnum(lsi, ITEM_ATTRIBUTE_ACTIONID);
	registerEnum(lsi, ITEM_ATTRIBUTE_UNIQUEID);
	registerEnum(lsi, ITEM_ATTRIBUTE_DESCRIPTION);
	registerEnum(lsi, ITEM_ATTRIBUTE_TEXT);
	registerEnum(lsi, ITEM_ATTRIBUTE_DATE);
	registerEnum(lsi, ITEM_ATTRIBUTE_WRITER);
	registerEnum(lsi, ITEM_ATTRIBUTE_NAME);
	registerEnum(lsi, ITEM_ATTRIBUTE_ARTICLE);
	registerEnum(lsi, ITEM_ATTRIBUTE_PLURALNAME);
	registerEnum(lsi, ITEM_ATTRIBUTE_WEIGHT);
	registerEnum(lsi, ITEM_ATTRIBUTE_ATTACK);
	registerEnum(lsi, ITEM_ATTRIBUTE_DEFENSE);
	registerEnum(lsi, ITEM_ATTRIBUTE_EXTRADEFENSE);
	registerEnum(lsi, ITEM_ATTRIBUTE_ARMOR);
	registerEnum(lsi, ITEM_ATTRIBUTE_HITCHANCE);
	registerEnum(lsi, ITEM_ATTRIBUTE_SHOOTRANGE);
	registerEnum(lsi, ITEM_ATTRIBUTE_OWNER);
	registerEnum(lsi, ITEM_ATTRIBUTE_DURATION);
	registerEnum(lsi, ITEM_ATTRIBUTE_DECAYSTATE);
	registerEnum(lsi, ITEM_ATTRIBUTE_CORPSEOWNER);
	registerEnum(lsi, ITEM_ATTRIBUTE_CHARGES);
	registerEnum(lsi, ITEM_ATTRIBUTE_FLUIDTYPE);
	registerEnum(lsi, ITEM_ATTRIBUTE_DOORID);
	registerEnum(lsi, ITEM_ATTRIBUTE_DECAYTO);
	registerEnum(lsi, ITEM_ATTRIBUTE_WRAPID);
	registerEnum(lsi, ITEM_ATTRIBUTE_STOREITEM);
	registerEnum(lsi, ITEM_ATTRIBUTE_ATTACK_SPEED);
	registerEnum(lsi, ITEM_ATTRIBUTE_OPENCONTAINER);
	registerEnum(lsi, ITEM_ATTRIBUTE_DURATION_MIN);
	registerEnum(lsi, ITEM_ATTRIBUTE_DURATION_MAX);

	registerEnum(lsi, ITEM_BROWSEFIELD);
	registerEnum(lsi, ITEM_BAG);
	registerEnum(lsi, ITEM_SHOPPING_BAG);
	registerEnum(lsi, ITEM_GOLD_COIN);
	registerEnum(lsi, ITEM_PLATINUM_COIN);
	registerEnum(lsi, ITEM_CRYSTAL_COIN);
	registerEnum(lsi, ITEM_AMULETOFLOSS);
	registerEnum(lsi, ITEM_PARCEL);
	registerEnum(lsi, ITEM_LABEL);
	registerEnum(lsi, ITEM_FIREFIELD_PVP_FULL);
	registerEnum(lsi, ITEM_FIREFIELD_PVP_MEDIUM);
	registerEnum(lsi, ITEM_FIREFIELD_PVP_SMALL);
	registerEnum(lsi, ITEM_FIREFIELD_PERSISTENT_FULL);
	registerEnum(lsi, ITEM_FIREFIELD_PERSISTENT_MEDIUM);
	registerEnum(lsi, ITEM_FIREFIELD_PERSISTENT_SMALL);
	registerEnum(lsi, ITEM_FIREFIELD_NOPVP);
	registerEnum(lsi, ITEM_FIREFIELD_NOPVP_MEDIUM);
	registerEnum(lsi, ITEM_POISONFIELD_PVP);
	registerEnum(lsi, ITEM_POISONFIELD_PERSISTENT);
	registerEnum(lsi, ITEM_POISONFIELD_NOPVP);
	registerEnum(lsi, ITEM_ENERGYFIELD_PVP);
	registerEnum(lsi, ITEM_ENERGYFIELD_PERSISTENT);
	registerEnum(lsi, ITEM_ENERGYFIELD_NOPVP);
	registerEnum(lsi, ITEM_MAGICWALL);
	registerEnum(lsi, ITEM_MAGICWALL_PERSISTENT);
	registerEnum(lsi, ITEM_MAGICWALL_SAFE);
	registerEnum(lsi, ITEM_WILDGROWTH);
	registerEnum(lsi, ITEM_WILDGROWTH_PERSISTENT);
	registerEnum(lsi, ITEM_WILDGROWTH_SAFE);
	registerEnum(lsi, ITEM_DECORATION_KIT);
	registerEnum(lsi, ITEM_MARKET);

	registerEnum(lsi, ITEM_TYPE_DEPOT);
	registerEnum(lsi, ITEM_TYPE_MAILBOX);
	registerEnum(lsi, ITEM_TYPE_TRASHHOLDER);
	registerEnum(lsi, ITEM_TYPE_CONTAINER);
	registerEnum(lsi, ITEM_TYPE_DOOR);
	registerEnum(lsi, ITEM_TYPE_MAGICFIELD);
	registerEnum(lsi, ITEM_TYPE_TELEPORT);
	registerEnum(lsi, ITEM_TYPE_KEY);
	registerEnum(lsi, ITEM_TYPE_RUNE);
	registerEnum(lsi, ITEM_TYPE_PODIUM);

	registerEnum(lsi, ITEM_GROUP_GROUND);
	registerEnum(lsi, ITEM_GROUP_CONTAINER);
	registerEnum(lsi, ITEM_GROUP_WEAPON);
	registerEnum(lsi, ITEM_GROUP_AMMUNITION);
	registerEnum(lsi, ITEM_GROUP_ARMOR);
	registerEnum(lsi, ITEM_GROUP_CHARGES);
	registerEnum(lsi, ITEM_GROUP_TELEPORT);
	registerEnum(lsi, ITEM_GROUP_MAGICFIELD);
	registerEnum(lsi, ITEM_GROUP_WRITEABLE);
	registerEnum(lsi, ITEM_GROUP_KEY);
	registerEnum(lsi, ITEM_GROUP_SPLASH);
	registerEnum(lsi, ITEM_GROUP_FLUID);
	registerEnum(lsi, ITEM_GROUP_DOOR);
	registerEnum(lsi, ITEM_GROUP_DEPRECATED);
	registerEnum(lsi, ITEM_GROUP_PODIUM);

	registerEnum(lsi, SLOTP_WHEREEVER);
	registerEnum(lsi, SLOTP_HEAD);
	registerEnum(lsi, SLOTP_NECKLACE);
	registerEnum(lsi, SLOTP_BACKPACK);
	registerEnum(lsi, SLOTP_ARMOR);
	registerEnum(lsi, SLOTP_RIGHT);
	registerEnum(lsi, SLOTP_LEFT);
	registerEnum(lsi, SLOTP_LEGS);
	registerEnum(lsi, SLOTP_FEET);
	registerEnum(lsi, SLOTP_RING);
	registerEnum(lsi, SLOTP_AMMO);
	registerEnum(lsi, SLOTP_DEPOT);
	registerEnum(lsi, SLOTP_TWO_HAND);

	registerEnum(lsi, FLUID_NONE);
	registerEnum(lsi, FLUID_WATER);
	registerEnum(lsi, FLUID_BLOOD);
	registerEnum(lsi, FLUID_BEER);
	registerEnum(lsi, FLUID_SLIME);
	registerEnum(lsi, FLUID_LEMONADE);
	registerEnum(lsi, FLUID_MILK);
	registerEnum(lsi, FLUID_MANA);
	registerEnum(lsi, FLUID_LIFE);
	registerEnum(lsi, FLUID_OIL);
	registerEnum(lsi, FLUID_URINE);
	registerEnum(lsi, FLUID_COCONUTMILK);
	registerEnum(lsi, FLUID_WINE);
	registerEnum(lsi, FLUID_MUD);
	registerEnum(lsi, FLUID_FRUITJUICE);
	registerEnum(lsi, FLUID_LAVA);
	registerEnum(lsi, FLUID_RUM);
	registerEnum(lsi, FLUID_SWAMP);
	registerEnum(lsi, FLUID_TEA);
	registerEnum(lsi, FLUID_MEAD);

	lsi.registerClass("ItemType", "", luaItemTypeCreate);
	lsi.registerMetaMethod("ItemType", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("ItemType", "isCorpse", luaItemTypeIsCorpse);
	lsi.registerMethod("ItemType", "isDoor", luaItemTypeIsDoor);
	lsi.registerMethod("ItemType", "isContainer", luaItemTypeIsContainer);
	lsi.registerMethod("ItemType", "isFluidContainer", luaItemTypeIsFluidContainer);
	lsi.registerMethod("ItemType", "isMovable", luaItemTypeIsMovable);
	lsi.registerMethod("ItemType", "isRune", luaItemTypeIsRune);
	lsi.registerMethod("ItemType", "isStackable", luaItemTypeIsStackable);
	lsi.registerMethod("ItemType", "isReadable", luaItemTypeIsReadable);
	lsi.registerMethod("ItemType", "isWritable", luaItemTypeIsWritable);
	lsi.registerMethod("ItemType", "isBlocking", luaItemTypeIsBlocking);
	lsi.registerMethod("ItemType", "isGroundTile", luaItemTypeIsGroundTile);
	lsi.registerMethod("ItemType", "isMagicField", luaItemTypeIsMagicField);
	lsi.registerMethod("ItemType", "isUseable", luaItemTypeIsUseable);
	lsi.registerMethod("ItemType", "isPickupable", luaItemTypeIsPickupable);
	lsi.registerMethod("ItemType", "isRotatable", luaItemTypeIsRotatable);

	lsi.registerMethod("ItemType", "getType", luaItemTypeGetType);
	lsi.registerMethod("ItemType", "getGroup", luaItemTypeGetGroup);
	lsi.registerMethod("ItemType", "getId", luaItemTypeGetId);
	lsi.registerMethod("ItemType", "getClientId", luaItemTypeGetClientId);
	lsi.registerMethod("ItemType", "getName", luaItemTypeGetName);
	lsi.registerMethod("ItemType", "getPluralName", luaItemTypeGetPluralName);
	lsi.registerMethod("ItemType", "getRotateTo", luaItemTypeGetRotateTo);
	lsi.registerMethod("ItemType", "getArticle", luaItemTypeGetArticle);
	lsi.registerMethod("ItemType", "getDescription", luaItemTypeGetDescription);
	lsi.registerMethod("ItemType", "getSlotPosition", luaItemTypeGetSlotPosition);

	lsi.registerMethod("ItemType", "getCharges", luaItemTypeGetCharges);
	lsi.registerMethod("ItemType", "getFluidSource", luaItemTypeGetFluidSource);
	lsi.registerMethod("ItemType", "getCapacity", luaItemTypeGetCapacity);
	lsi.registerMethod("ItemType", "getWeight", luaItemTypeGetWeight);
	lsi.registerMethod("ItemType", "getWorth", luaItemTypeGetWorth);

	lsi.registerMethod("ItemType", "getHitChance", luaItemTypeGetHitChance);
	lsi.registerMethod("ItemType", "getShootRange", luaItemTypeGetShootRange);

	lsi.registerMethod("ItemType", "getAttack", luaItemTypeGetAttack);
	lsi.registerMethod("ItemType", "getAttackSpeed", luaItemTypeGetAttackSpeed);
	lsi.registerMethod("ItemType", "getDefense", luaItemTypeGetDefense);
	lsi.registerMethod("ItemType", "getExtraDefense", luaItemTypeGetExtraDefense);
	lsi.registerMethod("ItemType", "getArmor", luaItemTypeGetArmor);
	lsi.registerMethod("ItemType", "getWeaponType", luaItemTypeGetWeaponType);

	lsi.registerMethod("ItemType", "getElementType", luaItemTypeGetElementType);
	lsi.registerMethod("ItemType", "getElementDamage", luaItemTypeGetElementDamage);

	lsi.registerMethod("ItemType", "getTransformEquipId", luaItemTypeGetTransformEquipId);
	lsi.registerMethod("ItemType", "getTransformDeEquipId", luaItemTypeGetTransformDeEquipId);
	lsi.registerMethod("ItemType", "getDestroyId", luaItemTypeGetDestroyId);
	lsi.registerMethod("ItemType", "getDecayId", luaItemTypeGetDecayId);
	lsi.registerMethod("ItemType", "getRequiredLevel", luaItemTypeGetRequiredLevel);
	lsi.registerMethod("ItemType", "getAmmoType", luaItemTypeGetAmmoType);
	lsi.registerMethod("ItemType", "getCorpseType", luaItemTypeGetCorpseType);
	lsi.registerMethod("ItemType", "getClassification", luaItemTypeGetClassification);

	lsi.registerMethod("ItemType", "getAbilities", luaItemTypeGetAbilities);

	lsi.registerMethod("ItemType", "hasShowAttributes", luaItemTypeHasShowAttributes);
	lsi.registerMethod("ItemType", "hasShowCount", luaItemTypeHasShowCount);
	lsi.registerMethod("ItemType", "hasShowCharges", luaItemTypeHasShowCharges);
	lsi.registerMethod("ItemType", "hasShowDuration", luaItemTypeHasShowDuration);
	lsi.registerMethod("ItemType", "hasAllowDistRead", luaItemTypeHasAllowDistRead);
	lsi.registerMethod("ItemType", "getWieldInfo", luaItemTypeGetWieldInfo);
	lsi.registerMethod("ItemType", "getDurationMin", luaItemTypeGetDurationMin);
	lsi.registerMethod("ItemType", "getDurationMax", luaItemTypeGetDurationMax);
	lsi.registerMethod("ItemType", "getLevelDoor", luaItemTypeGetLevelDoor);
	lsi.registerMethod("ItemType", "getRuneSpellName", luaItemTypeGetRuneSpellName);
	lsi.registerMethod("ItemType", "getVocationString", luaItemTypeGetVocationString);
	lsi.registerMethod("ItemType", "getMinReqLevel", luaItemTypeGetMinReqLevel);
	lsi.registerMethod("ItemType", "getMinReqMagicLevel", luaItemTypeGetMinReqMagicLevel);
	lsi.registerMethod("ItemType", "getMarketBuyStatistics", luaItemTypeGetMarketBuyStatistics);
	lsi.registerMethod("ItemType", "getMarketSellStatistics", luaItemTypeGetMarketSellStatistics);

	lsi.registerMethod("ItemType", "hasSubType", luaItemTypeHasSubType);

	lsi.registerMethod("ItemType", "isStoreItem", luaItemTypeIsStoreItem);
}
