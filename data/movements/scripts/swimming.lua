local outfit =
	{
		lookType = 267,
		lookHead = 0,
		lookBody = 0,
		lookLegs = 0,
		lookFeet = 0,
		lookAddons = 0
	}

function onStepIn(cid, item, position, fromPosition)
	doSetCreatureOutfit(cid, outfit, -1)
	local npos = fromPosition
	local next_tile = {x = npos.x, y = npos.y, z = npos.z, stackpos = 0}
	if not(getThingfromPos(next_tile).itemid >= 4620 and getThingfromPos(next_tile).itemid <= 4625) then
		return doSendMagicEffect(position, 53)
	end
end

function onStepOut(cid, item, position, fromPosition)
	doRemoveCondition(cid, CONDITION_OUTFIT)
	local npos = getThingPos(cid)
	local next_tile = {x = npos.x, y = npos.y, z = npos.z, stackpos = 0}
	if not(getThingfromPos(next_tile).itemid >= 4620 and getThingfromPos(next_tile).itemid <= 4625) then
		return doSendMagicEffect(position, 53)
	end
end
