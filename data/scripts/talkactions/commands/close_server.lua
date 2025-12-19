local talkaction = TalkAction("/closeserver")

function talkaction.onSay(player, words, param)
	if param == "shutdown" then
		Game.setGameState(GAME_STATE_SHUTDOWN)
	else
		Game.setGameState(GAME_STATE_CLOSED)
		player:sendTextMessage(MESSAGE_INFO_DESCR, "Server is now closed.")
	end
	return false
end

talkaction:separator(" ")
talkaction:access(true)
talkaction:accountType(ACCOUNT_TYPE_GOD)
talkaction:register()
