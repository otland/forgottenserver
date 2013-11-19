function onSay(cid, words, param)
	local player = Player(cid)
	local house = House(getTileHouseInfo(player:getPosition()))
	
	if not house then
		player:sendCancelMessage("You are not inside a house.")
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end
	
	if house:getOwnerGuid() ~= player:getGuid() then
		player:sendCancelMessage("You are not the owner of this house.")
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end
	
	house:setOwnerGuid(0)
	player:sendTextMessage(MESSAGE_INFO_DESCR, "You have successfully left your house.")
	player:getPosition():sendMagicEffect(CONST_ME_POFF)

	return false
end
