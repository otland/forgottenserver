local waterIds = {493, 4608, 4609, 4610, 4611, 4612, 4613, 4614, 4615, 4616, 4617, 4618, 4619, 4620, 4621, 4622, 4623, 4624, 4625, 7236, 10499, 15401, 15402}
local lootTrash = {2234, 2238, 2376, 2509, 2667}
local lootCommon = {2152, 2167, 2168, 2669, 7588, 7589}
local lootRare = {2143, 2146, 2149, 7158, 7159}
local lootVeryRare = {7632, 7633, 10220}
local useWorms = true

function onUse(cid, item, fromPosition, itemEx, toPosition)
	local targetId = itemEx.itemid
	if not isInArray(waterIds, itemEx.itemid) then
		return false
	end

	if targetId == 10499 then
		local targetItem = Item(itemEx.uid)
		local owner = targetItem:getAttribute(ITEM_ATTRIBUTE_CORPSEOWNER)
		if owner ~= 0 and owner ~= cid then
			Player(cid):sendCancelMessage("You are not the owner.")
			return true
		end

		toPosition:sendMagicEffect(CONST_ME_WATERSPLASH)
		targetItem:remove()

		local rareChance = math.random(1, 100)
		if rareChance == 1 then
			Player(cid):addItem(lootVeryRare[math.random(#lootVeryRare)], 1)
		elseif rareChance <= 3 then
			Player(cid):addItem(lootRare[math.random(#lootRare)], 1)
		elseif rareChance <= 10 then
			Player(cid):addItem(lootCommon[math.random(#lootCommon)], 1)
		else
			Player(cid):addItem(lootTrash[math.random(#lootTrash)], 1)
		end
		return true
	end

	if targetId ~= 7236 then
		toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
	end

	if targetId == 493 or targetId == 15402 then
		return true
	end

	local player = Player(cid)
	player:addSkillTries(SKILL_FISHING, 1)
	if math.random(1, 100) <= math.min(math.max(10 + (player:getEffectiveSkillLevel(SKILL_FISHING) - 10) * 0.597, 10), 50) then
		if useWorms and not player:removeItem(ITEM_WORM, 1) then
			return true
		end

		if targetId == 15401 then
			local targetItem = Item(itemEx.uid)
			targetItem:transform(targetId + 1)
			targetItem:decay()

			if math.random(1, 100) >= 97 then
				player:addItem(15405, 1)
				return true
			end
		elseif targetId == 7236 then
			local targetItem = Item(itemEx.uid)
			targetItem:transform(targetId + 1)
			targetItem:decay()

			local rareChance = math.random(1, 100)
			if rareChance == 1 then
				player:addItem(7158, 1)
				return true
			elseif rareChance <= 4 then
				player:addItem(2669, 1)
				return true
			elseif rareChance <= 10 then
				player:addItem(7159, 1)
				return true
			end
		end
		player:addItem(ITEM_FISH, 1)
	end
	return true
end
