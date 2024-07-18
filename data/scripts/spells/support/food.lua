local foods = {
	2666, -- meat
	2671, -- ham
	2681, -- grape
	2674, -- apple
	2689, -- bread
	2690, -- roll
	2696 -- cheese
}

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	if math.random(0, 1) == 1 then
		creature:addItem(foods[math.random(#foods)])
	end

	creature:addItem(foods[math.random(#foods)])
	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	return true
end

spell:group("support")
spell:id(42)
spell:name("Food")
spell:words("exevo pan")
spell:level(14)
spell:mana(120)
spell:soul(1)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:isAggressive(false)
spell:vocation("druid;true", "elder druid;true")
spell:register()
