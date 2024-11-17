do
	local playersRecord = 0

	function Game.getPlayersRecord()
		return playersRecord
	end

	function Game.setPlayersRecord(value)
		playersRecord = value
	end

	function Game.savePlayersRecord()
		db.query("UPDATE `server_config` SET `value` = '" .. Game.getPlayersRecord() .. "' WHERE `config` = 'players_record'")
	end
end
