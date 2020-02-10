function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	player:sendTextMessage(MESSAGE_INFO_DESCR, "The time is " .. getFormattedWorldTime() .. ".")
	if item.itemid == 6092 then
		item:transform(item.itemid-1)
		item:decay()
		return true
	end
	return true
end
