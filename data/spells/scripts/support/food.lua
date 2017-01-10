local food = {
	2666, -- meat
	2671, -- ham
	2681, -- grape
	2674, -- apple
	2689, -- bread
	2690, -- roll
	2696  -- cheese
}

function onCastSpell(creature, variant)
	creature:addItem(food[math.random(#food)])
	if math.random(1, 100) > 50 then
		creature:addItem(food[math.random(#food)])
	end

	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	return true
end
