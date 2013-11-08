function onSay(cid, words, param)
	local player = Player(cid)
	local hasAccess = player:getGroup():getAccess()
	local players = Game.getPlayers()
	local playerCount = Game.getPlayerCount()

	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, playerCount .. " players online.")

	local i = 0
	local msg = ""
	for k, tmpPlayer in ipairs(players) do
		if hasAccess or not tmpPlayer:isGhost() then
			if i > 0 then
				msg = msg .. ", "
			end
			msg = msg .. tmpPlayer:getName() .. " [" .. tmpPlayer:getLevel() .. "]"
			i = i + 1
		end

		if i == 10 then
			if k == playerCount then
				msg = msg .. "."
			else
				msg = msg .. ","
			end
			player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, msg)
			msg = ""
			i = 0
		end
	end

	if i > 0 then
		msg = msg .. "."
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, msg)
	end
	return false
end
