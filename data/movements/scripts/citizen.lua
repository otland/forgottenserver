function onStepIn(cid, item, position, fromPosition)
	if item.actionid > 30020 and item.actionid < 30050 then
		local player = Player(cid)
		if not player then
			return false
		end
		local town = Town(item.actionid - 30020)
		player:setTown(town)
	end

	return true
end
