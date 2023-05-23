local talk = TalkAction("/ss")

function talk.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local screenshotType = tonumber(param)
	if screenshotType then
		player:takeScreenshot(screenshotType)
	else
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Invalid screenshot type.")
	end
	return false
end

talk:separator(" ")
talk:register()
