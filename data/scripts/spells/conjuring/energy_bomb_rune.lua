local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2262, 2)
end

spell:group("support")
spell:name("Energy Bomb Rune")
spell:words("adevo mas vis")
spell:level(37)
spell:mana(880)
spell:soul(5)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "master sorcerer;true")
spell:register()
