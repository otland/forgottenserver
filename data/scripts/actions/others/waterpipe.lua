local waterpipe = Action()

function waterpipe.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if math.random(3) == 1 then
		item:getPosition():sendMagicEffect(CONST_ME_POFF)
	else
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return true
end

waterpipe:id(2974, 2980)
waterpipe:register()
