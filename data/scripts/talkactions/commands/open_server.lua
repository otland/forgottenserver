local talkaction = TalkAction("/openserver")

function talkaction.onSay(player, words, param)
	Game.setGameState(GAME_STATE_NORMAL)
	player:sendTextMessage(MESSAGE_INFO_DESCR, "Server is now open.")
	return false
end

talkaction:access(true)
talkaction:accountType(ACCOUNT_TYPE_GOD)
talkaction:register()
