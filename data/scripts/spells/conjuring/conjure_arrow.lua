local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(0, 2544, 10, CONST_ME_MAGIC_BLUE)
end

spell:name("Conjure Arrow")
spell:words("exevo con")
spell:group("support")
spell:vocation("paladin", "royal paladin")
spell:id(51)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(13)
spell:mana(100)
spell:soul(1)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()
