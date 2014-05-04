function onUse(cid, item, fromPosition, itemEx, toPosition)
	if item.itemid == 19510 then
	local player = Player(cid)
		player:teleportTo({x = toPosition.x - 1, y = toPosition.y, z = toPosition.z - 1}, false)
		return true
	end
	return true
end
