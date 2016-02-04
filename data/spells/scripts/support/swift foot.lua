local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local condition = Condition(CONDITION_HASTE)
condition:setParameter(CONDITION_PARAM_TICKS, 10000)
condition:setFormula(0.8, -72, 0.8, -72)
combat:setCondition(condition)

local pacified = Condition(CONDITION_PACIFIED)
pacified:setParameter(CONDITION_PARAM_TICKS, 10000)
combat:setCondition(pacified)

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
