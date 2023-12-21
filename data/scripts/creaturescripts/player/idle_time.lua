local config = {
    maxIdleTime = 15 * 60 * 1000, -- 15 minutes
    kickAfterWarn = 1 * 60 * 1000 -- 1 minute
}

-- register player event
local event = CreatureEvent("RegisterIdleTime")

function event.onLogin(player)
	if not player:getGroup():getAccess() then
		player:registerEvent("IdleTime")
	end
	return true
end

event:register()

-- event to check player idle time
event = CreatureEvent("IdleTime")

function event.onThink(player, interval)
	local tile = Tile(player:getPosition())
	if not tile or tile:hasFlag(TILESTATE_NOLOGOUT) then
		return true
	end

	local idleTime = player:getIdleTime() + interval
	player:setIdleTime(idleTime)

    if idleTime == config.maxIdleTime then
        player:sendTextMessage(MESSAGE_STATUS_WARNING, "There was no variation in your behaviour for " .. (config.maxIdleTime / (60 * 1000)) .. " minutes. You will be disconnected in one minute if there is no change in your actions until then.")
    elseif idleTime > (config.maxIdleTime + config.kickAfterWarn) then
        player:remove()
	end
	return true
end

event:register()
