function onSay(cid, words, param)
	local player = Player(cid)
	if player:getAccountType() <= ACCOUNT_TYPE_TUTOR then
		return true
	end

	local target = Player(param)
	if target == nil then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "A player with that name is not online.")
		return false
	end

	if target:getAccountType() ~= ACCOUNT_TYPE_NORMAL then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You can only promote a normal player to a tutor.")
		return false
	end

	target:setAccountType(ACCOUNT_TYPE_TUTOR)
	target:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have been promoted to a tutor by " .. player:getName() .. ".")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have promoted " .. target:getName() .. " to a tutor.")
	return false
end
