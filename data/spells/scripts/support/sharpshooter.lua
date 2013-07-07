local conditionAttrib = createConditionObject(CONDITION_ATTRIBUTES)
local conditionSlow = createConditionObject(CONDITION_HASTE)
local conditionExhaustHeal = createConditionObject(CONDITION_EXHAUST_HEAL)

setConditionParam(conditionAttrib, CONDITION_PARAM_TICKS, 10000)
setConditionParam(conditionAttrib, CONDITION_PARAM_SKILL_DISTANCEPERCENT, 150)

setConditionParam(conditionSlow, CONDITION_PARAM_TICKS, 10000)
setConditionFormula(conditionSlow, -0.7, 0, -0.7, 0)

setConditionParam(conditionExhaustHeal, CONDITION_PARAM_TICKS, 10000)

local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
setCombatParam(combat, COMBAT_PARAM_AGGRESSIVE, 0)
setCombatCondition(combat, conditionAttrib)
setCombatCondition(combat, conditionSlow)
setCombatCondition(combat, conditionExhaustHeal)

function onCastSpell(cid, var)
	if(doCombat(cid, combat, var) == LUA_NO_ERROR) then
		return LUA_NO_ERROR
	end
	return LUA_ERROR
end
