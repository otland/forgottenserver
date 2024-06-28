local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2293, 3)
end

spell:group("support")
spell:name("Magic Wall Rune")
spell:words("adevo grav tera")
spell:level(32)
spell:mana(750)
spell:soul(5)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "master sorcerer;true")
spell:register()
