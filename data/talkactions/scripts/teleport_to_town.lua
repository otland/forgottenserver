function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local town = Town(param)
	if not town then
		town = Town(tonumber(param))
	end

	if not town then
		player:sendCancelMessage("Town not found.")
		return false
	end

	player:teleportTo(town:getTemplePosition())
	return false
end
