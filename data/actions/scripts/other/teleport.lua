local upFloorIds = {1386, 3678, 5543}

function onUse(player, item, fromPosition, itemEx, toPosition, isHotkey)
	if not isInArray(upFloorIds, item.itemid) then
		fromPosition.z = fromPosition.z + 1
		player:teleportTo(fromPosition, true)
		return true
	end
	
	local posA = {x = fromPosition.x, y = fromPosition.y + 1, z = fromPosition.z - 1}
	local posB = {x = fromPosition.x, y = fromPosition.y - 1, z = fromPosition.z - 1}
	local tileA = Tile(posA)
	local tileB = Tile(posB)
	local newDir = SOUTH
	
	if tileA then
		if tileA:hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) and not tileA:getItemByType(ITEM_TYPE_MAGICFIELD) then
			if tileB then
				if tileB:hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) and not tileB:getItemByType(ITEM_TYPE_MAGICFIELD) then
					return false
				end

				newDir = NORTH
			else
				return false
			end
		end
	else
		newDir = NORTH
	end
	
	if newDir == SOUTH then
		player:teleportTo(posA, true)
	else
		player:teleportTo(posB, true)
	end
	
	player:setDirection(newDir)
	return true
end
