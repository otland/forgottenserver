local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2389, 7367, 1, CONST_ME_MAGIC_GREEN)
end

spell:group("support")
spell:id(110)
spell:name("Enchant Spear")
spell:words("exeta con")
spell:level(45)
spell:mana(350)
spell:soul(3)
spell:isAggressive(false)
spell:isSelfTarget(true)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("paladin;true", "royal paladin;true")
spell:register()
