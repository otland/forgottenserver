-- If you don't intend to use actions.xml, you can delete this file.
local function getIds(singleIdKey, fromIdKey, toIdKey)
    return function(node)
        local itemid = node:attribute(singleIdKey)
        if itemid then
            local ids = {}
            for _, itemid in ipairs(itemid:split(";")) do ids[#ids + 1] = tonumber(itemid) end
            return ids
        end

        local fromid = tonumber(node:attribute(fromIdKey))
        if not fromid then return {} end

        local toid = tonumber(node:attribute(toIdKey))
        if not toid then
            logError("[Error] Missing attribute " .. toIdKey .. ", check data/actions/actions.xml.")
            return {}
        end

        local ids = {}
        for id = fromid, toid do ids[#ids + 1] = id end
        return ids
    end
end

local getItemIds = getIds("itemid", "fromid", "toid")
local getActionIds = getIds("actionid", "fromaid", "toaid")
local getUniqueIds = getIds("uniqueid", "fromuid", "touid")

local function configureActionEvent(node)
    local action = Action()

    local itemIds = getItemIds(node)
    if #itemIds == 0 then
        local uniqueIds = getUniqueIds(node)

        if #uniqueIds == 0 then
            local actionIds = getActionIds(node)

            if #actionIds == 0 then
                logError("[Error] Missing attribute itemid or uniqueid or actionid, check 'data/actions/actions.xml'.")
                return nil
            end

            action:aid(unpack(actionIds))
        else
            action:uid(unpack(uniqueIds))
        end
    else
        action:id(unpack(itemIds))
    end

    local allowFarUse = tobool(node:attribute("allowfaruse"))
    if allowFarUse ~= nil then action:allowFarUse(allowFarUse) end

    local blockWalls = tobool(node:attribute("blockwalls"))
    if blockWalls ~= nil then action:blockWalls(blockWalls) end

    local checkFloor = tobool(node:attribute("checkfloor"))
    if checkFloor ~= nil then action:checkFloor(checkFloor) end

    local function_ = node:attribute("function")
    local script = node:attribute("script")
    if not function_ and not script then
        logError("[Warning] function or script attribute missing for action '" .. name .. "'.")
        return nil
    end

    if function_ then
        if function_ ~= "market" then
            logError("[Error] Invalid function attribute, check 'data/actions/actions.xml'.")
            return nil
        end

        function action.onUse(player, item, fromPosition, target, toPosition, isHotkey) return player:sendEnterMarket() end
    end

    if script then
        local scriptFile = "data/actions/scripts/" .. script
        dofile(scriptFile)
        if not onUse then
            logError("[Error] Can not load action script, check '" .. scriptFile .. "' for a missing onUse callback")
            return nil
        end

        action:onUse(onUse)

        -- let it be garbage collected
        onUse = nil
    end

    return action
end

local function loadXMLActions()
    local doc = XMLDocument("data/actions/actions.xml")
    if not doc then
        logError("[Warning - Scripts::XML::loadXMLActions] Could not load actions.xml.")
        return
    end

    local actions = doc:child("actions")

    logInfo(">> Loading legacy XML actions from data/actions/actions.xml...")

    local loaded, start = 0, os.mtime()
    for node in actions:children() do
        local action = configureActionEvent(node)
        if action then
            action:register()
            loaded = loaded + 1
        end
    end

    logInfo(">> Loaded " .. loaded .. " actions in " .. os.mtime() - start .. "ms.")
end

loadXMLActions()
