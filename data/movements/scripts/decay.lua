function onStepIn(cid, item, position, fromPosition)
	doTransformItem(item.uid, item.itemid + 1)
	doDecayItem(item.uid)
	return TRUE
end