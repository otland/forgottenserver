function onSay(cid, words, param)
	local player = Player(cid)
	if not player:getGroup():getAccess() then
		return true
	end

	local target = Creature(param)
	if target == nil then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "Creature not found.")
		return false
	end

	player:teleportTo(target:getPosition())
	return false
end
