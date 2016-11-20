local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)

function onTargetCreature(creature, target)
	local min = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.15) + 1
	local max = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.25) + 1
	creature:addDamageCondition(target, CONDITION_ENERGY, 2, target:isPlayer() and 13 or 25, {10, 12}, math.random(min, max))
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
