local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLHOLY)

function onTargetCreature(creature, target)
	local min = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.3) + 2
	local max = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.5) + 3
	local rounds = math.random(math.floor(min), math.floor(max))
	creature:addDamageCondition(target, CONDITION_DAZZLED, DAMAGELIST_CONSTANT_PERIOD, target:isPlayer() and 10 or 20, math.random(10, 12), rounds)
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
