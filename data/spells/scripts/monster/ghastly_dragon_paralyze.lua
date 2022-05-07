local condition = Condition(CONDITION_PARALYZE)
condition:setParameter(CONDITION_PARAM_TICKS, 20000)
condition:setFormula(-0.8, 0, -0.9, 0)

local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_BATS)
combat:addCondition(condition)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
