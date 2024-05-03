--[[
    This is a test Merchant NPC that sells weapons and shields.
    It includes a showcase of the shop discount feature.
    This example consists of 2 different shops, one for weapons and one for shields.
]]

local npc = Game.createNpcType("Merchant")
npc:setMasterPos(Position(95, 123, 7))
npc:defaultBehavior()

-- This table defines the items available in the weapon shops.
local weaponShop = {
    [2400] = {buy = 2000, sell = 1000},
    [2402] = {buy = 50, sell = 25}
}

-- The NpcsHandler class is used to handle the NPC's responses to player interactions.
local handler = NpcsHandler(npc)

-- this handles the weapons shop
local weapons = handler:keyword("weapons")
weapons:respond("I offer weapons, do you want to see them?")
-- this happens when the player says yes
local accept = weapons:keyword("yes")
accept:respond("Here are the fancy Weapons!")
-- this opens the shop with the id 1
accept:shop(1)
-- this happens when the player says no
local decline = weapons:keyword("no")
decline:respond("no weapons for you then!")

local shop1 = NpcShop(npc, 1)
-- this adds the items to the shop
shop1:addItems(weaponShop)
-- this adds a discount to the shop, the player requires to have storagevalue 9998 and the discount is 10%
shop1:addDiscount(9998, 10)

-- this handles the shields shop
local shields = handler:keyword("shields")
shields:respond("I offer shields, do you want to see them?")
-- this happens when the player says yes
local accept = shields:keyword("yes")
accept:respond("Here are the fancy Shields!")
-- this opens the shop with the id 2
accept:shop(2)
-- this happens when the player says no
local decline = shields:keyword("no")
decline:respond("no shields for you then!")

-- showcase of a shop with adding single items instead of a table
local shop2 = NpcShop(npc, 2)
-- this adds a single item to the shop with the id 2509, buy price 200 and sell price 50
shop2:addItem(2509, 200, 50)
-- this adds a single item to the shop with the id 2510, buy price 500 and sell price 200
shop2:addItem(2510, 500, 200)
-- this adds a discount to the shop, the player requires to have storagevalue 9999 and the discount is equal to the value of the storage in %
shop2:addDiscount(9999)

--[[
    This is a test Traveler NPC that demonstrates the use of the NPC system.
    This example relies on the Travel Module to handle the travel functionality and requirement checks.
]]

local npc = Game.createNpcType("Traveler")
npc:setMasterPos(Position(95, 123, 7))
npc:defaultBehavior()

-- The NpcsHandler class is used to handle the NPC's responses to player interactions.
local handler = NpcsHandler(npc)

handler:setGreetRespond("Hello |PLAYERNAME| I can {travel} you to wherever you want, just tell me your {destination}")

local destinations = {
    ["temple"] = {position = Position(94, 129, 7), money = 100, level = 1, premium = false, storage = {key = 9999, value = 1, equalOrAbove = true}},
    ["city"] = {position = Position(98, 106, 6), money = 200, level = 20, premium = false, storage = {key = 9998, value = 1, equalOrAbove = true}},
    ["village"] = {position = Position(79, 99, 6), money = 300, level = 30, premium = false, storage = {key = 9997, value = 1, equalOrAbove = true}},
}

handler:travelTo(destinations)
