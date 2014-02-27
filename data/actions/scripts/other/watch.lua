function onUse(cid, item, fromPosition, itemEx, toPosition)
	Player(cid):sendTextMessage(MESSAGE_INFO_DESCR, ("The time is %s."):format(getTibianTime()))
	return true
end
