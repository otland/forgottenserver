local handler = PacketHandler(0xD4)

function handler.onReceive(player, msg)
	local mount = msg:getByte() ~= 0
	player:toggleMount(mount)
end

handler:register()
