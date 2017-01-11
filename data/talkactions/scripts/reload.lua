function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	if Game.reload(param) then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Reloaded " .. param .. ".")
	else
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Reload type not found.")
	end
	return false
end
