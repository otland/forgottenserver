local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 19392, 10)
end

spell:group("support")
spell:name("Practise Magic Missile Rune")
spell:words("adori dis min vis")
spell:level(1)
spell:mana(5)
spell:soul(0)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("none;true")
spell:register()
