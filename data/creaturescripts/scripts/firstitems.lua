local firstItems = {2050, 2382}

function onLogin(player)
	if player:getLastLoginSaved() == 0 then
		for i = 1, #firstItems do
			player:addItem(firstItems[i], 1)
		end
		player:addItem(player:getSex() == 0 and 2651 or 2650, 1)
		player:addItem(1987, 1):addItem(2674, 1)
	end
	return true
end
