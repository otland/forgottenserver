local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2304, 4)
end

spell:group("support")
spell:name("Great Fireball Rune")
spell:words("adori mas flam")
spell:level(30)
spell:mana(530)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "master sorcerer;true")
spell:register()
