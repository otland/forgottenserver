local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 23722, 4)
end

spell:name("Light Stone Shower Rune")
spell:words("adori infir mas tera")
spell:group("support")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(1)
spell:mana(6)
spell:soul(3)
spell:isAggressive(false)
spell:register()
