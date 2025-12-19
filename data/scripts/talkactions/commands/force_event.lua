local talkaction = TalkAction("/event")

function talkaction.onSay(player, words, param)
	logCommand(player, words, param)

	local returnValue = Game.startEvent(param)
	if returnValue == nil then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "No such event exists.")
		return false
	end

	player:sendTextMessage(MESSAGE_INFO_DESCR, "Event started.")
	return returnValue
end

talkaction:separator(" ")
talkaction:access(true)
talkaction:accountType(ACCOUNT_TYPE_GAMEMASTER)
talkaction:register()
