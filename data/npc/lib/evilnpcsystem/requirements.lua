--[[
    >> NpcRequirements <<

    Description:
        This table stores the requirements for an NPC interaction.
        It provides functions to set various requirements such as storage, level, premium status, money, items, infight status, and PZ lock status.
        The init function checks if the player meets all the requirements specified and returns true if all requirements are met, otherwise false.

    Functions:
        NpcRequirements:storage(key, value, equalOrAbove, equalOrBelow)
        NpcRequirements:level(level, equalOrAbove, equalOrBelow)
        NpcRequirements:premium(premium)
        NpcRequirements:money(amount)
        NpcRequirements:removeMoney(amount)
        NpcRequirements:item(item, count)
        NpcRequirements:removeItem(item, count, subType, ignoreEquipped)
        NpcRequirements:isInfight(infight)
        NpcRequirements:isPzLocked(pzLocked)
        NpcRequirements:init(player)
]]

-- Make sure we are not overloading on reload
if not NpcRequirements then
    NpcRequirements = {}

    -- Sets the storage requirement for a keyword.
    ---@param key string: The key of the storage value.
    ---@param value number: The required value of the storage.
    ---@param equalOrAbove boolean (optional): If true, the storage value must be equal to or greater than the required value. Defaults to false.
    ---@param equalOrBelow boolean (optional): If true, the storage value must be equal to or less than the required value. Defaults to false.
    function NpcRequirements:storage(key, value, equalOrAbove, equalOrBelow)
        equalOrAbove = equalOrAbove or false
        equalOrBelow = equalOrBelow or false
        self.requireStorage = {key = key, value = value, equalOrAbove = equalOrAbove, equalOrBelow = equalOrBelow}
    end

    -- Sets the level requirement for a keyword.
    ---@param level number: The required level.
    ---@param equalOrAbove boolean (optional): If true, the players level must be equal to or greater than the required level. Defaults to true.
    ---@param equalOrBelow boolean (optional): If true, the players level must be equal to or less than the required level. Defaults to false.
    function NpcRequirements:level(level, equalOrAbove, equalOrBelow)
        equalOrAbove = equalOrAbove or true
        equalOrBelow = equalOrBelow or false
        self.requireLevel = {level = level, equalOrAbove = equalOrAbove, equalOrBelow = equalOrBelow}
    end

    -- Sets the premium requirement for a keyword.
    ---@param premium boolean: If true, the player must be premium to advance the talk state.
    function NpcRequirements:premium(premium)
        self.requirePremium = premium
    end

    -- Sets the money requirement for a keyword.
    ---@param amount number: The required amount of money.
    function NpcRequirements:money(amount)
        self.requireMoney = amount
    end

    -- Sets the money to remove for a keyword.
    ---@param amount number: The amount of money to remove.
    function NpcRequirements:removeMoney(amount)
        self.requireRemoveMoney = amount
    end

    -- Sets the item requirement for a keyword.
    ---@param item number: The item ID.
    ---@param count number (optional): The required amount of the item. Defaults to 1.
    function NpcRequirements:item(item, count)
        local count = count or 1
        self.requireItem = {item = item, count = count}
    end

    -- Sets the item to remove for a keyword.
    ---@param item number: The item ID.
    ---@param count number (optional): The amount of the item to remove. Defaults to 1.
    ---@param subType number (optional): The subtype of the item to remove. Defaults to -1.
    ---@param ignoreEquipped boolean (optional): If true, the item will not be removed even if it is equipped. Defaults to true.
    function NpcRequirements:removeItem(item, count, subType, ignoreEquipped)
        local count = count or 1
        local subType = subType or -1
        local ignoreEquipped = ignoreEquipped or true
        self.requireRemoveItem = {item = item, count = count, subType = subType, ignoreEquipped = ignoreEquipped}
    end

    -- Sets the infight requirement for a keyword.
    ---@param infight boolean: If true, the player must be in a fight.
    function NpcRequirements:isInfight(infight)
        self.requireInfight = infight
    end

    -- Sets the PZ lock requirement for a keyword.
    ---@param pzLocked boolean: If true, the player must be PZ locked.
    function NpcRequirements:isPzLocked(pzLocked)
        self.requirePzLocked = pzLocked
    end

    -- Initializes the requirements for a player.
    ---@param player Player: The player to check the requirements for.
    ---@return boolean: True if the player meets all requirements, false otherwise.
    ---@return string: The message to send to the player if the requirements are not met.
    function NpcRequirements:init(player)
        if self.requireStorage then
            local storageValue = player:getStorageValue(self.requireStorage.key)
            if self.requireStorage.equalOrAbove then
                if storageValue < self.requireStorage.value then
                    return false, MESSAGE_LIST.storageToLow:replaceTags({storage = {key = self.requireStorage.key, value = self.requireStorage.value}})
                end
            elseif self.requireStorage.equalOrBelow then
                if storageValue > self.requireStorage.value then
                    return false, MESSAGE_LIST.storageToHigh:replaceTags({storage = {key = self.requireStorage.key, value = self.requireStorage.value}})
                end
            else
                if storageValue ~= self.requireStorage.value then
                    return false, MESSAGE_LIST.storage:replaceTags({storage = {key = self.requireStorage.key, value = self.requireStorage.value}})
                end
            end
        end

        if self.requireLevel then
            local playerLevel = player:getLevel()
            if self.requireLevel.equalOrAbove then
                if playerLevel < self.requireLevel.level then
                    return false, MESSAGE_LIST.levelToLow:replaceTags({playerLevel = self.requireLevel.level})
                end
            elseif self.requireLevel.equalOrBelow then
                if playerLevel > self.requireLevel.level then
                    return false, MESSAGE_LIST.levelToHigh:replaceTags({playerLevel = self.requireLevel.level})
                end
            else
                if playerLevel ~= self.requireLevel.level then
                    return false, MESSAGE_LIST.level:replaceTags({playerLevel = self.requireLevel.level})
                end
            end
        end

        if self.requirePremium then
            if not player:isPremium() then
                return false, MESSAGE_LIST.premium
            end
        end

        if self.requireMoney then
            if player:getMoney() < self.requireMoney then
                return false, MESSAGE_LIST.money:replaceTags({total = self.requireMoney})
            end
        end

        if self.requireItem then
            if player:getItemCount(self.requireItem.item) < self.requireItem.count then
                return false, MESSAGE_LIST.item:replaceTags({itemName = self.requireItem.item, amount = self.requireItem.count})
            end
        end

        if self.requireInfight ~= nil then
            if self.requireInfight and not player:getCondition(CONDITION_INFIGHT, CONDITIONID_DEFAULT) and not player:getCondition(CONDITION_INFIGHT, CONDITIONID_COMBAT) then
                return false, MESSAGE_LIST.infight
            end
            if not self.requireInfight and player:getCondition(CONDITION_INFIGHT, CONDITIONID_DEFAULT) and player:getCondition(CONDITION_INFIGHT, CONDITIONID_COMBAT) then
                return false, MESSAGE_LIST.notInfight
            end
        end

        if self.requirePzLocked ~= nil then
            if self.requirePzLocked and not player:isPzLocked() then
                return false, MESSAGE_LIST.pzLocked
            end
            if not self.requirePzLocked and player:isPzLocked() then
                return false, MESSAGE_LIST.notPzLocked
            end
        end

        if self.requireRemoveMoney and self.requireRemoveItem then
            if player:getItemCount(self.requireRemoveItem.item) < self.requireRemoveItem.count then
                return false, MESSAGE_LIST.item:replaceTags({itemName = self.requireItem.item, amount = self.requireItem.count})
            end
            if player:getMoney() < self.requireRemoveMoney then
                return false, MESSAGE_LIST.money:replaceTags({total = self.requireRemoveMoney})
            end
        end

        if self.requireRemoveMoney then
            if not player:removeMoney(self.requireRemoveMoney) then
                return false, MESSAGE_LIST.money:replaceTags({total = self.requireRemoveMoney})
            end
        end

        if self.requireRemoveItem then
            if not player:removeItem(self.requireRemoveItem.item, self.requireRemoveItem.count, self.requireRemoveItem.subType, self.requireRemoveItem.ignoreEquipped) then
                return false, MESSAGE_LIST.item:replaceTags({itemName = self.requireItem.item, amount = self.requireItem.count})
            end
        end
        return true
    end    
end
