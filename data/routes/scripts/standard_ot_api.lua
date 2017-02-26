local json = nil

local function getPlayersOnline(session)
	session.responder:setResponseStatus(200, 'OK')
	session.responder:setResponseField("Content-type", "application/json")
	session.responder:setResponseBody(json.encode(Game.getPlayers()))
end

local function getServerStatus(session)
	local data = {
		uptime = getWorldUpTime(),
		ip = configManager.getString(configKeys.IP),
		serverName = configManager.getString(configKeys.SERVER_NAME),
		port = configManager.getNumber(configKeys.LOGIN_PORT),
		location = configManager.getString(configKeys.LOCATION),
		url = configManager.getString(configKeys.URL),
		server = 'The Forgotten Server',
		version = '1.3',
		client = Game.getClientVersion().string,
		owner = {
			name = configManager.getString(configKeys.OWNER_NAME),
			email = configManager.getString(configKeys.OWNER_EMAIL)
		},
		players = {
			online = Game.getPlayerCount(),
			max = 0,
			peak = 0,
		},
		monsters = {
			total = Game.getMonsterCount()
		},
		npcs = {
			total = Game.getNpcCount()
		},
		rates = {
			experience = configManager.getNumber(configKeys.RATE_EXPERIENCE),
			skill = configManager.getNumber(configKeys.RATE_SKILL),
			loot = configManager.getNumber(configKeys.RATE_LOOT),
			magic = configManager.getNumber(configKeys.RATE_MAGIC),
			spawn = configManager.getNumber(configKeys.RATE_SPAWN),
		},
		map = {
			name = configManager.getString(configKeys.MAP_NAME),
			author = configManager.getString(configKeys.MAP_AUTHOR),
			width = 0,
			height = 0
		},
		motd = configManager.getString(configKeys.MOTD)
	}

	session.responder:setResponseStatus(200, 'OK')
	session.responder:setResponseField("Content-type", "application/json")
	session.responder:setResponseBody(json.encode(data))
end

return {
	register = function(dependencies)
		json = dependencies.json
		dependencies.router:register('/ot_api/status/players', getPlayersOnline, {GET = {}})
		dependencies.router:register('/ot_api/status/info', getServerStatus, {GET = {}})
	end
}
