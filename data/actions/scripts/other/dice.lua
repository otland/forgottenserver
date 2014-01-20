function onUse(cid, item, fromPosition, itemEx, toPosition)
	local player = Player(cid)
	local dice = Item(item.uid)
	local dicePosition = dice:getPosition()
	local value = math.random(1, 6)

	local isInGhostMode = player:isInGhostMode()
	if isInGhostMode then
		dicePosition:sendMagicEffect(CONST_ME_CRAPS, player)
	else
		dicePosition:sendMagicEffect(CONST_ME_CRAPS)
	end

	player:say(player:getName() .. " rolled a " .. value .. ".", TALKTYPE_ORANGE_1, isInGhostMode, 0, dicePosition)
	dice:transform(5791 + value)
	return true
end
