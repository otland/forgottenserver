local outfit = {lookType = 267, lookHead = 0, lookBody = 0, lookLegs = 0, lookFeet = 0, lookTypeEx = 0, lookAddons = 0}

function onStepIn(creature, item, position, fromPosition)
	if not creature:isPlayer() then
		return false
	end

	doSetCreatureOutfit(creature, outfit, -1)
	return true
end

function onStepOut(creature, item, position, fromPosition)
	local player = creature:getPlayer()
	if player == nil then
		return false
	end

	player:removeCondition(CONDITION_OUTFIT)
	return true
end
