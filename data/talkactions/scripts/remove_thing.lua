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

	if thing:isCreature() then
		thing:remove()
	elseif thing:isItem() then
		if thing == tile:getGround() then
			player:sendCancelMessage("You may not remove a ground tile.")
			return false
		end
		thing:remove(tonumber(param) or -1)
	end

	position:sendMagicEffect(CONST_ME_MAGIC_RED)
	return false
end
