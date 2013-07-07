local function doRemoveField(cid, pos)
	pos.stackpos = 254
	local field = getThingfromPos(pos)
	local playerPos = getPlayerPosition(cid)

	if(field.uid > 0 and isInArray(FIELDS, field.itemid) == TRUE) then
		doRemoveItem(field.uid)
		doSendMagicEffect(pos, CONST_ME_POFF)
		return LUA_NO_ERROR
	end

	doPlayerSendDefaultCancel(cid, RETURNVALUE_NOTPOSSIBLE)
	doSendMagicEffect(playerPos, CONST_ME_POFF)
	return LUA_ERROR
end

function onCastSpell(cid, var)
	local pos = variantToPosition(var)
	if(pos.x ~= 0 and pos.y ~= 0 and pos.z ~= 0) then
		return doRemoveField(cid, pos)
	end

	doPlayerSendDefaultCancel(cid, RETURNVALUE_NOTPOSSIBLE)
	doSendMagicEffect(getPlayerPosition(cid), CONST_ME_POFF)
	return LUA_ERROR
end