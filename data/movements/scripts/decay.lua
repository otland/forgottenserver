function onStepIn(creature, item, position, fromPosition)
	item:transform(item:getId() + 1)
	item:decay()
	return true
end
