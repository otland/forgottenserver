local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2269, 2)
end

spell:group("support")
spell:name("Wild Growth Rune")
spell:words("adevo grav vita")
spell:level(27)
spell:mana(600)
spell:soul(5)
spell:isAggressive(false)
spell:vocation("druid;true", "elder druid;true")
spell:register()
