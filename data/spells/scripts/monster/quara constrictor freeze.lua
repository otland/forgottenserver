local condition = Condition(CONDITION_FREEZING)
condition:setParameter(CONDITION_PARAM_DELAYED, true)
condition:addDamage(10, 8000, -8)

local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_GREEN_RINGS)
combat:setArea(createCombatArea(AREA_SQUARE1X1))
combat:setCondition(condition)

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
