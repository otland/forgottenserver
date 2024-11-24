--[[
    >> NpcsHandler <<

    Description:
        - This file contains the handler for the NPCs. It's used to store the keywords, responses and shops for the NPCs.
        - The handler is created as a metatable, so it can be called as a function to get the NpcsHandler for the NPC.
        - The handler is also used to manage the talk state, greet and farewell responses, and requirements for the keywords.

    Functions:
        - NpcsHandler(npc): NpcsHandler
        - NpcsHandler:keyword(word)
        - NpcsHandler:onStorageValue(key, value, operator, value2, operator2)
        - NpcsHandler:onAnswer()
        - NpcsHandler:hasAnswer()
        - NpcsHandler:getData(player, key)
        - NpcsHandler:getAllData(player)
        - NpcsHandler:addData(player, key, data)
        - NpcsHandler:resetData(player)
        - NpcsHandler:requirements()
        - NpcsHandler:player()
        - NpcsHandler:isKeyword(word)
        - NpcsHandler:getKeywords()
        - NpcsHandler:getTalkState(player)
        - NpcsHandler:setTalkState(state, player)
        - NpcsHandler:respond(text)
        - NpcsHandler:getResponse()
        - NpcsHandler:getResponses()
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
        - NpcsHandler:callback(npc, player, message)
        - NpcsHandler:farewell()
        - NpcsHandler:talk(table)
        - NpcsHandler:checkOnStorage(creature, handler)
]]

---@class NpcsHandler
---@field keywords table<string, NpcsHandler>
---@field talkState table<number, NpcsHandler>
---@field shopActive table<number, number>
---@field greetWords string[]
---@field greetResponses string[]
---@field farewellWords string[]
---@field farewellResponses string[]
---@field response table<number, string>
---@field openShop number
---@field resetTalkstate boolean
---@field failureResponse string
---@field keyword fun(self: NpcsHandler, words: string|table): NpcsHandler
---@field onStorageValue fun(self: NpcsHandler, key: number, value: number, operator?: string, value2?: number, operator2?: string): NpcsHandler
---@field onAnswer fun(self: NpcsHandler): NpcsHandler
---@field hasAnswer fun(self: NpcsHandler): boolean
---@field getData fun(self: NpcsHandler, player: Player, key: string): string
---@field getAllData fun(self: NpcsHandler, player: Player): table
---@field addData fun(self: NpcsHandler, player: Player, key: string, data: string)
---@field resetAData fun(self: NpcsHandler, player: Player)
---@field requirements fun(self: NpcsHandler): NpcRequirements
---@field isKeyword fun(self: NpcsHandler, word: string): NpcsHandler|boolean
---@field getKeywords fun(self: NpcsHandler): table<string, NpcsHandler>
---@field getTalkState fun(self: NpcsHandler, player: Player): NpcsHandler
---@field setTalkState fun(self: NpcsHandler, state: NpcsHandler, player: Player)
---@field respond fun(self: NpcsHandler, responses: string|table)
---@field getResponse fun(self: NpcsHandler): string
---@field getResponses fun(self: NpcsHandler): table
---@field shop fun(self: NpcsHandler, id: number)
---@field getShop fun(self: NpcsHandler, word: string): number|boolean
---@field setActiveShop fun(self: NpcsHandler, player: Player, id: number)
---@field getActiveShop fun(self: NpcsHandler, player: Player): number
---@field setGreetKeywords fun(self: NpcsHandler, words: string|table)
---@field setGreetResponse fun(self: NpcsHandler, texts: string|table)
---@field setFarewellKeywords fun(self: NpcsHandler, words: string|table)
---@field setFarewellResponse fun(self: NpcsHandler, texts: string|table)
---@field resetTalkState fun(self: NpcsHandler)
---@field failureRespond fun(self: NpcsHandler, text: string)
---@field callback fun(self: NpcsHandler, npc: Npc, player: Player, message: string): boolean, string
---@field require NpcRequirements
---@field player NpcModules
---@field farewell fun(self: NpcsHandler)
---@field talk fun(self: NpcsHandler, params: table<number, table>)
---@field checkOnStorage fun(self: NpcsHandler, creature: Creature, handler: NpcsHandler)
---@field __call fun(npc: Npc): NpcsHandler
---@field __index NpcsHandler
-- Modules
---@field travelTo fun(self: NpcsHandler, params: table<string, table>)
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
                    data = {},
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
                setmetatable(self[npc:getName()].keywords, {})
            end
            -- The NpcsHandler is returned
            return self[npc:getName()]
        end
    })
end

-- This function adds a keyword to the NpcsHandler for the NPC and returns the NpcsHandler for the keyword
---@param words string|table The keyword or table containing keywords to be added.
---@return NpcsHandler
function NpcsHandler:keyword(words)
    -- Initializes a new keyword entry in the `self.keywords` table if it doesn't already exist.
    -- The `words` parameter specifies the keyword or table containing keywords to be added.
    -- If words is a table it'll create multiple keywords, all pointing to the same metatable.
    -- The new entry contains an empty `keywords` table and an empty `response` string.
    -- The `self.keywords[word]` table is set as a metatable for both `self.keywords[word]` and `self.keywords[word].keywords`.
    -- This allows accessing methods and properties from the `NpcsHandler` class.
    local ret = words
    if type(words) == "string" then
        if not self.keywords[words] then
            self.keywords[words] = {}
            self.keywords[words].keywords = {}
            setmetatable(self.keywords[words], {__index = NpcsHandler})
            setmetatable(self.keywords[words].keywords, {})
            self.keywords[words].response = {}
            self.keywords[words].failureResponse = ""
        end
    elseif type(words) == "table" then
        local root = {__index = NpcsHandler}
        local keywords = {}
        for _, word in ipairs(words) do
            if not self.keywords[word] then
                self.keywords[word] = root
                self.keywords[word].keywords = keywords
                setmetatable(self.keywords[word], root)
                setmetatable(self.keywords[word].keywords, keywords)
                self.keywords[word].response = {}
                self.keywords[word].failureResponse = ""
            end
        end
        _,ret = next(words)
    end
    -- The NpcsHandler for the keyword is returned
    return self.keywords[ret]
end

-- This function adds a sub-keyword to the NpcsHandler for the NPC and returns the NpcsHandler for the sub-keyword
---@param key number The key to add the sub-keyword to.
---@param value number The value to add the sub-keyword to.
---@param operator string The operator to use for the sub-keyword.
---@return NpcsHandler
function NpcsHandler:onStorageValue(key, value, operator, value2, operator2)
    if not self.onStorage then
        self.onStorage = {}
    end
    operator = operator or "=="
    value2 = value2 or nil
    operator2 = operator2 or nil
    local index = #self.onStorage + 1
    self.onStorage[index] = {}
    self.onStorage[index].keywords = {}
    setmetatable(self.onStorage[index], {__index = NpcsHandler})
    setmetatable(self.onStorage[index].keywords, {})
    self.onStorage[index].response = {}
    self.onStorage[index].failureResponse = ""
    self.onStorage[index].storage = {key = key, value = value, operator = operator, value2 = value2, operator2 = operator2}
    return self.onStorage[index]
end

-- This function expects the Player to give a response to the NPC.
---@return NpcsHandler
function NpcsHandler:onAnswer()
    if not self.answer then
        self.answer = {}
    end
    self.answer.keywords = {}
    setmetatable(self.answer, {__index = NpcsHandler})
    setmetatable(self.answer.keywords, {})
    self.answer.response = {}
    self.answer.failureResponse = ""
    return self.answer
end

-- This function checks if the NPC has an onAnswer.
---@return boolean
function NpcsHandler:hasAnswer()
    return self.answer and true or false
end

-- This function retrieves the data for the player with a certain key.
---@param player Player The player to get the data for.
---@param key string The key to get the data for.
function NpcsHandler:getData(player, key)
    return self.data[player:getGuid()][key] or nil
end

-- This function retrieves all of the data for the player.
---@param player Player The player to get the data for.
function NpcsHandler:getAllData(player)
    return self.data[player:getGuid()] or {}
end

-- This function adds data for the player with a certain key.
---@param player Player The player to add the data for.
---@param key string The key to add the data for.
---@param data string The data to add.
function NpcsHandler:addData(player, key, data)
    if not self.data[player:getGuid()] then
        self.data[player:getGuid()] = {}
    end
    self.data[player:getGuid()][key] = data
end

-- This function resets the data for the player.
---@param player Player The player to reset the data for.
function NpcsHandler:resetData(player)
    self.data[player:getGuid()] = {}
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

-- Retrieves the player modules for a keyword.
-- If the modules have not been initialized, it creates a new table and sets the metatable to NpcModules.
---@return NpcModules The modules table for the NPC.
function NpcsHandler:player()
    if not self.modules then
        self.modules = {}
        setmetatable(self.modules, {__index = NpcModules})
    end
    return self.modules
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
---@param responses string|table The response text.
function NpcsHandler:respond(responses)
    if type(responses) == "string" then
        responses = {responses}
    end
    self.response = responses
end

-- Retrieves the response text for the keyword.
---@return string The responses.
function NpcsHandler:getResponse()
    return self.response[math.random(#self.response)]
end

-- Retrieves all of the responses
---@return table The responses.
function NpcsHandler:getResponses()
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

-- Releases the focus of the player like we are saying goodbye.
function NpcsHandler:farewell()
    self.releaseFocus = true
end

-- Adds words which the npc will randomly say in public or yell.
---@param params table<number, table> The words with interval, chance and TALKTYPE_SAY/TALKTYPE_YELL.
---@param delay number The delay between the words, default is 30000ms (30s).
function NpcsHandler:talk(params, delay)
    self.voices = params
    self.voicesDelay = delay or 30000
end

-- Checks if the player has a storage value and sets the talk state accordingly.
---@param creature Creature The creature to check the storage value for.
---@param handler NpcsHandler The handler to set the talk state for.
function NpcsHandler:checkOnStorage(creature, handler)
    if self.onStorage then
        local state = self.onStorage
        for i = 1, #state do
            if checkStorageValueWithOperator(creature, state[i].storage) then
                handler:setTalkState(state[i], creature)
                break
            end
        end
    end
end
