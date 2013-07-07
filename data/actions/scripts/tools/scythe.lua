function onUse(cid, item, fromPosition, itemEx, toPosition)
	if itemEx.itemid == 2739 then
		doTransformItem(itemEx.uid, 2737)
		doCreateItem(2694, 1, toPosition)
		doDecayItem(itemEx.uid)
		return TRUE
	end
	return destroyItem(cid, itemEx, toPosition)
end