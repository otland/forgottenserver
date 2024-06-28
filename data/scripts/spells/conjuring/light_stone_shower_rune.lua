local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 23722, 4)
end

spell:group("support")
spell:name("Light Stone Shower Rune")
spell:words("adori infir mas tera")
spell:level(1)
spell:mana(6)
spell:soul(3)
spell:isAggressive(false)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "druid;true", "paladin;true", "master sorcerer;true", "elder druid;true", "royal paladin;true")
spell:register()
