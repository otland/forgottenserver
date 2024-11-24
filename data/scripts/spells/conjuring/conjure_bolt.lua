local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(0, 2543, 5, CONST_ME_MAGIC_BLUE)
end

spell:group("support")
spell:id(79)
spell:name("Conjure Bolt")
spell:words("exevo con mort")
spell:level(17)
spell:mana(140)
spell:soul(2)
spell:isAggressive(false)
spell:isSelfTarget(true)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("paladin;true", "royal paladin;true")
spell:register()
