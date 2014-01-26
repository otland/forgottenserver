local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
setCombatParam(combat, COMBAT_PARAM_AGGRESSIVE, 0)

local condition = createConditionObject(CONDITION_REGENERATION)
setConditionParam(condition, CONDITION_PARAM_SUBID, 1)
setConditionParam(condition, CONDITION_PARAM_BUFF_SPELL, 1)
setConditionParam(condition, CONDITION_PARAM_TICKS, 1 * 60 * 1000)
setConditionParam(condition, CONDITION_PARAM_HEALTHGAIN, 40)
setConditionParam(condition, CONDITION_PARAM_HEALTHTICKS, 3000)
setCombatCondition(combat, condition)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
