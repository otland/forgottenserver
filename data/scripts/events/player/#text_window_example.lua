local reactWindow = TalkAction("!textWindow")

function reactWindow.onSay(player, words, param, type)
	local txtWindow = TextWindow({
		itemId = 1234,
		text = "Write something here:",
		canWrite = true,
		length = 100,
		callback = function(player, item, text)
			player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "You wrote: " .. text)
			return true
		end
	})

	txtWindow:sendToPlayer(player)
	return false
end

reactWindow:register()
