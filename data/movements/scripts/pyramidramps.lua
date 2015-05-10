function onStepIn(creature, item, position, fromPosition)
	local player = creature:getPlayer()
	if not player then
		return true
	end
	
	local upStairs = Position(position['x'], position['y'], position['z']-1)
	local newPosition = player:getClosestFreePosition(upStairs, false)
	player:teleportTo(newPosition)

	return true
end
