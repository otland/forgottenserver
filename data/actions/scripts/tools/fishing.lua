-- Water Ids + 1 no fish -1 with fish
local waterWithFish = {4608, 4610, 4612, 4614, 4616, 4618, 4664, 7236, 15401}
local waterWithoutFish = {4609, 4611, 4613, 4615, 4617, 4619, 4665, 7237, 15402}
local iceHoles = {7236, 7237}
local waterSpiritId = 10449

local fish = {
	[1] = {itemid = 2667, baseChance = 25000}, -- Fish : 25%
	[2] = {itemid = 7159, baseChance = 2250}, -- Green Perch : 2.25%
	[3] = {itemid = 2669, baseChance = 1100}, -- Northern Pike : 1.1%
	[4] = {itemid = 2667, baseChance = 700}, -- Rainbow Trout : 0.7%
	[5] = {itemid = 15405, baseChance = 3000} -- Sand Fish : 3%
}

local lootTrash = {2234, 2238, 2376, 2509, 2667}
local lootCommon = {2152, 2167, 2168, 2669, 7588, 7589}
local lootRare = {2143, 2146, 2149, 7158, 7159}
local lootVeryRare = {7632, 7633, 10220}

local useWorms = true
local rodId = 2580
local mechanicalRodId = 10223
local wormId = 3976

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if not target.itemid then 
		return false 
	end
	
	if target.itemid == waterSpiritId then
		waterSpiritFishing(player, item, target, toPosition)
		return true
	elseif table.contains(waterWithFish, target.itemid) or table.contains(waterWithoutFish, target.itemid) then
		waterFishing(player, item, target, toPosition)
		return true
	end
	
	return false
end

function waterFishing(player, item, target, toPosition)
	-- Ice holes do not get water effect --
	if not table.contains(iceHoles, target.itemid) then
		toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
	end
		
	-- Fishing from PZ. Dont use worms, add skill, or catch a fish --
	if Tile(toPosition):hasFlag(TILESTATE_PROTECTIONZONE) then
		return true
	end
		
	-- Water doesn't have fish, dont try to catch one --
	if not table.contains(waterWithFish, target.itemid) then
		return true
	end
		
	-- Remove worm if we use them --
	if useWorms and not player:removeItem(wormId, 1) then
		return true
	end
		
	player:addSkillTries(SKILL_FISHING, 1)
			
	local skillChance = math.min(math.max(10 + (player:getEffectiveSkillLevel(SKILL_FISHING) - 10) * 0.597, 10), 50) / 100
	local rand = math.random(100000)
	
	for i = 1, #fish do
		-- Loop is at sand fish. They must be fishing on itemid 15401 --
		if i == 5 and targetId ~= 15401 then
			break
		end

		-- Chance to catch fish is baseChance + skill Chance
		if rand <= math.floor(fish[i].baseChance + (fish[i].baseChance * skillChance)) then
			player:addItem(fish[i].itemid, 1)
			target:transform(target.itemid + 1)
			target:decay()
					
			-- Add achievement based on which waterId is being fished on --
			if target.itemid == 7236 then
				player:addAchievementProgress("Exquisite Taste", 250)
			elseif target.itemid == 15401 then
				player:addAchievementProgress("Desert Fisher")
			else
				player:addAchievementProgress("Here, Fishy Fishy!", 1000)
			end

			break
		end
	end
end

function waterSpiritFishing(player, item, target, toPosition)
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
end
