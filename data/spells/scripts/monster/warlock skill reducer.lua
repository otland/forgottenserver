local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_ICEAREA)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ICE)

local condition = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(condition, CONDITION_PARAM_TICKS, 5000)
setConditionParam(condition, CONDITION_PARAM_SKILL_SHIELDPERCENT, 50)
setConditionParam(condition, CONDITION_PARAM_SKILL_MELEEPERCENT, 50)

local area = createCombatArea(AREA_BEAM1)
setCombatArea(combat, area)
setCombatCondition(combat, condition)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
