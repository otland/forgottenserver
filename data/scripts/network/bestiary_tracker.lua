local handler = PacketHandler(0x2A)

function handler.onReceive(player, msg)
	local raceId = msg:getU16()
	local checked = msg:getByte() == 1
	if player:setTrackedBestiary(raceId, checked) then
		player:sendBestiaryTracker()
	end
end

handler:register()
