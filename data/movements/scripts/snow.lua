function onStepOut(cid, item, position, fromPosition)
	local player = Player(cid)
	if player and player:isInGhostMode() then
		return true
	end
	local snow = Item(item.uid)
	if item.itemid == 670 then
		snow:transform(6594)
	else
		snow:transform(item.itemid + 15)
	end
	snow:decay()

	return true
end
