local teleportToTown = TalkAction("/town")

function teleportToTown.onSay(player, words, param)
	if(not player:getGroup():getAccess()) or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	local town = Town(param) or Town(tonumber(param))
	if town then
		player:teleportTo(town:getTemplePosition())
	else
		player:sendCancelMessage("Town not found.")
	end
	return false
end

teleportToTown:separator(" ")
teleportToTown:register()
