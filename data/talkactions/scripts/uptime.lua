function onSay(player, words, param)
	local uptime = getWorldUpTime()
	local hours = math.floor(uptime / (1 * 60 * 60))
	local minutes = math.floor((uptime - (hours * 60 * 60)) / 60)
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Uptime: " .. hours .. " hours and " .. minutes .. " minutes.")
	return false
end
