function onStepIn(cid, item, position, fromPosition)
	local player = Player(cid)
	if not player then
		return false
	end

	if item.actionid > 30020 and item.actionid < 30050 then
		local town = Town(item.actionid - 30020)
		player:setTown(town)
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You are the newest resident of " .. town:getName() .. ".")
		player:setStorageValue(item.actionid, 1)
	end

	return true
end
