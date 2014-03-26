function onStepIn(cid, item, position, fromPosition)

	local it = Item(item.uid)
	it:transform(item.itemid + 1)
	it:decay()

	return true
end
