local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2288, 4)
end

spell:group("support")
spell:name("Stone Shower Rune")
spell:words("adori mas tera")
spell:level(28)
spell:mana(430)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
