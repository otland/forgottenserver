local holeId = {294, 369, 370, 383, 392, 408, 409, 410, 427, 428, 430, 462, 469, 470, 482, 484, 485, 489, 924, 3135, 3136, 7933, 7938, 8170, 8286, 8285, 8284, 8281, 8280, 8279, 8277, 8276, 8323, 8567, 8585, 8596, 8595, 8249, 8250, 8251, 8252, 8253, 8254, 8255, 8256, 8972, 9606, 9625, 13190, 14461, 19519, 21536}

function onUse(cid, item, fromPosition, itemEx, toPosition)
	if toPosition.x == CONTAINER_POSITION or (toPosition.x == 0 and toPosition.y == 0 and toPosition.z == 0) then
		return false
	end

	local groundTile = getThingfromPos(toPosition)
	if isInArray(ropeSpots, groundTile.itemid) then
		doTeleportThing(cid, {x = toPosition.x, y = toPosition.y + 1, z = toPosition.z - 1}, FALSE)
		return true
	elseif isInArray(holeId, itemEx.itemid) then
		local thing = getThingfromPos({x = toPosition.x, y = toPosition.y, z = toPosition.z + 1, stackpos = STACKPOS_TOP_MOVEABLE_ITEM_OR_CREATURE})
		if thing.uid > 0 then
			if isCreature(thing.uid) and thing.type ~= THING_TYPE_PLAYER then
				return false
			end

			toPosition.y = toPosition.y + 1
			doTeleportThing(thing.uid, toPosition)
			return true
		end
	end
	return false
end
