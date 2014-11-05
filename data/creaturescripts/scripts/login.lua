local loginProtectionTime = configManager.getNumber(configKeys.LOGIN_PROTECTION_TIME)
local loginProtection = Condition(CONDITION_LOGINPROTECTION, CONDITIONID_DEFAULT)
loginProtection:setParameter(CONDITION_PARAM_TICKS, loginProtectionTime * 1000)

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
	
	if loginProtectionTime > 0 then
		player:addCondition(loginProtection)
	end
	
	player:registerEvent("PlayerDeath")
	return true
end
