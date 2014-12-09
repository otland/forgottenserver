local config = {
	loginProtection = true,
	loginProtectionTime = 10000 -- Login Protection Time / miliseconds
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
		loginProtectionTable[player:getId()] = os.mtime() + config.loginProtectionTime
	end

	player:registerEvent("PlayerDeath")
	return true
end
