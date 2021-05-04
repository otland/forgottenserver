local upFloorIds = {
	1386, -- ladder
	3678, -- ladder
	5543, -- rope-ladder
	22845, -- ladder rung
	22846 -- ladder rung
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if table.contains(upFloorIds, item:getId()) then
		fromPosition:moveUpstairs()
	else
		fromPosition.z = fromPosition.z + 1
	end

	if player:isPzLocked() and Tile(fromPosition):hasFlag(TILESTATE_PROTECTIONZONE) then
		player:sendCancelMessage(RETURNVALUE_PLAYERISPZLOCKED)
		return true
	end

	player:teleportTo(fromPosition, false)
	return true
end
