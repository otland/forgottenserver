local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_CARNIPHILA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EARTH)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	local min = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.55) + 6
	local max = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.75) + 7
	local damage = math.random(math.floor(min) * 1000, math.floor(max) * 1000) / 1000
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_POISON, DAMAGELIST_LOGARITHMIC_DAMAGE, target:isPlayer() and damage / 2 or damage)
	end
	return true
end

spell:name("Envenom")
spell:words("utori pox")
spell:group("attack")
spell:vocation("druid", "elder druid")
spell:id(142)
spell:cooldown(40 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(50)
spell:mana(30)
spell:range(3)
spell:needCasterTargetOrDirection(true)
spell:blockWalls(true)
spell:isPremium(true)
spell:register()
