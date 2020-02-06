local foods = {
	2666, -- meat
	2671, -- ham
	2681, -- grape
	2674, -- apple
	2689, -- bread
	2690, -- roll
	2696  -- cheese
}

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	if math.random(0, 1) == 1 then
		creature:addItem(foods[math.random(#foods)])
	end

	creature:addItem(foods[math.random(#foods)])
	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	return true
end

spell:name("Food")
spell:words("exevo pan")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:id(42)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(14)
spell:mana(120)
spell:soul(1)
spell:isAggressive(false)
spell:register()
