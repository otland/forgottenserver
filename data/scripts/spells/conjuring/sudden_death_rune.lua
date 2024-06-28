local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2268, 3)
end

spell:group("support")
spell:name("Sudden Death Rune")
spell:words("adori gran mort")
spell:level(45)
spell:mana(985)
spell:soul(5)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "master sorcerer;true")
spell:register()
