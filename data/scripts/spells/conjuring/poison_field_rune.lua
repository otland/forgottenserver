local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2285, 3)
end

spell:group("support")
spell:name("Poison Field Rune")
spell:words("adevo grav pox")
spell:level(14)
spell:mana(200)
spell:soul(1)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "druid;true", "master sorcerer;true", "elder druid;true")
spell:register()
