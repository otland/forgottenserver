function onStepIn(creature, item, position, fromPosition)
	if item.actionid > actionIds.citizenship and item.actionid < actionIds.citizenshipLast then
		if not creature:isPlayer() then
			return false
		end

		creature:setTown(Town(item.actionid - actionIds.citizenship))
	end
	return true
end
