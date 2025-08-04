local event = CreatureEvent("PlayersRecord")

function event.onLogin(player)
	local players = #Game.getPlayers()
	local record = Game.getPlayersRecord()
	if players > record then
		Game.setPlayersRecord(players)
		addEvent(Game.broadcastMessage, 150, "New record: " .. players .. " players are logged in.", MESSAGE_STATUS_DEFAULT)
	end
	return true
end

event:register()
