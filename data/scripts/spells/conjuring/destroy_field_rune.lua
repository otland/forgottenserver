local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2261, 3)
end

spell:group("support")
spell:name("Destroy Field Rune")
spell:words("adito grav")
spell:level(17)
spell:mana(120)
spell:soul(2)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "druid;true", "paladin;true", "master sorcerer;true", "elder druid;true", "royal paladin;true")
spell:register()
