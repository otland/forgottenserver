function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local town = Town(param) or Town(tonumber(param))
	if town then
		player:teleportTo(town:getTemplePosition(), true)
		player:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
	else
		player:sendCancelMessage("Town not found.")
	end
	return false
end
