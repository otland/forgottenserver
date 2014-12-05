local config = {
	loginProtection = true,
	loginProtectionTime = 10 -- Login Protection Time / seconds
}

function onLogin(player)
	local loginStr = "Welcome to " .. configManager.getString(configKeys.SERVER_NAME) .. "!"
	if player:getLastLoginSaved() <= 0 then
		loginStr = loginStr .. " Please choose your outfit."
		player:sendOutfitWindow()
	else
		if loginStr ~= "" then
			player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)
		end

		loginStr = string.format("Your last visit was on %s.", os.date("%a %b %d %X %Y", player:getLastLoginSaved()))
	end
	player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)

	if config.loginProtection then
		player:setStorageValue(1000, os.time() + config.loginProtectionTime)
	end

	player:registerEvent("PlayerDeath")
	return true
end
