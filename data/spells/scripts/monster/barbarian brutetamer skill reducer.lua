local combat = {}

for i = 20, 25 do

local z = math.random(1,6)

combat[i] = createCombatObject()
setCombatParam(combat[i], COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SNOWBALL)
setCombatParam(combat[i], COMBAT_PARAM_EFFECT, CONST_ME_POFF)
condition = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(condition, CONDITION_PARAM_TICKS, 5000)
setConditionParam(condition, i, -z)
setCombatCondition(combat[i], condition)
end

function onCastSpell(cid, var)
return doCombat(cid, combat[math.random(20, 25)], var)
end
