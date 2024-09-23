local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2273, 1)
end

spell:group("support")
spell:name("Ultimate Healing Rune")
spell:words("adura vita")
spell:level(24)
spell:mana(400)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
