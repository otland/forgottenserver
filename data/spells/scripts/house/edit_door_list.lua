local function getNextPosition(player)
	local position = player:getPosition()
	local direction = player:getDirection()

	-- a player can only be facing cardinal directions
	if direction == DIRECTION_NORTH then
		position.y = position.y - 1
	elseif direction == DIRECTION_SOUTH then
		position.y = position.y + 1
	elseif direction == DIRECTION_WEST then
		position.x = position.x - 1
	else
		position.x = position.x + 1
	end
	return position
end

function onCastSpell(player, variant)
	local house = player:getTile():getHouse()
	if not house then
		return false
	end

	local doorId = house:getDoorIdByPosition(getNextPosition(player))
	if doorId ~= nil and house:canEditAccessList(doorId, player) then
		player:setEditHouse(house, doorId)
		player:sendHouseWindow(house, doorId)
	else
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "Sorry, not possible.")
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return true
end
