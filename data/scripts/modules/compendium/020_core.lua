function sendCompendiumError(player, infoType, errorCode)
	local response = NetworkMessage();
	response:addByte(COMPENDIUM_RESPONSE_PLAYERDATA)
	response:addByte(infoType)
	response:addByte(errorCode)
	response:sendToPlayer(player)
end