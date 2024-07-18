local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2287, 10)
end

spell:group("support")
spell:name("Light Magic Missile Rune")
spell:words("adori min vis")
spell:level(15)
spell:mana(120)
spell:soul(1)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "druid;true", "master sorcerer;true", "elder druid;true")
spell:register()
