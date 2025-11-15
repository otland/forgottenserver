local talkaction = TalkAction("/openserver")

function talkaction.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	Game.setGameState(GAME_STATE_NORMAL)
	player:sendTextMessage(MESSAGE_INFO_DESCR, "Server is now open.")
	return false
end

talkaction:register()
