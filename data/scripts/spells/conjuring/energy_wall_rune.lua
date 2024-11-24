local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2279, 4)
end

spell:group("support")
spell:name("Energy Wall Rune")
spell:words("adevo mas grav vis")
spell:level(41)
spell:mana(1000)
spell:soul(5)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "druid;true", "master sorcerer;true", "elder druid;true")
spell:register()
