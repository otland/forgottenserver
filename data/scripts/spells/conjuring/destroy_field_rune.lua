local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2261, 3)
end

spell:name("Destroy Field Rune")
spell:words("adito grav")
spell:group("support")
spell:vocation("druid", "elder druid", "paladin", "royal paladin", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(17)
spell:mana(120)
spell:soul(2)
spell:isAggressive(false)
spell:register()
