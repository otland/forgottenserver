local maxPlayersPerMessage = 10

function onSay(player, words, param)
	local hasAccess = player:getGroup():getAccess()
	local onlineList = {}
	for _, targetPlayer in ipairs(Game.getPlayers()) do
		if hasAccess or not targetPlayer:isInGhostMode() then
			table.insert(onlineList, ("%s [%d]"):format(targetPlayer:getName(), targetPlayer:getLevel()))
		end
	end

	local playersOnline = #onlineList
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, ("%d players online."):format(playersOnline))

	for i = 1, playersOnline, maxPlayersPerMessage do
		local j = math.min(i + maxPlayersPerMessage - 1, playersOnline)
		local text = table.concat(onlineList, ", ", i, j) .. "."
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, text)
	end
	return false
end
