local waterIds = {
	493, -- water
	4608, -- shallow water
	4609, -- shallow water
	4610, -- shallow water
	4611, -- shallow water
	4612, -- shallow water
	4613, -- shallow water
	4614, -- shallow water
	4615, -- shallow water
	4616, -- shallow water
	4617, -- shallow water
	4618, -- shallow water
	4619, -- shallow water
	4620, -- shallow water (Swimmable)
	4621, -- shallow water (Swimmable)
	4622, -- shallow water (Swimmable)
	4623, -- shallow water (Swimmable)
	4624, -- shallow water (Swimmable)
	4625, -- shallow water (Swimmable)
	7236, -- ice hole
	10499, -- remains of a water elemental
	15401, -- shallow water
	15402 -- shallow water
}

local lootTrash = {
	2234, -- broken sword
	2238, -- worn leather boots
	2376, -- sword
	2509, -- steel shield
	2667 -- fish
}

local lootCommon = {
	2152, -- platinum coic
	2167, -- energy ring
	2168, -- life ring
	2669, -- northern pike
	7588, -- strong health potion
	7589 -- strong mana potion
}

local lootRare = {
	2143, -- white pearl
	2146, -- small sapphire
	2149, -- small emerald
	7158, -- rainbow trout
	7159 -- green perch
}

local lootVeryRare = {
	7632, -- giant shimmering pearl
	7633, -- giant shimmering pearl
	10220 -- leviathan's amulet
}

local useWorms = true

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if not table.contains(waterIds, target:getId()) then
		return false
	end

	if target:getId() == 10499 then -- remains of a water elemental
		local owner = target:getAttribute(ITEM_ATTRIBUTE_CORPSEOWNER)
		if owner ~= 0 and owner ~= player:getId() then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You are not the owner.")
			return true
		end

		toPosition:sendMagicEffect(CONST_ME_WATERSPLASH)
		target:transform(target:getId() + 1)
		target:decay()

		local chance = math.random(1, 100)
		if chance == 1 then
			player:addItem(lootVeryRare[math.random(#lootVeryRare)], 1)
		elseif chance <= 3 then
			player:addItem(lootRare[math.random(#lootRare)], 1)
		elseif chance <= 10 then
			player:addItem(lootCommon[math.random(#lootCommon)], 1)
		else
			player:addItem(lootTrash[math.random(#lootTrash)], 1)
		end
		return true
	end

	if target:getId() ~= 7236 then -- ice hole
		toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
	end

	if table.contains({493, 15402}, target:getId()) then -- water | shallow water
		return true
	end

	player:addSkillTries(SKILL_FISHING, 1)
	if math.random(1, 100) <= math.min(math.max(10 + (player:getEffectiveSkillLevel(SKILL_FISHING) - 10) * 0.597, 10), 50) then
		if useWorms and not player:removeItem(3976, 1) then -- worm
			return true
		end

		if target:getId() == 15401 then -- shallow water
			target:transform(target:getId() + 1)
			target:decay()

			if math.random(1, 100) >= 97 then
				player:addItem(15405, 1) -- sandfish
				player:addAchievement("Desert Fisher")
				return true
			end
		elseif target:getId() == 7236 then -- ice hole
			target:transform(target:getId() + 1)
			target:decay()
			player:addAchievementProgress("Exquisite Taste", 250)

			local chance = math.random(1, 100)
			if chance == 1 then
				player:addItem(7158, 1) -- rainbow trout
				return true
			elseif chance <= 4 then
				player:addItem(2669, 1) -- northern pike
				return true
			elseif chance <= 10 then
				player:addItem(7159, 1) -- green perch
				return true
			end
		end
		player:addAchievementProgress("Here, Fishy Fishy!", 1000)
		player:addItem(2667, 1) -- fish
	end
	return true
end
