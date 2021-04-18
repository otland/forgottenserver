local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_STUN)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EXPLOSION)
combat:setArea(createCombatArea(AREA_BEAM1))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 8 * 1000},
	{key = CONDITION_PARAM_SKILL_MELEEPERCENT, value = nil},
	{key = CONDITION_PARAM_SKILL_DISTANCEPERCENT, value = nil}
}

function onCastSpell(creature, variant)
	parameters[2].value = math.random(45, 65)
	parameters[3].value = parameters[2].value

	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end
