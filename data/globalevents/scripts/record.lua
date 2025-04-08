function onRecord(current, old)
	addEvent(Game.broadcastMessage, 150, string.format("New record: %d players are logged in.", current), MESSAGE_STATUS_DEFAULT)
	return true
end
