function onSay(cid, words, param)
	Game.setGameState(GAME_STATE_NORMAL)
	Player(cid):sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Server is now open.")
	return false
end
