local condition = Condition(CONDITION_OUTFIT, CONDITIONID_COMBAT)
condition:setTicks(-1)

local chameleon = TalkAction("/chameleon")

function chameleon.onSay(player, words, param)
	if(not player:getGroup():getAccess()) or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	local itemType = ItemType(param)
	if itemType:getId() == 0 then
		itemType = ItemType(tonumber(param))
		if itemType:getId() == 0 then
			player:sendCancelMessage("There is no item with that id or name.")
			return false
		end
	end

	condition:setOutfit(itemType:getId())
	player:addCondition(condition)
	return false
end

chameleon:separator(" ")
chameleon:register()
