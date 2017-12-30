local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
combat:setArea(createCombatArea(AREA_CROSS1X1))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 6 * 1000},
	{key = CONDITION_PARAM_SKILL_MELEEPERCENT, value = 45}
}

function onTargetCreature(creature, target)
	target:addAttributeCondition(parameters)
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
