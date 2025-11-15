local talkaction = TalkAction("/i")

local invalidIds = {
	1, 2, 3, 4, 5, 6, 7, 10, 11, 13, 14, 15, 19, 21, 26, 27, 28, 35, 43
}

function talkaction.onSay(player, words, param)
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

	local keyNumber = 0
	local count = tonumber(split[2])
	local subType = 1
	if not itemType:isStackable() and split[3] then
		subType = math.max(1, tonumber(split[3]) or 1)
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

	local result = nil
	if itemType:isStackable() then
		result = player:addItem(itemType:getId(), count, true, subType)
	else
		result = player:addItem(itemType:getId(), subType, true, count)
	end

	if result then
		if not itemType:isStackable() then
			if type(result) == "table" then
				for _, item in ipairs(result) do
					item:decay()
				end
			else
				if itemType:isKey() then
					result:setAttribute(ITEM_ATTRIBUTE_ACTIONID, keyNumber)
				end
				result:decay()
			end
		end
		player:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	end
	return false
end

talkaction:separator(" ")
talkaction:register()
