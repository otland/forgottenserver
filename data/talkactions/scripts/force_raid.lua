function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
		return false
	end

	logCommand(player, words, param)

	local returnValue = Game.startRaid(param)
	if returnValue ~= RETURNVALUE_NOERROR then
		player:sendTextMessage(MESSAGE_INFO_DESCR, Game.getReturnMessage(returnValue))
	else
		player:sendTextMessage(MESSAGE_INFO_DESCR, "Raid started.")
	end

	return false
end
