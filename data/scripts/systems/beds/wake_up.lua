local event = CreatureEvent("BedLogin")

local function findRelevantBed(player)
    local pos = player:getPosition()
    local tile = Tile(pos)
    if not tile then
        return nil, nil
    end

    local headboard = tile:getHeadboard()
    if headboard then
        local footboard = headboard:getPartnerBed()
        if footboard and footboard:isFootboard() then
            return headboard, footboard
        end
    end

    local directions = {DIRECTION_NORTH, DIRECTION_EAST, DIRECTION_SOUTH, DIRECTION_WEST}
    for _, dir in ipairs(directions) do
        local checkPos = Position(pos)
        checkPos:getNextPosition(dir)
        local adjacentTile = Tile(checkPos)
        if adjacentTile then
            local headboard = adjacentTile:getHeadboard()
            if headboard then
                local footboard = headboard:getPartnerBed()
                if footboard and footboard:isFootboard() then
                    return headboard, footboard
                end
            end
        end
    end

    return nil, nil
end

local function regeneratePlayer(player, sleptSeconds)
    if sleptSeconds <= 0 then
        return
    end

    local regenWindow = math.min(sleptSeconds, Beds.HealthManaMaxSeconds)
    local regenTicks = math.floor(regenWindow / Beds.HealthManaTickSeconds) * Beds.HealthManaPerTick
    if regenTicks > 0 then
        player:addHealth(regenTicks)
        player:addMana(regenTicks)
    end

    local soulTicks = math.floor(sleptSeconds / Beds.SoulTickSeconds) * Beds.SoulPerTick
    if soulTicks > 0 then
        player:addSoul(soulTicks)
    end
end

function event.onLogin(player)
    local headboard, footboard = findRelevantBed(player)
    if not headboard or not footboard or headboard:getSleeper() ~= player:getGuid() then
        return true
    end

    local lastLogout = player:getLastLogout()
    local sleptSeconds = 0
    if lastLogout > 0 then
        sleptSeconds = math.max(0, os.time() - lastLogout)
    end

    regeneratePlayer(player, sleptSeconds)

    headboard:removeSleeper()
    footboard:removeSleeper()
    return true
end

event:register()
