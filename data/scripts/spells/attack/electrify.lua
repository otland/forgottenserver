local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	local min = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.15) + 1
	local max = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.25) + 1
	local rounds = math.random(math.floor(min), math.floor(max))
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_ENERGY, DAMAGELIST_VARYING_PERIOD, target:isPlayer() and 13 or 25, {10, 12}, rounds)
	end
	return true
end

spell:name("Electrify")
spell:words("utori vis")
spell:group("attack")
spell:vocation("sorcerer", "master sorcerer")
spell:id(140)
spell:cooldown(30 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(34)
spell:mana(30)
spell:range(3)
spell:needCasterTargetOrDirection(true)
spell:blockWalls(true)
spell:isPremium(true)
spell:register()
