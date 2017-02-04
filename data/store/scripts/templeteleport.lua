local MIN_DISTANCE = 0

function onRender(player, offer)
	if player:isPzLocked() then
		return false, "You can not purchase a transportation service while PZ Locked."
	end

	if Tile(player:getPosition()):hasFlag(TILESTATE_NOLOGOUT) then
		return false, "You can not purchase a transportation service while in a no-logout zone."
	end

	local templePosition = player:getTown():getTemplePosition()
	if player:getPosition():getDistance(templePosition) < MIN_DISTANCE then
		return false, "You can not purchase this transportation service this close to your temple."
	end

	return true
end

function onBuy(player, offer)
	player:teleportTo(templePosition)
	return true
end
