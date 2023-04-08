local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ROOTING)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_LEAFSTAR)

local condition = Condition(CONDITION_ROOT)
condition:setParameter(CONDITION_PARAM_TICKS, 3000)
combat:addCondition(condition)

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
