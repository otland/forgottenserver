local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(0, 2547, 10, CONST_ME_MAGIC_BLUE)
end

spell:group("support")
spell:id(95)
spell:name("Conjure Power Bolt")
spell:words("exevo con vis")
spell:level(59)
spell:mana(700)
spell:soul(4)
spell:isAggressive(false)
spell:isSelfTarget(true)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("royal paladin;true")
spell:register()
