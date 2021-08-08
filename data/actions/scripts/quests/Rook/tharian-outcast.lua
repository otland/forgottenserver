local cfgItems = {{18413, 1}, {2120, 1}, {2789,10}} -- 1x Tharian gem cluster, 1x Rope, 10x Brown Mushrooms

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
    if  player:getStorageValue(2601) == 1 then
        player:sendTextMessage(MESSAGE_INFO_DESCR, "You already completed this quest.")
    elseif player:getStorageValue(2601) <= 1 then
        player:sendTextMessage(MESSAGE_INFO_DESCR, "You search the two outcast's cache, finding a Tharian gem cluster a rope, and 10 brown mushrooms!")
        for i = 1, #cfgItems do
            player:addItem(cfgItems[i][1], cfgItems[i][2])
        end
        player:setStorageValue(2601, 1)
    else
        player:sendTextMessage(MESSAGE_INFO_DESCR, "Woops, that wasn't supposed to happen.")
       end
end