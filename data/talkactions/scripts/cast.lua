function onSay(cid, words, param)
	local player = Player(cid)
	
	if player:isInCast() then
		player:sendCancelMessage("You are currently casting.")
		return false
	end

	player:setInCast(true)
	player:setPassword(param)

	player:sendTextMessage(MESSAGE_INFO_DESCR, "You have started casting.")
	return false
end
