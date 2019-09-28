local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2279, 4)
end

spell:name("Energy Wall Rune")
spell:words("adevo mas grav vis")
spell:group("support")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(41)
spell:mana(1000)
spell:soul(5)
spell:isAggressive(false)
spell:register()
