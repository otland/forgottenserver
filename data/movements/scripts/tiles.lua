local increasingItemID = {416, 446, 3216, 11062}
local decreasingItemID = {417, 447, 3217, 11063}

function onStepIn(cid, item, position, fromPosition)
	if isInArray(increasingItemID, item.itemid) then
		doTransformItem(item.uid, item.itemid + 1)
		if item.actionid > 1000 then
			getLevelTile(cid, item, position)
		elseif getTilePzInfo(position) then
			getDepotItems(cid, position)
		end
	elseif item.itemid == 426 then
		doTransformItem(item.uid, 425)
		if item.actionid > 1000 then
			getLevelTile(cid, item, position)
		elseif getTilePzInfo(position) then
			getDepotItems(cid, position)
		end
	end
	return true
end

function onStepOut(cid, item, position, fromPosition)
	if isInArray(decreasingItemID, item.itemid) then
		doTransformItem(item.uid, item.itemid - 1)
	elseif item.itemid == 425 then
		doTransformItem(item.uid, item.itemid + 1)
	end
	return true
end

function getLevelTile(cid, item, position)
	local player = Player(cid)
	if player == nil then
		return
	end

	if player:getLevel() < item.actionid - 1000 then
		local playerPosition = player:getPosition()
		doTeleportThing(cid, {x = playerPosition.x, y = playerPosition.y, z = playerPosition.z + 1})
		playerPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
	end
end

function getDepotItems(cid, position)
	local player = Player(cid)
	if player == nil then
		return
	end

	local possiblePositions = {
		{x = position.x - 1, y = position.y, z = position.z},
		{x = position.x + 1, y = position.y, z = position.z},
		{x = position.x, y = position.y - 1, z = position.z},
		{x = position.x, y = position.y + 1, z = position.z},
	}

	for i = 1, #possiblePositions do
		local tile = Tile(possiblePositions[i])
		if tile ~= nil then
			local item = tile:getItemByType(ITEM_TYPE_DEPOT)
			if item ~= nil then
				local depotItems = player:getDepotItems(getDepotId(item:getUniqueId()))
				if depotItems == 1 then
					player:sendTextMessage(MESSAGE_EVENT_DEFAULT, 'Your depot contains 1 item.')
				else
					player:sendTextMessage(MESSAGE_EVENT_DEFAULT, 'Your depot contains '  .. depotItems .. ' items.')
				end
				break
			end
		end
	end
end
