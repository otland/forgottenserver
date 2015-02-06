local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, 1)
condition:setParameter(CONDITION_PARAM_TICKS, 13000)
condition:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, 220)
combat:setCondition(condition)

local conditionExhaustCombat = Condition(CONDITION_EXHAUST_COMBAT)
conditionExhaustCombat:setParameter(CONDITION_PARAM_TICKS, 10000)
combat:setCondition(conditionExhaustCombat)

local conditionExhaustHeal = Condition(CONDITION_EXHAUST_HEAL)
conditionExhaustHeal:setParameter(CONDITION_PARAM_TICKS, 10000)
combat:setCondition(conditionExhaustHeal)

local conditionPacified = Condition(CONDITION_PACIFIED)
conditionPacified:setParameter(CONDITION_PARAM_TICKS, 10000)
combat:setCondition(conditionPacified)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
