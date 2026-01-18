local action = Action()

local function getBedParts(item)
    local headboard = item and item:getBed()
    if not headboard or not headboard:isHeadboard() then
        return nil, nil
    end

    local footboard = headboard:getPartnerBed()
    if not footboard or not footboard:isFootboard() then
        return nil, nil
    end

    return headboard, footboard
end

local function canEvictSleeper(player, house)
    if not house then
        return false
    end

    return player:getGroup():getAccess() or house:getOwnerGuid() == player:getGuid()
end

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    local headboard, footboard = getBedParts(item)
    if not headboard or not footboard then
        return true
    end

    local tile = Tile(headboard:getPosition())
    if not tile then
        return true
    end

    local house = tile:getHouse()
    if not house then
        return true
    end

    if not canEvictSleeper(player, house) then
        return true
    end

    headboard:removeSleeper()
    footboard:removeSleeper()
    return true
end

local function getOccupiedHeadboardIds()
    local headboards = {}
    for id, bed in pairs(Game.getBeds()) do
        if bed.occupied and (bed.partnerDirection == DIRECTION_SOUTH or bed.partnerDirection == DIRECTION_EAST) then
            table.insert(headboards, id)
        end
    end
    return headboards
end

action:id(table.unpack(getOccupiedHeadboardIds()))
action:register()
