Town = {}
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

function Town:getId()
    return self.id
end

function Town:getName()
    return self.name
end

function Town:getTemplePosition()
    return self.templePosition
end
