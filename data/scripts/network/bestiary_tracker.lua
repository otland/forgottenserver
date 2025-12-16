local handler = PacketHandler(0x2A)

function handler.onReceive(player, msg)
	local raceId = msg:getU16()
	local checked = msg:getByte() == 1

	if checked then
		local trackedBestiary = player:getTrackedBestiary()
		if #trackedBestiary >= player:getMaxTrackedBestiary() then
			player:sendTextMessage(MESSAGE_STATUS_WARNING, "You have reached the maximum number of trackable creatures.\nYou have to remove one of your currently tracked creatures before you can add another one.")
			return
		end

		player:trackBestiary(raceId)
	else
		player:untrackBestiary(raceId)
	end

	player:sendTrackedBestiary()
end

handler:register()
