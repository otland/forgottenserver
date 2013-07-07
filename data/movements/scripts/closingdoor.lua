function onStepOut(cid, item, position, fromPosition)
	local newPosition = {x = position.x, y = position.y, z = position.z}
	if isInArray(verticalOpenDoors, item.itemid) == TRUE then
		newPosition.x = newPosition.x + 1
	else
		newPosition.y = newPosition.y + 1
	end
	doRelocate(position, newPosition)

	local tmpPos = {x = position.x, y = position.y, z = position.z, stackpos = -1}
	local tileCount = getTileThingByPos(tmpPos)
	local i = 1
	local tmpItem = {uid = 1}
	while(tmpItem.uid ~= 0 and i < tileCount) do
		tmpPos.stackpos = i
		tmpItem = getTileThingByPos(tmpPos)
		if tmpItem.uid ~= item.uid and tmpItem.uid ~= 0 and isMoveable(tmpItem.uid) == TRUE then
			doRemoveItem(tmpItem.uid)
		else
			i = i + 1
		end
	end

	doTransformItem(item.uid, item.itemid - 1)
	return TRUE
end