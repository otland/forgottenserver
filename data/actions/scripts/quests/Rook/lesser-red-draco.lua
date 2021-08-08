local cfgItems = {{18413, 3}, {2133, 1}} -- 3x Tharian gem clusters, 1x Tharian amulet

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
    if  player:getStorageValue(2600) == 1 then
        player:sendTextMessage(MESSAGE_INFO_DESCR, "You already completed this quest.")
    elseif player:getStorageValue(2600) <= 1 then
        player:sendTextMessage(MESSAGE_INFO_DESCR, "You open the ash-covered chest, finding 3 Tharian gem clusters and a Tharian amulet!")
        for i = 1, #cfgItems do
            player:addItem(cfgItems[i][1], cfgItems[i][2])
        end
        player:setStorageValue(2600, 1)
    else
        player:sendTextMessage(MESSAGE_INFO_DESCR, "Woops, that wasn't supposed to happen.")
       end
end