local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_ENERGY, DAMAGELIST_VARYING_PERIOD, 25, {10, 12}, 20)
	end
	return true
end

spell:name("cliff strider electrify")
spell:words("###21")
spell:blockWalls(true)
spell:needTarget(true)
spell:needLearn(true)
spell:register()
