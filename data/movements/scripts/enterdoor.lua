function onStepIn(creature, item, position, fromPosition)
	if player:getLevel() < item.actionid - 1000 then
		creature:teleportTo(fromPosition, true)
		return false
	end

	return true
end
