local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2266, 1)
end

spell:group("support")
spell:name("Cure Poison Rune")
spell:words("adana pox")
spell:level(15)
spell:mana(200)
spell:soul(1)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
