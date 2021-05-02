local annihilatorReward = {1990, 2400, 2431, 2494}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item:getUniqueId() <= 1250 or item:getUniqueId() >= 30000 then
		return false
	end

	local itemType = ItemType(item:getUniqueId())
	if itemType:getId() == 0 then
		return false
	end

	local itemWeight = itemType:getWeight()
	local playerCap = player:getFreeCapacity()
	if table.contains(annihilatorReward, item:getUniqueId()) then
		if player:getStorageValue(PlayerStorageKeys.annihilatorReward) == -1 then
			if playerCap >= itemWeight then
				if item:getUniqueId() == 1990 then
					player:addItem(1990, 1):addItem(2326, 1)
				else
					player:addItem(item:getUniqueId(), 1)
				end
				player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a " .. itemType:getName() .. ".")
				player:setStorageValue(PlayerStorageKeys.annihilatorReward, 1)
				player:addAchievement("Annihilator")
			else
				player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a " .. itemType:getName() .. " weighing " .. itemWeight .. " oz it's too heavy.")
			end
		else
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "It is empty.")
		end
	elseif player:getStorageValue(item:getUniqueId()) == -1 then
		if playerCap >= itemWeight then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a " .. itemType:getName() .. ".")
			player:addItem(item:getUniqueId(), 1)
			player:setStorageValue(item:getUniqueId(), 1)
		else
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a " .. itemType:getName() .. " weighing " .. itemWeight .. " oz it's too heavy.")
		end
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "It is empty.")
	end
	return true
end
