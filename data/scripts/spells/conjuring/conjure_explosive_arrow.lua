local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(0, 2546, 8, CONST_ME_MAGIC_BLUE)
end

spell:name("Conjure Explosive Arrow")
spell:words("exevo con flam")
spell:group("support")
spell:vocation("paladin", "royal paladin")
spell:id(49)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(25)
spell:mana(290)
spell:soul(3)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()
