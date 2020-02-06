local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_DROWNDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_BUBBLES)
combat:setArea(createCombatArea(AREA_CIRCLE6X6))

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_DROWN, DAMAGELIST_CONSTANT_PERIOD, 20, 5, 50)
	end
	return true
end

spell:name("choking fear drown")
spell:words("###15")
spell:needLearn(true)
spell:isAggressive(true)
spell:blockWalls(true)
spell:register()
