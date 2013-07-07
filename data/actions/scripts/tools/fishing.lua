local useWorms = FALSE
local waterIds = {493, 4608, 4609, 4610, 4611, 4612, 4613, 4614, 4615, 4616, 4617, 4618, 4619, 4620, 4621, 4622, 4623, 4624, 4625}
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
	end
	return FALSE
end