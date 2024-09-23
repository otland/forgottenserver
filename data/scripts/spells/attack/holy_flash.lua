local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLHOLY)

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	local min = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.3) + 2
	local max = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.5) + 3
	local rounds = math.random(math.floor(min), math.floor(max))
	local period = math.random(10, 12)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_DAZZLED, DAMAGELIST_CONSTANT_PERIOD, target:isPlayer() and 10 or 20, period, rounds)
	end
	return true
end

spell:group("attack")
spell:id(143)
spell:name("Holy Flash")
spell:words("utori san")
spell:level(70)
spell:mana(30)
spell:range(3)
spell:needCasterTargetOrDirection(true)
spell:blockWalls(true)
spell:cooldown(40000)
spell:groupCooldown(2000)
spell:vocation("paladin;true", "royal paladin;true")
spell:register()
