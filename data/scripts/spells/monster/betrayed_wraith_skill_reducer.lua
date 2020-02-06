local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_YELLOW_RINGS)
combat:setArea(createCombatArea(AREA_SQUAREWAVE5))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 5 * 1000},
	{key = CONDITION_PARAM_SKILL_DISTANCEPERCENT, value = 80}
}

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end

spell:name("betrayed wraith skill reducer")
spell:words("###48")
spell:needDirection(true)
spell:needLearn(true)
spell:isAggressive(true)
spell:blockWalls(true)
spell:register()
