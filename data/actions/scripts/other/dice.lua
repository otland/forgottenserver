function onUse(cid, item, fromPosition, itemEx, toPosition)
	local player = Player(cid)
	local pos = Position(getThingPos(item.uid))
	local value = math.random(5792, 5797)
	
	player:say(player:getName() .. ' rolled a ' .. value - 5791 .. '.', TALKTYPE_ORANGE_1, player:isInGhostMode(),0,pos)
	doTransformItem(item.uid, value)
	return player:isInGhostMode() and pos:sendMagicEffect(CONST_ME_CRAPS, cid) or pos:sendMagicEffect(CONST_ME_CRAPS)
end
