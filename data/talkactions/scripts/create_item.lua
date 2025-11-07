local invalidIds = {
	1, 2, 3, 4, 5, 6, 7, 10, 11, 13, 14, 15, 19, 21, 26, 27, 28, 35, 43
}

local destinations = { "inbox", "storeinbox", "tile", "house", "depot" }

local function getDestination(target, param, index)
	if not param then
		return target
	end

	param = param:lower()
	if param == "inbox" then
		return target:getInbox()
	elseif param == "storeinbox" then
		return target:getStoreInbox()
	elseif param == "tile" then
		return target:getTile()
	elseif param == "house" then
		local house = target:getHouse()
		local doors = house and house:getDoors()
		local randomDoor = doors and doors[math.random(#doors)]
		return randomDoor and randomDoor:getTile() or target
	elseif param == "depot" then
		local depotId = tonumber(index) or 0
		local depot = target:getDepotChest(depotId, true)
		return depot or target
	end
	return target
end

local function addItemToDestination(destination, itemId, count, subType)
	local result
	if destination:isCreature() and destination:isPlayer() then
		-- itemId[, count = 1[, canDropOnMap = true[, subType = 1[, slot = CONST_SLOT_WHEREEVER]]]]
		result = destination:addItem(itemId, count, true, subType, CONST_SLOT_WHEREEVER)
	elseif destination:isContainer() then
		-- itemId[, count / subType = 1 [, index = INDEX_WHEREEVER[, flags = 0]]]
		result = destination:addItem(itemId, count, subType, INDEX_WHEREEVER)
	elseif destination:isTile() then
		-- itemId[, count / subType = 1 [, flags = 0]]
		result = destination:addItem(itemId, count, subType)
	end

	if type(result) == "table" then
		return result
	end
	return {result}
end

local function sendPlayerItemInformation(player, data)
    local lines = {}

    for label, value in pairs(data) do
        if value ~= nil then
            table.insert(lines, label .. ": " .. tostring(value))
        end
    end

    if #lines > 0 then
		player:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
        player:sendTextMessage(MESSAGE_STATUS_DEFAULT, "Item created successfully. [" .. table.concat(lines, ", ") .. "]")
    end
	return true
end

function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	local split = param:splitTrimmed(",")

	local itemType = ItemType(split[1])
	if itemType:getId() == 0 then
		itemType = ItemType(tonumber(split[1]))
		if not tonumber(split[1]) or itemType:getId() == 0 then
			player:sendCancelMessage("There is no item with that id or name.")
			return false
		end
	end

	if table.contains(invalidIds, itemType:getId()) then
		return false
	end

	local count, subType, keyNumber, destinationParam, targetName, indexParam = nil, nil, nil, nil, nil, nil

	for i = 2, #split do
		local arg = split[i]:trim()
		local lowerArg = arg:lower()
		local numArg = tonumber(arg)

		if table.contains(destinations, lowerArg) and not destinationParam then
			destinationParam = lowerArg
		elseif numArg then
			if not count then
				if itemType:isFluidContainer() then
					count = math.max(0, math.min(numArg, itemType:getCharges()))
				else
					count = math.min(10000, math.max(1, numArg or 1))
				end
			elseif not subType and not keyNumber then
				if not itemType:isStackable() then
					if itemType:isKey() then
						keyNumber = numArg or 1
					else
						subType = numArg or 1
					end
				else
					subType = 1
				end
			elseif not indexParam and destinationParam then
				indexParam = numArg or 1
			end
		elseif not targetName then
			targetName = arg
		end
	end

	local targetPlayer = player
	if targetName then
		local found = Player(targetName)
		if not found then
			player:sendCancelMessage("Player \"" .. targetName .. "\" not found.")
			return false
		end

		targetPlayer = found
	end

	local destination = getDestination(targetPlayer, destinationParam, indexParam)
	local result = addItemToDestination(destination, itemType:getId(), count, subType)
	if result then
		for _, item in ipairs(result) do
			if itemType:isKey() then
				item:setAttribute(ITEM_ATTRIBUTE_ACTIONID, keyNumber)
			end

			if not itemType:isStackable() then
				item:decay()
			end
		end

		local itemInformation = {
			["id"] = itemType:getId(),
			["count"] = count,
			["subtype"] = subType,
			["keynumber"] = keyNumber,
			["destination"] = destinationParam,
			["target"] = targetName,
			["index"] = indexParam
		}

		sendPlayerItemInformation(player, itemInformation)
	end
	return false
end
