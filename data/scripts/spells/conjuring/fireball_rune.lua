local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2302, 5)
end

spell:name("Fireball Rune")
spell:words("adori flam")
spell:group("support")
spell:vocation("sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(27)
spell:mana(460)
spell:soul(3)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
