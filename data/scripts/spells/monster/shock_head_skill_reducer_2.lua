local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_STUN)
combat:setArea(createCombatArea(AREA_CIRCLE6X6))

local parameters = {{key = CONDITION_PARAM_TICKS, value = 4 * 1000}, {key = CONDITION_PARAM_STAT_MAGICPOINTSPERCENT, value = 70}}

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end

spell:name("shock head skill reducer 2")
spell:words("###44")
spell:blockWalls(true)
spell:needLearn(true)
spell:register()
