local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(0, 2260, 1)
end

spell:group("support")
spell:name("Blank Rune")
spell:words("adori blank")
spell:level(20)
spell:mana(50)
spell:soul(1)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true", "paladin;true", "royal paladin;true", "sorcerer;true", "master sorcerer;true")
spell:register()
