function onLogout(player)
	local playerId = player:getId()
	if nextUseStaminaTime[playerId] then
		nextUseStaminaTime[playerId] = nil
	end

	player:setLastPing(nil)
	player:setLastPong(nil)

	return true
end
