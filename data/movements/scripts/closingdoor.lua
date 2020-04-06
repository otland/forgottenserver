function onStepOut(creature, item, toPosition, fromPosition)
	local tile = Tile(fromPosition)
	if tile:getCreatureCount() > 0 then
		return true
	end

	local newPosition = {x = fromPosition.x + 1, y = fromPosition.y, z = fromPosition.z}
	local query = Tile(newPosition):queryAdd(creature)
	if query ~= RETURNVALUE_NOERROR or query == RETURNVALUE_NOTENOUGHROOM then
		newPosition.x = newPosition.x - 1
		newPosition.y = newPosition.y + 1
		query = Tile(newPosition):queryAdd(creature)
	end

	if query == RETURNVALUE_NOERROR or query ~= RETURNVALUE_NOTENOUGHROOM then
		doRelocate(fromPosition, newPosition)
	end

	local i, tileItem, tileCount = 1, true, tile:getThingCount()
	while tileItem and i < tileCount do
		tileItem = tile:getThing(i)
		if tileItem and tileItem:getUniqueId() ~= item.uid and tileItem:getType():isMovable() then
			tileItem:remove()
		else
			i = i + 1
		end
	end

	item:transform(item.itemid - 1)
	return true
end
