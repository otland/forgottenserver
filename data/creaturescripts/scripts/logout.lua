function onLogout(player)
	
	local protection = loginProtectionTable[player:getId()]
	if protection then
		protection = nil
	end
	return true
end
