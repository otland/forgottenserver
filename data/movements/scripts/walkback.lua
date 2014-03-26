function onStepIn(cid, item, position, fromPosition)
	if item.uid > 0 and item.uid <= 65535 then
		local player = Player(cid)
		if player then
			player:teleportTo(fromPosition, false)
		end
	end

	return true
end
