function onStepIn(creature, item, position, fromPosition)
	local player = creature:getPlayer()
	if player == nil then
		return false
	end

	if player:getLevel() < item.actionid - 1000 then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "Only the worthy may pass.")
		player:teleportTo(fromPosition, true)
		return false
	end
	return true
end
