do
	-- Load players record
	local event = GlobalEvent("LoadPlayersRecord")

	function event.onStartup()
		local resultId = db.storeQuery("SELECT `value` FROM `server_config` WHERE `config` = 'players_record'")
		if resultId then
			Game.setPlayersRecord(result.getNumber(resultId, "value"))
		else
			db.query("INSERT INTO `server_config` (`config`, `value`) VALUES ('players_record', '0')")
		end
		return true
	end

	event:register()
end

do
	-- Save players record
	local event = GlobalEvent("SavePlayersRecord")

	function event.onSave()
		Game.savePlayersRecord()
		return true
	end
	
	event:register()
end
