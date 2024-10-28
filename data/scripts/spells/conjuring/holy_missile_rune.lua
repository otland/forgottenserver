local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2295, 5)
end

spell:group("support")
spell:name("Holy Missile Rune")
spell:words("adori san")
spell:level(27)
spell:mana(300)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("paladin;true", "royal paladin;true")
spell:register()
