local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2265, 1)
end

spell:name("Intense Healing Rune")
spell:words("adura gran")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(15)
spell:mana(120)
spell:soul(2)
spell:isAggressive(false)
spell:register()
