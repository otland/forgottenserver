function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local target = Player(param)
	if not target then
		player:sendCancelMessage("Player not found.")
		return false
	end

	if target:getAccountType() > player:getAccountType() then
		player:sendCancelMessage("You can not get info about this player.")
		return false
	end

	local targetIp = target:getIp()
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Name: " .. target:getName())
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Access: " .. (target:getGroup():getAccess() and "1" or "0"))
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Level: " .. target:getLevel())
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Magic Level: " .. target:getMagicLevel())
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Speed: " .. target:getSpeed())
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Position: " .. string.format("(%0.5d / %0.5d / %0.3d)", target:getPosition().x, target:getPosition().y, target:getPosition().z))
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "IP: " .. Game.convertIpToString(targetIp))

	local players = {}
	for _, targetPlayer in ipairs(Game.getPlayers()) do
		if targetPlayer:getIp() == targetIp and targetPlayer ~= target then
			players[#players + 1] = targetPlayer:getName() .. " [" .. targetPlayer:getLevel() .. "]"
		end
	end

	if #players > 0 then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Other players on same IP: " .. table.concat(players, ", ") .. ".")
	end
	return false
end
