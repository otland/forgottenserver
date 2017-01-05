function onStepIn(creature, item, position, fromPosition)
	return item.uid == 0 or item.uid > 65535
end
