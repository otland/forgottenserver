local talk = TalkAction("/event", "!event")

function talk.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
		return false
	end

	logCommand(player, words, param)

	local returnValue = Game.startEvent(param)
	if returnValue == nil then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "No such event exists.")
		return false
	end

	player:sendTextMessage(MESSAGE_INFO_DESCR, "Event started.")
	return returnValue
end

talk:separator(" ")
talk:register()
