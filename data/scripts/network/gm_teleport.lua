local handler = PacketHandler(CLIENT_MAP_CLICK)

function handler.onReceive(player, msg)
	if not player:getGroup():getAccess() then
		return
	end
	player:teleportTo(msg:getPosition())
end

handler:register()
