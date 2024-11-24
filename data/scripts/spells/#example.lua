local conjureRune = Spell(SPELL_INSTANT)

function conjureRune.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2275, 25)
end

conjureRune:name("Test")
conjureRune:id(221)
conjureRune:words("adori mas test")
conjureRune:level(30)
conjureRune:mana(530)
conjureRune:group("support")
conjureRune:soul(3)
conjureRune:isAggressive(false)
conjureRune:cooldown(2000)
conjureRune:groupCooldown(2000)
conjureRune:needLearn(false)
conjureRune:vocation("sorcerer", "master sorcerer")
conjureRune:register()
