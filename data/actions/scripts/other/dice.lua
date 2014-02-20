function onUse(cid, item, fromPosition, itemEx, toPosition)
	local player = Player(cid)
	local dice = Item(item.uid)
	local dicePosition = dice:getPosition()
	local value = math.random(1, 6)
	local playerMode = player:isInGhostMode()

	dicePosition:sendMagicEffect(CONST_ME_CRAPS, playerMode and player)

	local spectators = Game.getSpectators(dicePosition, false, true, 3, 3)
	for _, pid in ipairs(spectators) do
		player:say(player:getName() .. " rolled a " .. value .. ".", TALKTYPE_ORANGE_1, playerMode, pid, dicePosition)
	end

	dice:transform(5791 + value)
	return true
end
