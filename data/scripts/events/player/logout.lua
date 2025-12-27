local event = Event()

event.onPlayerLogout = function(self)
	local playerId = self:getId()
	if nextUseStaminaTime[playerId] then
		nextUseStaminaTime[playerId] = nil
	end

	self:setLastPing(nil)
	self:setLastPong(nil)
	return true
end

event:register()
