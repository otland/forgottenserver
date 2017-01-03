function onStepIn(creature, item, position, fromPosition)
	if not creature:isPlayer() then
		return false
	end

	if creature:getStorageValue(item.actionid) == -1 then
		creature:sendTextMessage(MESSAGE_INFO_DESCR, "The door seems to be sealed against unwanted intruders.")
		return false
	end
	return true
end
