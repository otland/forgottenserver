local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2304, 4)
end

spell:name("Great Fireball Rune")
spell:words("adori mas flam")
spell:group("support")
spell:vocation("sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(30)
spell:mana(530)
spell:soul(3)
spell:isAggressive(false)
spell:register()
