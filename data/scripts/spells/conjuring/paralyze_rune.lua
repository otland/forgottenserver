local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2278, 1)
end

spell:group("support")
spell:name("Paralyze Rune")
spell:words("adana ani")
spell:level(54)
spell:mana(1400)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
