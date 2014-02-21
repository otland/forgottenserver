local useWorms = FALSE
local waterIds = {493, 4608, 4609, 4610, 4611, 4612, 4613, 4614, 4615, 4616, 4617, 4618, 4619, 4620, 4621, 4622, 4623, 4624, 4625}
local iceholeIds = {7236}
function onUse(cid, item, fromPosition, itemEx, toPosition)
	if isInArray(waterIds, itemEx.itemid) == TRUE then
		if itemEx.itemid ~= 493 then
			if useWorms == FALSE or useWorms == TRUE and doPlayerRemoveItem(cid, ITEM_WORM, 1) == TRUE then
				if math.random(1, (100 + (getPlayerSkill(cid, SKILL_FISHING) / 10))) <= getPlayerSkill(cid, SKILL_FISHING) then
					doPlayerAddItem(cid, ITEM_FISH, 1)
				end
				doPlayerAddSkillTry(cid, SKILL_FISHING, 1)
			end
		end
		doSendMagicEffect(toPosition, CONST_ME_LOSEENERGY)
		return TRUE
	elseif isInArray(iceholeIds, itemEx.itemid) == TRUE then -- fish in icehole --
		if useWorms == FALSE or useWorms == TRUE and doPlayerRemoveItem(cid, ITEM_WORM, 1) == TRUE then
			if math.random(1, (100 + (getPlayerSkill(cid, SKILL_FISHING) / 10))) <= getPlayerSkill(cid, SKILL_FISHING) then
				local rand = math.random(1, 100)
				if(rand > 98) then
					doPlayerAddItem(cid,7158, 1) -- rainbow trout 2% --
				elseif(rand > 95) then
					doPlayerAddItem(cid,2669, 1) -- Northern Pike 3% --
				elseif(rand > 89) then
					doPlayerAddItem(cid,7159, 1) -- Green Perch 6% --
				else
					doPlayerAddItem(cid, ITEM_FISH, 1) -- normal fish --
				end
				doPlayerAddSkillTry(cid, SKILL_FISHING, 1)
				Item(itemEx.uid):transform(7237)
				Item(itemEx.uid):decay()
			end
			doSendMagicEffect(toPosition, CONST_ME_LOSEENERGY)
			return TRUE
		end
	end
	return FALSE
end
