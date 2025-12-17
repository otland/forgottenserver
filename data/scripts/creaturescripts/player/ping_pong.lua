local event = CreatureEvent("PingPong")

local LOST_CONNECTION_SECS = 5
local REMOVE_TARGET_PLAYER_SECS = 7
local PZ_LOCKED_NO_PONG_KICK_SECS = 60

function event.onThink(player, interval)
    local timeNow = os.time()

    local hasLostConnection = false
    if timeNow - player:getLastPing() >= LOST_CONNECTION_SECS then
        player:setLastPing(timeNow)
        if player:getClient() then
            local msg = NetworkMessage()
            msg:addByte(0x1D)
            msg:sendToPlayer(player)
            msg:delete()
        else
            hasLostConnection = true
        end
    end

    local noPongTime = timeNow - player:getLastPong()
    if hasLostConnection or noPongTime >= REMOVE_TARGET_PLAYER_SECS then
        local target = player:getTarget()
        if target and target:isPlayer() then
            player:setTarget(nil)
        end
    end

    local noPongKickTime = player:getVocation():getNoPongKickTime() / 1000
    if player:isPzLocked() and noPongKickTime < PZ_LOCKED_NO_PONG_KICK_SECS then
        noPongKickTime = PZ_LOCKED_NO_PONG_KICK_SECS
    end

    if noPongTime >= noPongKickTime then
        if player:getTile():hasFlag(TILESTATE_NOLOGOUT) then
            return true
        end

        player:remove()
    end
    return true
end

event:register()
