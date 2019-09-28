local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 23723, 10)
end

spell:name("Lightest Missile Rune")
spell:words("adori infir vis")
spell:group("support")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(1)
spell:mana(6)
spell:soul(0)
spell:isAggressive(false)
spell:register()
