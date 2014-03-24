local combat = {}

for i = 40, 170 do
combat[i] = createCombatObject()
setCombatParam(combat[i], COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
setCombatParam(combat[i], COMBAT_PARAM_EFFECT, CONST_ME_SMALLCLOUDS)
setCombatParam(combat[i], COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_DEATH)

local condition = createConditionObject(CONDITION_CURSED)
setConditionParam(condition, CONDITION_PARAM_DELAYED, 1)

local z = math.random(5,7)
local p = 1.2

if z == 5 then
addDamageCondition(condition, 1, 4000, -i)
addDamageCondition(condition, 1, 4000, -(i*p))
addDamageCondition(condition, 1, 4000, -(i*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p))
end

if z == 6 then
addDamageCondition(condition, 1, 4000, -i)
addDamageCondition(condition, 1, 4000, -(i*p))
addDamageCondition(condition, 1, 4000, -(i*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p*p))
end

if z == 7 then
addDamageCondition(condition, 1, 4000, -i)
addDamageCondition(condition, 1, 4000, -(i*p))
addDamageCondition(condition, 1, 4000, -(i*p*p))
addDamageCondition(condition, 1, 4000, -((i*p*p*p)))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p*p))
addDamageCondition(condition, 1, 4000, -(i*p*p*p*p*p*p))
end

setCombatCondition(combat[i], condition)
end

function onCastSpell(cid, var)
	return doCombat(cid, combat[math.random(40,170)], var)
end
