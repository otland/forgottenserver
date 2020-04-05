local startRaid = TalkAction("/raid")

function startRaid.onSay(player, words, param)
	if(not player:getGroup():getAccess()) or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	if(param == "") then
		player:sendCancelMessage("Command param required.")
		return false
	end
	
	logCommand(player, words, param)
	local returnValue = Game.startRaid(param)
	if returnValue ~= RETURNVALUE_NOERROR then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, Game.getReturnMessage(returnValue))
	else
		player:sendTextMessage(MESSAGE_INFO_DESCR, "Raid started.")
	end
	return false
end

startRaid:separator(" ")
startRaid:register()
