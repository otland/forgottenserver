local combat = {}

for i = 45, 65 do
combat[i] = createCombatObject()
setCombatParam(combat[i], COMBAT_PARAM_EFFECT, CONST_ME_STUN)
setCombatParam(combat[i], COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EXPLOSION)


local condition = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(condition, CONDITION_PARAM_TICKS, 8000)
setConditionParam(condition, CONDITION_PARAM_SKILL_MELEEPERCENT, i)
setConditionParam(condition, CONDITION_PARAM_SKILL_DISTANCEPERCENT, i)

local area = createCombatArea(AREA_BEAM1)
setCombatArea(combat[i], area)
setCombatCondition(combat[i], condition)
end

function onCastSpell(cid, var)
	return doCombat(cid, combat[math.random(45,65)], var)
end
