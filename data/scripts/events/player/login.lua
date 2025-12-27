local event = Event()

event.onPlayerJoin = function(self)
	local serverName = configManager.getString(configKeys.SERVER_NAME)
	local loginStr = "Welcome to " .. serverName .. "!"
	if self:getLastLoginSaved() <= 0 then
		loginStr = loginStr .. " Please choose your outfit."
		self:sendOutfitWindow()
	else
		self:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)
		loginStr = string.format("Your last visit in %s: %s.", serverName, os.date("%d %b %Y %X", self:getLastLoginSaved()))
	end
	self:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)

	-- Promotion
	local vocation = self:getVocation()
	local promotion = vocation:getPromotion()
	if self:isPremium() then
		local value = self:getStorageValue(PlayerStorageKeys.promotion)
		if value and value == 1 then
			self:setVocation(promotion)
		end
	elseif not promotion then
		self:setVocation(vocation:getDemotion())
	end

	-- Update client stats
	self:updateClientExpDisplay()
	self:sendHotkeyPreset()
	self:disableLoginMusic()
	self:sendBlessings()
	self:sendTrackedBestiary()

	-- achievements points for highscores
	if not self:getStorageValue(PlayerStorageKeys.achievementsTotal) then
		self:setStorageValue(PlayerStorageKeys.achievementsTotal, self:getAchievementPoints())
	end

	-- initialize ping-pong timestamps
	local timeNow = os.time()
	self:setLastPing(timeNow)
	self:setLastPong(timeNow)
end

event:register()
