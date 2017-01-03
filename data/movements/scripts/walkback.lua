function onStepIn(creature, item, position, fromPosition)
	if item.uid > 0 and item.uid <= 65535 then
		return false
	end
	return true
end
