local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(0, 7363, 5, CONST_ME_MAGIC_BLUE)
end

spell:group("support")
spell:id(109)
spell:name("Conjure Piercing Bolt")
spell:words("exevo con grav")
spell:level(33)
spell:mana(180)
spell:soul(3)
spell:isAggressive(false)
spell:isSelfTarget(true)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("paladin;true", "royal paladin;true")
spell:register()
