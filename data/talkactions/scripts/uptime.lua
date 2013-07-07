function onSay(cid, words, param)
	local hours = math.ceil(getWorldUpTime() / 3600) - 1
	local minutes = math.ceil((getWorldUpTime() - (3600 * hours)) / 60)
	if minutes == 60 then
		minutes = 0
		hours = hours + 1
	end
	doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_BLUE, "Uptime: " .. hours .. " hours and " .. minutes .. " minutes.")
end