function onUse(cid, item, fromPosition, itemEx, toPosition)
	if itemEx.itemid == 2782 then
		local iEx = Item(itemEx.uid)
		iEx:transform(2781)
		iEx:decay()
		return true
	end

	return destroyItem(cid, itemEx, toPosition)
end
