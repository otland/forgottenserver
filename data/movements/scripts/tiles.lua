local increasing = {
	[416] = 417, -- stone tile
	[426] = 425, -- stone tile
	[446] = 447, -- wooden floor
	[3216] = 3217, -- wooden floor
	[3202] = 3215, -- stone floor
	[11062] = 11063 -- glowing switch
}

local decreasing = {
	[417] = 416, -- stone tile
	[425] = 426, -- stone tile
	[447] = 446, -- wooden floor
	[3217] = 3216, -- wooden floor
	[3215] = 3202, -- stone floor
	[11063] = 11062 -- glowing switch
}

function onStepIn(creature, item, position, fromPosition)
	if not increasing[item:getId()] then
		return true
	end

	if not creature:isPlayer() or creature:isInGhostMode() then
		return true
	end

	item:transform(increasing[item:getId()])

	if item:getActionId() >= actionIds.levelDoor then
		if creature:getLevel() < item:getActionId() - actionIds.levelDoor then
			creature:teleportTo(fromPosition, false)
			position:sendMagicEffect(CONST_ME_MAGIC_BLUE)
			creature:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The tile seems to be protected against unwanted intruders.")
		end
		return true
	end

	if Tile(position):hasFlag(TILESTATE_PROTECTIONZONE) then
		local lookPosition = creature:getPosition()
		lookPosition:getNextPosition(creature:getDirection())
		local depotItem = Tile(lookPosition):getItemByType(ITEM_TYPE_DEPOT)
		if depotItem then
			local depotItems = creature:getDepotChest(getDepotId(depotItem:getUniqueId()), true):getItemHoldingCount()
			creature:sendTextMessage(MESSAGE_STATUS_DEFAULT, "Your depot contains " .. depotItems .. " item" .. (depotItems > 1 and "s." or "."))
			creature:addAchievementProgress("Safely Stored Away", 1000)
			return true
		end
	end

	if item:getActionId() ~= 0 and creature:getStorageValue(item:getActionId()) <= 0 then
		creature:teleportTo(fromPosition, false)
		position:sendMagicEffect(CONST_ME_MAGIC_BLUE)
		creature:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The tile seems to be protected against unwanted intruders.")
	end
	return true
end

function onStepOut(creature, item, position, fromPosition)
	if not decreasing[item:getId()] then
		return true
	end

	if creature:isPlayer() and creature:isInGhostMode() then
		return true
	end

	item:transform(decreasing[item:getId()])
	return true
end
