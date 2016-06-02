function Game.broadcastMessage(message, messageType)
	if messageType == nil then
		messageType = MESSAGE_STATUS_WARNING
	end

	for _, player in ipairs(Game.getPlayers()) do
		player:sendTextMessage(messageType, message)
	end
end

function Game.convertIpToString(ip)
	local band = bit.band
	local rshift = bit.rshift
	return string.format("%d.%d.%d.%d",
		band(ip, 0xFF),
		band(rshift(ip, 8), 0xFF),
		band(rshift(ip, 16), 0xFF),
		rshift(ip, 24)
	)
end

function Game.getReverseDirection(direction)
	local config = {
		[DIRECTION_NORTH] = DIRECTION_SOUTH,
		[DIRECTION_SOUTH] = DIRECTION_NORTH,
		[DIRECTION_EAST] = DIRECTION_WEST,
		[DIRECTION_WEST] = DIRECTION_EAST
	}

	return config[direction] or DIRECTION_NORTH
end

function Game.getSkillType(weaponType)
	local config = {
		[WEAPON_CLUB] = SKILL_CLUB
		[WEAPON_SWORD] = SKILL_SWORD
		[WEAPON_AXE] = SKILL_AXE
		[WEAPON_DISTANCE] = SKILL_DISTANCE
		[WEAPON_SHIELD] = SKILL_SHIELD
	}

	return config[weaponType] or SKILL_FIST
end

if not globalStorageTable then
	globalStorageTable = {}
end

function Game.getStorageValue(key)
	return globalStorageTable[key]
end

function Game.setStorageValue(key, value)
	globalStorageTable[key] = value
end
