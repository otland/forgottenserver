local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SOUND_PURPLE)
combat:setArea(createCombatArea(AREA_SQUAREWAVE6))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 8 * 1000},
	{key = CONDITION_PARAM_SKILL_SHIELDPERCENT, value = 85}
}

function onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end
