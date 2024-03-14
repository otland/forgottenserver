function onLogin(player)
	local serverName = configManager.getString(configKeys.SERVER_NAME)
	local loginStr = "Welcome to " .. serverName .. "!"
	if player:getLastLoginSaved() <= 0 then
		loginStr = loginStr .. " Please choose your outfit."
		player:sendOutfitWindow()
	else
		player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)
		loginStr = string.format("Your last visit in %s: %s.", serverName, os.date("%d %b %Y %X", player:getLastLoginSaved()))
	end
	player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)

	-- Promotion
	local vocation = player:getVocation()
	local promotion = vocation:getPromotion()
	if player:isPremium() then
		local value = player:getStorageValue(PlayerStorageKeys.promotion)
		if value and value == 1 then
			player:setVocation(promotion)
		end
	elseif not promotion then
		player:setVocation(vocation:getDemotion())
	end

	-- Update client stats
	player:updateClientExpDisplay()
	player:sendHotkeyPreset()
	player:disableLoginMusic()

	-- achievements points for highscores
	if not player:getStorageValue(PlayerStorageKeys.achievementsTotal) then
		player:setStorageValue(PlayerStorageKeys.achievementsTotal, player:getAchievementPoints())
	end

	local loyaltyBonus = player:getLoyaltyBonus()
	if loyaltyBonus > 0 then
		loginStr = string.format("Due to your long-term loyalty to %s, you currently benefit from a %d%% bonus on all of your skills.", serverName, loyaltyBonus * 100)
		player:sendTextMessage(MESSAGE_EVENT_DEFAULT, loginStr)
	end

	-- Events
	player:registerEvent("PlayerDeath")
	player:registerEvent("DropLoot")
	player:registerEvent("BestiaryKills")
	return true
end
