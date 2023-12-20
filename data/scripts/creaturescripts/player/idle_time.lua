local config = {
    maxIdleTime = 15 * 60 * 1000, -- 15 minutes
    kickAfterWarn = 1 * 60 * 1000 -- 1 minute
}

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
	
	if time > config.maxIdleTime + config.kickAfterWarn then
		player:remove()
	elseif player:hasClient() and time == config.maxIdleTime then
		player:sendTextMessage(MESSAGE_STATUS_WARNING, "There was no variation in your behaviour for " .. kickAfterMinutes .. " minutes. You will be disconnected in one minute if there is no change in your actions until then.")
	end
	return true
end

event:register()
