local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(0, 28416, 1, CONST_ME_MAGIC_GREEN)
end

spell:group("support")
spell:id(92)
spell:name("Conjure Wand of Darkness")
spell:words("exevo gran mort")
spell:level(41)
spell:mana(250)
spell:isAggressive(false)
spell:isSelfTarget(true)
spell:cooldown(1800)
spell:groupCooldown(2000)
spell:vocation("master sorcerer;true")
spell:register()
