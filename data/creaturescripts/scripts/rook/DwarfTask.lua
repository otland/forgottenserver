local missions = { -- Use missionStorage to link the monsters to each mission. It is set up this way so multiple missions can have the same monsters.
    [1] = {name = "Dwarven Doorman", amount = 1, missionStorage = 43100, storage = 41100}
}

function onKill(creature, target)
    if isPlayer(creature) and isMonster(target) then
        for i = 1, #missions do
            if target:getName() == missions[i].name then
                if creature:getStorageValue(missions[i].missionStorage) == 1 then
                    if creature:getStorageValue(missions[i].storage) < missions[i].amount then
                        creature:setStorageValue(missions[i].storage, creature:getStorageValue(missions[i].storage) + 1)
                        creature:sendTextMessage(MESSAGE_STATUS_CONSOLE_ORANGE, "You have killed "..creature:getStorageValue(missions[i].storage).."/"..missions[i].amount.." "..missions[i].name.."('s)")
                    end
                end
            end
        end
    end
    return true
end