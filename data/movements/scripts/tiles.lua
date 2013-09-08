local increasingItemID = {416, 446, 3216, 11062}
local decreasingItemID = {417, 447, 3217, 11063}

function onStepIn(cid, item, position, fromPosition)
	if isInArray(increasingItemID, item.itemid) then
		doTransformItem(item.uid, item.itemid + 1)
		if item.actionid > 1000 then
			getLevelTile(cid, item, position)
		elseif getTilePzInfo(position) then
			getDepotItems(cid, item)
		end
	elseif item.itemid == 426 then
		doTransformItem(item.uid, 425)
		if item.actionid > 1000 then
			getLevelTile(cid, item, position)
		elseif getTilePzInfo(position) then
			getDepotItems(cid, item)
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
	if isPlayer(cid) then
		if getPlayerLevel(cid) < item.actionid - 1000 then
			local playerPosition = getPlayerPosition(cid)
			doTeleportThing(cid, {x = playerPosition.x, y = playerPosition.y, z = playerPosition.z + 1})
			doSendMagicEffect(position, CONST_ME_MAGIC_BLUE)
		end
	end
end

function getDepotItems(cid, position)
	local possiblePositions = {
		{x = position.x - 1, y = position.y, z = position.z},
		{x = position.x + 1, y = position.y, z = position.z},
		{x = position.x, y = position.y - 1, z = position.z},
		{x = position.x, y = position.y + 1, z = position.z},
	}

	for i = 1, #possiblePositions do
		local thing = getTileItemByType(possiblePositions[i], ITEM_TYPE_DEPOT)
		if thing.itemid ~= 0 and isDepot(thing.uid) then
			local depotItems = getPlayerDepotItems(cid, getDepotId(thing.uid))
			if depotItems == 1 then
				doPlayerSendTextMessage(cid, MESSAGE_EVENT_DEFAULT, 'Your depot contains 1 item.')
			else
				doPlayerSendTextMessage(cid, MESSAGE_EVENT_DEFAULT, 'Your depot contains '  .. depotItems .. ' items.')
			end
			break
		end
	end
end
