local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
setCombatParam(combat, COMBAT_PARAM_AGGRESSIVE, 0)

local exhaust = createConditionObject(CONDITION_EXHAUST_COMBAT)
setConditionParam(exhaust, CONDITION_PARAM_TICKS, 10000)
setCombatCondition(combat, exhaust)

local condition = createConditionObject(CONDITION_HASTE)
setConditionParam(condition, CONDITION_PARAM_TICKS, 10000)
setConditionFormula(condition, 0.8, -72, 0.8, -72)
setCombatCondition(combat, condition)

local exhaustAttackGroup = createConditionObject(CONDITION_SPELLGROUPCOOLDOWN)
setConditionParam(exhaustAttackGroup, CONDITION_PARAM_SUBID, 1)
setConditionParam(exhaustAttackGroup, CONDITION_PARAM_TICKS, 10000)
setCombatCondition(combat, exhaustAttackGroup)

local disable = createConditionObject(CONDITION_PACIFIED)
setConditionParam(disable, CONDITION_PARAM_TICKS, 10000)
setCombatCondition(combat, disable)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
