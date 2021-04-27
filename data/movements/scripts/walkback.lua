function onStepIn(creature, item, position, fromPosition)
	if item:getUniqueId() > 0 and item:getUniqueId() <= 65535 then
		creature:teleportTo(fromPosition, false)
	end
	return true
end
