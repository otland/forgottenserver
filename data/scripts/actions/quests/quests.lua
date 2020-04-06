local annihilatorReward = {1990, 2400, 2431, 2494}

local quests = Action()

function quests.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item.uid <= 1250 or item.uid >= 30000 then
		return false
	end

	local itemType = ItemType(item.uid)
	if itemType:getId() == 0 then
		return false
	end

	local itemWeight = itemType:getWeight()
	local playerCap = player:getFreeCapacity()
	if table.contains(annihilatorReward, item.uid) then
		if player:getStorageValue(PlayerStorageKeys.annihilatorReward) == -1 then
			if playerCap >= itemWeight then
				if item.uid == 1990 then
					player:addItem(2856, 1):addItem(3213, 1)
				else
					player:addItem(item.uid, 1)
				end
				player:sendTextMessage(MESSAGE_INFO_DESCR, 'You have found a ' .. itemType:getName() .. '.')
				player:setStorageValue(PlayerStorageKeys.annihilatorReward, 1)
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
	return true
end

quests:id(2472, 2480, 2481, 2482)
quests:register()
