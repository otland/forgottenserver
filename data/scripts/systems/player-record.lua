do
	local event = Event()

	event.onGameStartup = function()
		local resultId = db.storeQuery("SELECT `value` FROM `server_config` WHERE `config` = 'players_record'")
		if resultId then
			Game.setPlayerRecord(result.getNumber(resultId, "value"))
			result.free(resultId)
		else
			db.query("INSERT INTO `server_config` (`config`, `value`) VALUES ('players_record', '0')")
		end
	end

	event:register()
end

do
	local event = Event()

	event.onPlayerJoin = function(self)
		local playerCount = Game.getPlayerCount()
		if playerCount <= Game.getPlayerRecord() then
			return
		end

		Game.setPlayerRecord(playerCount)
		Game.broadcastMessage("New record: " .. playerCount .. " players are logged in.", MESSAGE_STATUS_DEFAULT)

		db.asyncQuery("UPDATE `server_config` SET `value` = '" .. playerCount .. "' WHERE `config` = 'players_record'")
	end

	event:register()
end
