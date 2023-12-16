local event = CreatureEvent("DebugAssert")

function event.onLogout(player)
	local playerGuid = player:getGuid()
	Game.removeDebugAssert(playerGuid)
	return true
end

event:register()
