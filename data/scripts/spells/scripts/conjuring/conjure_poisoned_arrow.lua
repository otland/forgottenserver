local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(0, 2545, 7, CONST_ME_MAGIC_BLUE)
end

spell:name("Conjure Poisoned Arrow")
spell:words("exevo con pox")
spell:group("support")
spell:vocation("paladin", "royal paladin")
spell:id(48)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(16)
spell:mana(130)
spell:soul(2)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()
