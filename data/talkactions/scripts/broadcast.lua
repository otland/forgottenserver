function onSay(cid, words, param)
	if not getPlayerFlagValue(cid, PlayerFlag_CanBroadcast) then
		return true
	end

	local player = Player(cid)
	print("> " .. player:getName() .. " broadcasted: \"" .. param .. "\".")
	for _, tmpPlayer in ipairs(Game.getPlayers()) do
		player:channelSay(tmpPlayer, TALKTYPE_BROADCAST, param, 0)
	end
	return false
end
