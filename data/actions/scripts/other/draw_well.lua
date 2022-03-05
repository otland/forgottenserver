function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item:getActionId() == actionIds.drawWell then
		fromPosition.z = fromPosition.z + 1
		player:teleportTo(fromPosition, false)
		return true
	end
end
