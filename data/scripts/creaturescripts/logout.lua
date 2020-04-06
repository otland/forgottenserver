local logout = CreatureEvent("Player Logout")

function logout.onLogout(player)
	local playerId = player:getId()
	if nextUseStaminaTime[playerId] then
		nextUseStaminaTime[playerId] = nil
	end
	return true
end

logout:register()
