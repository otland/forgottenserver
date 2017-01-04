function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	local split = param:split(",")
	if #split < 1 then
		player:sendCancelMessage("You need to specify an item name or id.")
		return false
	end

	local itemType = ItemType(split[1])
	if itemType:getId() == 0 then
		if tonumber(split[1]) then
			itemType = ItemType(tonumber(split[1]))
		end

		if itemType:getId() == 0 then
			player:sendCancelMessage("There is no item with that id or name.")
			return false
		end
	end

	local count = tonumber(split[2])
	if count ~= nil then
		if itemType:isStackable() then
			count = math.min(10000, math.max(1, count))
		elseif not itemType:isFluidContainer() then
			count = math.min(100, math.max(1, count))
		else
			count = math.max(0, count)
		end
	else
		count = itemType:isFluidContainer() and 0 or 1
	end

	local result = player:addItem(itemType:getId(), count)
	if result ~= nil then
		if not itemType:isStackable() then
			if type(result) == "table" then
				for _, item in ipairs(result) do
					item:decay()
				end
			else
				result:decay()
			end
		end
		player:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	end
	return false
end
