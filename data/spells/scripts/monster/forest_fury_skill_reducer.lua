local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_LARGEROCK)
combat:setArea(createCombatArea(AREA_CIRCLE2X2))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 4 * 1000},
	{key = CONDITION_PARAM_SKILL_SHIELDPERCENT, value = 50},
	{key = CONDITION_PARAM_SKILL_DISTANCEPERCENT, value = 50}
}

function onTargetCreature(creature, target)
	target:addAttributeCondition(parameters)
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
