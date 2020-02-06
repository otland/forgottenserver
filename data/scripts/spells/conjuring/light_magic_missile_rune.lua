local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2287, 10)
end

spell:name("Light Magic Missile Rune")
spell:words("adori min vis")
spell:group("support")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(15)
spell:mana(120)
spell:soul(1)
spell:isAggressive(false)
spell:register()
