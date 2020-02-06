local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2289, 4)
end

spell:name("Poison Wall Rune")
spell:words("adevo mas grav pox")
spell:group("support")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(29)
spell:mana(640)
spell:soul(3)
spell:isAggressive(false)
spell:register()
