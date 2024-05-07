--[[
    >> NpcFocus <<

    Description:
        - The NpcFocus module provides a way to manage the focus of NPCs on players.
        - It keeps track of the players that are currently being focused by an NPC, as well as the duration of the focus.
        - The class provides methods to add, check, and remove focus on players, as well as retrieve the currently focused player.
        - The focus duration and distance are defined in the FOCUS table in constants.lua.

    Functions:
        - NpcFocus:addFocus(player)
        - NpcFocus:isFocused(player)
        - NpcFocus:removeFocus(player)
        - NpcFocus:getCurrentFocus()
]]

---@class NpcFocus
---@field focus table<number, number>
---@field currentFocus Player
---@field addFocus fun(player: Player)
---@field isFocused fun(player: Player): boolean
---@field removeFocus fun(player: Player)
---@field getCurrentFocus fun(): nil|Player

-- Make sure we are not overloading on reload
if not NpcFocus then
    -- If NpcFocus doesn't exist, it's created as an empty table
    NpcFocus = {}
    -- The metatable is set up to call the function when NpcFocus is called
    setmetatable(NpcFocus, {
        __call = function(self, npc)
            -- If the NPC doesn't have a NpcFocus, one is created for it
            if not self[npc:getId()] then
                self[npc:getId()] = {
                    focus = {},
                    currentFocus = 0
                }
                setmetatable(self[npc:getId()], {__index = NpcFocus})
            end
            -- The NpcFocus is returned
            return self[npc:getId()]
        end
    })

    -- Adds focus on a player for a certain duration.
    ---@param player Player The player to add focus on.
    function NpcFocus:addFocus(player)
        self.focus[player:getGuid()] = os.time() + FOCUS.time
        self.currentFocus = player
    end

    -- Checks if a player is currently being focused by the NPC.
    ---@param player Player The player to check focus on.
    ---@return boolean True if the player is being focused, false otherwise.
    function NpcFocus:isFocused(player)
        if self.focus[player:getGuid()] then
            return self.focus[player:getGuid()] > os.time()
        end
        return false
    end

    -- Removes focus from a player.
    ---@param player Player The player to remove focus from.
    function NpcFocus:removeFocus(player)
        self.focus[player:getGuid()] = nil
        if self.currentFocus == player then
            self.currentFocus = 0
        end
    end

    -- Retrieves the currently focused player.
    ---@return nil|Player The currently focused player or nil if there is no focus.
    function NpcFocus:getCurrentFocus()
        return self.currentFocus
    end
end
