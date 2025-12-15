local IDLE_LIMIT_MILLIS = 2 * 60 * 1000
local GRACE_PERIOD_MILLIS = 1 * 60 * 1000

local event = CreatureEvent("Idle Timeout")

function event.onThink(player, interval)
	if player:getGroup():getAccess() then
		return
	end

	local tile = player:getTile()
	if not tile or tile:hasFlag(TILESTATE_NOLOGOUT) then
		return
	end

	player:setIdleTime(player:getIdleTime() + interval)

	local idleTime = player:getIdleTime()
	if idleTime == IDLE_LIMIT_MILLIS then
		player:sendTextMessage(MESSAGE_STATUS_WARNING, "There was no variation in your behaviour for " .. IDLE_LIMIT_MILLIS / 60000 .. " minutes. You will be disconnected in one minute if there is no change in your actions until then.")
		return
	end

	if idleTime > IDLE_LIMIT_MILLIS + GRACE_PERIOD_MILLIS then
		player:remove()
	end
end

event:register()
