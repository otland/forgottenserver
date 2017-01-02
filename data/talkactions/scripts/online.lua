local maxPlayersPerMessage = 10

function onSay(player, words, param)
	local hasAccess = player:getGroup():getAccess()
	local players = Game.getPlayers()
	local onlineList = {}

	for k, targetPlayer in ipairs(players) do
		if hasAccess or not targetPlayer:isInGhostMode() then
			table.insert(onlineList, targetPlayer:getName() .. " [" .. targetPlayer:getLevel() .. "]")
		end
	end

	local playersOnline = #onlineList
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, playersOnline .. " players online.")
	
	local i = 1
	while i <= playersOnline do
		local msg = table.concat(onlineList, ", ", i, math.min(i + maxPlayersPerMessage - 1, playersOnline))
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, msg .. ".")
		i = i + maxPlayersPerMessage
	end
	return false
end
