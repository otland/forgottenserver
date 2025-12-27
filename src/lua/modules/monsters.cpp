#include "../../otpch.h"

#include "../../monsters.h"

#include "../../item.h"
#include "../../script.h"
#include "../../spells.h"
#include "../../tools.h"
#include "../api.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Monsters g_monsters;
extern Scripts* g_scripts;

namespace {

int luaCreateMonsterSpell(lua_State* L)
{
	// MonsterSpell() will create a new Monster Spell
	tfs::lua::pushUserdata(L, new MonsterSpell);
	tfs::lua::setMetatable(L, -1, "MonsterSpell");
	return 1;
}

int luaDeleteMonsterSpell(lua_State* L)
{
	// monsterSpell:delete() monsterSpell:__gc()
	MonsterSpell** monsterSpellPtr = tfs::lua::getRawUserdata<MonsterSpell>(L, 1);
	if (monsterSpellPtr && *monsterSpellPtr) {
		delete *monsterSpellPtr;
		*monsterSpellPtr = nullptr;
	}
	return 0;
}

int luaMonsterSpellSetType(lua_State* L)
{
	// monsterSpell:setType(type)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->name = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetScriptName(lua_State* L)
{
	// monsterSpell:setScriptName(name)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->scriptName = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetChance(lua_State* L)
{
	// monsterSpell:setChance(chance)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->chance = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetInterval(lua_State* L)
{
	// monsterSpell:setInterval(interval)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->interval = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetRange(lua_State* L)
{
	// monsterSpell:setRange(range)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->range = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetCombatValue(lua_State* L)
{
	// monsterSpell:setCombatValue(min, max)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->minCombatValue = tfs::lua::getNumber<int32_t>(L, 2);
		spell->maxCombatValue = tfs::lua::getNumber<int32_t>(L, 3);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetCombatType(lua_State* L)
{
	// monsterSpell:setCombatType(combatType_t)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->combatType = tfs::lua::getNumber<CombatType_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetAttackValue(lua_State* L)
{
	// monsterSpell:setAttackValue(attack, skill)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->attack = tfs::lua::getNumber<int32_t>(L, 2);
		spell->skill = tfs::lua::getNumber<int32_t>(L, 3);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetNeedTarget(lua_State* L)
{
	// monsterSpell:setNeedTarget(bool)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->needTarget = tfs::lua::getBoolean(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetNeedDirection(lua_State* L)
{
	// monsterSpell:setNeedDirection(bool)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->needDirection = tfs::lua::getBoolean(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetCombatLength(lua_State* L)
{
	// monsterSpell:setCombatLength(length)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->length = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetCombatSpread(lua_State* L)
{
	// monsterSpell:setCombatSpread(spread)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->spread = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetCombatRadius(lua_State* L)
{
	// monsterSpell:setCombatRadius(radius)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->radius = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetCombatRing(lua_State* L)
{
	// monsterSpell:setCombatRing(ring)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->ring = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetConditionType(lua_State* L)
{
	// monsterSpell:setConditionType(type)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetConditionDamage(lua_State* L)
{
	// monsterSpell:setConditionDamage(min, max, start)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->conditionMinDamage = tfs::lua::getNumber<int32_t>(L, 2);
		spell->conditionMaxDamage = tfs::lua::getNumber<int32_t>(L, 3);
		spell->conditionStartDamage = tfs::lua::getNumber<int32_t>(L, 4);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetConditionSpeedChange(lua_State* L)
{
	// monsterSpell:setConditionSpeedChange(minSpeed[, maxSpeed])
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->minSpeedChange = tfs::lua::getNumber<int32_t>(L, 2);
		spell->maxSpeedChange = tfs::lua::getNumber<int32_t>(L, 3, 0);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetConditionDuration(lua_State* L)
{
	// monsterSpell:setConditionDuration(duration)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->duration = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetConditionDrunkenness(lua_State* L)
{
	// monsterSpell:setConditionDrunkenness(drunkenness)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->drunkenness = tfs::lua::getNumber<uint8_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetConditionTickInterval(lua_State* L)
{
	// monsterSpell:setConditionTickInterval(interval)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->tickInterval = tfs::lua::getNumber<int32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetCombatShootEffect(lua_State* L)
{
	// monsterSpell:setCombatShootEffect(effect)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->shoot = tfs::lua::getNumber<ShootType_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetCombatEffect(lua_State* L)
{
	// monsterSpell:setCombatEffect(effect)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		spell->effect = tfs::lua::getNumber<MagicEffectClasses>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterSpellSetOutfit(lua_State* L)
{
	// monsterSpell:setOutfit(outfit)
	MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 1);
	if (spell) {
		if (lua_istable(L, 2)) {
			spell->outfit = tfs::lua::getOutfit(L, 2);
		} else if (tfs::lua::isNumber(L, 2)) {
			spell->outfit.lookTypeEx = tfs::lua::getNumber<uint16_t>(L, 2);
		} else if (lua_isstring(L, 2)) {
			MonsterType* mType = g_monsters.getMonsterType(tfs::lua::getString(L, 2));
			if (mType) {
				spell->outfit = mType->info.outfit;
			}
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeCreate(lua_State* L)
{
	// MonsterType(name or raceId)
	MonsterType* monsterType;
	if (tfs::lua::isNumber(L, 2)) {
		monsterType = g_monsters.getMonsterType(tfs::lua::getNumber<uint32_t>(L, 2));
	} else {
		monsterType = g_monsters.getMonsterType(tfs::lua::getString(L, 2));
	}

	if (monsterType) {
		tfs::lua::pushUserdata(L, monsterType);
		tfs::lua::setMetatable(L, -1, "MonsterType");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsAttackable(lua_State* L)
{
	// get: monsterType:isAttackable() set: monsterType:isAttackable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isAttackable);
		} else {
			monsterType->info.isAttackable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsChallengeable(lua_State* L)
{
	// get: monsterType:isChallengeable() set: monsterType:isChallengeable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isChallengeable);
		} else {
			monsterType->info.isChallengeable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsConvinceable(lua_State* L)
{
	// get: monsterType:isConvinceable() set: monsterType:isConvinceable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isConvinceable);
		} else {
			monsterType->info.isConvinceable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsSummonable(lua_State* L)
{
	// get: monsterType:isSummonable() set: monsterType:isSummonable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isSummonable);
		} else {
			monsterType->info.isSummonable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsIgnoringSpawnBlock(lua_State* L)
{
	// get: monsterType:isIgnoringSpawnBlock() set: monsterType:isIgnoringSpawnBlock(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isIgnoringSpawnBlock);
		} else {
			monsterType->info.isIgnoringSpawnBlock = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsIllusionable(lua_State* L)
{
	// get: monsterType:isIllusionable() set: monsterType:isIllusionable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isIllusionable);
		} else {
			monsterType->info.isIllusionable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsHostile(lua_State* L)
{
	// get: monsterType:isHostile() set: monsterType:isHostile(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isHostile);
		} else {
			monsterType->info.isHostile = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsPushable(lua_State* L)
{
	// get: monsterType:isPushable() set: monsterType:isPushable(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.pushable);
		} else {
			monsterType->info.pushable = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsHealthHidden(lua_State* L)
{
	// get: monsterType:isHealthHidden() set: monsterType:isHealthHidden(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.hiddenHealth);
		} else {
			monsterType->info.hiddenHealth = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeIsBoss(lua_State* L)
{
	// get: monsterType:isBoss() set: monsterType:isBoss(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.isBoss);
		} else {
			monsterType->info.isBoss = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeCanPushItems(lua_State* L)
{
	// get: monsterType:canPushItems() set: monsterType:canPushItems(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canPushItems);
		} else {
			monsterType->info.canPushItems = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeCanPushCreatures(lua_State* L)
{
	// get: monsterType:canPushCreatures() set: monsterType:canPushCreatures(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canPushCreatures);
		} else {
			monsterType->info.canPushCreatures = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeCanWalkOnEnergy(lua_State* L)
{
	// get: monsterType:canWalkOnEnergy() set: monsterType:canWalkOnEnergy(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canWalkOnEnergy);
		} else {
			monsterType->info.canWalkOnEnergy = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeCanWalkOnFire(lua_State* L)
{
	// get: monsterType:canWalkOnFire() set: monsterType:canWalkOnFire(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canWalkOnFire);
		} else {
			monsterType->info.canWalkOnFire = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeCanWalkOnPoison(lua_State* L)
{
	// get: monsterType:canWalkOnPoison() set: monsterType:canWalkOnPoison(bool)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, monsterType->info.canWalkOnPoison);
		} else {
			monsterType->info.canWalkOnPoison = tfs::lua::getBoolean(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t luaMonsterTypeName(lua_State* L)
{
	// get: monsterType:name() set: monsterType:name(name)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushString(L, monsterType->name);
		} else {
			monsterType->name = tfs::lua::getString(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeNameDescription(lua_State* L)
{
	// get: monsterType:nameDescription() set: monsterType:nameDescription(desc)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushString(L, monsterType->nameDescription);
		} else {
			monsterType->nameDescription = tfs::lua::getString(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t luaMonsterTypeMonsterName(lua_State* L)
{
	// monsterType:monsterName()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		tfs::lua::pushString(L, monsterType->monsterName);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeHealth(lua_State* L)
{
	// get: monsterType:health() set: monsterType:health(health)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.health);
		} else {
			monsterType->info.health = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeMaxHealth(lua_State* L)
{
	// get: monsterType:maxHealth() set: monsterType:maxHealth(health)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.healthMax);
		} else {
			monsterType->info.healthMax = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeRunHealth(lua_State* L)
{
	// get: monsterType:runHealth() set: monsterType:runHealth(health)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.runAwayHealth);
		} else {
			monsterType->info.runAwayHealth = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeExperience(lua_State* L)
{
	// get: monsterType:experience() set: monsterType:experience(exp)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.experience);
		} else {
			monsterType->info.experience = tfs::lua::getNumber<uint64_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeSkull(lua_State* L)
{
	// get: monsterType:skull() set: monsterType:skull(str/constant)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.skull);
		} else {
			if (tfs::lua::isNumber(L, 2)) {
				monsterType->info.skull = tfs::lua::getNumber<Skulls_t>(L, 2);
			} else {
				monsterType->info.skull = getSkullType(tfs::lua::getString(L, 2));
			}
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeCombatImmunities(lua_State* L)
{
	// get: monsterType:combatImmunities() set: monsterType:combatImmunities(immunity)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.damageImmunities);
		} else {
			std::string immunity = tfs::lua::getString(L, 2);
			if (immunity == "physical") {
				monsterType->info.damageImmunities |= COMBAT_PHYSICALDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "energy") {
				monsterType->info.damageImmunities |= COMBAT_ENERGYDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "fire") {
				monsterType->info.damageImmunities |= COMBAT_FIREDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "poison" || immunity == "earth") {
				monsterType->info.damageImmunities |= COMBAT_EARTHDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "drown") {
				monsterType->info.damageImmunities |= COMBAT_DROWNDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "ice") {
				monsterType->info.damageImmunities |= COMBAT_ICEDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "holy") {
				monsterType->info.damageImmunities |= COMBAT_HOLYDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "death") {
				monsterType->info.damageImmunities |= COMBAT_DEATHDAMAGE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "lifedrain") {
				monsterType->info.damageImmunities |= COMBAT_LIFEDRAIN;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "manadrain") {
				monsterType->info.damageImmunities |= COMBAT_MANADRAIN;
				tfs::lua::pushBoolean(L, true);
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown immunity name " << immunity
				          << " for monster: " << monsterType->name << '\n';
				lua_pushnil(L);
			}
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeConditionImmunities(lua_State* L)
{
	// get: monsterType:conditionImmunities() set: monsterType:conditionImmunities(immunity)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.conditionImmunities);
		} else {
			std::string immunity = tfs::lua::getString(L, 2);
			if (immunity == "physical") {
				monsterType->info.conditionImmunities |= CONDITION_BLEEDING;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "energy") {
				monsterType->info.conditionImmunities |= CONDITION_ENERGY;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "fire") {
				monsterType->info.conditionImmunities |= CONDITION_FIRE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "poison" || immunity == "earth") {
				monsterType->info.conditionImmunities |= CONDITION_POISON;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "drown") {
				monsterType->info.conditionImmunities |= CONDITION_DROWN;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "ice") {
				monsterType->info.conditionImmunities |= CONDITION_FREEZING;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "holy") {
				monsterType->info.conditionImmunities |= CONDITION_DAZZLED;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "death") {
				monsterType->info.conditionImmunities |= CONDITION_CURSED;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "paralyze") {
				monsterType->info.conditionImmunities |= CONDITION_PARALYZE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "outfit") {
				monsterType->info.conditionImmunities |= CONDITION_OUTFIT;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "drunk") {
				monsterType->info.conditionImmunities |= CONDITION_DRUNK;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "invisible" || immunity == "invisibility") {
				monsterType->info.conditionImmunities |= CONDITION_INVISIBLE;
				tfs::lua::pushBoolean(L, true);
			} else if (immunity == "bleed") {
				monsterType->info.conditionImmunities |= CONDITION_BLEEDING;
				tfs::lua::pushBoolean(L, true);
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown immunity name " << immunity
				          << " for monster: " << monsterType->name << '\n';
				lua_pushnil(L);
			}
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeGetAttackList(lua_State* L)
{
	// monsterType:getAttackList()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, monsterType->info.attackSpells.size(), 0);

	int index = 0;
	for (const auto& spellBlock : monsterType->info.attackSpells) {
		lua_createtable(L, 0, 8);

		tfs::lua::setField(L, "chance", spellBlock.chance);
		tfs::lua::setField(L, "isCombatSpell", spellBlock.combatSpell ? 1 : 0);
		tfs::lua::setField(L, "isMelee", spellBlock.isMelee ? 1 : 0);
		tfs::lua::setField(L, "minCombatValue", spellBlock.minCombatValue);
		tfs::lua::setField(L, "maxCombatValue", spellBlock.maxCombatValue);
		tfs::lua::setField(L, "range", spellBlock.range);
		tfs::lua::setField(L, "speed", spellBlock.speed);
		tfs::lua::pushUserdata(L, static_cast<CombatSpell*>(spellBlock.spell));
		lua_setfield(L, -2, "spell");

		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaMonsterTypeAddAttack(lua_State* L)
{
	// monsterType:addAttack(monsterspell)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 2);
		if (spell) {
			spellBlock_t sb;
			if (g_monsters.deserializeSpell(spell, sb, monsterType->name)) {
				monsterType->info.attackSpells.push_back(std::move(sb));
			} else {
				std::cout << monsterType->name << '\n';
				std::cout << "[Warning - Monsters::loadMonster] Cant load spell. " << spell->name << '\n';
			}
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeGetDefenseList(lua_State* L)
{
	// monsterType:getDefenseList()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, monsterType->info.defenseSpells.size(), 0);

	int index = 0;
	for (const auto& spellBlock : monsterType->info.defenseSpells) {
		lua_createtable(L, 0, 8);

		tfs::lua::setField(L, "chance", spellBlock.chance);
		tfs::lua::setField(L, "isCombatSpell", spellBlock.combatSpell ? 1 : 0);
		tfs::lua::setField(L, "isMelee", spellBlock.isMelee ? 1 : 0);
		tfs::lua::setField(L, "minCombatValue", spellBlock.minCombatValue);
		tfs::lua::setField(L, "maxCombatValue", spellBlock.maxCombatValue);
		tfs::lua::setField(L, "range", spellBlock.range);
		tfs::lua::setField(L, "speed", spellBlock.speed);
		tfs::lua::pushUserdata(L, static_cast<CombatSpell*>(spellBlock.spell));
		lua_setfield(L, -2, "spell");

		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaMonsterTypeAddDefense(lua_State* L)
{
	// monsterType:addDefense(monsterspell)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		MonsterSpell* spell = tfs::lua::getUserdata<MonsterSpell>(L, 2);
		if (spell) {
			spellBlock_t sb;
			if (g_monsters.deserializeSpell(spell, sb, monsterType->name)) {
				monsterType->info.defenseSpells.push_back(std::move(sb));
			} else {
				std::cout << monsterType->name << '\n';
				std::cout << "[Warning - Monsters::loadMonster] Cant load spell. " << spell->name << '\n';
			}
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeGetElementList(lua_State* L)
{
	// monsterType:getElementList()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, monsterType->info.elementMap.size(), 0);
	for (auto&& [combatType, percent] : monsterType->info.elementMap | std::views::as_const) {
		tfs::lua::pushNumber(L, percent);
		lua_rawseti(L, -2, combatType);
	}
	return 1;
}

int luaMonsterTypeAddElement(lua_State* L)
{
	// monsterType:addElement(type, percent)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		CombatType_t element = tfs::lua::getNumber<CombatType_t>(L, 2);
		monsterType->info.elementMap[element] = tfs::lua::getNumber<int32_t>(L, 3);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeGetVoices(lua_State* L)
{
	// monsterType:getVoices()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	int index = 0;
	lua_createtable(L, monsterType->info.voiceVector.size(), 0);
	for (const auto& voiceBlock : monsterType->info.voiceVector) {
		lua_createtable(L, 0, 2);
		tfs::lua::setField(L, "text", voiceBlock.text);
		tfs::lua::setField(L, "yellText", voiceBlock.yellText);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaMonsterTypeAddVoice(lua_State* L)
{
	// monsterType:addVoice(sentence, interval, chance, yell)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		voiceBlock_t voice;
		voice.text = tfs::lua::getString(L, 2);
		monsterType->info.yellSpeedTicks = tfs::lua::getNumber<uint32_t>(L, 3);
		monsterType->info.yellChance = tfs::lua::getNumber<uint32_t>(L, 4);
		voice.yellText = tfs::lua::getBoolean(L, 5);
		monsterType->info.voiceVector.push_back(voice);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeGetLoot(lua_State* L)
{
	// monsterType:getLoot()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushLoot(L, monsterType->info.lootItems);
	return 1;
}

int luaMonsterTypeAddLoot(lua_State* L)
{
	// monsterType:addLoot(loot)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		Loot* loot = tfs::lua::getUserdata<Loot>(L, 2);
		if (loot) {
			monsterType->loadLoot(monsterType, loot->lootBlock);
			tfs::lua::pushBoolean(L, true);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeGetCreatureEvents(lua_State* L)
{
	// monsterType:getCreatureEvents()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	int index = 0;
	lua_createtable(L, monsterType->info.scripts.size(), 0);
	for (const std::string& creatureEvent : monsterType->info.scripts) {
		tfs::lua::pushString(L, creatureEvent);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaMonsterTypeRegisterEvent(lua_State* L)
{
	// monsterType:registerEvent(name)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		monsterType->info.scripts.push_back(tfs::lua::getString(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeEventOnCallback(lua_State* L)
{
	// monsterType:onThink(callback)
	// monsterType:onAppear(callback)
	// monsterType:onDisappear(callback)
	// monsterType:onMove(callback)
	// monsterType:onSay(callback)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (monsterType->loadCallback(&g_scripts->getScriptInterface())) {
			tfs::lua::pushBoolean(L, true);
			return 1;
		}
		tfs::lua::pushBoolean(L, false);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeEventType(lua_State* L)
{
	// monstertype:eventType(event)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		monsterType->info.eventType = tfs::lua::getNumber<MonstersEvent_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeGetSummonList(lua_State* L)
{
	// monsterType:getSummonList()
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	int index = 0;
	lua_createtable(L, monsterType->info.summons.size(), 0);
	for (const auto& summonBlock : monsterType->info.summons) {
		lua_createtable(L, 0, 6);
		tfs::lua::setField(L, "name", summonBlock.name);
		tfs::lua::setField(L, "speed", summonBlock.speed);
		tfs::lua::setField(L, "chance", summonBlock.chance);
		tfs::lua::setField(L, "max", summonBlock.max);
		tfs::lua::setField(L, "effect", summonBlock.effect);
		tfs::lua::setField(L, "masterEffect", summonBlock.masterEffect);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaMonsterTypeAddSummon(lua_State* L)
{
	// monsterType:addSummon(name, interval, chance[, max = -1[, effect = CONST_ME_TELEPORT[, masterEffect =
	// CONST_ME_NONE]]])
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		summonBlock_t summon;
		summon.name = tfs::lua::getString(L, 2);
		summon.speed = tfs::lua::getNumber<int32_t>(L, 3);
		summon.chance = tfs::lua::getNumber<int32_t>(L, 4);
		summon.max = tfs::lua::getNumber<int32_t>(L, 5, -1);
		summon.effect = tfs::lua::getNumber<MagicEffectClasses>(L, 6, CONST_ME_TELEPORT);
		summon.masterEffect = tfs::lua::getNumber<MagicEffectClasses>(L, 7, CONST_ME_NONE);
		monsterType->info.summons.push_back(summon);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeMaxSummons(lua_State* L)
{
	// get: monsterType:maxSummons() set: monsterType:maxSummons(ammount)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.maxSummons);
		} else {
			monsterType->info.maxSummons = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeArmor(lua_State* L)
{
	// get: monsterType:armor() set: monsterType:armor(armor)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.armor);
		} else {
			monsterType->info.armor = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeDefense(lua_State* L)
{
	// get: monsterType:defense() set: monsterType:defense(defense)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.defense);
		} else {
			monsterType->info.defense = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeOutfit(lua_State* L)
{
	// get: monsterType:outfit() set: monsterType:outfit(outfit)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushOutfit(L, monsterType->info.outfit);
		} else {
			monsterType->info.outfit = tfs::lua::getOutfit(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeRace(lua_State* L)
{
	// get: monsterType:race() set: monsterType:race(race)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	std::string race = tfs::lua::getString(L, 2);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.race);
		} else {
			if (race == "venom") {
				monsterType->info.race = RACE_VENOM;
			} else if (race == "blood") {
				monsterType->info.race = RACE_BLOOD;
			} else if (race == "undead") {
				monsterType->info.race = RACE_UNDEAD;
			} else if (race == "fire") {
				monsterType->info.race = RACE_FIRE;
			} else if (race == "energy") {
				monsterType->info.race = RACE_ENERGY;
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown race type " << race << ".\n";
				lua_pushnil(L);
				return 1;
			}
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeCorpseId(lua_State* L)
{
	// get: monsterType:corpseId() set: monsterType:corpseId(id)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.lookcorpse);
		} else {
			monsterType->info.lookcorpse = tfs::lua::getNumber<uint16_t>(L, 2);
			lua_pushboolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeManaCost(lua_State* L)
{
	// get: monsterType:manaCost() set: monsterType:manaCost(mana)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.manaCost);
		} else {
			monsterType->info.manaCost = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeBaseSpeed(lua_State* L)
{
	// get: monsterType:baseSpeed() set: monsterType:baseSpeed(speed)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.baseSpeed);
		} else {
			monsterType->info.baseSpeed = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeLight(lua_State* L)
{
	// get: monsterType:light() set: monsterType:light(color, level)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}
	if (lua_gettop(L) == 1) {
		tfs::lua::pushNumber(L, monsterType->info.light.level);
		tfs::lua::pushNumber(L, monsterType->info.light.color);
		return 2;
	} else {
		monsterType->info.light.color = tfs::lua::getNumber<uint8_t>(L, 2);
		monsterType->info.light.level = tfs::lua::getNumber<uint8_t>(L, 3);
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int luaMonsterTypeStaticAttackChance(lua_State* L)
{
	// get: monsterType:staticAttackChance() set: monsterType:staticAttackChance(chance)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.staticAttackChance);
		} else {
			monsterType->info.staticAttackChance = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeTargetDistance(lua_State* L)
{
	// get: monsterType:targetDistance() set: monsterType:targetDistance(distance)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.targetDistance);
		} else {
			monsterType->info.targetDistance = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeYellChance(lua_State* L)
{
	// get: monsterType:yellChance() set: monsterType:yellChance(chance)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.yellChance);
		} else {
			monsterType->info.yellChance = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeYellSpeedTicks(lua_State* L)
{
	// get: monsterType:yellSpeedTicks() set: monsterType:yellSpeedTicks(rate)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.yellSpeedTicks);
		} else {
			monsterType->info.yellSpeedTicks = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeChangeTargetChance(lua_State* L)
{
	// get: monsterType:changeTargetChance() set: monsterType:changeTargetChance(chance)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.changeTargetChance);
		} else {
			monsterType->info.changeTargetChance = tfs::lua::getNumber<int32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeChangeTargetSpeed(lua_State* L)
{
	// get: monsterType:changeTargetSpeed() set: monsterType:changeTargetSpeed(speed)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (monsterType) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, monsterType->info.changeTargetSpeed);
		} else {
			monsterType->info.changeTargetSpeed = tfs::lua::getNumber<uint32_t>(L, 2);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMonsterTypeBestiaryInfo(lua_State* L)
{
	// get: monsterType:bestiaryInfo() set: monsterType:bestiaryInfo(info)
	MonsterType* monsterType = tfs::lua::getUserdata<MonsterType>(L, 1);
	if (!monsterType) {
		lua_pushnil(L);
		return 1;
	}

	if (lua_gettop(L) == 1) {
		const auto& info = monsterType->bestiaryInfo;
		lua_createtable(L, 0, 9);

		tfs::lua::setField(L, "class", info.className);
		tfs::lua::setField(L, "raceId", info.raceId);
		tfs::lua::setField(L, "prowess", info.prowess);
		tfs::lua::setField(L, "expertise", info.expertise);
		tfs::lua::setField(L, "mastery", info.mastery);
		tfs::lua::setField(L, "charmPoints", info.charmPoints);
		tfs::lua::setField(L, "difficulty", info.difficulty);
		tfs::lua::setField(L, "occurrence", info.occurrence);
		tfs::lua::setField(L, "locations", info.locations);
		return 1;
	}

	if (lua_istable(L, 2)) {
		BestiaryInfo info{
		    .className = tfs::lua::getFieldString(L, 2, "class"),
		    .raceId = tfs::lua::getField<uint32_t>(L, 2, "raceId"),
		    .prowess = tfs::lua::getField<uint32_t>(L, 2, "prowess"),
		    .expertise = tfs::lua::getField<uint32_t>(L, 2, "expertise"),
		    .mastery = tfs::lua::getField<uint32_t>(L, 2, "mastery"),
		    .charmPoints = tfs::lua::getField<uint32_t>(L, 2, "charmPoints"),
		    .difficulty = tfs::lua::getField<uint32_t>(L, 2, "difficulty"),
		    .occurrence = tfs::lua::getField<uint32_t>(L, 2, "occurrence"),
		    .locations = tfs::lua::getFieldString(L, 2, "locations"),
		};
		lua_pop(L, 9);

		if (g_monsters.isValidBestiaryInfo(info)) {
			monsterType->bestiaryInfo = std::move(info);
			tfs::lua::pushBoolean(L, g_monsters.addBestiaryMonsterType(monsterType));
		} else {
			tfs::lua::pushBoolean(L, false);
		}
		return 1;
	}

	std::cout << "[Warning - luaMonsterTypeBestiaryInfo] bestiaryInfo must be a table.\n";
	lua_pushnil(L);
	return 1;
}

int luaCreateLoot(lua_State* L)
{
	// Loot() will create a new loot item
	tfs::lua::pushUserdata(L, new Loot);
	tfs::lua::setMetatable(L, -1, "Loot");
	return 1;
}

int luaDeleteLoot(lua_State* L)
{
	// loot:delete() loot:__gc()
	Loot** lootPtr = tfs::lua::getRawUserdata<Loot>(L, 1);
	if (lootPtr && *lootPtr) {
		delete *lootPtr;
		*lootPtr = nullptr;
	}
	return 0;
}

int luaLootSetId(lua_State* L)
{
	// loot:setId(id or name)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		if (tfs::lua::isNumber(L, 2)) {
			loot->lootBlock.id = tfs::lua::getNumber<uint16_t>(L, 2);
		} else {
			auto name = tfs::lua::getString(L, 2);
			const auto&& [it, end] = Item::items.nameToItems.equal_range(boost::algorithm::to_lower_copy(name));

			if (it == Item::items.nameToItems.cend()) {
				std::cout << "[Warning - Loot:setId] Unknown loot item \"" << name << "\".\n";
				tfs::lua::pushBoolean(L, false);
				return 1;
			}

			if (std::next(it) != end) {
				std::cout << "[Warning - Loot:setId] Non-unique loot item \"" << name << "\".\n";
				tfs::lua::pushBoolean(L, false);
				return 1;
			}

			loot->lootBlock.id = it->second;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaLootSetSubType(lua_State* L)
{
	// loot:setSubType(type)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.subType = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaLootSetChance(lua_State* L)
{
	// loot:setChance(chance)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.chance = tfs::lua::getNumber<uint32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaLootSetMaxCount(lua_State* L)
{
	// loot:setMaxCount(max)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.countmax = tfs::lua::getNumber<uint32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaLootSetActionId(lua_State* L)
{
	// loot:setActionId(actionid)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.actionId = tfs::lua::getNumber<uint32_t>(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaLootSetDescription(lua_State* L)
{
	// loot:setDescription(desc)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		loot->lootBlock.text = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaLootAddChildLoot(lua_State* L)
{
	// loot:addChildLoot(loot)
	Loot* loot = tfs::lua::getUserdata<Loot>(L, 1);
	if (loot) {
		Loot* childLoot = tfs::lua::getUserdata<Loot>(L, 2);
		if (childLoot) {
			loot->lootBlock.childLoot.push_back(childLoot->lootBlock);
			tfs::lua::pushBoolean(L, true);
		} else {
			tfs::lua::pushBoolean(L, false);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerMonsters(LuaScriptInterface& lsi)
{
	registerEnum(lsi, MONSTERS_EVENT_THINK);
	registerEnum(lsi, MONSTERS_EVENT_APPEAR);
	registerEnum(lsi, MONSTERS_EVENT_DISAPPEAR);
	registerEnum(lsi, MONSTERS_EVENT_MOVE);
	registerEnum(lsi, MONSTERS_EVENT_SAY);

	registerEnum(lsi, MAX_LOOTCHANCE);

	lsi.registerClass("MonsterSpell", "", luaCreateMonsterSpell);
	lsi.registerMetaMethod("MonsterSpell", "__gc", luaDeleteMonsterSpell);
	lsi.registerMethod("MonsterSpell", "delete", luaDeleteMonsterSpell);

	lsi.registerMethod("MonsterSpell", "setType", luaMonsterSpellSetType);
	lsi.registerMethod("MonsterSpell", "setScriptName", luaMonsterSpellSetScriptName);
	lsi.registerMethod("MonsterSpell", "setChance", luaMonsterSpellSetChance);
	lsi.registerMethod("MonsterSpell", "setInterval", luaMonsterSpellSetInterval);
	lsi.registerMethod("MonsterSpell", "setRange", luaMonsterSpellSetRange);
	lsi.registerMethod("MonsterSpell", "setCombatValue", luaMonsterSpellSetCombatValue);
	lsi.registerMethod("MonsterSpell", "setCombatType", luaMonsterSpellSetCombatType);
	lsi.registerMethod("MonsterSpell", "setAttackValue", luaMonsterSpellSetAttackValue);
	lsi.registerMethod("MonsterSpell", "setNeedTarget", luaMonsterSpellSetNeedTarget);
	lsi.registerMethod("MonsterSpell", "setNeedDirection", luaMonsterSpellSetNeedDirection);
	lsi.registerMethod("MonsterSpell", "setCombatLength", luaMonsterSpellSetCombatLength);
	lsi.registerMethod("MonsterSpell", "setCombatSpread", luaMonsterSpellSetCombatSpread);
	lsi.registerMethod("MonsterSpell", "setCombatRadius", luaMonsterSpellSetCombatRadius);
	lsi.registerMethod("MonsterSpell", "setCombatRing", luaMonsterSpellSetCombatRing);
	lsi.registerMethod("MonsterSpell", "setConditionType", luaMonsterSpellSetConditionType);
	lsi.registerMethod("MonsterSpell", "setConditionDamage", luaMonsterSpellSetConditionDamage);
	lsi.registerMethod("MonsterSpell", "setConditionSpeedChange", luaMonsterSpellSetConditionSpeedChange);
	lsi.registerMethod("MonsterSpell", "setConditionDuration", luaMonsterSpellSetConditionDuration);
	lsi.registerMethod("MonsterSpell", "setConditionDrunkenness", luaMonsterSpellSetConditionDrunkenness);
	lsi.registerMethod("MonsterSpell", "setConditionTickInterval", luaMonsterSpellSetConditionTickInterval);
	lsi.registerMethod("MonsterSpell", "setCombatShootEffect", luaMonsterSpellSetCombatShootEffect);
	lsi.registerMethod("MonsterSpell", "setCombatEffect", luaMonsterSpellSetCombatEffect);
	lsi.registerMethod("MonsterSpell", "setOutfit", luaMonsterSpellSetOutfit);

	lsi.registerClass("MonsterType", "", luaMonsterTypeCreate);
	lsi.registerMetaMethod("MonsterType", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("MonsterType", "isAttackable", luaMonsterTypeIsAttackable);
	lsi.registerMethod("MonsterType", "isChallengeable", luaMonsterTypeIsChallengeable);
	lsi.registerMethod("MonsterType", "isConvinceable", luaMonsterTypeIsConvinceable);
	lsi.registerMethod("MonsterType", "isSummonable", luaMonsterTypeIsSummonable);
	lsi.registerMethod("MonsterType", "isIgnoringSpawnBlock", luaMonsterTypeIsIgnoringSpawnBlock);
	lsi.registerMethod("MonsterType", "isIllusionable", luaMonsterTypeIsIllusionable);
	lsi.registerMethod("MonsterType", "isHostile", luaMonsterTypeIsHostile);
	lsi.registerMethod("MonsterType", "isPushable", luaMonsterTypeIsPushable);
	lsi.registerMethod("MonsterType", "isHealthHidden", luaMonsterTypeIsHealthHidden);
	lsi.registerMethod("MonsterType", "isBoss", luaMonsterTypeIsBoss);

	lsi.registerMethod("MonsterType", "canPushItems", luaMonsterTypeCanPushItems);
	lsi.registerMethod("MonsterType", "canPushCreatures", luaMonsterTypeCanPushCreatures);

	lsi.registerMethod("MonsterType", "canWalkOnEnergy", luaMonsterTypeCanWalkOnEnergy);
	lsi.registerMethod("MonsterType", "canWalkOnFire", luaMonsterTypeCanWalkOnFire);
	lsi.registerMethod("MonsterType", "canWalkOnPoison", luaMonsterTypeCanWalkOnPoison);

	lsi.registerMethod("MonsterType", "name", luaMonsterTypeName);
	lsi.registerMethod("MonsterType", "nameDescription", luaMonsterTypeNameDescription);
	lsi.registerMethod("MonsterType", "monsterName", luaMonsterTypeMonsterName);

	lsi.registerMethod("MonsterType", "health", luaMonsterTypeHealth);
	lsi.registerMethod("MonsterType", "maxHealth", luaMonsterTypeMaxHealth);
	lsi.registerMethod("MonsterType", "runHealth", luaMonsterTypeRunHealth);
	lsi.registerMethod("MonsterType", "experience", luaMonsterTypeExperience);
	lsi.registerMethod("MonsterType", "skull", luaMonsterTypeSkull);

	lsi.registerMethod("MonsterType", "combatImmunities", luaMonsterTypeCombatImmunities);
	lsi.registerMethod("MonsterType", "conditionImmunities", luaMonsterTypeConditionImmunities);

	lsi.registerMethod("MonsterType", "getAttackList", luaMonsterTypeGetAttackList);
	lsi.registerMethod("MonsterType", "addAttack", luaMonsterTypeAddAttack);

	lsi.registerMethod("MonsterType", "getDefenseList", luaMonsterTypeGetDefenseList);
	lsi.registerMethod("MonsterType", "addDefense", luaMonsterTypeAddDefense);

	lsi.registerMethod("MonsterType", "getElementList", luaMonsterTypeGetElementList);
	lsi.registerMethod("MonsterType", "addElement", luaMonsterTypeAddElement);

	lsi.registerMethod("MonsterType", "getVoices", luaMonsterTypeGetVoices);
	lsi.registerMethod("MonsterType", "addVoice", luaMonsterTypeAddVoice);

	lsi.registerMethod("MonsterType", "getLoot", luaMonsterTypeGetLoot);
	lsi.registerMethod("MonsterType", "addLoot", luaMonsterTypeAddLoot);

	lsi.registerMethod("MonsterType", "getCreatureEvents", luaMonsterTypeGetCreatureEvents);
	lsi.registerMethod("MonsterType", "registerEvent", luaMonsterTypeRegisterEvent);

	lsi.registerMethod("MonsterType", "eventType", luaMonsterTypeEventType);
	lsi.registerMethod("MonsterType", "onThink", luaMonsterTypeEventOnCallback);
	lsi.registerMethod("MonsterType", "onAppear", luaMonsterTypeEventOnCallback);
	lsi.registerMethod("MonsterType", "onDisappear", luaMonsterTypeEventOnCallback);
	lsi.registerMethod("MonsterType", "onMove", luaMonsterTypeEventOnCallback);
	lsi.registerMethod("MonsterType", "onSay", luaMonsterTypeEventOnCallback);

	lsi.registerMethod("MonsterType", "getSummonList", luaMonsterTypeGetSummonList);
	lsi.registerMethod("MonsterType", "addSummon", luaMonsterTypeAddSummon);

	lsi.registerMethod("MonsterType", "maxSummons", luaMonsterTypeMaxSummons);

	lsi.registerMethod("MonsterType", "armor", luaMonsterTypeArmor);
	lsi.registerMethod("MonsterType", "defense", luaMonsterTypeDefense);
	lsi.registerMethod("MonsterType", "outfit", luaMonsterTypeOutfit);
	lsi.registerMethod("MonsterType", "race", luaMonsterTypeRace);
	lsi.registerMethod("MonsterType", "corpseId", luaMonsterTypeCorpseId);
	lsi.registerMethod("MonsterType", "manaCost", luaMonsterTypeManaCost);
	lsi.registerMethod("MonsterType", "baseSpeed", luaMonsterTypeBaseSpeed);
	lsi.registerMethod("MonsterType", "light", luaMonsterTypeLight);

	lsi.registerMethod("MonsterType", "staticAttackChance", luaMonsterTypeStaticAttackChance);
	lsi.registerMethod("MonsterType", "targetDistance", luaMonsterTypeTargetDistance);
	lsi.registerMethod("MonsterType", "yellChance", luaMonsterTypeYellChance);
	lsi.registerMethod("MonsterType", "yellSpeedTicks", luaMonsterTypeYellSpeedTicks);
	lsi.registerMethod("MonsterType", "changeTargetChance", luaMonsterTypeChangeTargetChance);
	lsi.registerMethod("MonsterType", "changeTargetSpeed", luaMonsterTypeChangeTargetSpeed);

	lsi.registerMethod("MonsterType", "bestiaryInfo", luaMonsterTypeBestiaryInfo);

	lsi.registerClass("Loot", "", luaCreateLoot);
	lsi.registerMetaMethod("Loot", "__gc", luaDeleteLoot);
	lsi.registerMethod("Loot", "delete", luaDeleteLoot);

	lsi.registerMethod("Loot", "setId", luaLootSetId);
	lsi.registerMethod("Loot", "setMaxCount", luaLootSetMaxCount);
	lsi.registerMethod("Loot", "setSubType", luaLootSetSubType);
	lsi.registerMethod("Loot", "setChance", luaLootSetChance);
	lsi.registerMethod("Loot", "setActionId", luaLootSetActionId);
	lsi.registerMethod("Loot", "setDescription", luaLootSetDescription);
	lsi.registerMethod("Loot", "addChildLoot", luaLootAddChildLoot);
}
