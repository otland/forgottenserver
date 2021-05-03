local foods = {2666, 2671, 2681, 2674, 2689, 2690, 2696}

function onCastSpell(creature, variant)
	if math.random(0, 1) == 1 then
		creature:addItem(foods[math.random(#foods)])
	end

	creature:addItem(foods[math.random(#foods)])
	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	return true
end
