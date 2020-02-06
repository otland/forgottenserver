local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2266, 1)
end

spell:name("Cure Poison Rune")
spell:words("adana pox")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(15)
spell:mana(200)
spell:soul(1)
spell:isAggressive(false)
spell:register()
