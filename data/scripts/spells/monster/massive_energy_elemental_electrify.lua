local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_BLOCKHIT)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_ENERGY, DAMAGELIST_VARYING_PERIOD, 25, {10, 12}, 10)
	end
	return true
end

spell:name("massive energy elemental electrify")
spell:words("###4")
spell:needLearn(true)
spell:isAggressive(true)
spell:blockWalls(true)
spell:register()
