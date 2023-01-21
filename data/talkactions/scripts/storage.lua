function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	local split = param:splitTrimmed(",")
	local target = Player(split[1])

	if not target then
		player:sendCancelMessage("Player not found.")
		return false
	end

	local key = tonumber(split[2])
	if not key or key < 0 then
		player:sendCancelMessage("Key must be a positive integer.")
		return false
	end

	if not split[3] then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, ("%s: %d = %d"):format(target:getName(), key, target:getStorageValue(key)))
		return false
	end

	local value = tonumber(split[3])
	if not value then
		player:sendCancelMessage("Value must be an integer.")
		return false
	end

	target:setStorageValue(key, value)
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, ("%s: %d = %d"):format(target:getName(), key, value))
	return false
end
