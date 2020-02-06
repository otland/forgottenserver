local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2278, 1)
end

spell:name("Paralyze Rune")
spell:words("adana ani")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(54)
spell:mana(1400)
spell:soul(3)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
