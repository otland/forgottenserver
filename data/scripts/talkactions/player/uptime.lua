local uptime = TalkAction("!uptime")

function uptime.onSay(player, words, param)
	local hours = math.floor(getWorldUpTime() / 3600)
	local minutes = math.floor((getWorldUpTime() - (3600 * hours)) / 60)
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Uptime: " .. hours .. " hours and " .. minutes .. " minutes.")
	return false
end

uptime:register()
