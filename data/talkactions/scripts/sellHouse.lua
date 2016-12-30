function onSay(player, words, param)
	local buyer = Player(param)
	if buyer == nil then
		player:sendCancelMessage("Trade player not found.")
		return false
	end

	local position = player:getPosition()
	if position:getDistance(buyer:getPosition()) > 2 then
		player:sendCancelMessage("Trade player is too far away.")
		return false
	end

	if not buyer:isPremium() then
		player:sendCancelMessage("Trade player does not have a premium account.")
		return false
	end

	local tile = Tile(position)
	local house = tile and tile:getHouse()
	if house == nil then
		player:sendCancelMessage("You must stand in your house to initiate the trade.")
		return false
	end

	if house:getOwnerGuid() ~= player:getGuid() then
		player:sendCancelMessage("You don't own this house.")
		return false
	end

	if buyer:getHouse() then
		player:sendCancelMessage("Trade player already owns a house.")
		return false
	end

	if buyer:hasBidOnHouse() then
		player:sendCancelMessage("Trade player is currently the highest bidder of an auctioned house.")
		return false
	end

	local transferItem = house:getTransferItem()
	if transferItem == nil then
		player:sendCancelMessage("You can not trade this house.")
		return false
	end

	house:moveTransferItemTo(transferItem, buyer)

	if not player:startTrade(buyer, transferItem) then
		house:resetTransferItem()
	end

	return true
end
