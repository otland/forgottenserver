function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end
	
	local position = player:getPosition()
	position:getNextPosition(player:getDirection())

	local tile = Tile(position)
	if not tile then
		player:sendCancelMessage("Object not found.")
		return false
	end

	local thing = tile:getTopVisibleThing(player)
	if not thing then
		player:sendCancelMessage("Thing not found.")
		return false
	end

	if thing:isItem() then
		if thing == tile:getGround() then
			player:sendCancelMessage("There is nothing here to roll.")
			return false
		end
		local roll = param
		if roll == "" then
			roll = true
		end
		rollRarity(thing, roll)
		position:sendMagicEffect(73)
	end
	return true
end
