function onStepIn(cid, item, position, fromPosition)
	if item.actionid > 30020 and item.actionid < 30050 then
		local player = Player(cid)
		local town = Town(item.actionid - 30020)
		if player and town then
			player:setTown(town)
		end
	end

	return true
end
