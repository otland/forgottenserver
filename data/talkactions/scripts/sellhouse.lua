function onSay(player, words, param)
	local tradePartner = Player(param)
	if not tradePartner or tradePartner == player then
		player:sendCancelMessage("Trade player not found.")
		return false
	end

	local house = player:getTile():getHouse()
	if not house then
		player:sendCancelMessage("You must stand in your house to initiate the trade.")
		return false
	end

	local returnValue = house:startTrade(player, tradePartner)
	if returnValue ~= RETURNVALUE_NOERROR then
		player:sendCancelMessage(returnValue)
	end
	return false
end
