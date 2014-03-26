local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)

local condition = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(condition, CONDITION_PARAM_TICKS, 4000)
setConditionParam(condition, CONDITION_PARAM_SKILL_SHIELDPERCENT, 85)
setConditionParam(condition, CONDITION_PARAM_SKILL_MELEEPERCENT, 85)

local area = createCombatArea(AREA_SQUARE1X1)
setCombatArea(combat, area)
setCombatCondition(combat, condition)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
