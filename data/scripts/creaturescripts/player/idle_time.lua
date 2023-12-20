local MAX_IDLE_TIME = 15 * 60000 -- 15 minutes
local KICK_AFTER_WARN = 1 * 60000 -- 1 minute

-- register idle time event
local event = CreatureEvent("RegisterIdleTime")

function event.onLogin(player)
	if not player:getGroup():getAccess() then
		player:registerEvent("IdleTime")
	end
	return true
end

event:register()

-- check player idle time
event = CreatureEvent("IdleTime")

function event.onThink(player, interval)
	local tile = Tile(player:getPosition())
	if not tile or tile:hasFlag(TILESTATE_NOLOGOUT) then
		return true
	end

	local time = player:getIdleTime() + interval
	player:setIdleTime(time)
	
	if time > MAX_IDLE_TIME + KICK_AFTER_WARN then
		player:remove()
	elseif player:hasClient() and time == MAX_IDLE_TIME then
		player:sendTextMessage(MESSAGE_STATUS_WARNING, "There was no variation in your behaviour for " .. kickAfterMinutes .. " minutes. You will be disconnected in one minute if there is no change in your actions until then.")
	end
	return true
end

event:register()
