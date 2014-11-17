function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	player:sendTextMessage(MESSAGE_INFO_DESCR, "The time is " .. getTibianTime() .. ".")
	return true
end
