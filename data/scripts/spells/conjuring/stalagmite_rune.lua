local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2292, 10)
end

spell:group("support")
spell:name("Stalagmite Rune")
spell:words("adori tera")
spell:level(24)
spell:mana(350)
spell:soul(2)
spell:isPremium(true)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "master sorcerer;true", "druid;true", "elder druid;true")
spell:register()
