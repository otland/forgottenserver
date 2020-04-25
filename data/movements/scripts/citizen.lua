function onStepIn(creature, item, position, fromPosition)
	if item.actionid > aId.citizenship and item.actionid < aId.citizenshipLast then
		if not creature:isPlayer() then
			return false
		end

		creature:setTown(Town(item.actionid - aId.citizenship))
	end
	return true
end
