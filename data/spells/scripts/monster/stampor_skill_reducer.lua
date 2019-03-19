local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SMALLPLANTS)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLEARTH)
combat:setArea(createCombatArea(AREA_BEAM1))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 4 * 1000},
	{key = CONDITION_PARAM_SKILL_SHIELDPERCENT, value = nil},
	{key = CONDITION_PARAM_SKILL_MELEEPERCENT, value = nil}
}

function onCastSpell(creature, variant)
	parameters[2].value = math.random(60, 85)
	parameters[3].value = parameters[2].value

	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end
