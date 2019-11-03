local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SOUND_PURPLE)
combat:setArea(createCombatArea(AREA_BEAM1))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 4 * 1000},
	{key = CONDITION_PARAM_SKILL_MELEEPERCENT, value = 25}
}

function onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end
