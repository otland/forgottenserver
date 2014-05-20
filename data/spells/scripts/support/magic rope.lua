function onCastSpell(creature, var)
	local position = creature:getPosition()
	position:sendMagicEffect(CONST_ME_POFF)
	local tile = position:getTile()

	if isInArray(ropeSpots, tile:getGround():getId()) or tile:getItemById(14435) then
		position.z = position.z - 1
		position.y = position.y + 1
		tile = position:getTile()
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
