function onLogout(player)
	if nextUseStaminaTime[player:getGuid()] then
		nextUseStaminaTime[player:getGuid()] = nil
	end
	return true
end
