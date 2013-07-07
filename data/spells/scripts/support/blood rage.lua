local conditionAttrib = createConditionObject(CONDITION_ATTRIBUTES)

setConditionParam(conditionAttrib, CONDITION_PARAM_TICKS, 10000)
setConditionParam(conditionAttrib, CONDITION_PARAM_SKILL_SHIELDPERCENT, 0)
setConditionParam(conditionAttrib, CONDITION_PARAM_SKILL_MELEEPERCENT, 135)

local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
setCombatParam(combat, COMBAT_PARAM_AGGRESSIVE, 0)
setCombatCondition(combat, conditionAttrib)

function onCastSpell(cid, var)
	if(doCombat(cid, combat, var) == LUA_NO_ERROR) then
		return LUA_NO_ERROR
	end
	return LUA_ERROR
end
