function onUse(cid, item, fromPosition, itemEx, toPosition)
	if item.itemid == 2148 and item.type == 100 then -- gold coin
		Item(item.uid):remove()
		Player(cid):addItem(2152, 1) -- platinum coin
	elseif item.itemid == 2152 and item.type == 100 then -- platinum coin
		Item(item.uid):remove()
		Player(cid):addItem(2160, 1) -- crystal coin
	elseif item.itemid == 2152 and item.type < 100 then -- platinum coin
		Item(item.uid):transform(item.itemid, item.type - 1)
		Player(cid):addItem(2148, 100) -- gold coin
	elseif item.itemid == 2160 then  -- crystal coin
		Item(item.uid):transform(item.itemid, item.type - 1)
		Player(cid):addItem(2152, 100) -- platinum coin
	else
		return false
	end

	return true
end
