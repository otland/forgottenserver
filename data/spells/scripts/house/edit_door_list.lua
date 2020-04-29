function onCastSpell(creature, variant)
	local creaturePos = creature:getPosition()
	creaturePos:getNextPosition(creature:getDirection())
	local tile = Tile(creaturePos)
	local house = tile and tile:getHouse()
	local doorId = house and house:getDoorIdByPosition(creaturePos)
	if not doorId or not house:canEditAccessList(doorId, creature) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:setEditHouse(house, doorId)
	creature:sendHouseWindow(house, doorId)
	return true
end
