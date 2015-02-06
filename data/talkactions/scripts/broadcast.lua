function onSay(player, words, param)
	if not getPlayerFlagValue(player, PlayerFlag_CanBroadcast) then
		return true
	end

	print("> " .. player:getName() .. " broadcasted: \"" .. param .. "\".")
	for _, tmpPlayer in ipairs(Game.getPlayers()) do
		tmpPlayer:sendPrivateMessage(player, param, TALKTYPE_BROADCAST)
	end
	return false
end
