function onCastSpell(cid, var)
	local pos = getPlayerPosition(cid)
	pos.stackpos = STACKPOS_GROUND
	local thing = getThingfromPos(pos)
	if not isInArray(ropeSpots, thing.itemid) then
		pos.stackpos = STACKPOS_FIRST_ITEM_ABOVE_GROUNDTILE
		thing = getThingfromPos(pos)
		if not isInArray(ropeSpots, thing.itemid) then
			doPlayerSendDefaultCancel(cid, RETURNVALUE_NOTPOSSIBLE)
			doSendMagicEffect(pos, CONST_ME_POFF)
			return false
		end
	end

	local destination = {x = pos.x, y = pos.y + 1, z = pos.z - 1}
	doTeleportThing(cid, destination, false)
	doSendMagicEffect(destination, CONST_ME_TELEPORT)
	return true
end
