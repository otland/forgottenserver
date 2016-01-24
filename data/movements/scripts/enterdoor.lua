function onStepIn(creature, item, position, fromPosition)
	local player = creature:getPlayer()
	if player == nil then
		return false
	end

	if player:getLevel() < item.actionid - 1000 then
		player:teleportTo(fromPosition, true)
		return false
	end

	return true
end
