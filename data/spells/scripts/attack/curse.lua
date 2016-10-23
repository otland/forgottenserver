local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MORTAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_DEATH)

function onTargetCreature(creature, target)
	local min = (creature:getLevel() * 0.03) + (creature:getMagicLevel() * 0.45) + 4
	local max = (creature:getLevel() * 0.03) + (creature:getMagicLevel() * 0.9) + 5
	local damage = math.random(math.floor(min), math.floor(max))
	creature:addDamageCondition(target, CONDITION_CURSED, 0, damage)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
