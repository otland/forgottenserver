local combat = Combat()
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
combat:setArea(createCombatArea(AREA_BEAM1))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 6 * 1000},
	{key = CONDITION_PARAM_SKILL_MELEEPERCENT, value = nil}
}

function onTargetCreature(creature, target)
	target:addAttributeCondition(parameters)
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	parameters[2].value = math.random(70, 80)
	return combat:execute(creature, variant)
end
