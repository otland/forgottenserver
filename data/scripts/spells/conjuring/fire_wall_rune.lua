local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2303, 4)
end

spell:group("support")
spell:name("Fire Wall Rune")
spell:words("adevo mas grav flam")
spell:level(33)
spell:mana(780)
spell:soul(4)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "druid;true", "master sorcerer;true", "elder druid;true")
spell:register()
