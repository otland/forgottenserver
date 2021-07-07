function onParsePacket(player, recvbyte, msg)
	-- structure for parseUseItem packet, recbyte: 130 (0x82)
	local pos = msg:getPosition()
	local clientId = msg:getU16()
	local stackpos = msg:getByte()
	local index = msg:getByte()
	print(("Player: %s wants to use item at position (%d, %d, %d) with sprite id %d"):format(player:getName(), pos.x, pos.y, pos.z, clientId))
	msg:delete()
end
