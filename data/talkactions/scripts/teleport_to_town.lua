function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local town = Town(param)
	if town == nil then
		player:sendCancelMessage("Town not found.")
		return false
	end

	player:teleportTo(town:getTemplePosition())
	return false
end
