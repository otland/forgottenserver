function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
		return false
	end

	logCommand(player, words, param)

	if not Game.startEvent(param) then
		player:sendCancelMessage("There is no event with that name.")
	end
	return true
end
