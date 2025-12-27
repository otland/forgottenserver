local LOST_CONNECTION_SECS = 5
local REMOVE_TARGET_PLAYER_SECS = 7
local PZ_LOCKED_NO_PONG_KICK_SECS = 60

local event = Event()

event.onCreatureThink = function(self, interval)
    if not self:isPlayer() then
        return
    end

    local timeNow = os.time()

    local hasLostConnection = false
    if timeNow - self:getLastPing() >= LOST_CONNECTION_SECS then
        self:setLastPing(timeNow)
        if self:getClient() then
            local msg = NetworkMessage()
            msg:addByte(0x1D)
            msg:sendToPlayer(self)
            msg:delete()
        else
            hasLostConnection = true
        end
    end

    local noPongTime = timeNow - self:getLastPong()
    if hasLostConnection or noPongTime >= REMOVE_TARGET_PLAYER_SECS then
        local target = self:getTarget()
        if target and target:isPlayer() then
            self:setTarget(nil)
        end
    end

    local noPongKickTime = self:getVocation():getNoPongKickTime() / 1000
    if self:isPzLocked() and noPongKickTime < PZ_LOCKED_NO_PONG_KICK_SECS then
        noPongKickTime = PZ_LOCKED_NO_PONG_KICK_SECS
    end

    if noPongTime >= noPongKickTime then
        if self:getTile():hasFlag(TILESTATE_NOLOGOUT) then
            return
        end

        self:remove()
    end
end

event:register()
