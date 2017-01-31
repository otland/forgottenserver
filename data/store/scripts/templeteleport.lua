function onRender(player, offer)
	return not player:isPzLocked() and not Tile(player:getPosition()):hasFlag(TILESTATE_NOLOGOUT)
end

local MIN_DISTANCE = 0
function onBuy(player, offer)
	if player:isPzLocked() then
		player:sendStoreError(STORE_ERROR_PURCHASE, "You can not purchase a transportation service while PZ Locked.")
		return false
	end

	if Tile(player:getPosition()):hasFlag(TILESTATE_NOLOGOUT) then
		player:sendStoreError(STORE_ERROR_PURCHASE, "You can not purchase a transportation service while in a no-logout zone.")
		return false
	end

	local templePosition = player:getTown():getTemplePosition()
	if player:getPosition():getDistance(templePosition) < MIN_DISTANCE then
		player:sendStoreError(STORE_ERROR_PURCHASE, "You can not purchase this transportation service this close to your temple.")
		return false
	end

	player:teleportTo(templePosition)
	return true
end
