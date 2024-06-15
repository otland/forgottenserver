local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2290, 1)
end

spell:group("support")
spell:name("Convince Creature Rune")
spell:words("adeta sio")
spell:level(16)
spell:mana(200)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
