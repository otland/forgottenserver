local shutdownAtServerSave = false
local cleanMapAtServerSave = false

local function serverSave()
	if shutdownAtServerSave then
		Game.setGameState(GAME_STATE_SHUTDOWN)
	else
		Game.setGameState(GAME_STATE_CLOSED)

		if cleanMapAtServerSave then
			cleanMap()
		end

		Game.setGameState(GAME_STATE_NORMAL)
	end
end

local function secondServerSaveWarning()
	Game.broadcastMessage("Server is saving game in one minute. Please logout.")
	addEvent(serverSave, 60000)
end

local function firstServerSaveWarning()
	Game.broadcastMessage("Server is saving game in 3 minutes. Please logout.")
	addEvent(secondServerSaveWarning, 120000)
end

function onTime(interval)
	Game.broadcastMessage("Server is saving game in 5 minutes. Please logout.")
	Game.setGameState(GAME_STATE_STARTUP)
	addEvent(firstServerSaveWarning, 120000)
	return not shutdownAtServerSave
end
