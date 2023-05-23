local handler = PacketHandler(0x73)

function handler.onReceive(player, msg)
	if not player:getGroup():getAccess() then
		return
	end
	player:teleportTo(msg:getPosition())
end

handler:register()
