function onCastSpell(creature, variant)
	local currentPosition = creature:getPosition()
	local destinationPosition = creature:getPosition()
	destinationPosition:getNextPosition(creature:getDirection())

	local returnValue = RETURNVALUE_NOTPOSSIBLE
	local parameter = variant:getString():lower()
	if parameter == "up" and currentPosition.z ~= 8 then
		local tile = Tile(currentPosition.x, currentPosition.y, currentPosition.z - 1)
		if not tile or not tile:getGround() and not tile:hasFlag(TILESTATE_IMMOVABLEBLOCKSOLID) then
			tile = Tile(destinationPosition.x, destinationPosition.y, destinationPosition.z - 1)

			if tile and tile:getGround() and not tile:hasFlag(bit.bor(TILESTATE_IMMOVABLEBLOCKSOLID, TILESTATE_FLOORCHANGE)) then
				returnValue = creature:move(nil, tile, bit.bor(FLAG_IGNOREBLOCKITEM, FLAG_IGNOREBLOCKCREATURE))
			end
		end
	elseif parameter == "down" and currentPosition.z ~= 7 then
		local tile = Tile(destinationPosition)
		if not tile or not tile:getGround() and not tile:hasFlag(TILESTATE_BLOCKSOLID) then
			tile = Tile(destinationPosition.x, destinationPosition.y, destinationPosition.z + 1)

			if tile and tile:getGround() and not tile:hasFlag(bit.bor(TILESTATE_IMMOVABLEBLOCKSOLID, TILESTATE_FLOORCHANGE)) then
				returnValue = creature:move(nil, tile, bit.bor(FLAG_IGNOREBLOCKITEM, FLAG_IGNOREBLOCKCREATURE))
			end
		end
	end

	if returnValue ~= RETURNVALUE_NOERROR then
		creature:sendCancelMessage(returnValue)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
	return true
end
