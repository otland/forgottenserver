local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2290, 1)
end

spell:name("Convince Creature Rune")
spell:words("adeta sio")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(16)
spell:mana(200)
spell:soul(3)
spell:isAggressive(false)
spell:register()
