local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2286, 2)
end

spell:group("support")
spell:name("Poison Bomb Rune")
spell:words("adevo mas pox")
spell:level(25)
spell:mana(520)
spell:soul(2)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
