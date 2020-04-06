local items = {2920, 3270}

local firstItems = CreatureEvent("First Items")

function firstItems.onLogin(player)
	if player:getLastLoginSaved() == 0 then
		for i = 1, #items do
			player:addItem(items[i], 1)
		end
		player:addItem(player:getSex() == 0 and 3562 or 3561, 1)
		player:addItem(ITEM_BAG, 1):addItem(3585, 1)
	end
	return true
end

firstItems:register()
