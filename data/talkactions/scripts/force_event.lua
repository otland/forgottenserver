function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
		return false
	end

	logCommand(player, words, param)

	local returnValue = Game.startEvent(param)
	if returnValue then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "Event started.")
	else
		player:sendTextMessage(MESSAGE_INFO_DESCR, "No such event exists.")
	end
	return returnValue
end
