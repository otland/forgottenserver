local hiddenNest = Action()

function hiddenNest.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if (player:getStorageValue(PlayerStorageKeys.hiddenNest)) ~= 1 then
		player:addItem(16102, 1) -- surprise nest
		player:setStorageValue(PlayerStorageKeys.hiddenNest, 1)
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a surprise nest.")
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The hidden nest is empty.")
	end
	return true
end

hiddenNest:id(16093)
hiddenNest:register()
