local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2291, 1)
end

spell:group("support")
spell:name("Chameleon Rune")
spell:words("adevo ina")
spell:level(27)
spell:mana(600)
spell:soul(2)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
