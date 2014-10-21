function onUse(cid, item, fromPosition, itemEx, toPosition, isHotkey)
	Player(cid):sendTextMessage(MESSAGE_INFO_DESCR, "The time is " .. getTibianTime() .. ".")
	return true
end
