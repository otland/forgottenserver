local talk = TalkAction("!autoloot")

function talk.onSay(player, words, param)
    local i = player:getAutoLootList()
    local cache = "Check your loot list: "
    local split = param:split(",")
    local action = split[1]

    if param == "list" then
        if i then
            for _, item in ipairs(i) do
                cache = cache .. (ItemType(item)):getName() .. ", "
            end
        else
            player:sendTextMessage(MESSAGE_INFO_DESCR, "Your list is empty! Add some item and try again.")
            return false
        end

        player:sendTextMessage(MESSAGE_INFO_DESCR, cache:sub(1, -3))
        return false
    end

   if action == "add" then
        local item = split[2]:gsub("%s+", "", 1)
        local itemType = ItemType(item)
        if itemType:getId() == 0 then
            itemType = ItemType(tonumber(item))
            if itemType:getName() == '' then
                player:sendCancelMessage("There is no item with that id or name.")
                return false
            end
        end

        if player:getItemFromAutoLoot(itemType:getId()) then
            player:sendCancelMessage("You're already autolooting this item.")
            return false
        end

        player:addItemToAutoLoot(itemType:getId())
        player:sendTextMessage(MESSAGE_INFO_DESCR, "You're now auto looting " .. itemType:getName())
        return false
    elseif action == "remove" then
        local item = split[2]:gsub("%s+", "", 1)
        local itemType = ItemType(item)
        if itemType:getId() == 0 then
            itemType = ItemType(tonumber(item))
            if itemType:getName() == '' then
                player:sendCancelMessage("There is no item with that id or name.")
                return false
            end
        end

        if player:getItemFromAutoLoot(itemType:getId()) then
            player:removeItemFromAutoLoot(itemType:getId())
            player:sendTextMessage(MESSAGE_INFO_DESCR, "You removed the " .. itemType:getName() .. " from your loot list.")
        else
            player:sendCancelMessage("This item does not exist in your loot list.")
        end
        return false           
    end


    player:sendTextMessage(MESSAGE_EVENT_ORANGE, "Auto Loot commands (items are automatically moved to your bp if you open monster corpse):"..'\n'.."!addloot add, nameItem - add item to auto loot by name"..'\n'.."!autoloot remove, itemName - remove item from auto loot by name"..'\n'.."!autoloot list - list your current auto loot items")
    return false
end

talk:separator(" ")
talk:register()