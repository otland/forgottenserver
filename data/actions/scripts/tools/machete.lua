function onUse(cid, item, fromPosition, itemEx, toPosition)
	if itemEx.itemid == 2782 then
		doTransformItem(itemEx.uid, 2781)
		doDecayItem(itemEx.uid)
		return TRUE
	end
	return destroyItem(cid, itemEx, toPosition)
end