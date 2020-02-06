local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2401, 2433, 1, CONST_ME_MAGIC_GREEN)
end

spell:name("Enchant Staff")
spell:words("exeta vis")
spell:group("support")
spell:vocation("master sorcerer")
spell:id(92)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(41)
spell:mana(80)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
