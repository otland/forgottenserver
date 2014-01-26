local conditionAttrib = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(conditionAttrib, CONDITION_PARAM_TICKS, 10000)
setConditionParam(conditionAttrib, CONDITION_PARAM_SKILL_MELEEPERCENT, 135)
setConditionParam(conditionAttrib, CONDITION_PARAM_SKILL_SHIELDPERCENT, 0)
setConditionParam(conditionAttrib, CONDITION_PARAM_BUFF_SPELL, 1)

local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
setCombatParam(combat, COMBAT_PARAM_AGGRESSIVE, 0)
setCombatCondition(combat, conditionAttrib)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
