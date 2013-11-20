function onSay(cid, words, param)
	local player = Player(cid)
	if not player:getGroup():getAccess() then
		return true
	end
	
	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end
		
	local itemType = ItemType(param)
	if itemType:getId() == 0 then
		itemType = ItemType(tonumber(param))
		if itemType:getId() == 0 then
			player:sendCancelMessage("There is no look type with that id or name.")
			return false
		end
	end
	
	local condition = Condition(CONDITION_OUTFIT, CONDITIONID_COMBAT)
	condition:setTicks(-1)
	condition:addOutfit(itemType:getId())
	player:addCondition(condition)	
	return false
end