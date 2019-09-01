local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2310, 3)
end

spell:name("Disintegrate Rune")
spell:words("adito tera")
spell:group("support")
spell:vocation("druid", "elder druid", "paladin", "royal paladin", "sorcerer", "master sorcerer")
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(21)
spell:mana(200)
spell:soul(3)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
