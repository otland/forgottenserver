local annihilatorReward = {
	1990, -- present
	2400, -- magic sword
	2431, -- stonecutter axe
	2494 -- demon armor
}

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
		if player:getStorageValue(PlayerStorageKeys.annihilatorReward) ~= -1 then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "It is empty.")
			return true
		end

		if playerCap < itemWeight then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a " .. itemType:getName() .. " weighing " .. itemWeight .. " oz it's too heavy.")
			return true
		end

		if item:getUniqueId() == 1990 then -- present
			player:addItem(1990, 1):addItem(2326, 1) -- present with annihilation bear
		else
			player:addItem(item:getUniqueId(), 1)
		end
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a " .. itemType:getName() .. ".")
		player:setStorageValue(PlayerStorageKeys.annihilatorReward, 1)
		player:addAchievement("Annihilator")
	elseif player:getStorageValue(item:getUniqueId()) == -1 then
		if playerCap < itemWeight then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a " .. itemType:getName() .. " weighing " .. itemWeight .. " oz it's too heavy.")
			return true
		end

		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a " .. itemType:getName() .. ".")
		player:addItem(item:getUniqueId(), 1)
		player:setStorageValue(item:getUniqueId(), 1)
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "It is empty.")
	end
	return true
end
