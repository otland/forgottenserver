local handler = PacketHandler(CLIENT_MOUNT)

function handler.onReceive(player, msg)
	local mount = msg:getByte() ~= 0
	player:toggleMount(mount)
end

handler:register()
