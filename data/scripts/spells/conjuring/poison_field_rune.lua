local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2285, 3)
end

spell:name("Poison Field Rune")
spell:words("adevo grav pox")
spell:group("support")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(14)
spell:mana(200)
spell:soul(1)
spell:isAggressive(false)
spell:register()
