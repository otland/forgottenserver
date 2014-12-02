function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	if isInArray(questDoors, item:getId()) then
		if player:getStorageValue(item:getActionId()) ~= -1 then
			item:transform(item:getId() + 1)
			player:teleportTo(toPosition, true)
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "The door seems to be sealed against unwanted intruders.")
		end
		return true
	elseif isInArray(levelDoors, item:getId()) then
		if item:getActionId() > 0 and player:getLevel() >= item:getActionId() - 1000 then
			item:transform(item:getId() + 1)
			player:teleportTo(toPosition, true)
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "Only the worthy may pass.")
		end
		return true
	elseif isInArray(keys, item:getId()) then
		if targetEx:getActionId() > 0 then
			if item:getActionId() == targetEx:getActionId() then
				if doors[targetEx:getId()] then
					targetEx:transform(doors[targetEx:getId()])
					return true
				end
			end
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "The key does not match.")
			return true
		end
		return false
	end

	local tileToPos = toPosition:getTile()
	local thing = tileToPos:getThing(STACKPOS_TOP_MOVEABLE_ITEM_OR_CREATURE)
	if thing and item.uid ~= thing:getUniqueId() and fromPosition:getTile():getItemByType(ITEM_TYPE_MAGICFIELD) then
		return false
	end

	if isInArray(horizontalOpenDoors, item:getId()) or isInArray(verticalOpenDoors, item:getId()) then
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
		if not isInArray(openSpecialDoors, item:getId()) then
			item:transform(item:getId() - 1)
		end
		return true
	end

	if doors[item:getId()] then
		if item:getActionId() == 0 then
			item:transform(doors[item:getId()])
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "It is locked.")
		end
		return true
	end
	return false
end
