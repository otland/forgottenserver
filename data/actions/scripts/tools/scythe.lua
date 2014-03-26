function onUse(cid, item, fromPosition, itemEx, toPosition)
	if itemEx.itemid == 2739 then
		local iEx = Item(itemEx.uid)
		iEx:transform(2737)
		iEx:decay()
		Game.createItem(2694, 1, toPosition)
		return true
	end

	return destroyItem(cid, itemEx, toPosition)
end
