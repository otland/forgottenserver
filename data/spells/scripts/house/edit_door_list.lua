function onCastSpell(creature, variant)
	local cPosition = creature:getPosition()
	local house = Tile(cPosition):getHouse()
	cPosition:getNextPosition(creature:getDirection())
	local doorId = house and house:getDoorIdByPosition(cPosition)
	if not doorId or not house:canEditAccessList(doorId, creature) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:setEditHouse(house, doorId)
	creature:sendHouseWindow(house, doorId)
	return true
end
