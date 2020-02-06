local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 19392, 10)
end

spell:name("Practice Magic Missile Rune")
spell:words("adori dis min vis")
spell:group("support")
spell:vocation("none")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(1)
spell:mana(5)
spell:soul(0)
spell:isAggressive(false)
spell:register()
