local event = Event()

event.onPlayerJoin = function(self)
	if not configManager.getBoolean(configKeys.STAMINA_SYSTEM) then
		return
	end

	local lastLogout = self:getLastLogout()
	local offlineTime = lastLogout ~= 0 and math.min(os.time() - lastLogout, 86400 * 21) or 0
	offlineTime = offlineTime - 600
	if offlineTime < 180 then
		return
	end

	local staminaMinutes = self:getStamina()
	local maxNormalStaminaRegen = 2340 - math.min(2340, staminaMinutes)

	local regainStaminaMinutes = offlineTime / configManager.getNumber(configKeys.STAMINA_REGEN_MINUTE)
	if regainStaminaMinutes > maxNormalStaminaRegen then
		local happyHourStaminaRegen = (offlineTime - (maxNormalStaminaRegen * 180)) / configManager.getNumber(configKeys.STAMINA_REGEN_PREMIUM)
		staminaMinutes = math.min(2520, math.max(2340, staminaMinutes) + happyHourStaminaRegen)
	else
		staminaMinutes = staminaMinutes + regainStaminaMinutes
	end

	self:setStamina(staminaMinutes)
end

event:register()
