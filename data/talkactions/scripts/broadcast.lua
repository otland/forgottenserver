function onSay(cid, words, param)
	if not getPlayerFlagValue(cid, PlayerFlag_CanBroadcast) then
		return true
	end

	local player = Player(cid)
	print("> " .. player:getName() .. " broadcasted: \"" .. param .. "\".")
	for _, tmpPlayer in ipairs(Game.getPlayers()) do
		tmpPlayer:sendPrivateMessage(player, param, TALKTYPE_BROADCAST)
	end
	return false
end
