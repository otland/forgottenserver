local function int_to_ip(val)
	return string.format("%d.%d.%d.%d",
				bit.band(bit.rshift(val, 0),  	0x000000FF),
				bit.band(bit.rshift(val, 8),  	0x000000FF),
				bit.band(bit.rshift(val, 16),  	0x000000FF),
				bit.band(bit.rshift(val, 24),  	0x000000FF))
end

function onSay(cid, words, param)
	local player = Player(cid)	
	if not player:getGroup():getAccess() then
		return false
	end

	if player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
		return false
	end

	local target = Player(param)
	if target == nil then
		player:sendCancelMessage("Player not found.")
		return false
	end

	if target:getAccountType() > player:getAccountType() then
		player:sendCancelMessage("You can not get info about this player.")
		return false
	end

	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Name: " .. target:getName()
				.. "\nAccess: " .. (target:getGroup():getAccess() and "1" or "0")
				.. "\nLevel: " .. target:getLevel()
				.. "\nMagic Level: " .. target:getMagicLevel()
				.. "\nSpeed: " .. getCreatureSpeed(player:getId())
				.. "\nPosition: " .. string.format("(%0.5d / %0.5d / %0.3d)", target:getPosition().x, target:getPosition().y, target:getPosition().z)
				.. "\nIP: " .. int_to_ip(target:getIp()))
	
	
	local players = {}
	for _, tmp in ipairs(Game.getPlayers()) do
		if tmp:getIp() == target:getIp() and tmp ~= target then
			table.insert(players, tmp:getName() .. " [" .. tmp:getLevel() .. "]")
		end
	end

	if #players > 0 then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "\nOther players on same IP: " .. table.concat(players, ", ") .. ".")
	end
	return false
end
