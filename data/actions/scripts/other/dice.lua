function onUse(player, item, fromPosition, itemEx, toPosition, isHotkey)
	local dice = Item(item.uid)
	local dicePosition = dice:getPosition()
	local value = math.random(1, 6)
	local isInGhostMode = player:isInGhostMode()

	dicePosition:sendMagicEffect(CONST_ME_CRAPS, isInGhostMode and player)

	local spectators = Game.getSpectators(dicePosition, false, true, 3, 3)
	for _, pid in ipairs(spectators) do
		player:say(player:getName() .. " rolled a " .. value .. ".", TALKTYPE_MONSTER_SAY, isInGhostMode, pid, dicePosition)
	end

	dice:transform(5791 + value)
	return true
end
