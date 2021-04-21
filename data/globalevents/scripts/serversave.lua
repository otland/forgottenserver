local function ServerSave()
	if configManager.getBoolean(configKeys.SERVER_SAVE_CLEAN_MAP) then
		cleanMap()
	end

	if configManager.getBoolean(configKeys.SERVER_SAVE_CLOSE) then
		Game.setGameState(GAME_STATE_CLOSED)
	end

	if configManager.getBoolean(configKeys.SERVER_SAVE_SHUTDOWN) then
		Game.setGameState(GAME_STATE_SHUTDOWN)
	end
end

local function ServerSaveWarning(time)
	local remaningTime = tonumber(time) - (1 * 60 * 1000)

	if configManager.getBoolean(configKeys.SERVER_SAVE_NOTIFY_MESSAGE) then
		Game.broadcastMessage("Server is saving game in " .. (remaningTime / (1 * 60 * 1000)) .. " minute(s). Please logout.", MESSAGE_STATUS_WARNING)
	end

	if remaningTime > (1 * 60 * 1000) then
		addEvent(ServerSaveWarning, 1 * 60 * 1000, remaningTime)
	else
		addEvent(ServerSave, 1 * 60 * 1000)
	end
end

function onTime(interval)
	local remaningTime = configManager.getNumber(configKeys.SERVER_SAVE_NOTIFY_DURATION) * 60 * 1000
	if configManager.getBoolean(configKeys.SERVER_SAVE_NOTIFY_MESSAGE) then
		Game.broadcastMessage("Server is saving game in " .. (remaningTime / (1 * 60 * 1000)) .. " minute(s). Please logout.", MESSAGE_STATUS_WARNING)
	end

	addEvent(ServerSaveWarning, 1 * 60 * 1000, remaningTime)
	return not configManager.getBoolean(configKeys.SERVER_SAVE_SHUTDOWN)
end
