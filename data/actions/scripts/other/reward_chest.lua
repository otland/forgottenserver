function onUse(player, item, fromPosition, target, toPosition, isHotkey)
    -- Reward Chest
    if item:getId() == 21584 then
        if player:getExhaustion(REWARD_CHEST.STORAGE) > 0 then
            return player:sendCancelMessage('You need to wait ' .. string.diff(player:getStorageValue(REWARD_CHEST.STORAGE) - os.time()) .. ' before using this chest again.')
        end
        player:updateRewardChest()
    -- Boss Corpse
    elseif item:getActionId() == 21584 then
        local reward = REWARD_CHEST.LOOT[tonumber(item:getAttribute('text'))][player:getGuid()]
        if reward then
            local rewardBag = Container(doCreateItemEx(REWARD_CHEST.CONTAINER, 1))
            addContainerItems(rewardBag, reward)
            if player:getCapacity() < rewardBag:getCapacity() then
                return player:sendCancelMessage(RETURNVALUE_NOTENOUGHCAPACITY)
            end

            if player:addItemEx(rewardBag, false) == RETURNVALUE_NOERROR then
                REWARD_CHEST.LOOT[tonumber(item:getAttribute('text'))][player:getGuid()] = nil
                player:sendCancelMessage('You have picked up a reward container.')
            else
                player:sendCancelMessage(RETURNVALUE_NOTENOUGHROOM)
                return true
            end
        end
    end
    return false
end