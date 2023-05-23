local talk = TalkAction("/ss")

function talk.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local screenshotType = tonumber(param)
	if screenshotType then
		player:takeScreenshot(screenshotType)
	else
		player:sendTextMessage(MESSAGE_INFO_DESCR, "Invalid screenshot type.")
	end
	return false
end

talk:separator(" ")
talk:register()
