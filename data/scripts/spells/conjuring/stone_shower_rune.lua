local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2288, 4)
end

spell:name("Stone Shower Rune")
spell:words("adori mas tera")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(28)
spell:mana(430)
spell:soul(3)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
