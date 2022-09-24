local waterIds = {493, 4608, 4609, 4610, 4611, 4612, 4613, 4614, 4615, 4616, 4617, 4618, 4619, 4620, 4621, 4622, 4623, 4624, 4625, 7236, 10499, 15401, 15402}
local lootTrash = {2234, 2238, 2376, 2509, 2667}
local lootCommon = {2152, 2167, 2168, 2669, 7588, 7589}
local lootRare = {2143, 2146, 2149, 7158, 7159}
local lootVeryRare = {7632, 7633, 10220}
local useWorms = true

local fishers = {}
local fishingSpotCooldown = 120 -- Seconds

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local targetId = target.itemid
	if not table.contains(waterIds, targetId) then
		return false
	end

	if targetId == 10499 then
		local owner = target:getAttribute(ITEM_ATTRIBUTE_CORPSEOWNER)
		if owner ~= 0 and owner ~= player:getId() then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You are not the owner.")
			return true
		end

		toPosition:sendMagicEffect(CONST_ME_WATERSPLASH)
		target:transform(targetId + 1)
		target:decay()

		local rareChance = math.random(1, 100)
		if rareChance == 1 then
			player:addItem(lootVeryRare[math.random(#lootVeryRare)], 1)
		elseif rareChance <= 3 then
			player:addItem(lootRare[math.random(#lootRare)], 1)
		elseif rareChance <= 10 then
			player:addItem(lootCommon[math.random(#lootCommon)], 1)
		else
			player:addItem(lootTrash[math.random(#lootTrash)], 1)
		end
		return true
	end

	if targetId ~= 7236 then
		toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
	end

	if targetId == 493 or targetId == 15402 then
		return true
	end
	
	-- We caught a fish at this spot. We cant gain anymore skill or catch another for a while.
	if fishers[player:getName()] and fishers[player:getName()].positions[toPosition] then
		if fishers[player:getName()].positions[toPosition] then
			return true
		end
	end

	player:addSkillTries(SKILL_FISHING, 1)
	if math.random(1, 100) <= math.min(math.max(10 + (player:getEffectiveSkillLevel(SKILL_FISHING) - 10) * 0.597, 10), 50) then
		if useWorms and not player:removeItem(3976, 1) then
			return true
		end

		if targetId == 15401 then
			target:transform(targetId + 1)
			target:decay()

			if math.random(1, 100) >= 97 then
				player:addItem(15405, 1)
				player:addAchievement("Desert Fisher")
				return true
			end
		elseif targetId == 7236 then
			target:transform(targetId + 1)
			target:decay()
			player:addAchievementProgress("Exquisite Taste", 250)

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
		
		local playerName = player:getName()
		if not fishers[playerName] then
			fishers[playerName] = {positions = {}}
		end
		
		fishers[player:getName()].positions[toPosition] = true
		addEvent(resetFishingSpot, fishingSpotCooldown * 1000, playerName, toPosition)
		
		player:addAchievementProgress("Here, Fishy Fishy!", 1000)
		player:addItem(2667, 1)
	end
	return true
end

function resetFishingSpot(name, pos)
	if fishers[playerName] and fishers[playerName].positions then
		if fishers[playerName].positions[pos] then
			fishers[playerName].positions[pos] = nil
		end
	end
end
