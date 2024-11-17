do
	local playersRecord = 0

	function Game.getPlayersRecord()
		return playersRecord
	end

	function Game.setPlayersRecord(value)
		playersRecord = value
	end
end
