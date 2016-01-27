function onStepIn(creature, item, position, fromPosition)
	local player = creature:getPlayer()
	if player == nil then
		return false
	end

	local itemId = item:getId()
	if isInArray(questDoors, itemId) then
		if player:getStorageValue(item.actionid) == -1 then
			player:sendTextMessage(MESSAGE_INFO_DESCR, "The door seems to be sealed against unwanted intruders.")
			player:teleportTo(fromPosition, true)
			return false
		end
	elseif isInArray(levelDoors, itemId) then
		if player:getLevel() < item.actionid - 1000 then
			player:sendTextMessage(MESSAGE_INFO_DESCR, "Only the worthy may pass.")
			player:teleportTo(fromPosition, true)
			return false
		end
	end
	return true
end
