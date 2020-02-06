local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_EXPLOSIONHIT)
combat:setArea(createCombatArea(AREA_CIRCLE2X2))

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_FIRE, DAMAGELIST_VARYING_PERIOD, 10, {8, 10}, 40)
	end
	return true
end

spell:name("lava golem soulfire")
spell:words("###26")
spell:needLearn(true)
spell:isAggressive(true)
spell:blockWalls(true)
spell:register()
