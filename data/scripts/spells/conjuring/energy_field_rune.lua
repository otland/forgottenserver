local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2277, 3)
end

spell:group("support")
spell:name("Energy Field Rune")
spell:words("adevo grav vis")
spell:level(18)
spell:mana(320)
spell:soul(2)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "druid;true", "master sorcerer;true", "elder druid;true")
spell:register()
