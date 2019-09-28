local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2295, 5)
end

spell:name("Holy Missile Rune")
spell:words("adori san")
spell:group("support")
spell:vocation("paladin", "royal paladin")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(27)
spell:mana(300)
spell:soul(3)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
