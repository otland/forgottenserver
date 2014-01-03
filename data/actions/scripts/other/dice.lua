function onUse(cid, item, fromPosition, itemEx, toPosition)
	local c,p,v = Player(cid), getThingPos(item.uid), math.random(5792, 5797)
	c:say(c:getName() .. ' rolled a ' .. v - 5791 .. '.', TALKTYPE_ORANGE_1, c:isInGhostMode(),0,p)
	doTransformItem(item.uid, v)
	return c:isInGhostMode() and Position(p):sendMagicEffect(CONST_ME_CRAPS, cid) or Position(p):sendMagicEffect(CONST_ME_CRAPS)
end
