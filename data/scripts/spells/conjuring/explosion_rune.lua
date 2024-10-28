local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2313, 6)
end

spell:group("support")
spell:name("Explosion Rune")
spell:words("adevo mas hur")
spell:level(31)
spell:mana(570)
spell:soul(4)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "druid;true", "master sorcerer;true", "elder druid;true")
spell:register()
