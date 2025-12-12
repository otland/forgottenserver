local handler = PacketHandler(0xD2)

function handler.onReceive(player, msg)
	if not configManager.getBoolean(configKeys.ALLOW_CHANGEOUTFIT) then
		return
	end

	player:sendOutfitWindow()
end

handler:register()
