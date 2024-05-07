--[[
    >> NpcEvents <<

    Description:
        - This module contains event functions related to NPCs.
        - These functions handle the behavior of NPCs when certain events occur, such as when a player appears, disappears, or says something.
        - The module provides functions for handling NPC appearances, disappearances, thinking, and speech.
        - The functions in this module are called by the NPC system to execute the corresponding behavior.
        - This module also includes callbacks that can be used to extend or modify the default behavior of NPCs.
        - The module is designed to be used in conjunction with other modules and scripts that define NPC behavior.

    Functions:
        - NpcEvents.onAppear(npc, creature)
        - NpcEvents.onMove(npc, oldPos, newPos)
        - NpcEvents.onPlayerCloseChannel(npc, creature)
        - NpcEvents.onPlayerEndTrade(npc, creature)
        - NpcEvents.onDisappear(npc, creature)
        - NpcEvents.onThink(npc)
        - NpcEvents.onSay(npc, creature, messageType, message)
]]

---@class NpcEvents
---@field onAppear fun(npc: Npc, creature: Creature)
---@field onMove fun(npc: Npc, oldPos: Position, newPos: Position)
---@field onPlayerCloseChannel fun(npc: Npc, creature: Creature)
---@field onPlayerEndTrade fun(npc: Npc, creature: Creature)
---@field onDisappear fun(npc: Npc, creature: Creature)
---@field onThink fun(npc: Npc): boolean
---@field onSay fun(npc: Npc, creature: Creature, messageType: number, message: string)
---@field onAppearCallback fun(creature: Creature)
---@field onMoveCallback fun(creature: Creature, oldPos: Position, newPos: Position)
---@field onPlayerCloseChannelCallback fun(creature: Creature)
---@field onPlayerEndTradeCallback fun(creature: Creature)
---@field onDisappearCallback fun(creature: Creature)
---@field onThinkCallback fun()
---@field onSayCallback fun(creature: Creature, messageType: number, message: string)

-- Make sure we are not overloading on reload
if not NpcEvents then
    -- If NpcEvents doesn't exist, it's created as an empty table
    NpcEvents = {}
    -- onAppear function is called when an NPC appears to a creature (player) or when the creature appears to the NPC.
    -- It executes the onAppearCallback function of the NPC if it is defined.
    ---@param npc Npc The NPC that appeared.
    ---@param creature Creature The creature (player) that the NPC appeared to.
    function NpcEvents.onAppear(npc, creature)
    end

    -- onMove function is called when an NPC moves.
    -- It executes the onMoveCallback function of the NPC if it is defined.
    ---@param npc Npc The NPC that moved.
    ---@param oldPos Position The old position of the NPC.
    ---@param newPos Position The new position of the NPC.
    function NpcEvents.onMove(npc, oldPos, newPos)
    end

    -- onPlayerCloseChannel function is called when a player closes the channel with an NPC.
    -- It executes the onPlayerCloseChannelCallback function of the NPC if it is defined.
    ---@param npc Npc The NPC that the player closed the channel with.
    ---@param creature Creature The creature (player) that closed the channel with the NPC.
    function NpcEvents.onPlayerCloseChannel(npc, creature)
    end

    -- onPlayerEndTrade function is called when a player ends the trade with an NPC.
    -- It executes the onPlayerEndTradeCallback function of the NPC if it is defined.
    ---@param npc Npc The NPC that the player ended the trade with.
    ---@param creature Creature The creature (player) that ended the trade with the NPC.
    function NpcEvents.onPlayerEndTrade(npc, creature)
    end

    -- onDisappear function is called when an NPC disappears from a creature (player) or when the creature disappears from the NPC.
    -- It executes the onDisappearCallback function of the NPC if it is defined.
    ---@param npc Npc The NPC that disappeared.
    ---@param creature Creature The creature (player) that the NPC disappeared from.
    function NpcEvents.onDisappear(npc, creature)
        if not creature:isPlayer() then
            return
        end
        local focus = NpcFocus(npc)
        if focus:isFocused(creature) then
            focus:removeFocus(creature)
            local talkQueue = NpcTalkQueue(npc)
            talkQueue:clearQueue(creature)
        end
    end

    -- onThink function is called when an NPC thinks.
    -- It handles the behavior of the NPC when thinking, such as removing focus from players who are too far away or have been focused for too long.
    -- It also processes the talk queue and adjusts the NPC's orientation based on the current focus.
    -- It executes the onThinkCallback function of the NPC if it is defined.
    ---@param npc Npc The NPC that is thinking.
    function NpcEvents.onThink(npc)
        local handler = NpcsHandler(npc)
        local focus = NpcFocus(npc)
        for playerid, releaseTime in pairs(focus.focus) do
            local player = Player(playerid)
            if getDistanceTo(player:getId()) >= FOCUS.distance or releaseTime < os.time() then
                focus:removeFocus(player)
                closeShopWindow(player)
                selfSay(handler.farewellResponses[math.random(1, #handler.farewellResponses)]:replaceTags({playerName = player:getName()}), player)
            end
        end

        local talkQueue = NpcTalkQueue(npc)
        talkQueue:processQueue()

        if #focus.focus == 0 then
            doNpcSetCreatureFocus(nil)
        else
            local player = focus:getCurrentFocus()
            if player then
                if player:getPosition().y > npc:getPosition().y and player:getPosition().x == npc:getPosition().x then
                    selfTurn(DIRECTION_SOUTH)
                elseif player:getPosition().y < npc:getPosition().y and player:getPosition().x == npc:getPosition().x then
                    selfTurn(DIRECTION_NORTH)
                elseif player:getPosition().x > npc:getPosition().x and player:getPosition().y == npc:getPosition().y then
                    selfTurn(DIRECTION_EAST)
                elseif player:getPosition().x < npc:getPosition().x and player:getPosition().y == npc:getPosition().y then
                    selfTurn(DIRECTION_WEST)
                end
            end
        end
    end

    -- onSay function is called when a creature (player) says something to an NPC.
    -- It handles the behavior of the NPC when a creature says something, such as greeting the player, responding to messages, opening shop windows, and executing callbacks.
    -- It executes the onSayCallback function of the NPC if it is defined.
    -- It checks requirements and modules for the NPC's responses and adjusts the talk state accordingly.
    ---@param npc Npc The NPC that is being spoken to.
    ---@param creature Creature The creature (player) that is speaking.
    ---@param messageType number The type of the message.
    ---@param message string The message spoken by the creature.
    function NpcEvents.onSay(npc, creature, messageType, message)
        -- Gracefully return if the talking creature is not a player
        if not creature:isPlayer() then
            return
        end

        local message = message:lower()
        -- initlialize the handler, focus and talkQueue
        local handler = NpcsHandler(npc)
        local focus = NpcFocus(npc)
        local talkQueue = NpcTalkQueue(npc)

        if not focus:isFocused(creature) then
            -- If the player is not focused, the NPC will greet the player if the player says a greeting word and is in range
            if getDistanceTo(creature:getId()) > FOCUS.greetDistance then
                return
            end

            for _, word in pairs(handler.greetWords) do
                if message == word then
                    focus:addFocus(creature)
                    doNpcSetCreatureFocus(creature:getId())
                    local msg = handler.greetResponses[math.random(1, #handler.greetResponses)]:replaceTags({playerName = creature:getName()})
                    talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                    handler:setTalkState(handler, creature)
                    return
                end
            end
            -- didn't greet yet, so we return
            return
        else
            -- If the player is focused, the NPC will say goodbye if the player says a farewell word
            for _, word in pairs(handler.farewellWords) do
                if message == word then
                    focus:removeFocus(creature)
                    closeShopWindow(creature)
                    local msg = handler.farewellResponses[math.random(1, #handler.farewellResponses)]:replaceTags({playerName = creature:getName()})
                    talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                    return
                end
            end
        end

        -- Checks if the NPC has a response for the given message
        if handler:getTalkState(creature):isKeyword(message) then
            -- renewing the focus for the player
            focus:addFocus(creature)
            -- If the NPC has a response, it sets the talk state to the one associated with the message
            handler:setTalkState(handler:getTalkState(creature):isKeyword(message), creature)
            -- check if we have a callback for this talk state
            if handler:getTalkState(creature).callback then
                local ret, failureMessage = handler:getTalkState(creature):callback(npc, creature)
                if not ret then
                    local msg = handler:getTalkState(creature).failureResponse:replaceTags({playerName = creature:getName()})
                    if failureMessage then
                        msg = failureMessage:replaceTags({playerName = creature:getName()})
                    end
                    talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                    handler:setTalkState(handler, creature)
                    return
                end
            end
            -- checking for requirements
            local ret, msg = handler:getTalkState(creature):requirements():init(creature)
            if not ret then
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                handler:setTalkState(handler, creature)
                return
            end
            -- checking for modules
            -- todo implement modules
            if handler:getTalkState(creature).teleportPosition then
                focus:removeFocus(creature)
                closeShopWindow(creature)
                local msg = handler:getTalkState(creature):getResponse():replaceTags({playerName = creature:getName()})
                selfSay(msg, creature)
                creature:teleportTo(handler:getTalkState(creature).teleportPosition)
                handler:setTalkState(handler, creature)
                return
            end
            -- If the NPC has a shop for the message, it opens the shop window
            if handler:getTalkState(creature):getShop(message) then
                handler:setActiveShop(creature, handler:getTalkState(creature):getShop(message))
                local shop = NpcShop(npc, handler:getActiveShop(creature))
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
                    npc:openShopWindow(creature, afterDiscount, shop.onBuy, shop.onSell)
                else
                    npc:openShopWindow(creature, items, shop.onBuy, shop.onSell)
                end
            end
            -- If the NPC has a response for the current topic, it says the response
            if handler:getTalkState(creature):getResponse() then
                local msg = handler:getTalkState(creature):getResponse():replaceTags({playerName = creature:getName()})
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
            end
            -- if the NPC has reached the last keyword, it resets the talk state
            if next(handler:getTalkState(creature).keywords) == nil then
                handler:setTalkState(handler, creature)
            end
            -- If the NPC has a resetTalkstate, it resets the talk state
            if handler:getTalkState(creature).resetTalkstate then
                handler:setTalkState(handler, creature)
            end
        elseif message == "help" then
            -- If the player asks for help, the NPC will respond with the available keywords
            local words = {}
            for k, v in pairs(handler:getKeywords()) do
                table.insert(words, "{".. k .."}")
            end
            local msg = "I only react to these words: " .. table.concat(words, ", ")
            talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
        end
    end
end
