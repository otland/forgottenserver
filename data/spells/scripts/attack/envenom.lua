local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_CARNIPHILA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EARTH)

function onTargetCreature(creature, target)
	local min = (creature:getLevel() * 0.01) + (creature:getMagicLevel() * 0.55) + 6
	local max = (creature:getLevel() * 0.01) + (creature:getMagicLevel() * 0.75) + 7
	local damage = math.random(math.floor(min), math.floor(max))
	creature:addDamageCondition(target, CONDITION_POISON, 1, target:isPlayer() and math.floor(damage / 2 + 0.5) or damage)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
