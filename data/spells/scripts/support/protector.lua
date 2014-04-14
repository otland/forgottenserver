local conditionAttrib = createConditionObject(CONDITION_ATTRIBUTES)
local conditionExhaustCombat = createConditionObject(CONDITION_EXHAUST_COMBAT)
local conditionExhaustHeal = createConditionObject(CONDITION_EXHAUST_HEAL)
local conditionPacified = createConditionObject(CONDITION_PACIFIED)

setConditionParam(conditionAttrib, CONDITION_PARAM_BUFF_SPELL, 1)
setConditionParam(conditionAttrib, CONDITION_PARAM_TICKS, 13000)
setConditionParam(conditionAttrib, CONDITION_PARAM_SKILL_SHIELDPERCENT, 220)
setConditionParam(conditionExhaustCombat, CONDITION_PARAM_TICKS, 10000)
setConditionParam(conditionExhaustHeal, CONDITION_PARAM_TICKS, 10000)
setConditionParam(conditionPacified, CONDITION_PARAM_TICKS, 10000)

local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
setCombatParam(combat, COMBAT_PARAM_AGGRESSIVE, 0)
setCombatCondition(combat, conditionAttrib)
setCombatCondition(combat, conditionExhaustCombat)
setCombatCondition(combat, conditionExhaustHeal)
setCombatCondition(combat, conditionPacified)

function onCastSpell(cid, var)
	if(doCombat(cid, combat, var) == LUA_NO_ERROR) then
		return LUA_NO_ERROR
	end
	return LUA_ERROR
end
