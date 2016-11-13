function onSay(player, words, param)
	local tradePartner = Player(param)
	if not tradePartner or tradePartner == player then
		player:sendCancelMessage("Trade player not found.")
		return false
	end

	local tile = Tile(player:getPosition())
	local house = tile:getHouse()
	if not house then
		player:sendCancelMessage("You must stand in your house to initiate the trade.")
		return false
	end

	local ret = house:startTrade(player, tradePartner)
	if ret ~= RETURNVALUE_NOERROR then
		player:sendCancelMessage(ret)
	end
	return false
end
