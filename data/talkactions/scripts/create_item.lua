local invalidIds = {1, 2, 3, 4, 5, 6, 7, 10, 11, 13, 14, 15, 19, 21, 26, 27, 28, 35, 43}
local destinations = {"inbox", "storeinbox", "tile", "house", "depot"}

local function getDestination(target, param, index)
	if not param then
		return target
	end

	local destinationMap = {
		inbox = function(t)
			return t:getInbox()
		end,
		storeinbox = function(t)
			return t:getStoreInbox()
		end,
		tile = function(t)
			return t:getTile()
		end,
		house = function(t)
			local house = t:getHouse()
			local doors = house and house:getDoors()
			local randomDoor = doors and doors[math.random(#doors)]
			return randomDoor and randomDoor:getTile() or t
		end,
		depot = function(t)
			local depotId = tonumber(index) or 0
			return t:getDepotChest(depotId, true) or t
		end
	}

	param = param:lower()
	return destinationMap[param] and destinationMap[param](target) or target
end

local function addItemToDestination(destination, itemId, count, subType)
	local result

	if destination:isCreature() and destination:isPlayer() then
		result = destination:addItem(itemId, count, true, subType, CONST_SLOT_WHEREEVER)
	elseif destination:isContainer() then
		result = destination:addItem(itemId, count, subType, INDEX_WHEREEVER)
	elseif destination:isTile() then
		result = destination:addItem(itemId, count, subType)
	end

	return type(result) == "table" and result or {result}
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

	if not param:find(',') then
		player:sendTextMessage(MESSAGE_INFO_DESCR, string.format("Usage: %s [item id or name], [count], [subtype or key number], [destination], [player name], [index].\nPossible destinations: %s", words, table.concat(destinations, ", ")))
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

	local count, subType, keyNumber, destinationParam, targetName, indexParam

	for i = 2, #split do
		local arg = split[i]:trim()
		local lowerArg = arg:lower()
		local numArg = tonumber(arg)

		if table.contains(destinations, lowerArg) and not destinationParam then
			destinationParam = lowerArg
		elseif numArg then
			if not count then
				count = itemType:isFluidContainer() and math.max(0, math.min(numArg, itemType:getCharges())) or
							math.min(10000, math.max(1, numArg))
			elseif not subType and not keyNumber then
				if not itemType:isStackable() then
					if itemType:isKey() then
						keyNumber = numArg
					else
						subType = numArg
					end
				else
					subType = 1
				end
			elseif not indexParam and destinationParam then
				indexParam = numArg
			end
		elseif not targetName then
			targetName = arg
		end
	end

	local targetPlayer = targetName and Player(targetName) or player
	if targetName and not targetPlayer then
		player:sendCancelMessage("Player \"" .. targetName .. "\" not found.")
		return false
	end

	local destination = getDestination(targetPlayer, destinationParam, indexParam)
	local result = addItemToDestination(destination, itemType:getId(), count, subType)

	for _, item in ipairs(result) do
		if itemType:isKey() then
			item:setAttribute(ITEM_ATTRIBUTE_ACTIONID, keyNumber)
		end
		if not itemType:isStackable() then
			item:decay()
		end
	end

	sendPlayerItemInformation(player, {
		id = itemType:getId(),
		count = count,
		subtype = subType,
		keynumber = keyNumber,
		destination = destinationParam,
		target = targetName,
		index = indexParam
	})

	return false
end
