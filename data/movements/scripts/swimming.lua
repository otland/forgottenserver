local outfit = {lookType = 267, lookHead = 0, lookBody = 0, lookLegs = 0, lookFeet = 0, lookTypeEx = 0, lookAddons = 0}

function onStepIn(cid, item, position, fromPosition)
	local player = Player(cid)
	if not player then
		return false
	end

	doSetCreatureOutfit(cid, outfit, -1)
end

function onStepOut(cid, item, position, fromPosition)
	local player = Player(cid)
	if not player then
		return false
	end

	player:removeCondition(CONDITION_OUTFIT)
end
