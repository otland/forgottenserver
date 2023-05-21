local cleanTrackedQuests = CreatureEvent("cleanTrackedQuests")

function cleanTrackedQuests.onLogout(player)
	Game.getTrackedQuests()[player:getId()] = nil
	return true
end

cleanTrackedQuests:register()
