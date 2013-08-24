local ArrayRopeSpot = {384, 418, 8278, 8592, 13189, 14435, 14436, 15635, 19518}

function onCastSpell(cid, var)
	local pos = getPlayerPosition(cid)
	pos.stackpos = 0
	local posStack = getPlayerPosition(cid)
	posStack.stackpos = 1
	
	local ground = getThingFromPos(pos)	
	local groundStack = getThingFromPos(posStack)	
	if(isInArray(ArrayRopeSpot, ground.itemid) or isInArray(ArrayRopeSpot, groundStack.itemid)) then
		local newPos = {x = pos.x, y = pos.y + 1, z = pos.z - 1}
		
		doTeleportThing(cid, newPos, false)
		doSendMagicEffect(newPos, CONST_ME_TELEPORT)
		return true
	else
		doPlayerSendDefaultCancel(cid, RETURNVALUE_NOTPOSSIBLE)
		doSendMagicEffect(pos, CONST_ME_POFF)
		return false		
	end
	return false
end