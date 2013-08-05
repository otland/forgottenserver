function onSpeak(cid, type, message)
	if getPlayerLevel(cid) == 1 and getPlayerAccountType(cid) < ACCOUNT_TYPE_GAMEMASTER then
		doPlayerSendCancel(cid, "You may not speak into channels as long as you are on level 1.")
		return false
	end
	return true
end
