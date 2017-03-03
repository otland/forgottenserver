local function levitate(creature, parameter)
	local fromPosition = creature:getPosition()

	if parameter == "up" and fromPosition.z ~= 8 or parameter == "down" and fromPosition.z ~= 7 then
		local toPosition = creature:getPosition()
		toPosition:getNextPosition(creature:getDirection())

		local flag = parameter == "up" and TILESTATE_IMMOVABLEBLOCKSOLID or TILESTATE_BLOCKSOLID
		local floorChange = parameter == "up" and -1 or 1
		local tile = Tile(parameter == "up" and Position(fromPosition.x, fromPosition.y, fromPosition.z + floorChange) or toPosition)
		if not tile or not tile:getGround() and not tile:hasFlag(flag) then
			tile = Tile(toPosition.x, toPosition.y, toPosition.z + floorChange)

			if tile and tile:getGround() and not tile:hasFlag(bit.bor(TILESTATE_IMMOVABLEBLOCKSOLID, TILESTATE_FLOORCHANGE)) then
				return creature:move(tile, bit.bor(FLAG_IGNOREBLOCKITEM, FLAG_IGNOREBLOCKCREATURE))
			end
		end
	end
	return RETURNVALUE_NOTPOSSIBLE
end

function onCastSpell(creature, variant)
	local returnValue = levitate(creature, variant:getString())
	if returnValue ~= RETURNVALUE_NOERROR then
		creature:sendCancelMessage(returnValue)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
	return true
end
