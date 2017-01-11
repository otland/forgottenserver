function onCastSpell(player, variant)
	local house = player:getTile():getHouse()
	if not house then
		return false
	end

	local doorPos = player:getPosition():getNextPosition(player:getDirection())
	local doorId = house:getDoorIdByPosition(doorPos)
	if doorId ~= nil and house:canEditAccessList(doorId, player) then
		player:setEditHouse(house, doorId)
		player:sendHouseWindow(house, doorId)
	else
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return true
end
