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
        - NpcEvents.onSight(npc, creature)
        - NpcEvents.onSpeechBubble(npc, player, speechBubble)
]]

---@class NpcEvents
---@field onAppear fun(npc: Npc, creature: Creature)
---@field onMove fun(npc: Npc, oldPos: Position, newPos: Position)
---@field onPlayerCloseChannel fun(npc: Npc, creature: Creature)
---@field onPlayerEndTrade fun(npc: Npc, creature: Creature)
---@field onDisappear fun(npc: Npc, creature: Creature)
---@field onThink fun(npc: Npc): boolean
---@field onSay fun(npc: Npc, creature: Creature, messageType: number, message: string)
---@field onSight fun(npc: Npc, creature: Creature)
---@field onSpeechBubble fun(npc: Npc, player: Player, speechBubble: number): number

-- If NpcEvents doesn't exist, it's created as an empty table
NpcEvents = {}
-- onAppear function is called when an NPC appears to a creature (player) or when the creature appears to the NPC.
-- It executes the onAppearCallback function of the NPC if it is defined.
---@param npc Npc The NPC that appeared.
---@param creature Creature The creature (player) that the NPC appeared to.
function NpcEvents.onAppear(npc, creature)
end

function NpcEvents.onSpeechBubble(npc, player, speechBubble)
    return speechBubble
end

-- onSight function is called when an NPC sees a creature.
-- It executes the onSightCallback function of the NPC if it is defined.
---@param npc Npc The NPC that sees the creature.
---@param creature Creature The creature (player) that the NPC sees.
function NpcEvents.onSight(npc, creature)
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
    local focus = NpcFocus(npc)
    local talkQueue = NpcTalkQueue(npc)
    local voices = NpcVoices(npc)
    -- If the creature is a player and is focused on the NPC, the focus is removed and the talk state is reset
    if creature:isPlayer() and focus:isFocused(creature) then
        local handler = NpcsHandler(npc)
        focus:removeFocus(creature)
        talkQueue:clearQueue(creature)
        handler:setTalkState(handler, creature)
        handler:resetData(creature)
    end

    -- Npc is being removed we clear all the data which is specific to it's npc id he holds to not leak memory
    if npc == creature then
        focus:clear()
        talkQueue:clear()
        voices:clear()
    end
end

-- onThink function is called when an NPC thinks.
-- It handles the behavior of the NPC when thinking, such as removing focus from creatures who are too far away or have been focused for too long.
-- It also processes the talk queue and adjusts the NPC's orientation based on the current focus.
-- It executes the onThinkCallback function of the NPC if it is defined.
---@param npc Npc The NPC that is thinking.
function NpcEvents.onThink(npc)
    local handler = NpcsHandler(npc)
    local focus = NpcFocus(npc)
    local voices = NpcVoices(npc)
    for creatureid, releaseTime in pairs(focus.focus) do
        local creature = Creature(creatureid)
        if getDistanceTo(creature:getId()) >= FOCUS.distance or releaseTime < os.mtime() then
            focus:removeFocus(creature)
            if creature:isPlayer() then
                closeShopWindow(creature)
                selfSay(handler.farewellResponses[math.random(1, #handler.farewellResponses)]:replaceTags({playerName = creature:getName()}), creature)
                handler:setTalkState(handler, creature)
                handler:resetData(creature)
            end
        end
    end

    local talkQueue = NpcTalkQueue(npc)
    talkQueue:processQueue()

    if not next(focus.focus) then
        doNpcSetCreatureFocus(nil)
    else
        local creature = focus:getCurrentFocus()
        if creature then
            if creature:getPosition().y > npc:getPosition().y and creature:getPosition().x == npc:getPosition().x then
                selfTurn(DIRECTION_SOUTH)
            elseif creature:getPosition().y < npc:getPosition().y and creature:getPosition().x == npc:getPosition().x then
                selfTurn(DIRECTION_NORTH)
            elseif creature:getPosition().x > npc:getPosition().x and creature:getPosition().y == npc:getPosition().y then
                selfTurn(DIRECTION_EAST)
            elseif creature:getPosition().x < npc:getPosition().x and creature:getPosition().y == npc:getPosition().y then
                selfTurn(DIRECTION_WEST)
            end
        else
            local creatureid,_ = next(focus.focus)
            focus.currentFocus = creatureid
        end
    end

    if handler.voices then
        local canUse, voice = voices:canUseVoice(handler)
        if canUse then
            npc:say(voice.words, voice.talkType)
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

    local normalMessage = message
    local message = message:lower()
    -- initlialize the handler, focus and talkQueue
    local handler = NpcsHandler(npc)
    local focus = NpcFocus(npc)
    local talkQueue = NpcTalkQueue(npc)

    if focus:isFocused(creature) then
        -- If the player is focused, the NPC will say goodbye if the player says a farewell word
        for _, word in pairs(handler.farewellWords) do
            if message == word then
                focus:removeFocus(creature)
                closeShopWindow(creature)
                local msg = handler.farewellResponses[math.random(1, #handler.farewellResponses)]:replaceTags({playerName = creature:getName()})
                if handler:getTalkState(creature).farewellResponses then
                    msg = handler:getTalkState(creature).farewellResponses[math.random(1, #handler:getTalkState(creature).farewellResponses)]:replaceTags({playerName = creature:getName()})
                end
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                handler:setTalkState(handler, creature)
                handler:resetData(creature)
                return
            end
        end
    -- incase the player is not focused but he has a talk state which was not resetted
    -- this can only happen if player talked with the npc and the npc somehow disappeared
    else
        if not handler:getTalkState(creature):isKeyword(message) then
            if handler ~= handler:getTalkState(creature) then
                handler:setTalkState(handler, creature)
                handler:getTalkState(creature):checkOnStorage(creature, handler)
                handler:resetData(creature)
            end
        end
    end

    -- Checks if the NPC has a response for the given message
    if handler:getTalkState(creature):isKeyword(message) then
        local greeted = false
        if not focus:isFocused(creature) then
            if getDistanceTo(creature:getId()) > FOCUS.distance then
                return
            end
            -- If the NPC has a response, it sets the talk state to the one associated with the message
            handler:setTalkState(handler:getTalkState(creature):isKeyword(message), creature)
            handler:getTalkState(creature):checkOnStorage(creature, handler)

            for _, word in pairs(handler.greetWords) do
                if message == word then
                    focus:addFocus(creature)
                    doNpcSetCreatureFocus(creature:getId())
                    local msg = handler.greetResponses[math.random(1, #handler.greetResponses)]:replaceTags({playerName = creature:getName()})
                    if handler:getTalkState(creature).greetResponses then
                        msg = handler:getTalkState(creature).greetResponses[math.random(1, #handler:getTalkState(creature).greetResponses)]:replaceTags({playerName = creature:getName()})
                    end
                    talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                    greeted = true
                    break
                end
            end
            if not greeted then
                return
            end
        end
        -- incase shop is open and the player swaps the talk state, then we need to close the shop
        closeShopWindow(creature)
        -- renewing the focus for the player
        focus:addFocus(creature)

        if not greeted then
            -- If the NPC has a response, it sets the talk state to the one associated with the message
            handler:setTalkState(handler:getTalkState(creature):isKeyword(message), creature)
            -- check if we have a sub-keyword and set the talk state to it
            handler:getTalkState(creature):checkOnStorage(creature, handler)
        end
        -- checking for requirements
        local ret, msg = handler:getTalkState(creature):requirements():init(creature)
        if not ret then
            if msg then
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
            end
            local _, start = next(handler.keywords)
            handler:setTalkState(start, creature)
            handler:getTalkState(creature):checkOnStorage(creature, handler)
            return
        end
        -- check if we have a callback for this talk state
        local messageSent = false
        if handler:getTalkState(creature).callback then
            local ret, retMessage = handler:getTalkState(creature):callback(npc, creature, normalMessage, handler)
            if not ret then
                local msg = handler:getTalkState(creature).failureResponse:replaceTags({playerName = creature:getName()})
                if retMessage then
                    msg = retMessage:replaceTags({playerName = creature:getName()})
                end
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                local _, start = next(handler.keywords)
                handler:setTalkState(start, creature)
                handler:getTalkState(creature):checkOnStorage(creature, handler)
                if msg == "" then
                    print("[Warning - NpcEvents.onSay] Npc: ".. npc:getName() .." There is no failureResponse set for keyword: ".. message)
                    print(debug.getinfo(handler:getTalkState(creature).callback).source:match("@?(.*)"))
                end
                return
            end
            if retMessage then
                talkQueue:addToQueue(creature, retMessage:replaceTags({playerName = creature:getName()}), TALK.defaultDelay)
                messageSent = true
            end
        end
        -- checking for modules
        if handler:getTalkState(creature).modules then
            if handler:getTalkState(creature).modules:init(npc, creature) == false then
                -- need to do that because of teleport
                return
            end
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
                if shop.callback then
                    afterDiscount = shop:callback(npc, creature, handler, items, afterDiscount)
                end
                if type(afterDiscount) == "table" then
                    npc:openShopWindow(creature, afterDiscount, shop.onBuy, shop.onSell)
                else
                    print("[Warning - NpcEvents.onSay] Callback for Npc: ".. npc:getName() .." with shop: ".. handler:getActiveShop(creature) .." did not return a table.")
                    print(debug.getinfo(shop.callback).source:match("@?(.*)"))
                end
            else
                if shop.callback then
                    items = shop:callback(npc, creature, handler, items)
                end
                if type(items) == "table" then
                    npc:openShopWindow(creature, items, shop.onBuy, shop.onSell)
                else
                    print("[Warning - NpcEvents.onSay] Callback for Npc: ".. npc:getName() .." with shop: ".. handler:getActiveShop(creature) .." did not return a table.")
                    print(debug.getinfo(shop.callback).source:match("@?(.*)"))
                end
            end
        end
        if not greeted then
            -- If the NPC has a response for the current topic, it says the response
            if handler:getTalkState(creature):getResponse() and not messageSent then
                local msg = handler:getTalkState(creature):getResponse():replaceTags({playerName = creature:getName()})
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
            end
        end
        -- check if we want to release focus for this keyword
        if handler:getTalkState(creature).releaseFocus then
            if handler:getTalkState(creature):getResponse() then
                local msg = handler:getTalkState(creature):getResponse():replaceTags({playerName = creature:getName()})
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
            end
            focus:removeFocus(creature)
            closeShopWindow(creature)
            handler:setTalkState(handler, creature)
            handler:resetData(creature)
            return
        end
        -- if the NPC has reached the last keyword, it resets the talk state
        if next(handler:getTalkState(creature).keywords) == nil and not handler:getTalkState(creature).answer then
            local _, start = next(handler.keywords)
            handler:setTalkState(start, creature)
            handler:getTalkState(creature):checkOnStorage(creature, handler)
            handler:resetData(creature)
        end
        -- If the NPC has a resetTalkstate, it resets the talk state
        if handler:getTalkState(creature).resetTalkstate then
            local _, start = next(handler.keywords)
            handler:setTalkState(start, creature)
            handler:getTalkState(creature):checkOnStorage(creature, handler)
            handler:resetData(creature)
        end
    elseif message == "help" then
        if focus:isFocused(creature) then
            -- If the player asks for help, the NPC will respond with the available keywords
            local words = {}
            for k, v in pairs(handler:getTalkState(creature):getKeywords()) do
                table.insert(words, "{".. k .."}")
            end
            local msg = "I only react to these words: " .. table.concat(words, ", ")
            talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
        end
    elseif handler:getTalkState(creature):hasAnswer() then
        if focus:isFocused(creature) then
            focus:addFocus(creature)
            handler:setTalkState(handler:getTalkState(creature).answer, creature)

            -- checking for requirements
            local ret, msg, reqType = handler:getTalkState(creature):requirements():init(creature)
            if not ret then
                if handler:getTalkState(creature):requirements():getFailureRespond(reqType) then
                    msg = handler:getTalkState(creature):requirements():getFailureRespond(reqType):replaceTags({playerName = creature:getName()})
                end
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                local _, start = next(handler.keywords)
                handler:setTalkState(start, creature)
                handler:getTalkState(creature):checkOnStorage(creature, handler)
                handler:resetData(creature)
                return
            end

            local messageSent = false
            if handler:getTalkState(creature).callback then
                local ret, retMessage = handler:getTalkState(creature):callback(npc, creature, normalMessage, handler)
                if not ret then
                    local msg = handler:getTalkState(creature).failureResponse:replaceTags({playerName = creature:getName()})
                    if retMessage then
                        msg = retMessage:replaceTags({playerName = creature:getName()})
                    end
                    talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
                    local _, start = next(handler.keywords)
                    handler:setTalkState(start, creature)
                    handler:getTalkState(creature):checkOnStorage(creature, handler)
                    if msg == "" then
                        print("[Warning - NpcEvents.onSay] Npc: ".. npc:getName() .." has no failureResponse set for keyword: ".. message ..".")
                        print(debug.getinfo(handler:getTalkState(creature).callback).source:match("@?(.*)"))
                    end
                    handler:resetData(creature)
                    return
                end
                if retMessage then
                    talkQueue:addToQueue(creature, retMessage:replaceTags({playerName = creature:getName()}), TALK.defaultDelay)
                    messageSent = true
                end
            else
                print("[Error - NpcEvents.onSay] Npc: ".. npc:getName() .." has no callback set for onAnswer")
                print(debug.getinfo(handler:getTalkState(creature).callback).source:match("@?(.*)"))
                local _, start = next(handler.keywords)
                handler:setTalkState(start, creature)
                handler:getTalkState(creature):checkOnStorage(creature, handler)
                handler:resetData(creature)
                return
            end

            if handler:getTalkState(creature):getResponse() and not messageSent then
                local msg = handler:getTalkState(creature):getResponse():replaceTags({playerName = creature:getName()})
                talkQueue:addToQueue(creature, msg, TALK.defaultDelay)
            end

            -- if the NPC has reached the last keyword, it resets the talk state
            if next(handler:getTalkState(creature).keywords) == nil then
                local _, start = next(handler.keywords)
                handler:setTalkState(start, creature)
                handler:getTalkState(creature):checkOnStorage(creature, handler)
                handler:resetData(creature)
            end
        end
    end
end
