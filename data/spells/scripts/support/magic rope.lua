local ArrayRopeSpot = {384, 418, 8278}

function onCastSpell(cid, var)
	local pos = getPlayerPosition(cid)
	pos.stackpos = 0
	local grounditem = getThingfromPos(pos)

	if(isInArray(ArrayRopeSpot, grounditem.itemid) == TRUE) then
		local newpos = pos
		newpos.y = newpos.y + 1
		newpos.z = newpos.z - 1
		doTeleportThing(cid, newpos, 0)
		doSendMagicEffect(pos, CONST_ME_TELEPORT)
		return LUA_NO_ERROR
	else
		doPlayerSendDefaultCancel(cid, RETURNVALUE_NOTPOSSIBLE)
		doSendMagicEffect(pos, CONST_ME_POFF)
		return LUA_ERROR
	end
end