local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2271, 5)
end

spell:group("support")
spell:name("Icicle Rune")
spell:words("adori frigo")
spell:level(28)
spell:mana(460)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
