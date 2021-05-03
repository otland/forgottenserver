function onSay(player, words, param)
	local fragTime = configManager.getNumber(configKeys.FRAG_TIME)
	if fragTime <= 0 then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "You do not have any unjustified kill.")
		return false
	end

	local skullTime = player:getSkullTime()
	if skullTime <= 0 then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "You do not have any unjustified kill.")
		return false
	end

	local kills = math.ceil(skullTime / fragTime)
	local remainingSeconds = math.floor(skullTime % fragTime)
	local hours = math.floor(remainingSeconds / (1 * 60 * 60))
	local minutes = math.floor((remainingSeconds % (1 * 60 * 60)) / 60)
	local seconds = remainingSeconds % 60
	local text = "You have " .. kills .. " unjustified kill" .. (kills > 1 and "s" or "") .. ". The amount of unjustified kills will decrease after: "
	if hours ~= 0 then
		if hours == 1 then
			text = text .. hours .. " hour, "
		else
			text = text .. hours .. " hours, "
		end
	end

	if hours ~= 0 or minutes ~= 0 then
		if minutes == 1 then
			text = text .. minutes .. " minute and "
		else
			text = text .. minutes .. " minutes and "
		end
	end

	if seconds == 1 then
		text = text .. seconds .. " second."
	else
		text = text .. seconds .. " seconds."
	end

	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, text)
	return false
end
