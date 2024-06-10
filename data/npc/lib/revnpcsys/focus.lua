--[[
    >> NpcFocus <<

    Description:
        - The NpcFocus module provides a way to manage the focus of NPCs on creatures.
        - It keeps track of the creatures that are currently being focused by an NPC, as well as the duration of the focus.
        - The class provides methods to add, check, and remove focus on creatures, as well as retrieve the currently focused creature.
        - The focus duration and distance are defined in the FOCUS table in constants.lua.

    Functions:
        - NpcFocus:clear()
        - NpcFocus:addFocus(creature, focusTime)
        - NpcFocus:isFocused(creature)
        - NpcFocus:removeFocus(creature)
        - NpcFocus:getCurrentFocus()
]]

---@class NpcFocus
---@field focus table<number, number>
---@field currentFocus Creature
---@field clear fun()
---@field addFocus fun(creature: Creature, focusTime?: number)
---@field isFocused fun(creature: Creature): boolean
---@field removeFocus fun(creature: Creature)
---@field getCurrentFocus fun(): nil|Creature

-- Make sure we are not overloading on reload
if not NpcFocus then
    -- If NpcFocus doesn't exist, it's created as an empty table
    NpcFocus = {}
end
-- The metatable is set up to call the function when NpcFocus is called
setmetatable(NpcFocus, {
    __call = function(self, npc)
        -- If the NPC doesn't have a NpcFocus, one is created for it
        if not self[npc:getId()] then
            self[npc:getId()] = {
                focus = {},
                currentFocus = nil
            }
            setmetatable(self[npc:getId()], {__index = NpcFocus})
        end
        -- The NpcFocus is returned
        return self[npc:getId()]
    end
})

-- Clears all NpcFocus data for an NPC.
function NpcFocus:clear()
    self = nil
end

-- Adds focus on a creature for a certain duration.
---@param creature Creature The creature to add focus on.
function NpcFocus:addFocus(creature, focusTime)
    self.focus[creature:getId()] = os.mtime() + (focusTime or FOCUS.time)
    self.currentFocus = creature:getId()
end

-- Checks if a creature is currently being focused by the NPC.
---@param creature Creature The creature to check focus on.
---@return boolean True if the creature is being focused, false otherwise.
function NpcFocus:isFocused(creature)
    if self.focus[creature:getId()] then
        return self.focus[creature:getId()] > os.mtime()
    end
    return false
end

-- Removes focus from a creature.
---@param creature Creature The creature to remove focus from.
function NpcFocus:removeFocus(creature)
    self.focus[creature:getId()] = nil
    if self.currentFocus == creature:getId() then
        self.currentFocus = nil
    end
end

-- Retrieves the currently focused creature.
---@return nil|Creature The currently focused creature or nil if there is no focus.
function NpcFocus:getCurrentFocus()
    return not self.currentFocus and nil or Creature(self.currentFocus)
end
