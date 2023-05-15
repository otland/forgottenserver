function onStepIn(creature, item, toPosition, fromPosition)
	if not creature:isPlayer() or creature:isInGhostMode() then
		return true
	end

	item:transform(item.itemid + 1)
	item:decay()
	return true
end
