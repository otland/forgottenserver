local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2301, 3)
end

spell:name("Fire Field Rune")
spell:words("adevo grav flam")
spell:group("support")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(15)
spell:mana(240)
spell:soul(1)
spell:isAggressive(false)
spell:register()
