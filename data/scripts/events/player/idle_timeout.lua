local IDLE_LIMIT_MILLIS = 2 * 60 * 1000
local GRACE_PERIOD_MILLIS = 1 * 60 * 1000

local event = Event()

event.onCreatureThink = function(self, interval)
	if not self:isPlayer() then
		return
	end

	if self:getGroup():getAccess() then
		return
	end

	local tile = self:getTile()
	if not tile or tile:hasFlag(TILESTATE_NOLOGOUT) then
		return
	end

	self:setIdleTime(self:getIdleTime() + interval)

	local idleTime = self:getIdleTime()
	if idleTime == IDLE_LIMIT_MILLIS then
		self:sendTextMessage(MESSAGE_STATUS_WARNING, "There was no variation in your behaviour for " .. IDLE_LIMIT_MILLIS / 60000 .. " minutes. You will be disconnected in one minute if there is no change in your actions until then.")
		return
	end

	if idleTime > IDLE_LIMIT_MILLIS + GRACE_PERIOD_MILLIS then
		self:remove()
	end
end

event:register()
