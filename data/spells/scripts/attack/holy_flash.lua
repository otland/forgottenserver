local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLHOLY)

function onTargetCreature(creature, target)
	local condition = Condition(CONDITION_DAZZLED)
	condition:setParameter(CONDITION_PARAM_OWNER, creature:getId())
	condition:setParameter(CONDITION_PARAM_DELAYED, true)

	local min = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.3) + 2
	local max = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.5) + 3
	condition:addDamage(math.random(min, max), math.random(10, 12) * 1000, -(target:isPlayer() and 10 or 20))
	target:addCondition(condition)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
