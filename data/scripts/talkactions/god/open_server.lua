local openServer = TalkAction("/openserver")

function openServer.onSay(player, words, param)
	if(not player:getGroup():getAccess()) or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	Game.setGameState(GAME_STATE_NORMAL)
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Server is now open.")
	return false
end

openServer:register()
