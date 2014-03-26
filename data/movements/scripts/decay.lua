function onStepIn(cid, item, position, fromPosition)
	local decayItem = Item(item.uid)
	decayItem:transform(item.itemid + 1)
	decayItem:decay()

	return true
end
