local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITBYFIRE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	local min = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.3) + 2
	local max = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.6) + 4
	local rounds = math.random(math.floor(min), math.floor(max))
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_FIRE, DAMAGELIST_VARYING_PERIOD, target:isPlayer() and 5 or 10, {8, 10}, rounds)
	end
	return true
end

spell:name("Ignite")
spell:words("utori flam")
spell:group("attack")
spell:vocation("sorcerer", "master sorcerer")
spell:id(138)
spell:cooldown(30 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(26)
spell:mana(30)
spell:range(3)
spell:needCasterTargetOrDirection(true)
spell:blockWalls(true)
spell:isPremium(true)
spell:register()
