local holeId = {294, 369, 370, 383, 392, 408, 409, 427, 428, 430, 462, 469, 470, 482, 484, 485, 489, 924, 3135, 3136}
function onUse(cid, item, fromPosition, itemEx, toPosition)
	if toPosition.x == CONTAINER_POSITION or toPosition.x == 0 and toPosition.y == 0 and toPosition.z == 0 then
		return FALSE
	end

	local groundTile = getThingfromPos(toPosition)
	if groundTile.itemid == 384 or groundTile.itemid == 418 or groundTile.itemid == 8278 then
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
