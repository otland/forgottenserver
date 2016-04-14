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
	player:teleportTo(fromPosition, false)
	return true
end
