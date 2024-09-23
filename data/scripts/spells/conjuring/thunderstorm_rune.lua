local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2315, 4)
end

spell:group("support")
spell:name("Thunderstorm Rune")
spell:words("adori mas vis")
spell:level(28)
spell:mana(430)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "master sorcerer;true")
spell:register()
