local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_DROWNDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MORTAREA)
combat:setArea(createCombatArea(AREA_CIRCLE2X2))

function onTargetCreature(creature, target)
	creature:addDamageCondition(target, CONDITION_DROWN, DAMAGELIST_CONSTANT_PERIOD, 20, 5, 10)
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
