--[[
    >> NpcsHandler <<

    Description:
        - This file contains the handler for the NPCs. It's used to store the keywords, responses and shops for the NPCs.
        - The handler is created as a metatable, so it can be called as a function to get the NpcsHandler for the NPC.
        - The handler is also used to manage the talk state, greet and farewell responses, and requirements for the keywords.

    Functions:
        - NpcsHandler(npc): NpcsHandler
        - NpcsHandler:keyword(word)
        - NpcsHandler:requirements()
        - NpcsHandler:isKeyword(word)
        - NpcsHandler:getKeywords()
        - NpcsHandler:getTalkState(player)
        - NpcsHandler:setTalkState(state, player)
        - NpcsHandler:respond(text)
        - NpcsHandler:getResponse()
        - NpcsHandler:shop(id)
        - NpcsHandler:getShop(word)
        - NpcsHandler:setActiveShop(player, id)
        - NpcsHandler:getActiveShop(player)
        - NpcsHandler:setGreetKeywords(words)
        - NpcsHandler:setGreetResponse(texts)
        - NpcsHandler:setFarewellKeywords(words)
        - NpcsHandler:setFarewellResponse(texts)
        - NpcsHandler:resetTalkState()
        - NpcsHandler:failureRespond(text)
        - NpcsHandler:teleport(position)
        - NpcsHandler:callback(npc, player)
]]

-- Make sure we are not overloading on reload
if not NpcsHandler then
    -- If NpcsHandler doesn't exist, it's created as an empty table
    NpcsHandler = {}
    -- The metatable is set up to call the function when NpcsHandler is called
    setmetatable(NpcsHandler, {
        __call = function(self, npc)
            -- If the NPC doesn't have a NpcsHandler, one is created for it
            if not self[npc:getName()] then
                -- Adds an entry to the handler table with the given NPC name as the key.
                -- The value is a table that can be accessed using the NPC name as the index.
                self[npc:getName()] = {
                    keywords = {},
                    talkState = {},
                    shopActive = {},
                    greetWords = KEYWORDS_GREET,
                    greetResponses = MESSAGES_GREET,
                    farewellWords = KEYWORDS_FAREWELL,
                    farewellResponses = MESSAGES_FAREWELL
                }
                -- Sets the metatable for the NPC with the given name to the NpcsHandler table.
                -- This allows the NPC to inherit functions and properties from the NpcsHandler table.
                setmetatable(self[npc:getName()], {__index = NpcsHandler})
                -- Sets the metatable for the keywords of the NPC with the given name to the NpcsHandler table.
                -- This allows the keywords of the NPC to inherit functions and properties from the NpcsHandler table.
                setmetatable(self[npc:getName()].keywords, {__index = NpcsHandler})
            end
            -- The NpcsHandler is returned
            return self[npc:getName()]
        end
    })
    
    -- This function adds a keyword to the NpcsHandler for the NPC and returns the NpcsHandler for the keyword
    ---@param word string
    ---@return NpcsHandler
    function NpcsHandler:keyword(word)
        -- Initializes a new keyword entry in the `self.keywords` table if it doesn't already exist.
        -- The `word` parameter specifies the keyword to be added.
        -- The new entry contains an empty `keywords` table and an empty `response` string.
        -- The `self.keywords[word]` table is set as a metatable for both `self.keywords[word]` and `self.keywords[word].keywords`.
        -- This allows accessing methods and properties from the `NpcsHandler` class.
        if not self.keywords[word] then
            self.keywords[word] = {}
            self.keywords[word].keywords = {}
            setmetatable(self.keywords[word], {__index = NpcsHandler})
            setmetatable(self.keywords[word].keywords, {__index = NpcsHandler})
            self.keywords[word].response = ""
        end
        -- The NpcsHandler for the keyword is returned
        return self.keywords[word]
    end

    -- Retrieves the requirements for a keyword.
    -- If the requirements have not been initialized, it creates a new table and sets the metatable to NpcRequirements.
    ---@return NpcRequirements The requirements table for the NPC.
    function NpcsHandler:requirements()
        if not self.require then
            self.require = {}
            setmetatable(self.require, {__index = NpcRequirements})
        end
        return self.require
    end

    -- Checks if a word is a keyword.
    ---@param word string The word to check.
    ---@return NpcsHandler|boolean if the word is a keyword, false otherwise.
    function NpcsHandler:isKeyword(word)
        return self.keywords[word] and self.keywords[word] or false
    end

    -- Retrieves the keywords for the NPC.
    ---@return table The keywords for the NPC.
    function NpcsHandler:getKeywords()
        return self.keywords
    end
    
    -- Retrieves the talk state for a given player.
    -- If the talk state is not found, returns the handler itself.
    ---@param player Player The player object.
    ---@return NpcsHandler The talk state for the player, or the handler itself if not found.
    function NpcsHandler:getTalkState(player)
        return self.talkState[player:getGuid()] == nil and self or self.talkState[player:getGuid()]
    end
    
    -- Sets the talk state for a player.
    ---@param state NpcsHandler The talk state to set.
    ---@param player Player The player object.
    function NpcsHandler:setTalkState(state, player)
        self.talkState[player:getGuid()] = state
    end

    -- Sets the response text for the keyword.
    ---@param text string The response text.
    function NpcsHandler:respond(text)
        self.response = text
    end

    -- Retrieves the response text for the keyword.
    ---@return string The response text.
    function NpcsHandler:getResponse()
        return self.response
    end

    -- Sets the shop ID for the keyword.
    ---@param id number The shop ID.
    function NpcsHandler:shop(id)
        self.openShop = id
    end

    -- Retrieves the shop ID for the given keyword.
    ---@param word string The keyword.
    ---@return number|boolean The shop ID if found, false otherwise.
    function NpcsHandler:getShop(word)
        return not self.openShop and false or self.openShop
    end

    -- Sets the active shop for a player.
    ---@param player Player The player object.
    ---@param id number The shop ID.
    function NpcsHandler:setActiveShop(player, id)
        self.shopActive[player:getGuid()] = id
    end

    -- Retrieves the active shop for a player.
    ---@param player Player The player object.
    ---@return number The active shop ID.
    function NpcsHandler:getActiveShop(player)
        return self.shopActive[player:getGuid()]
    end

    -- Sets the greet words for the NPC.
    ---@param words string|table The greet words.
    function NpcsHandler:setGreetKeywords(words)
        if type(words) == "string" then
            words = {words}
        end
        self.greetWords = words
    end

    -- Sets the greet response texts for the NPC.
    ---@param texts string|table The greet response texts.
    function NpcsHandler:setGreetResponse(texts)
        if type(texts) == "string" then
            texts = {texts}
        end
        self.greetResponses = texts
    end

    -- Sets the farewell words for the NPC.
    ---@param words string|table The farewell words.
    function NpcsHandler:setFarewellKeywords(words)
        if type(words) == "string" then
            words = {words}
        end
        self.farewellWords = words
    end

    -- Sets the farewell response texts for the NPC.
    ---@param texts string|table The farewell response texts.
    function NpcsHandler:setFarewellResponse(texts)
        if type(texts) == "string" then
            texts = {texts}
        end
        self.farewellResponses = texts
    end

    -- Resets the talk state for the NPC of the interacting Player.
    function NpcsHandler:resetTalkState()
        self.resetTalkstate = true
    end

    -- Sets the failure response text for the keyword.
    ---@param text string The failure response text.
    function NpcsHandler:failureRespond(text)
        self.failureResponse = text
    end

    -- Sets the position to teleport the player to.
    ---@param position Position The position to teleport the player to.
    function NpcsHandler:teleport(position)
        self.teleportPosition = position
    end

    -- Callback function for the keyword.
    ---@param npc Npc The NPC object.
    ---@param player Player The player object.
    ---@return boolean Whether the callback was successful.
    ---@return string (optional) The failure response text.
    -- example:
    --[[
    function NpcsHandler:callback(npc, player)
        if player:getStorageValue(9999) >= 1 then
            -- we let all other checks run and let us gracefully advance in talk state.
            return true
        end
        -- we return false which now triggers the default failureResponse to be sent to the player, resulting in a resetTalkState().
        return false
        -- we return false and a set string, which will be sent as the failure response to the player, resulting in a resetTalkState().
        return false, "You need storage value 9999 to proceed."
    end
    ]]
end
