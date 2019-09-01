local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2286, 2)
end

spell:name("Poison Bomb Rune")
spell:words("adevo mas pox")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(25)
spell:mana(520)
spell:soul(2)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
