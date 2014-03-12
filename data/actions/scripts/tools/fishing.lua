function onUse(cid, item, fromPosition, itemEx, toPosition)
	local waterIds = {493, 4608, 4609, 4610, 4611, 4612, 4613, 4614, 4615, 4616, 4617, 4618, 4619, 4620, 4621, 4622, 4623, 4624, 4625, 7236, 15401, 15402}
	local useWorms = true
	local player = Player(cid)
	
	if(not isInArray(waterIds, itemEx.itemid)) then
		return false
	end

	if itemEx.itemid ~= 7236 then
		doSendMagicEffect(toPosition, CONST_ME_LOSEENERGY)
	end
	
	if itemEx.itemid == 493 or itemEx.itemid == 15402 then
		return true
	end

	player:addSkillTries(SKILL_FISHING, 1)
	if math.random(1, (100 + (player:getEffectiveSkillLevel(SKILL_FISHING) / 10))) <= player:getEffectiveSkillLevel(SKILL_FISHING) then
		if useWorms then
			if(not player:removeItem(ITEM_WORM, 1)) then
				return true
			end
		end
	
		if itemEx.itemid == 15401 then
			if math.random(1, 100) >= 97 then
				player:addItem(15405, 1)
			else
				player:addItem(ITEM_FISH, 1)
			end
		else
			player:addItem(ITEM_FISH, 1)
		end
		
		if itemEx.itemid == 7236 or itemEx.itemid == 15401 then
			doTransformItem(itemEx.uid, itemEx.itemid + 1)
			doDecayItem(itemEx.uid)
		end
	end
	return true
end
