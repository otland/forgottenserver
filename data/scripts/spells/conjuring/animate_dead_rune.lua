local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2316, 1)
end

spell:group("support")
spell:name("Animate Dead Rune")
spell:words("adana mort")
spell:level(27)
spell:mana(600)
spell:soul(5)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "druid;true", "master sorcerer;true", "elder druid;true")
spell:register()
