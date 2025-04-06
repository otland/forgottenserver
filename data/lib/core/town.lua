--- A town is a table that contains information about a town.
--- <br>
--- It is returned when using `Game.getTowns()`, `player:getTown()` or `house:getTown()`
--- <br><br>
--- Example Usage:
--- ```lua
--- local towns = Game.getTowns() -- Gets all the towns in the game
--- 
--- for i, town in ipairs(towns) do -- This will iterate through every town registered
---	    print("Town Name: " .. town.name) -- Example: "Thais"
---	    print("Town ID: " .. town.id) -- Example: 1
---	    print("Town Position: " .. town.position) -- Example: Position(100, 100, 7)
--- end
--- ```
---@class Town
---@field id uint32 -- The ID of the town
---@field name string -- The name of the town
---@field position Position -- The position of the town
---@field getId fun(self: Town):uint32  -- Returns the ID of the town
---@field getName fun(self: Town):string -- Returns the name of the town
---@field getTemplePosition fun(self: Town):Position -- Returns the position of the temple in the town
Town = {}
Town.__index = Town

setmetatable(Town, {
    __call = function(self, idOrName)
        local towns = Game.getTowns()
        for _, town in ipairs(towns) do
            if town:getId() == idOrName or town:getName() == idOrName then
                return town
            end
        end
        return nil
    end
})

---Returns the ID of the town.
---@return number
function Town:getId()
    return self.id
end

---Returns the name of the town.
---@return string
function Town:getName()
    return self.name
end

---Returns the town's temple position.
---@return Position
function Town:getTemplePosition()
    return self.templePosition
end
