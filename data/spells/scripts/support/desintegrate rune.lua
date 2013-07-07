local function doRemoveObject(cid, pos)
	pos.stackpos = 255
	local object = getThingfromPos(pos)

	if(object.uid > 65535 and isCreature(object.uid) == FALSE and isMovable(object.uid) == TRUE and object.actionid == 0 and getTilePzInfo(pos) == FALSE) then
		doRemoveItem(object.uid)
		doSendMagicEffect(pos, CONST_ME_BLOCKHIT)
		return LUA_NO_ERROR
	end

	doPlayerSendDefaultCancel(cid, RETURNVALUE_NOTPOSSIBLE)
	doSendMagicEffect(getPlayerPosition(cid), CONST_ME_POFF)
	return LUA_ERROR
end

function onCastSpell(cid, var)
	local pos = variantToPosition(var)
	if(pos.x ~= 0 and pos.y ~= 0 and pos.z ~= 0) then
		return doRemoveObject(cid, pos)
	end

	doPlayerSendDefaultCancel(cid, RETURNVALUE_NOTPOSSIBLE)
	doSendMagicEffect(getPlayerPosition(cid), CONST_ME_POFF)
	return LUA_ERROR
end