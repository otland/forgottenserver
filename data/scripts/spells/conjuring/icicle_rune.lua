local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2271, 5)
end

spell:name("Icicle Rune")
spell:words("adori frigo")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(28)
spell:mana(460)
spell:soul(3)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
