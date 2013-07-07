function onStepOut(cid, item, position, fromPosition)
	if isPlayer(cid) and not isPlayerGhost(cid) then
		if item.itemid == 670 then
			doTransformItem(item.uid, 6594)
		else
			doTransformItem(item.uid, item.itemid + 15)
		end
		doDecayItem(item.uid)
	end
	return true
end
