local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLHOLY)

function onCastSpell(creature, variant)
	local min = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.3) + 2
	local max = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.5) + 3
	local rounds = math.random(math.floor(min), math.floor(max))
	local period = math.random(10, 12)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_DAZZLED, DAMAGELIST_CONSTANT_PERIOD, target:isPlayer() and 10 or 20, period, rounds)
	end
	return true
end
