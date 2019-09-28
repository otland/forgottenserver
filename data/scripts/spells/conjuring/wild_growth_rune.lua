local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2269, 2)
end

spell:name("Wild Growth Rune")
spell:words("adevo grav vita")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(27)
spell:mana(600)
spell:soul(5)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
