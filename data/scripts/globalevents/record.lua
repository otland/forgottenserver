local record = GlobalEvent("Player Record")

function record.onRecord(current, old)
	addEvent(broadcastMessage, 150, "New record: " .. current .. " players are logged in.", MESSAGE_STATUS_DEFAULT)
	return true
end

record:register()
