function onSay(player, words, param)
	if(player:isLiveCasting()) then
			player:stopLiveCasting()
			player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "You have stopped live casting.")
	else
		local password = param:trim()
		password = password:gsub('%W','')
		if(password:len() > 30) then
			player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Invalid cast password. Your password may not exceed 30 characters in length.")
			return false
		end
		player:startLiveCasting(password)
		player:sendChannelMessage("", "You have started a live cast. To stop live casting simply type " .. words, TALKTYPE_CHANNEL_O, CHANNEL_CAST)
		if(password:len() > 0) then
			player:sendChannelMessage("", "The password to your cast is: " .. password .. ".", TALKTYPE_CHANNEL_O, CHANNEL_CAST)
		end
		player:sendChannelMessage("", "To view a list of available live cast commands, type /commands in the cast channel.", TALKTYPE_CHANNEL_O, CHANNEL_CAST)
	end
	return false
end
