local handler = PacketHandler(0xE8)

function handler.onReceive(player, msg)
	local playerGuid = player:getGuid()
	if Game.hasDebugAssert(playerGuid) then
		return
	end

	local assertLine = msg:getString()
	local date = msg:getString()
	local description = msg:getString()
	local comment = msg:getString()
	Game.saveDebugAssert(playerGuid, assertLine, date, description, comment)
end

handler:register()
