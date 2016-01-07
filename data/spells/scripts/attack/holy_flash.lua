local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLHOLY)

function onTargetCreature(creature, target)
	local damage = target:isPlayer() and 10 or 20
	local time = math.random(10, 12) * 1000
	local min = (creature:getLevel() * 0.01) + (creature:getMagicLevel() * 0.3) + 2
	local max = (creature:getLevel() * 0.01) + (creature:getMagicLevel() * 0.5) + 3
	local rounds = math.random(math.floor(min), math.floor(max))

	local condition = Condition(CONDITION_DAZZLED)
	condition:setParameter(CONDITION_PARAM_OWNER, creature:getId())
	condition:setParameter(CONDITION_PARAM_DELAYED, true)
	condition:addDamage(rounds, time, -damage)
	target:addCondition(condition)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
