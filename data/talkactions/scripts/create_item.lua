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

    local count, subType, keyNumber, destinationParam, targetName, indexParam = nil, 1, 0, nil, nil, 0

	for i = 2, #split do
        local arg = split[i]:trim()
        local lowerArg = arg:lower()
        local numArg = tonumber(arg)

        if i == 2 then
            count = numArg or 1
        elseif i == 3 and not table.contains(destinations, lowerArg) then
            if numArg then
                subType = numArg
            else
                targetName = arg
            end
        elseif table.contains(destinations, lowerArg) then
            destinationParam = lowerArg
        elseif numArg then
            indexParam = numArg
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

	if count then
		if itemType:isFluidContainer() then
			count = math.max(0, math.min(count, 99))
		elseif itemType:isKey() then
			keyNumber = count
			count = 1
		else
			count = math.min(10000, math.max(1, count))
		end
	else
		if not itemType:isFluidContainer() then
			count = math.max(1, itemType:getCharges())
		else
			count = 0
		end
	end

	local destination = getDestination(targetPlayer, destinationParam, indexParam)
	
	local result = {}
	if itemType:isStackable() then
		local item = destination:addItem(itemType:getId(), count, true, subType)
		if item then
			if type(result) == "table" then
				table.concat(result, item)
			else
				table.insert(result, item)
			end
		end
	else
		for i = 1, count do
			local item = destination:addItem(itemType:getId(), subType, true, 1)
			if item then
				if type(result) == "table" then
					table.concat(result, item)
				else
					table.insert(result, item)
				end
			end
		end
	end

	if result then
		for _, item in ipairs(result) do
			if itemType:isKey() then
				item:setAttribute(ITEM_ATTRIBUTE_ACTIONID, keyNumber)
			end
			item:decay()
		end
		player:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	end
	return false
end
