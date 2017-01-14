local upFloorIds = {1386, 3678, 5543}
function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local tile = item:getTile()
	if tile and tile:hasFlag(TILESTATE_PROTECTIONZONE) and player:isPzLocked() then
		return false
	end

	if isInArray(upFloorIds, item.itemid) then
		fromPosition:moveUpstairs()
	else
		fromPosition.z = fromPosition.z + 1
	end
	if player:isPzLocked() and Tile(fromPosition):hasFlag(TILESTATE_PROTECTIONZONE) then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_PLAYERISPZLOCKED))
		return true
	end
	player:teleportTo(fromPosition, false)
	return true
end
