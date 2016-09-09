function onSay(player, words, param)
	if player:startLiveCast(param) then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You have started casting your gameplay.")
	else
		player:sendCancelMessage("You're already casting your gameplay.")
	end
	return false
end
