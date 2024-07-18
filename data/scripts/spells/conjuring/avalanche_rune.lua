local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2274, 4)
end

spell:group("support")
spell:name("Avalanche Rune")
spell:words("adori mas frigo")
spell:level(30)
spell:mana(530)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
