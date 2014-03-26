local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_SOUND_YELLOW)

local condition = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(condition, CONDITION_PARAM_TICKS, 5000)
setConditionParam(condition, CONDITION_PARAM_SKILL_SHIELDPERCENT, 60)
setConditionParam(condition, CONDITION_PARAM_SKILL_MELEEPERCENT, 70)

local area = createCombatArea(AREA_CIRCLE3X3)
setCombatArea(combat, area)
setCombatCondition(combat, condition)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
