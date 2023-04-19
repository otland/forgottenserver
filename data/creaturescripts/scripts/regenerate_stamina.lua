function onLogin(player)
	if not configManager.getBoolean(configKeys.STAMINA_SYSTEM) then
		return true
	end

	local lastLogout = player:getLastLogout()
	local offlineTime = lastLogout ~= 0 and math.min(os.time() - lastLogout, 86400 * 21) or 0
	offlineTime = offlineTime - 600

	if offlineTime < 180 then
		return true
	end

	local staminaMinutes = player:getStamina()
	local maxNormalStaminaRegen = math.floor((3600 / 6) * (100 / (100 + staminaMinutes)))

	local regainStaminaSeconds = offlineTime / configManager.getNumber(configKeys.STAMINA_REGEN_MINUTE) * 60
	if regainStaminaSeconds > maxNormalStaminaRegen then
		local happyHourStaminaRegen = (regainStaminaSeconds - maxNormalStaminaRegen) * configManager.getNumber(configKeys.STAMINA_REGEN_PREMIUM) / 180
		staminaMinutes = math.min(2520, math.max(2340, staminaMinutes) + happyHourStaminaRegen / 60)
	else
		staminaMinutes = staminaMinutes + regainStaminaSeconds / 60
	end

	player:setStamina(staminaMinutes)
	return true
end
