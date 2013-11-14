function onSay(cid, words, param)
	local uptime = getWorldUpTime()

	local hours = math.floor(uptime / 3600)
	local minutes = math.floor((uptime - (3600 * hours)) / 60)
	Player(cid):sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Uptime: " .. hours .. " hours and " .. minutes .. " minutes.")
	return false
end
