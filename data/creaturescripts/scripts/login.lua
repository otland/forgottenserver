function onLogin(cid)
	local player = Player(cid)

	local loginStr = "Welcome to " .. configManager.getString(configKeys.SERVER_NAME) .. "!"
	if player:getLastLoginSaved() <= 0 then
		loginStr = loginStr .. " Please choose your outfit."
		player:sendOutfitWindow()
		
		local firstItems = {2050, 2382}
		for i = 1, #firstItems do
			player:addItem(firstItems[i], 1)
		end
		
		player:addItem(player:getSex() == 0 and 2651 or 2650, 1)
		
		local bag = player:addItem(1987, 1)
		bag:addItem(2674, 1)
	else
		if loginStr ~= "" then
			player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)
		end

		loginStr = string.format("Your last visit was on %s.", os.date("%a %b %d %X %Y", player:getLastLoginSaved()))
	end
	player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)

	player:registerEvent("PlayerDeath")
	return true
end