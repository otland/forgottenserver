local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2292, 10)
end

spell:name("Stalagmite Rune")
spell:words("adori tera")
spell:group("support")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(24)
spell:mana(350)
spell:soul(2)
spell:isAggressive(false)
spell:register()
