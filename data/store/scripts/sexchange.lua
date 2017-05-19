function onRender(player, offer)
	return true
end

function onBuy(player, offer)
	player:setSex(player:getSex() == PLAYERSEX_FEMALE and PLAYERSEX_MALE or PLAYERSEX_FEMALE)

	local playerOutfit = player:getOutfit()
	playerOutfit.lookType = player:getSex() == PLAYERSEX_FEMALE and 137 or 129
	player:setOutfit(playerOutfit)
	
	return true
end
