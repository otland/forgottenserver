function onSay(cid, words, param)
	local player = Player(cid)
	if not player:getGroup():getAccess() then
		return true
	end

	local town = Town(param)
	if town == nil then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "Town not found.")
		return false
	end

	player:teleportTo(town:getTemplePosition())
	return false
end
