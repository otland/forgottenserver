--[[
    >> NpcShop <<

    Description:
        - NpcShop is a table that stores the items and discounts for each NPC shop.
        - It provides functions to add items and discounts to the shop, as well as to handle buying and selling items.
        
    Functions:
        - NpcShop:addItems(shop)
        - NpcShop:addItem(id/name, buy, sell, subType)
        - NpcShop:addDiscount(storage, percent)
        - NpcShop:hasDiscount(player)
        - NpcShop:getItems()
        - NpcShop:getItem(itemid/name, subType)
        - NpcShop:callback(npc, player, handler, items, afterDiscount)
        - NpcShop.onBuy(player, itemid, subType, amount, ignoreCap, inBackpacks)
        - NpcShop.onSell(player, itemid, subType, amount, ignoreEquipped)
]]

---@class NpcShop
---@field items table
---@field discounts table
---@field addItems fun(shop: table)
---@field addItem fun(id: number|string, buy: number, sell: number, subType: number)
---@field addDiscount fun(storage: number, percent: number)
---@field hasDiscount fun(player: Player): number|boolean
---@field getItems fun(): table
---@field getItem fun(itemid: number|string, subType?: number): Item|boolean
---@field callback fun(npc: Npc, player: Player, handler: table, items: table, afterDiscount?: number): table
---@field onBuy fun(player: Player, itemid: number, subType: number, amount: number, ignoreCap: boolean, inBackpacks: boolean): boolean
---@field onSell fun(player: Player, itemid: number, subType: number, amount: number, ignoreEquipped: boolean): boolean
---@type table<string, table<string, NpcShop>>

---@alias NpcShop.callback fun(npc, player, handler, items, afterDiscount?): table

-- Make sure we are not overloading on reload
if not NpcShop then
    -- If NpcShop doesn't exist, it's created as an empty table
    NpcShop = {}
    -- The metatable is set up to call the function when NpcShop is called
    setmetatable(NpcShop, {
        __call = function(self, npc, topic)
            -- If the NPC doesn't have a NpcShop, one is created for it
            if not self[npc:getName()] then
                self[npc:getName()] = {}
            end
            if not self[npc:getName()][topic] then
                self[npc:getName()][topic] = {
                    items = {},
                    discounts = {}
                }
                setmetatable(self[npc:getName()][topic], {__index = NpcShop})
            end
            -- The NpcShop is returned
            return self[npc:getName()][topic]
        end
    })
end

-- Adds items to the NPC shop.
---@param shop table<number|string, table<string, number>> The items to be added to the shop.
function NpcShop:addItems(shop)
    for id, items in pairs(shop) do
        local found = false
        for _, item in pairs(self.items) do
            if item.id == id or item.name == id then
                found = true
                break
            end
        end
        if not found then
            if ItemType(id):getName() ~= "" then
                if ItemType(id):isPickupable() then
                    local name = ItemType(id):getName()
                    if type(id) == "string" then
                        name = id
                        id = ItemType(id):getId()
                    end
                    table.insert(self.items, {
                        id = id, name = name, buy = items.buy, sell = items.sell, subtype = items.subType == nil and nil or items.subType
                    })
                else
                    print("[Warning - NpcShop:addItems] item: ".. id .." is not pickupable.\n".. debug.getinfo(2).source:match("@?(.*)"))
                end
            else
                print("[Warning - NpcShop:addItems] item: ".. id .." does not exist.\n".. debug.getinfo(2).source:match("@?(.*)"))
            end
        end
    end
end

-- Adds an item to the NPC shop.
---@param id number The ID of the item to add.
---@param buy number (optional): The buy price of the item. Defaults to 0 if not provided.
---@param sell number (optional): The sell price of the item. Defaults to 0 if not provided.
---@param subType number (optional): The subtype of the item. Defaults to 1 if not provided.
function NpcShop:addItem(id, buy, sell, subType)
    local buy = buy and buy or 0
    local sell = sell and sell or 0
    local sub = subType and subType or 1
    local found = false
    for _, item in pairs(self.items) do
        if item.id == id or item.name == id then
            found = true
            break
        end
    end
    if not found then
        if ItemType(id):getName() ~= "" then
            if ItemType(id):isPickupable() then
                local name = ItemType(id):getName()
                if type(id) == "string" then
                    name = id
                    id = ItemType(id):getId()
                end
                table.insert(self.items, {
                    id = id, name = name, buy = buy, sell = sell, subtype = subType == nil and nil or subType
                })
            else
                print("[Warning - NpcShop:addItem] item: ".. id .." is not pickupable.\n".. debug.getinfo(2).source:match("@?(.*)"))
            end
        else
            print("[Warning - NpcShop:addItem] item: ".. id .." does not exist.\n".. debug.getinfo(2).source:match("@?(.*)"))
        end
    end
end


-- Adds a discount to the specified storage in the NpcShop.
---@param storage number The storage required for the discount.
---@param percent number (optional) The percentage of the discount. Defaults to 0 if not provided.
function NpcShop:addDiscount(storage, percent)
    local percent = percent and percent or 0
    self.discounts[storage] = percent
end

--- Checks if a player has a discount in the NpcShop.
---@param player Player The player to check for discounts.
---@return number|boolean ret If the player has a discount, returns the storage value or the discount percentage. Otherwise, returns false.
function NpcShop:hasDiscount(player)
    for storage, percent in pairs(self.discounts) do
        if percent == 0 then
            if player:getStorageValue(storage) > 0 then
                return player:getStorageValue(storage)
            end
        end
        if player:getStorageValue(storage) > 0 then
            return percent
        end
    end
    return false
end

--- Retrieves the items in the NpcShop.
---@return table The items in the shop.
function NpcShop:getItems()
    return self.items
end

-- Retrieves an item from the shop based on its ID and subtype.
-- If the item is found, it is returned. Otherwise, false is returned.
---@param itemid number The ID of the item to retrieve.
---@param subType number The subtype of the item to retrieve (optional).
---@return Item|boolean The item if found, false otherwise.
function NpcShop:getItem(itemid, subType)
    for _, item in pairs(self.items) do
        if item.id == itemid then
            if item.subtype then
                if item.subType == subType then
                    return item
                end
            else
                return item
            end
        end
    end
    return false
end

-- Function: NpcShop.onBuy
-- Description: Handles the buying process when a player interacts with an NPC shop.
-- Notes:
--   - This function checks if the item is available in the shop and if it is buyable.
--   - It calculates the total cost of the purchase, taking into account any discounts or backpack costs.
--   - It checks if the player has enough money to make the purchase.
--   - It sells the item to the player and deducts the cost from their money.
--   - It handles cases where the player doesn't have enough capacity to carry all the items.
--   - It sends appropriate messages to the player during the buying process.
--   - It returns true if the buying process was successful, false otherwise.
---@param player Player The player object.
---@param itemid number The ID of the item to buy.
---@param subType number The subtype of the item to buy.
---@param amount number The amount of the item to buy.
---@param ignoreCap boolean Whether to ignore the players capacity when buying the item.
---@param inBackpacks boolean Whether to buy the item in backpacks.
---@return boolean True if the buying process was successful, false otherwise.
NpcShop.onBuy = function(player, itemid, subType, amount, ignoreCap, inBackpacks)
    local npc = Npc(getNpcCid())
    local npcHandler = NpcsHandler(npc)
    local shop = NpcShop(npc, npcHandler:getActiveShop(player))
    local shopItem = shop:getItem(itemid, subType)
    local focus = NpcFocus(npc)
    local talkQueue = NpcTalkQueue(npc)
    if not shopItem then
        error("[NpcShop.onBuy] shopItem == nil")
        return false
    end

    if shopItem.buy == 0 then
        error("[NpcShop.onBuy] attempt to buy a non-buyable item")
        return false
    end

    local discount = shop:hasDiscount(player) and shop:hasDiscount(player) or 0
    local totalCost = amount * (shopItem.buy - math.ceil(shopItem.buy / 100 * discount))
    if inBackpacks then
        totalCost = ItemType(itemid):isStackable() and totalCost + 20 or totalCost + (math.max(1, math.floor(amount / ItemType(ITEM_SHOPPING_BAG):getCapacity())) * 20)
    end

    if player:getTotalMoney() < totalCost then
        local msg = MESSAGE_LIST.needMoney:replaceTags({playerName = player:getName(), total = totalCost, itemName = shopItem.name, amount = amount})
        talkQueue:addToQueue(player, msg, TALK.defaultDelay)
        player:sendCancelMessage(msg)
        return false
    end

    local subType = shopItem.subType or 1
    local a, b = doNpcSellItem(player, itemid, amount, subType, ignoreCap, inBackpacks, ITEM_SHOPPING_BAG)
    if a < amount then
        local msgId = MESSAGE_LIST.needMoreSpace
        if a == 0 then
            msgId = MESSAGE_LIST.needSpace
        end

        local msg = msgId:replaceTags({playerName = player:getName(), total = totalCost, itemName = shopItem.name, amount = amount})
        player:sendCancelMessage(msg)
        focus:addFocus(player)

        if a > 0 then
            if not player:removeTotalMoney((a * (shopItem.buy - math.ceil(shopItem.buy / 100 * discount))) + (b * 20)) then
                return false
            end
            return true
        end

        return false
    else
        local msg = MESSAGE_LIST.bought:replaceTags({playerName = player:getName(), total = totalCost, itemName = shopItem.name, amount = amount})
        talkQueue:addToQueue(player, msg, TALK.defaultDelay)
        if not player:removeTotalMoney(totalCost) then
            return false
        end
        focus:addFocus(player)
        return true
    end
end

-- Function: NpcShop.onSell
-- Description: Handles the selling process when a player interacts with an NPC shop.
-- Notes:
--   - This function removes the sold item from the player's inventory, adds money to the player's balance,
--     and sends a message to the player confirming the sale.
--   - If the item is not sellable or the player does not have enough of the item, the function returns false.
--   - If the item is a fluid container, the subtype is set to -1.
--   - If an error occurs, an error message is logged and false is returned.
--   - If the sale is successful, true is returned.
---@param player Player The player object.
---@param itemid number The ID of the item to sell.
---@param subType number The subtype of the item to sell.
---@param amount number The amount of the item to sell.
---@param ignoreEquipped boolean Whether to ignore equipped items when selling.
---@return boolean True if the sale was successful, false otherwise.
NpcShop.onSell = function(player, itemid, subType, amount, ignoreEquipped, _)
    local npc = Npc(getNpcCid())
    local npcHandler = NpcsHandler(npc)
    local shop = NpcShop(npc, npcHandler:getActiveShop(player))
    local shopItem = shop:getItem(itemid, subType)
    local focus = NpcFocus(npc)
    local talkQueue = NpcTalkQueue(npc)
    if not shopItem then
        error("[NpcShop.onSell] items[itemid] == nil")
        return false
    end

    if shopItem.sell == 0 then
        error("[NpcShop.onSell] attempt to sell a non-sellable item")
        return false
    end

    if not ItemType(itemid):isFluidContainer() then
        subType = -1
    end

    if player:removeItem(itemid, amount, subType, true) then
        local msg = MESSAGE_LIST.sold:replaceTags({playerName = player:getName(), amount = amount, itemName = shopItem.name, total = amount * shopItem.sell})
        talkQueue:addToQueue(player, msg, TALK.defaultDelay)
        player:addMoney(amount * shopItem.sell)
        focus:addFocus(player)
        return true
    end

    focus:addFocus(player)
    return false
end
