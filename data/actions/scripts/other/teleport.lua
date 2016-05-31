local upFloorIds = {1386, 3678, 5543}
function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if isInArray(upFloorIds, item.itemid) then
		fromPosition:moveUpstairs()
	else
		fromPosition.z = fromPosition.z + 1
	end
	local t = Tile(fromPosition)
	if t:hasFlag(TILESTATE_PROTECTIONZONE) and player:isPzLocked() then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_PLAYERISPZLOCKED))
		return true
	end
	player:teleportTo(fromPosition, false)
	return true
end
