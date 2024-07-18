local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2265, 1)
end

spell:group("support")
spell:name("Intense Healing Rune")
spell:words("adura gran")
spell:level(15)
spell:mana(120)
spell:soul(2)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
