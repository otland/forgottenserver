local combat = {}

for i = 2, 4 do
combat[i] = createCombatObject()
setCombatParam(combat[i], COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
setCombatParam(combat[i], COMBAT_PARAM_EFFECT, CONST_ME_SOUND_PURPLE)

local condition = createConditionObject(CONDITION_CURSED)
setConditionParam(condition, CONDITION_PARAM_DELAYED, 1)

local p = 1.2

addDamageCondition(condition, 1, 4000, -i)
addDamageCondition(condition, 1, 4000, -(i*p))
addDamageCondition(condition, 1, 4000, -(i*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p*p*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p*p*p*p*p*p))


local arr1 = {
{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0}
}

local area = createCombatArea(arr1)
setCombatArea(combat[i], area)
setCombatCondition(combat[i], condition)
end

function onCastSpell(cid, var)
	return doCombat(cid, combat[math.random(2,4)], var)
end