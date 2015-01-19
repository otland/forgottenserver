function onLogout(player)
	local protection = loginProtectionTable[player:getGuid()]
	if protection then
		loginProtectionTable[player:getGuid()] = nil
	end
	return true
end
