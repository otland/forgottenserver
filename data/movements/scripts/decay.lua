function onStepIn(creature, item, toPosition, fromPosition)
	item:transform(item.itemid + 1)
	item:decay()
	return true
end
