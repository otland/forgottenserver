function onCastSpell(creature, var)
	local pos = creature:getPosition()
	pos.stackpos = STACKPOS_GROUND
	local tile = Tile(pos)
	if tile then
		local thing = tile:getThing()
		if thing and thing:isItem() and not isInArray(ropeSpots, thing:getType():getId()) then
			creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end
	end

	local destination = {x = pos.x, y = pos.y + 1, z = pos.z - 1}
	creature:teleportTo(destination, false)
	creature:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
	return true
end
