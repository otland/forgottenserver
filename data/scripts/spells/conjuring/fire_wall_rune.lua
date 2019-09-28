local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2303, 4)
end

spell:name("Fire Wall Rune")
spell:words("adevo mas grav flam")
spell:group("support")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(33)
spell:mana(780)
spell:soul(4)
spell:isAggressive(false)
spell:register()
