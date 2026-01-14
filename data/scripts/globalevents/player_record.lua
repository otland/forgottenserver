local event = GlobalEvent("PlayerRecord")

function event.onRecord(current, old)
	addEvent(Game.broadcastMessage, 150, "New record: " .. current .. " players are logged in.", MESSAGE_STATUS_DEFAULT)
	return true
end

event:register()
