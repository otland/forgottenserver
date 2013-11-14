function onSay(cid, words, param)
	if param == "shutdown" then
		Game.setGameState(GAME_STATE_SHUTDOWN)
	else
		Game.setGameState(GAME_STATE_NORMAL)
		Player(cid):sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Server is now closed.")
	end
	return false
end
