local useWorms = false
local waterIds = {493, 4608, 4609, 4610, 4611, 4612, 4613, 4614, 4615, 4616, 4617, 4618, 4619, 4620, 4621, 4622, 4623, 4624, 4625}
function onUse(cid, item, fromPosition, itemEx, toPosition)
local iEx = Item(itemEx.uid)
local player = Player(cid)
	if isInArray(waterIds, itemEx.itemid) == true then
		if itemEx.itemid ~= 493 then
			if useWorms == false or useWorms == true and player:removeItem(ITEM_WORM, 1) == true then
				if math.random(1, (100 + (player:getSkillLevel(SKILL_FISHING) / 10))) <= player:getSkillLevel(SKILL_FISHING) then
					player:addItem(ITEM_FISH, 1)
				end
				player:addSkillTries(SKILL_FISHING, 1)
			end
		end
		toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
		return true
	elseif(itemEx.itemid == 7236) then -- fish in ice hole --
		if useWorms == false or useWorms == true and player:removeItem(ITEM_WORM, 1) == true then
			if math.random(1, (100 + (player:getSkillLevel(SKILL_FISHING) / 10))) <= player:getSkillLevel(SKILL_FISHING) then
				local rand = math.random(1, 100)
				if(rand > 98) then
					player:addItem(7158, 1) -- rainbow trout 2% --
				elseif(rand > 95) then
					player:addItem(2669, 1) -- Northern Pike 3% --
				elseif(rand > 89) then
					player:addItem(7159, 1) -- Green Perch 6% --
				else
					player:addItem(ITEM_FISH, 1) -- normal fish --
				end
				player:addSkillTries(SKILL_FISHING, 1)
				iEx:transform(7237)
				iEx:decay()
			end
			toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
			return true
		end
	elseif(itemEx.itemid == 10499) then -- water elemental corpse --
		if math.random(1, (100 + (player:getSkillLevel(SKILL_FISHING) / 10))) <= player:getSkillLevel(SKILL_FISHING) then
			local rand = math.random(1, 1000)
			if(rand > 995) then
				player:addItem(7632, 1) --  Giant Shimmering Pearl 0,5% --
			elseif(rand > 983) then
				player:addItem(2149, 1) -- small emerald 1,2% --
			elseif(rand > 972) then
				player:addItem(2167, 1) -- energy ring 1,1% --
			elseif(rand > 962) then
				player:addItem(7159, 1) -- Green Perch 1% --
			elseif(rand > 952) then
				player:addItem(2145, 1) -- Small Diamond 1% --
			elseif(rand > 942) then
				player:addItem(7158, 1) -- rainbow trout 1% --
			elseif(rand > 933) then
				player:addItem(2168, 1) -- life ring 0,9% --
			elseif(rand > 924) then
				player:addItem(2143, 1) -- white pearl 0,9% --
			elseif(rand > 916) then
				player:addItem(10220, 1) -- leviathan's amulet 0,8% --
			elseif(rand > 904) then
				player:addItem(2146, 1) -- small sapphire 1,2% --	
			end
			player:addSkillTries(SKILL_FISHING, 1)
			iEx:transform(10500)
		end
		toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
		return true
	end
	return false
end
