local attr = {
    --ATTR_DESCRIPTION = 1,
    --ATTR_EXT_FILE = 2,
    ATTR_TILE_FLAGS = 3,
    ATTR_ACTION_ID = 4,
    ATTR_UNIQUE_ID = 5,
    ATTR_TEXT = 6,
    ATTR_DESC = 7,
    ATTR_TELE_DEST = 8,
    ATTR_ITEM = 9,
    ATTR_DEPOT_ID = 10,
    --ATTR_EXT_SPAWN_FILE = 11,
    ATTR_RUNE_CHARGES = 12,
    --ATTR_EXT_HOUSE_FILE = 13,
    ATTR_HOUSEDOORID = 14,
    ATTR_COUNT = 15,
    ATTR_DURATION = 16,
    ATTR_DECAYING_STATE = 17,
    ATTR_WRITTENDATE = 18,
    ATTR_WRITTENBY = 19,
    ATTR_SLEEPERGUID = 20,
    ATTR_SLEEPSTART = 21,
    ATTR_CHARGES = 22,
    ATTR_CONTAINER_ITEMS = 23,
    ATTR_NAME = 24,
    ATTR_ARTICLE = 25,
    ATTR_PLURALNAME = 26,
    ATTR_WEIGHT = 27,
    ATTR_ATTACK = 28,
    ATTR_DEFENSE = 29,
    ATTR_EXTRADEFENSE = 30,
    ATTR_ARMOR = 31,
    ATTR_HITCHANCE = 32,
    ATTR_SHOOTRANGE = 33,
}


local function addI32(msg, x)
    local b4=x%256  x=(x-x%256)/256
    local b3=x%256  x=(x-x%256)/256
    local b2=x%256  x=(x-x%256)/256
    local b1=x%256  x=(x-x%256)/256
    msg:addByte(b4)
    msg:addByte(b3)
    msg:addByte(b2)
    msg:addByte(b1)
end

local function addI8(msg, x)
    msg:addByte(x%256)
end


local attributes = {
    ITEM_ATTRIBUTE_ARTICLE = {"string", "ATTR_ARTICLE"},
    ITEM_ATTRIBUTE_PLURALNAME = {"string", "ATTR_PLURALNAME"},
    ITEM_ATTRIBUTE_WEIGHT = {"u32", "ATTR_WEIGHT"},
    ITEM_ATTRIBUTE_DECAYSTATE = {"byte", "ATTR_DECAYING_STATE"},
    ITEM_ATTRIBUTE_DURATION = {"u32", "ATTR_DURATION"},
    ITEM_ATTRIBUTE_WRITER = {"string", "ATTR_WRITTENBY"},
    ITEM_ATTRIBUTE_TEXT = {"string", "ATTR_TEXT"},
    ITEM_ATTRIBUTE_DATE = {"u32", "ATTR_WRITTENDATE"},
    ITEM_ATTRIBUTE_ATTACK = {"i32", "ATTR_ATTACK"},
    ITEM_ATTRIBUTE_DEFENSE = {"i32", "ATTR_DEFENSE"},
    ITEM_ATTRIBUTE_EXTRADEFENSE = {"i32", "ATTR_EXTRADEFENSE"},
    ITEM_ATTRIBUTE_ARMOR = {"i32", "ATTR_ARMOR"},
    ITEM_ATTRIBUTE_HITCHANCE = {"i8", "ATTR_HITCHANCE"},
    ITEM_ATTRIBUTE_SHOOTRANGE = {"byte", "ATTR_SHOOTRANGE"}
}

local function serializeAttributes(item)
    local itemType = item:getType()
    local msg = NetworkMessage()
    local size = 0

    if itemType:isStackable() or itemType:isFluidContainer() then
        msg:addByte(attr.ATTR_COUNT)
        msg:addByte(item:getSubType())
        size = size + 2
    end

    local charges = item:getCharges()
    if charges and charges ~= 0 then
        msg:addByte(attr.ATTR_CHARGES)
        msg:addU16(charges)
        size = size + 3
    end

    if itemType:isMovable() then
        local actionId = item:getActionId()
        if(actionId ~= 0) then
            msg:addByte(attr.ATTR_ACTION_ID)
            msg:addU16(actionId)
            size = size + 3
        end
    end

-- Only necessary with special descriptions
--[[local desc = item:getDescription(0)
    if #desc ~= 0 then
        msg:addByte(attr.ATTR_DESC)
        msg:addString(desc)
        size = size + #desc + 3
    end
]]

-- Only necessary with special names
--[[
    local name = item:getName()
    if name and name ~= 0 then
        msg:addByte(attr.ATTR_NAME)
        msg:addString(name)
        size = size + #name + 3
    end
]]

    for attribute, p in pairs(attributes) do
        local value = item:getAttribute(attribute)
        if(p[1] == "string") then
            if(value and #value ~= 0) then
                msg:addByte(attr[p[2]])
                msg:addString(value)
                size = size + #value + 3
            end
        elseif(value and value ~= 0) then
            local t, func, n = p[1]
            if(t == "byte") then 
                func, n = msg.addByte, 1 
            elseif(t == "i8") then 
                func, n = addI8, 1 
            elseif(t == "u32") then
                func, n = msg.addU32, 4
            elseif(t == "i32") then
                func, n = addI32, 4
            end
            msg:addByte(attr[p[2]])
            func(msg, value)
            size = size + 1 + n
        end
    end

    msg:skipBytes(-size)
    local buffer = {}
    for i = 1, size do
        table.insert(buffer, string.char(msg:getByte()))
    end

    return table.concat(buffer)
end

local function pushValues(buffer, sep, ...)
    local argv = {...}
    local argc = #argv
    for k, v in ipairs(argv) do
        table.insert(buffer, v)
        if k < argc and sep then
            table.insert(buffer, sep)
        end
    end
end

local function insertItems(buffer, info, parent, items)
    local start = info.running
    for _, item in ipairs(items) do
        if _ ~= 1 or parent > 100 then
            table.insert(buffer, ",")
        end
        info.running = info.running + 1
        table.insert(buffer, "(")        
        pushValues(buffer, ",", info.playerGuid, parent, info.running, item:getId(), item:getSubType(), db.escapeString(serializeAttributes(item)))
        table.insert(buffer, ")")

        if item:isContainer() then
            local size = item:getSize()
            if size > 0 then              
                local subItems = {}
                for i = 1, size do
                    table.insert(subItems, item:getItem(i - 1))
                end

                insertItems(buffer, info, info.running, subItems)
            end
        end
    end
    return info.running - start
end

local function insertRewardBag(playerGuid, timestamp, itemList)
    db.asyncStoreQuery('select `pid`, `sid` from `player_rewards` where player_id = ' .. playerGuid .. ' order by `sid` ASC;', 
        function(query)
            local lastReward = 0
            local lastStoreId   
            if(query) then             
                repeat
                    local sid = result.getDataInt(query, 'sid')
                    local pid = result.getDataInt(query, 'pid')

                    if pid < 100 then
                        lastReward = pid
                    end
                    lastStoreId = sid
                until not result.next(query)
            end

            local buffer = {'INSERT INTO `player_rewards` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES'}

            --reward bag
            local info = {
                playerGuid = playerGuid,
                running = lastStoreId or 100
            }

            local bag = Game.createItem(ITEM_REWARD_CONTAINER)
            bag:setAttribute(ITEM_ATTRIBUTE_DATE, timestamp)
            if itemList then
                for _, p in ipairs(itemList) do
                    bag:addItem(p[1], p[2])
                end
            end

            local total = insertItems(buffer, info, lastReward + 1, {bag})
            table.insert(buffer, ";")

            if(total ~= 0) then
                db.query(table.concat(buffer))
            end
        end
    )
end

local function getPlayerStats(bossId, playerGuid, autocreate)
    local ret = globalBosses[bossId][playerGuid]
    if not ret and autocreate then
        ret = {
            bossId = bossId, 
            damageIn = 0, -- damage taken from the boss
            healing = 0, -- healing (other players) done
        }
        globalBosses[bossId][playerGuid] = ret
        return ret
    end
    return ret
end

function onDeath(creature, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
    local monsterType = creature:getType()
    if monsterType:isRewardBoss() then -- Make sure it is a boss
        local bossId = creature:getId()
        local timestamp = os.time()
        corpse:setAttribute(ITEM_ATTRIBUTE_DATE, timestamp)

        local totalDamageOut, totalDamageIn, totalHealing = 0.1, 0.1, 0.1 -- avoid dividing by zero

        local scores = {}
        local info = globalBosses[bossId]
        local damageMap = creature:getDamageMap()

        for guid, stats in pairs(info) do
            local player = Player(stats.playerId)
            local part = damageMap[stats.playerId]
            local damageOut, damageIn, healing = (stats.damageOut or 0) + (part and part.total or 0), stats.damageIn or 0, stats.healing or 0

            totalDamageOut = totalDamageOut + damageOut
            totalDamageIn = totalDamageIn + damageIn
            totalHealing = totalHealing + healing

            table.insert(scores, {
                player = player, 
                guid = guid,
                damageOut = damageOut, 
                damageIn = damageIn,
                healing = healing,
            })            
        end

        local participants = 0
        for _, con in ipairs(scores) do
            local score = (con.damageOut / totalDamageOut) + (con.damageIn / totalDamageIn) + (con.healing / totalHealing)
            con.score = score / 3 -- normalize to 0-1
            if score ~= 0 then
                participants = participants + 1
            end
        end
        table.sort(scores, function(a, b) return a.score > b.score end)

        local expectedScore = 1 / participants

        for _, con in ipairs(scores) do
            local reward, stamina -- ignoring stamina for now because I heard you get receive rewards even when it's depleted   
            if con.player then   
                reward = con.player:getReward(timestamp, true)
                stamina = con.player:getStamina()
            else
                stamina = con.stamina or 0
            end

            local playerLoot
            if --[[stamina > 840 and]] con.score ~= 0 then
                local lootFactor = 1
                lootFactor = lootFactor / participants ^ (1 / 3) -- tone down the loot a notch if there are many participants
                lootFactor = lootFactor * (1 + lootFactor) ^ (con.score / expectedScore) -- increase the loot multiplicatively by how many times the player surpassed the expected score
                playerLoot = monsterType:getBossReward(lootFactor, _ == 1)

                if con.player then
                    for _, p in ipairs(playerLoot) do
                        reward:addItem(p[1], p[2])
                    end
                end
            end

            if con.player then
                local lootMessage = {"The following items are available in your reward chest: "}

                if --[[stamina > 840]]true then
                    reward:getContentDescription(lootMessage)
                else
                    table.insert(lootMessage, 'nothing (due to low stamina)')
                end
                table.insert(lootMessage, ".")
                con.player:sendTextMessage(MESSAGE_EVENT_ADVANCE, table.concat(lootMessage))
            else
                insertRewardBag(con.guid, timestamp, playerLoot)
            end
        end

        globalBosses[bossId] = nil
    end
    return true
end

function onThink(creature, interval)
    local bossId = creature:getId()
    local info = globalBosses[bossId]
    -- Reset all players' status
    for _, player in pairs(info) do
        player.active = false
    end
    -- Set all players in boss' target list as active in the fight
    local targets = creature:getTargetList()
    for _, target in ipairs(targets) do
        if target:isPlayer() then
            local stats = getPlayerStats(bossId, target:getGuid(), true)
            stats.playerId = target:getId() -- Update player id
            stats.active = true            
        end
    end
end

function onHealthChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
    if not next(globalBosses) then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    if not creature or not attacker then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    local stats = creature:inBossFight()
    if not stats then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    local creatureId, attackerId = creature:getId(), attacker:getId()
    stats.playerId = creatureId -- Update player id

    -- Account for healing of others active in the boss fight
    if primaryType == COMBAT_HEALING and attacker:isPlayer() and attackerId ~= creatureId then
        local healerStats = getPlayerStats(stats.bossId, attacker:getGuid(), true)
        healerStats.active = true
        healerStats.playerId = attackerId -- Update player id
        healerStats.healing = healerStats.healing + primaryDamage
    elseif stats.bossId == attackerId then
        -- Account for damage taken from the boss
        stats.damageIn = stats.damageIn + primaryDamage
    end
    return primaryDamage, primaryType, secondaryDamage, secondaryType
end