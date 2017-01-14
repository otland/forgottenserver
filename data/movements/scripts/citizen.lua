function onStepIn(creature, item, position, fromPosition)
	if item.actionid > 30020 and item.actionid < 30050 then
		local player = creature:getPlayer()
		if not player then
			return false
		end

		player:setTown(Town(item.actionid - 30020))
	end
	return true
end
