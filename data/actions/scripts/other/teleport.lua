local upFloorIds = {1386, 3678, 5543}
function onUse(player, item, fromPosition, itemEx, toPosition, isHotkey)
	if isInArray(upFloorIds, item.itemid) then
		fromPosition.y = fromPosition.y + 1
		fromPosition.z = fromPosition.z - 1
		newDir = SOUTH
		local tile = Tile(fromPosition)
		if tile then
			if not tile:isWalkable() then
				fromPosition.y = fromPosition.y - 2
				newDir = NORTH
			end
		else
			fromPosition.y = fromPosition.y - 2
			newDir = NORTH
		end
	else
		fromPosition.z = fromPosition.z + 1
	end
	
	player:teleportTo(fromPosition, true)
	if newDir then
		player:setDirection(newDir)
	end
	return true
end
