-- constants
MESSAGE_TAGS = {
    playerName = { tag = "|PLAYERNAME|", func = function(playerName, amount, total, itemName) return playerName end },
    itemCount = { tag = "|ITEMCOUNT|", func = function(playerName, amount, total, itemName) return amount end },
    totalCost = { tag = "|TOTALCOST|", func = function(playerName, amount, total, itemName) return total end},
    itemName = { tag = "|ITEMNAME|", func = function(playerName, amount, total, itemName) return itemName end}
}

MESSAGE_LIST = {
    needMoney = "You need more money",
    needSpace = "You do not have enough capacity.",
    needMoreSpace = "You do not have enough capacity for all items.",
    bought = "You bought |ITEMCOUNT| |ITEMNAME|(s) for |TOTALCOST| gold",
    sold = "You sold |ITEMCOUNT| |ITEMNAME|(s) for |TOTALCOST| gold"
}

KEYWORDS_GREET = {
    "hi",
    "hello",
    "hey",
    "greetings"
}

MESSAGES_GREET = {
    "Hello |PLAYERNAME| how can I help you?",
    "Greetings |PLAYERNAME|, what can I do for you?",
    "Hi |PLAYERNAME|, what's your desire today?"
}

KEYWORDS_FAREWELL = {
    "bye",
    "goodbye",
    "farewell",
    "cya",
    "ciao"
}

MESSAGES_FAREWELL = {
    "Goodbye |PLAYERNAME|, have a nice day!",
    "Farewell |PLAYERNAME|, see you soon!",
    "See you later |PLAYERNAME|, take care!"
}

FOCUS = {
    -- how long the npc will focus the player in seconds
    time = 60,
    -- how far the player can step away until the npc loses focus
    distance = 5,
    -- how near the player has to be to greet the npc
    greetDistance = 3
}

TALK = {
    -- how long in ms the npc will wait before responding
    defaultDelay = 1000
}

function string:replaceTags(playerName, amount, total, itemName)
    local playerName = playerName and playerName or ""
    local amount = amount and amount or ""
    local total = total and total or ""
    local itemName = itemName and itemName or ""
    local ret = self
    for _, handler in pairs(MESSAGE_TAGS) do
        ret = ret:gsub(handler.tag, handler.func(playerName, amount, total, itemName))
    end
    return ret
end

function Npc:respond(message, player, delay)
    delay = delay and delay or TALK.defaultDelay
    local talkQueue = NpcTalkQueue(self)
    talkQueue:addToQueue(player, message, delay)
end

-- This function sets the default Event behavior for a NPC
function Npc:defaultBehavior()
    self.onAppear = function(creature)
        return true
    end
    self.onDisappear = function(creature)
        if not creature:isPlayer() then
            return true
        end
        local focus = NpcFocus(self)
        if focus:isFocused(creature) then
            focus:removeFocus(creature)
            local talkQueue = NpcTalkQueue(self)
            talkQueue:clearQueue(creature)
        end
        return true
    end
    -- The onThink function is called when the NPC thinks
    self.onThink = function()
        local handler = NpcsHandler(self)
        local focus = NpcFocus(self)
        for playerid, releaseTime in pairs(focus.focus) do
            local player = Player(playerid)
            if getDistanceTo(player:getId()) >= FOCUS.distance or releaseTime < os.time() then
                focus:removeFocus(player)
                closeShopWindow(player)
                selfSay(handler.farewellWords[math.random(1, #handler.farewellWords)]:replaceTags(player:getName()), player)
            end
        end

        local talkQueue = NpcTalkQueue(self)
        talkQueue:processQueue()

        if #focus.focus == 0 then
            doNpcSetCreatureFocus(nil)
        else
            local player = focus:getCurrentFocus()
            if player then
                if player:getPosition().y > self:getPosition().y and player:getPosition().x == self:getPosition().x then
                    selfTurn(DIRECTION_SOUTH)
                elseif player:getPosition().y < self:getPosition().y and player:getPosition().x == self:getPosition().x then
                    selfTurn(DIRECTION_NORTH)
                elseif player:getPosition().x > self:getPosition().x and player:getPosition().y == self:getPosition().y then
                    selfTurn(DIRECTION_EAST)
                elseif player:getPosition().x < self:getPosition().x and player:getPosition().y == self:getPosition().y then
                    selfTurn(DIRECTION_WEST)
                end
            end
        end
        return true
    end
    -- The onSay function is called when the Player says something to the NPC
    self.onSay = function(creature, messageType, message)
        local message = message:lower()
        -- Creates a new KeywordsHandler instance for the NPC
        local handler = NpcsHandler(self)
        local focus = NpcFocus(self)
        local talkQueue = NpcTalkQueue(self)

        if not focus:isFocused(creature) then
            -- If the player is not focused, the NPC will greet the player if the player says a greeting word and is in range
            if getDistanceTo(creature:getId()) > FOCUS.greetDistance then
                return
            end

            for _, word in pairs(KEYWORDS_GREET) do
                if message == word then
                    focus:addFocus(creature)
                    doNpcSetCreatureFocus(creature:getId())
                    local msg = handler.greetWords[math.random(1, #handler.greetWords)]:replaceTags(creature:getName())
                    talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                    handler:setTopic(handler, creature)
                    return true
                end
            end
        else
            -- If the player is focused, the NPC will say goodbye if the player says a farewell word
            for _, word in pairs(KEYWORDS_FAREWELL) do
                if message == word then
                    focus:removeFocus(creature)
                    closeShopWindow(creature)
                    local msg = handler.farewellWords[math.random(1, #handler.farewellWords)]:replaceTags(creature:getName())
                    talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                    return true
                end
            end
        end

        -- Checks if the NPC has a response for the given message
        if handler:getTopic(creature):isKeyword(message) then
            -- renewing the focus for the player
            focus:addFocus(creature)
            -- If the NPC has a shop for the message, it opens the shop window
            if handler:getTopic(creature):getShop(message) then
                handler:setActiveShop(creature, handler:getTopic(creature):getShop(message))
                local shop = NpcShop(self, handler:getActiveShop(creature))
                local items = shop:getItems()
                if shop:hasDiscount(creature) then
                    local afterDiscount = {}
                    for _, item in pairs(items) do
                        table.insert(afterDiscount, {
                            id = item.id, name = item.name, 
                            buy = (item.buy - math.ceil(item.buy / 100 * (shop:hasDiscount(creature) and shop:hasDiscount(creature) or 0))),
                            sell = item.sell,
                            subtype = item.subtype == nil and nil or item.subtype
                        })
                    end
                    self:openShopWindow(creature, afterDiscount, shop.onBuy, shop.onSell)
                else
                    self:openShopWindow(creature, items, shop.onBuy, shop.onSell)
                end
            end
            -- If the NPC has a response, it sets the topic to the one associated with the message
            handler:setTopic(handler:getTopic(creature):isKeyword(message), creature)
            -- check if we have a callback for this topic
            if handler:getTopic(creature).callback then
                if not handler:getTopic(creature):callback(Npc(getNpcCid()), creature) then
                    handler:setTopic(handler, creature)
                    return
                end
            end
            -- If the NPC has a response for the current topic, it says the response
            if handler:getTopic(creature):getResponse() then
                local msg = handler:getTopic(creature):getResponse():replaceTags(creature:getName())
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
            end
            -- If the NPC has a resetTopic, it resets the topic
            if handler:getTopic(creature).resetTopic then
                handler:setTopic(handler, creature)
            end
        else
            -- If the NPC doesn't have a response, it says a default message
            selfSay("What do you want from me!?!", creature)
        end
    end
end
    
    -- This block of code creates a new metatable for the KeywordsHandler table
if not NpcsHandler then
    -- If KeywordsHandler doesn'handler exist, it's created as an empty table
    NpcsHandler = {}
    -- The metatable is set up to call the function when KeywordsHandler is called
    setmetatable(NpcsHandler, {
        __call = function(self, npc)
            -- If the NPC doesn'handler have a KeywordsHandler, one is created for it
            if not self[npc:getName()] then
                self[npc:getName()] = {
                    keywords = {},
                    topic = {},
                    shopActive = {},
                    greetWords = MESSAGES_GREET,
                    farewellWords = MESSAGES_FAREWELL
                }
                setmetatable(self[npc:getName()], {__index = NpcsHandler})
                setmetatable(self[npc:getName()].keywords, {__index = NpcsHandler})
            end
            -- The KeywordsHandler is returned
            return self[npc:getName()]
        end
    })
    
    -- This function adds a keyword to the KeywordsHandler for the NPC
    function NpcsHandler:keyword(word)
        -- If the keyword doesn'handler exist, it's created
        if not self.keywords[word] then
            self.keywords[word] = {}
            self.keywords[word].keywords = {}
            setmetatable(self.keywords[word], {__index = NpcsHandler})
            setmetatable(self.keywords[word].keywords, {__index = NpcsHandler})
            self.keywords[word].response = ""
        end
        return self.keywords[word]
    end
    
    -- This function returns the keywords for the NPC
    function NpcsHandler:getKeywords()
        return self.keywords
    end

    function NpcsHandler:isKeyword(word)
        return self.keywords[word] and self.keywords[word] or false
    end
    
    -- This function returns the current topic of Player for the NPC
    function NpcsHandler:getTopic(player)
        return self.topic[player:getGuid()] == nil and self or self.topic[player:getGuid()]
    end
    
    -- This function sets the current topic of Player for the NPC
    function NpcsHandler:setTopic(topic, player)
        self.topic[player:getGuid()] = topic
    end
    
    -- This function adds a response to the KeywordsHandler for the NPC
    function NpcsHandler:respond(text)
        -- If topic is not provided, it's set to 1
        self.response = text
    end
    
    -- This function returns the responses for the NPC
    function NpcsHandler:getResponse()
        return self.response
    end

    function NpcsHandler:shop(id)
        self.openShop = id
    end

    function NpcsHandler:getShop(word)
        return not self.keywords[word].openShop and false or self.keywords[word].openShop
    end

    function NpcsHandler:setActiveShop(player, id)
        self.shopActive[player:getGuid()] = id
    end

    function NpcsHandler:getActiveShop(player)
        return self.shopActive[player:getGuid()]
    end

    function NpcsHandler:setGreetRespond(texts)
        if type(texts) == "string" then
            texts = {texts}
        end
        self.greetWords = texts
    end

    function NpcsHandler:setFarewellRespond(texts)
        if type(texts) == "string" then
            texts = {texts}
        end
        self.farewellWords = texts
    end

    function NpcsHandler:resetTalkState()
        self.resetTopic = true
    end
end

    
-- This block of code creates a new metatable for the KeywordsHandler table
if not NpcShop then
    -- If KeywordsHandler doesn'handler exist, it's created as an empty table
    NpcShop = {}
    -- The metatable is set up to call the function when KeywordsHandler is called
    setmetatable(NpcShop, {
        __call = function(self, npc, topic)
            -- If the NPC doesn'handler have a KeywordsHandler, one is created for it
            if not self[npc:getName()] then
                self[npc:getName()] = {}
            end
            if not self[npc:getName()][topic] then
                self[npc:getName()][topic] = {
                    items = {},
                    discounts = {}
                }
                setmetatable(self[npc:getName()][topic], {__index = NpcShop})
            end
            -- The KeywordsHandler is returned
            return self[npc:getName()][topic]
        end
    })

    function NpcShop:addItems(shop)
        for id, items in pairs(shop) do
            local found = false
            for _, item in pairs(self.items) do
                if item.id == id then
                    found = true
                    break
                end
            end
            if not found then
                table.insert(self.items, {
                    id = id, name = ItemType(id):getName(), buy = items.buyPrice, sell = items.sellPrice, subtype = items.subType == nil and nil or items.subType
                })
            end
        end
    end

    function NpcShop:addItem(id, buyPrice, sellPrice, subType)
        local buy = buyPrice and buyPrice or 0
        local sell = sellPrice and sellPrice or 0
        local sub = subType and subType or 1
        local found = false
        for _, item in pairs(self.items) do
            if item.id == id then
                found = true
                break
            end
        end
        if not found then
            table.insert(self.items, {
                id = id, name = ItemType(id):getName(), buy = buy, sell = sell, subtype = sub
            })
        end
    end


    function NpcShop:addDiscount(storage, percent)
        local percent = percent and percent or 0
        self.discounts[storage] = percent
    end

    function NpcShop:hasDiscount(creature)
        for storage, percent in pairs(self.discounts) do
            if percent == 0 then
                if creature:getStorageValue(storage) > 0 then
                    return creature:getStorageValue(storage)
                end
            end
            if creature:getStorageValue(storage) > 0 then
                return percent
            end
        end
        return false
    end

    function NpcShop:getItems()
        return self.items
    end

    function NpcShop:getItem(itemid, subType)
        for _, item in pairs(self.items) do
            if item.id == itemid then
                if item.subtype then
                    if item.subType == subType then
                        return item
                    end
                else
                    return item
                end
            end
        end
        return false
    end

    NpcShop.onBuy = function(player, itemid, subType, amount, ignoreCap, inBackpacks)
        local npc = Npc(getNpcCid())
        local npcHandler = NpcsHandler(npc)
        local shop = NpcShop(npc, npcHandler:getActiveShop(player))
        local shopItem = shop:getItem(itemid, subType)
        local focus = NpcFocus(npc)
        local talkQueue = NpcTalkQueue(npc)
        if not shopItem then
            error("[NpcShop.onBuy] shopItem == nil")
            return false
        end

        if shopItem.buy == 0 then
            error("[NpcShop.onBuy] attempt to buy a non-buyable item")
            return false
        end

        local discount = shop:hasDiscount(player) and shop:hasDiscount(player) or 0
        local totalCost = amount * (shopItem.buy - math.ceil(shopItem.buy / 100 * discount))
        if inBackpacks then
            totalCost = ItemType(itemid):isStackable() and totalCost + 20 or totalCost + (math.max(1, math.floor(amount / ItemType(ITEM_SHOPPING_BAG):getCapacity())) * 20)
        end

        if player:getTotalMoney() < totalCost then
            local msg = MESSAGE_LIST.needMoney:replaceTags(player:getName(), amount, totalCost, shopItem.name)
            talkQueue:addToQueue(player, msg, TALK.defaultDelay)
            player:sendCancelMessage(msg)
            return false
        end

        local subType = shopItem.subType or 1
        local a, b = doNpcSellItem(player, itemid, amount, subType, ignoreCap, inBackpacks, ITEM_SHOPPING_BAG)
        if a < amount then
            local msgId = MESSAGE_LIST.needMoreSpace
            if a == 0 then
                msgId = MESSAGE_LIST.needSpace
            end

            local msg = msgId:replaceTags(player:getName(), amount, totalCost, shopItem.name)
            player:sendCancelMessage(msg)
            focus:addFocus(player)

            if a > 0 then
                if not player:removeTotalMoney((a * (shopItem.buy - math.ceil(shopItem.buy / 100 * discount))) + (b * 20)) then
                    return false
                end
                return true
            end

            return false
        else
            local msg = MESSAGE_LIST.bought:replaceTags(player:getName(), amount, totalCost, shopItem.name)
            talkQueue:addToQueue(player, msg, TALK.defaultDelay)
            if not player:removeTotalMoney(totalCost) then
                return false
            end
            focus:addFocus(player)
            return true
        end
    end

    NpcShop.onSell = function(player, itemid, subType, amount, ignoreEquipped, _)
        local npc = Npc(getNpcCid())
        local npcHandler = NpcsHandler(npc)
        local shop = NpcShop(npc, npcHandler:getActiveShop(player))
        local shopItem = shop:getItem(itemid, subType)
        local focus = NpcFocus(npc)
        local talkQueue = NpcTalkQueue(npc)
        if not shopItem then
            error("[NpcShop.onSell] items[itemid] == nil")
            return false
        end

        if shopItem.sell == 0 then
            error("[NpcShop.onSell] attempt to sell a non-sellable item")
            return false
        end

        if not ItemType(itemid):isFluidContainer() then
            subType = -1
        end

        if player:removeItem(itemid, amount, subType, true) then
            local msg = MESSAGE_LIST.sold:replaceTags(player:getName(), amount, amount * shopItem.sell, shopItem.name)
            talkQueue:addToQueue(player, msg, TALK.defaultDelay)
            player:addMoney(amount * shopItem.sell)
            focus:addFocus(player)
            return true
        end

        focus:addFocus(player)
        return false
    end
end

if not NpcFocus then
    -- If KeywordsHandler doesn'handler exist, it's created as an empty table
    NpcFocus = {}
    -- The metatable is set up to call the function when KeywordsHandler is called
    setmetatable(NpcFocus, {
        __call = function(self, npc)
            -- If the NPC doesn'handler have a KeywordsHandler, one is created for it
            if not self[npc:getId()] then
                self[npc:getId()] = {
                    focus = {},
                    currentFocus = nil
                }
                setmetatable(self[npc:getId()], {__index = NpcFocus})
            end
            -- The KeywordsHandler is returned
            return self[npc:getId()]
        end
    })

    function NpcFocus:addFocus(player)
        self.focus[player:getGuid()] = os.time() + FOCUS.time
        self.currentFocus = player
    end

    function NpcFocus:isFocused(player)
        if self.focus[player:getGuid()] then
            return self.focus[player:getGuid()] > os.time()
        end
        return false
    end

    function NpcFocus:removeFocus(player)
        self.focus[player:getGuid()] = nil
        if self.currentFocus == player then
            self.currentFocus = nil
        end
    end

    function NpcFocus:getCurrentFocus()
        return self.currentFocus
    end
end

if not NpcTalkQueue then
    -- If NpcTalkQueue doesn'handler exist, it's created as an empty table
    NpcTalkQueue = {}
    -- The metatable is set up to call the function when NpcTalkQueue is called
    setmetatable(NpcTalkQueue, {
        __call = function(self, npc)
            -- If the NPC doesn'handler have a NpcTalkQueue, one is created for it
            if not self[npc:getId()] then
                self[npc:getId()] = {
                    queue = {}
                }
                setmetatable(self[npc:getId()], {__index = NpcTalkQueue})
            end
            -- The KeywordsHandler is returned
            return self[npc:getId()]
        end
    })

    function NpcTalkQueue:addToQueue(player, message, delay)
        if not self.queue[player:getId()] then
            self.queue[player:getId()] = {}
        end
        table.insert(self.queue[player:getId()], {
            delay = os.mtime() + delay,
            message = message
        })
    end

    function NpcTalkQueue:processQueue()
        for playerid, queue in pairs(self.queue) do
            local player = Player(playerid)
            if player then
                for id, data in pairs(queue) do
                    if not Player(playerid) then
                        self.queue[playerid] = nil
                        break
                    end
                    if data.delay <= os.mtime() then
                        self.queue[playerid][id] = nil
                        selfSay(data.message, player)
                    end
                end
            else
                self.queue[playerid] = nil
            end
        end
    end

    function NpcTalkQueue:clearQueue(player)
        self.queue[player:getId()] = nil
    end
end
