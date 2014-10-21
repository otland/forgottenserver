local annihilatorReward = {1990, 2400, 2431, 2494}
function onUse(cid, item, fromPosition, itemEx, toPosition, isHotkey)
	if item.uid > 1000 and item.uid <= 22670 then
		local itemType = ItemType(item.itemid)
		local itemWeight = itemType:getWeight()
		local player = Player(cid)
		local playerCap = player:getFreeCapacity()
		if isInArray(annihilatorReward, item.uid) then
			if player:getStorageValue(30015) == -1 then
				if playerCap >= itemWeight then
					if item.uid == 1990 then
						player:addItem(1990, 1):addItem(2326, 1)
					else
						player:addItem(item.uid, 1)
					end
					player:sendTextMessage(MESSAGE_INFO_DESCR, 'You have found a ' .. itemType:getName() .. '.')
					player:setStorageValue(30015, 1)
				else
					player:sendTextMessage(MESSAGE_INFO_DESCR, 'You have found a ' .. itemType:getName() .. ' weighing ' .. itemWeight .. ' oz it\'s too heavy.')
				end
			else
				player:sendTextMessage(MESSAGE_INFO_DESCR, "It is empty.")
			end
		elseif player:getStorageValue(item.uid) == -1 then
			if playerCap >= itemWeight then
				player:sendTextMessage(MESSAGE_INFO_DESCR, 'You have found a ' .. itemType:getName() .. '.')
				player:addItem(item.uid, 1)
				player:setStorageValue(item.uid, 1)
			else
				player:sendTextMessage(MESSAGE_INFO_DESCR, 'You have found a ' .. itemType:getName() .. ' weighing ' .. itemWeight .. ' oz it\'s too heavy.')
			end
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "It is empty.")
		end
	else
		return false
	end

	return true
end
