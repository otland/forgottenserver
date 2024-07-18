local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(0, 7364, 5, CONST_ME_MAGIC_BLUE)
end

spell:group("support")
spell:id(108)
spell:name("Conjure Sniper Arrow")
spell:words("exevo con hur")
spell:level(24)
spell:mana(160)
spell:soul(3)
spell:isAggressive(false)
spell:isSelfTarget(true)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("paladin;true", "royal paladin;true")
spell:register()
