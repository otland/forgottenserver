local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SOUND_YELLOW)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 5 * 1000},
	{key = CONDITION_PARAM_SKILL_SHIELDPERCENT, value = 60},
	{key = CONDITION_PARAM_SKILL_MELEEPERCENT, value = 70}
}

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end

spell:name("fury skill reducer")
spell:words("###37")
spell:needLearn(true)
spell:isAggressive(true)
spell:blockWalls(true)
spell:register()
