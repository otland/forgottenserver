function onCastSpell(creature, variant)
	local position = creature:getPosition()
	position:sendMagicEffect(CONST_ME_POFF)

	local tile = Tile(position)
	if not table.contains(ropeSpots, tile:getGround():getId()) and not tile:getItemById(14435) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return false
	end

	tile = Tile(position:moveUpstairs())
	if not tile then
		creature:sendCancelMessage(RETURNVALUE_NOTENOUGHROOM)
		return false
	end

	creature:teleportTo(position, false)
	position:sendMagicEffect(CONST_ME_TELEPORT)
	return true
end
