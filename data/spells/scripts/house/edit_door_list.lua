function onCastSpell(creature, variant)
	local house = creature:getTile():getHouse()
	if not house then
		return false
	end

	local doorId = house:getDoorIdByPosition(variant:getPosition())
	if not doorId or not house:canEditAccessList(doorId, creature) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:setEditHouse(house, doorId)
	creature:sendHouseWindow(house, doorId)
	return true
end
