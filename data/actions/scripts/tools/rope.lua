ropeSpots = {384, 418, 8278, 8592, 13189, 14435, 14436, 15635, 19518}
holeId = {294, 369, 370, 383, 392, 408, 409, 427, 428, 430, 462, 469, 470, 482, 484, 485, 489, 924, 3135, 3136, 7933, 7938, 8170, 8286, 8285, 8284,
		  8281, 8280, 8279, 8277, 8276, 8323, 8380, 8567, 8585, 8596, 8595, 8249, 8250, 8251, 8252, 8253, 8254, 8255, 8256, 8972, 9606, 9625, 14461, 19519}

function onUse(cid, item, fromPosition, itemEx, toPosition)
	if toPosition.x == CONTAINER_POSITION or toPosition.x == 0 and toPosition.y == 0 and toPosition.z == 0 then
		return FALSE
	end

	local groundTile = getThingfromPos(toPosition)
	if (isInArray(ropeSpots, groundTile.itemid)) then
		doTeleportThing(cid, {x = toPosition.x, y = toPosition.y + 1, z = toPosition.z - 1}, FALSE)
	elseif isInArray(holeId, itemEx.itemid) == TRUE then
		local hole = getThingfromPos({x = toPosition.x, y = toPosition.y, z = toPosition.z + 1, stackpos = STACKPOS_TOP_MOVEABLE_ITEM_OR_CREATURE})
		if hole.itemid > 0 then
			doTeleportThing(hole.uid, {x = toPosition.x, y = toPosition.y + 1, z = toPosition.z}, FALSE)
		else
			doPlayerSendCancel(cid, "Sorry, not possible.")
		end
	else
		return FALSE
	end
	return TRUE
end