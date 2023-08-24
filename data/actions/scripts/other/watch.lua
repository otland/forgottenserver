function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	player:sendTextMessage(MESSAGE_INFO_DESCR, "The time is " .. getFormattedWorldTime() .. ".")
	return true
end
