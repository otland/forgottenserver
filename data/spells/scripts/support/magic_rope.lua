function onCastSpell(creature, variant)
	local position = creature:getPosition()
	position:sendMagicEffect(CONST_ME_POFF)

	local tile = Tile(position)
	if isInArray(ropeSpots, tile:getGround():getId()) or tile:getItemById(14435) then
		tile = Tile(position:moveUpstairs())
		if tile then
			creature:teleportTo(position, false)
			position:sendMagicEffect(CONST_ME_TELEPORT)
		else
			creature:sendCancelMessage(RETURNVALUE_NOTENOUGHROOM)
		end
	else
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
	end
	return true
end
