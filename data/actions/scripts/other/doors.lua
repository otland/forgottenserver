function onUse(cid, item, fromPosition, itemEx, toPosition)
	if isInArray(questDoors, item.itemid) == TRUE then
		if getPlayerStorageValue(cid, item.actionid) ~= -1 then
			doTransformItem(item.uid, item.itemid + 1)
			doTeleportThing(cid, toPosition, TRUE)
		else
			doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, "The door seems to be sealed against unwanted intruders.")
		end
		return TRUE
	elseif isInArray(levelDoors, item.itemid) == TRUE then
		if item.actionid > 0 and getPlayerLevel(cid) >= item.actionid - 1000 then
			doTransformItem(item.uid, item.itemid + 1)
			doTeleportThing(cid, toPosition, TRUE)
		else
			doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, "Only the worthy may pass.")
		end
		return TRUE
	elseif isInArray(keys, item.itemid) == TRUE then
		if itemEx.actionid > 0 then
			if item.actionid == itemEx.actionid then
				if doors[itemEx.itemid] ~= nil then
					doTransformItem(itemEx.uid, doors[itemEx.itemid])
					return TRUE
				end
			end
			doPlayerSendCancel(cid, "The key does not match.")
			return TRUE
		end
		return FALSE
	elseif isInArray(horizontalOpenDoors, item.itemid) == TRUE then
		local newPosition = toPosition
		newPosition.y = newPosition.y + 1
		local doorPosition = fromPosition
		doorPosition.stackpos = STACKPOS_TOP_MOVEABLE_ITEM_OR_CREATURE
		local doorCreature = getThingfromPos(doorPosition)
		if doorCreature.itemid ~= 0 then
			if getTilePzInfo(doorPosition) == TRUE and getTilePzInfo(newPosition) == FALSE and doorCreature.uid ~= cid then
				doPlayerSendCancel(cid, "Sorry, not possible.")
			else
				doTeleportThing(doorCreature.uid, newPosition, TRUE)
				if isInArray(openSpecialDoors, item.itemid) ~= TRUE then
					doTransformItem(item.uid, item.itemid - 1)
				end
			end
			return TRUE
		end
		doTransformItem(item.uid, item.itemid - 1)
		return TRUE
	elseif isInArray(verticalOpenDoors, item.itemid) == TRUE then
		local newPosition = toPosition
		newPosition.x = newPosition.x + 1
		local doorPosition = fromPosition
		doorPosition.stackpos = STACKPOS_TOP_MOVEABLE_ITEM_OR_CREATURE
		local doorCreature = getThingfromPos(doorPosition)
		if doorCreature.itemid ~= 0 then
			if getTilePzInfo(doorPosition) == TRUE and getTilePzInfo(newPosition) == FALSE and doorCreature.uid ~= cid then
				doPlayerSendCancel(cid, "Sorry, not possible.")
			else
				doTeleportThing(doorCreature.uid, newPosition, TRUE)
				if isInArray(openSpecialDoors, item.itemid) ~= TRUE then
					doTransformItem(item.uid, item.itemid - 1)
				end
			end
			return TRUE
		end
		doTransformItem(item.uid, item.itemid - 1)
		return TRUE
	elseif doors[item.itemid] ~= nil then
		if item.actionid == 0 then
			doTransformItem(item.uid, doors[item.itemid])
		else
			doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, "It is locked.")
		end
		return TRUE
	end
	return FALSE
end