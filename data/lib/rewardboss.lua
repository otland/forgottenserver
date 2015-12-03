if not globalBosses then
    globalBosses = {}
end

function Monster.setReward(self, enable)
    if enable then
        if not self:getType():isBoss() then
            error("Rewards can only be enabled to bosses.")
            return false
        end
        globalBosses[self:getId()] = {}
        self:registerEvent("BossDeath")    
        self:registerEvent("BossThink")    
    else
        globalBosses[self:getId()] = nil
        self:unregisterEvent("BossDeath")
        self:unregisterEvent("BossThink")
    end
    return true
end

function Item.getNameDescription(self)
    local subType = self:getSubType()
    local itemType = self:getType()

    local buffer = {}

    local name = self:getName() or ''
    if(#name ~= 0) then
        if(itemType:isStackable() and subType > 1) then
            pushValues(buffer, ' ', subType, self:getPluralName())
        else
            local article = self:getArticle() or ''
            pushValues(buffer, ' ', select(#article ~= 0 and 1 or 2, article, name))
        end
    else
        pushValues(buffer, ' ', 'an item of type', self:getId())
    end 

    return table.concat(buffer)
end

function Container.getContentDescription(self, outputBuffer)
    local firstItem = true
    local buffer = outputBuffer or {}
    for i = 1, self:getSize() do
        local item = self:getItem(i - 1)

        if(firstItem) then
            firstItem = false
        else
            table.insert(buffer, ", ")
        end

        table.insert(buffer, item:getNameDescription())
    end

    if firstItem then
        table.insert(buffer, "nothing")
    end

    if not outputBuffer then
        return table.concat(buffer)
    end
end

function Player.getRewardChest(self, autocreate)
    return self:getDepotChest(99, autocreate)
end

function Player.inBossFight(self)
    if not next(globalBosses) then
        return false
    end
    local playerGuid = self:getGuid()

    for _, info in pairs(globalBosses) do
        local stats = info[playerGuid]
        if stats and stats.active then
            return stats
        end
    end
    return false
end
