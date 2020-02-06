local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2274, 4)
end

spell:name("Avalanche Rune")
spell:words("adori mas frigo")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(30)
spell:mana(530)
spell:soul(3)
spell:isAggressive(false)
spell:register()
