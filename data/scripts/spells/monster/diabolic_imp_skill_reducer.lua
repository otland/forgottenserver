local combat = Combat()
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
combat:setArea(createCombatArea(AREA_BEAM1))

local parameters = {{key = CONDITION_PARAM_TICKS, value = 6 * 1000}, {key = CONDITION_PARAM_SKILL_MELEEPERCENT, value = nil}}

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	parameters[2].value = math.random(70, 80)

	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end

spell:name("diabolic imp skill reducer")
spell:words("###33")
spell:blockWalls(true)
spell:needTarget(true)
spell:needLearn(true)
spell:register()
