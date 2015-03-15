function onUse(player, item, fromPosition, itemEx, toPosition, isHotkey)
	if isInArray(questDoors, item.itemid) then
		if player:getStorageValue(item.actionid) ~= -1 then
			Item(item.uid):transform(item.itemid + 1)
			player:teleportTo(toPosition, true)
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "The door seems to be sealed against unwanted intruders.")
		end
		return true
	elseif isInArray(levelDoors, item.itemid) then
		if item.actionid > 0 and player:getLevel() >= item.actionid - 1000 then
			Item(item.uid):transform(item.itemid + 1)
			player:teleportTo(toPosition, true)
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "Only the worthy may pass.")
		end
		return true
	elseif isInArray(keys, item.itemid) then
		if itemEx.actionid > 0 then
			if item.actionid == itemEx.actionid then
				if doors[itemEx.itemid] then
					Item(itemEx.uid):transform(doors[itemEx.itemid])
					return true
				end
			end
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "The key does not match.")
			return true
		end
		return false
	end

	if isInArray(horizontalOpenDoors, item.itemid) or isInArray(verticalOpenDoors, item.itemid) then
		local doorCreature = tileToPos:getTopCreature()
		if doorCreature ~= nil then
			toPosition.x = toPosition.x + 1
			local query = toPosition:getTile():queryAdd(doorCreature, 20)
			if query ~= RETURNVALUE_NOERROR then
				toPosition.x = toPosition.x - 1
				toPosition.y = toPosition.y + 1
				query = toPosition:getTile():queryAdd(doorCreature, 20)
			end

			if query ~= RETURNVALUE_NOERROR then
				player:sendTextMessage(MESSAGE_STATUS_SMALL, query)
				return true
			end

			doorCreature:teleportTo(toPosition, true)
		end
		if not isInArray(openSpecialDoors, item.itemid) then
			Item(item.uid):transform(item.itemid - 1)
		end
		return true
	end

	if doors[item.itemid] then
		if item.actionid == 0 then
			Item(item.uid):transform(doors[item.itemid])
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "It is locked.")
		end
		return true
	end
	return false
end
