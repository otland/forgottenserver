function onStepIn(cid, item, position, fromPosition)
	if item.actionid > 30020 and item.actionid < 30100 then
		doPlayerSetTown(cid, item.actionid - 30020)
	end
	return TRUE
end