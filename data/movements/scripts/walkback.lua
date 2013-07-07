function onStepIn(cid, item, position, fromPosition)
	if item.uid > 0 and item.uid <= 65535 and isPlayer(cid) == TRUE then
		doTeleportThing(cid, fromPosition, FALSE)
	end
	return TRUE
end