--[[
    This is a test Merchant NPC that sells weapons and shields.
    It includes a showcase of the shop discount feature.
    This example consists of 2 different shops, one for weapons and one for shields.
]]

local npc = Game.createNpcType("Merchant")
npc:speechBubble(SPEECHBUBBLE_TRADE)
npc:outfit({lookType = 128, lookHead = 114, lookBody = 114, lookLegs = 114, lookFeet = 114})
npc:defaultBehavior()
npc:sight(5, 5)
local callbacks = npc:callbacks()
function callbacks:onSay(creature, type, message)
   print(self, creature, type, message)
end

-- This table defines the items available in the weapon shops.
local weaponShop = {
    [2400] = {buy = 2000, sell = 1000},
    [2402] = {buy = 50, sell = 25}
}

-- The NpcsHandler class is used to handle the NPC's responses to player interactions.
local handler = NpcsHandler(npc)

local greet = handler:keyword(handler.greetWords)
--greet:setGreetResponse("Hello |PLAYERNAME|.")
-- this handles the weapons shop
-- example with 2 different keywords pointing to the same response
local items = greet:keyword({"items", "item"})
local req = items:requirements()
--[[
req:storage(9999, 10, ">=", 50, "<=", "Your storage is to low to trade with me", "Your storage is to high to trade with me")
req:level(4, ">=", 10, "<=", "You are to low level to trade with me", "You are to high level to trade with me")
req:premium(false, "You are not allowed to be premium to trade with me")
req:money(1000, false, "You are not allowed to have that much money to trade with me")
req:mount(2, false, "You are not allowed to have a mount to trade with me")
req:outfit(1211, 1211, 0, false, "You are not allowed to have that outfit to trade with me")
req:items({{item = 2400, count = 1}, {item = 2509, count = 2}, {item = "crystal coin", count = 50}}, false, "You are not allowed to have those items to trade with me")
req:town(1, false, "You are not allowed to be a citizen of this town to trade with me")
req:sex(PLAYERSEX_FEMALE, true, "You are not allowed to be a men to trade with me")
req:party(false, "You are not allowed to be in a party to trade with me")
req:guild(true, "You need to be in a guild to trade with me")
req:removeItems({{item = 2400, count = 1}, {item = 2509, count = 2}, {item = "crystal coin", count = 50}}, "You need to have those items for me to remove in order for me to trade with you")
req:learnedSpell("exori", false, "You are not allowed to know exori to trade with me")
]]
local give = items:player()
--[[
give:storage(9999, 10)
give:experience(1000)
give:money(1000)
give:blessings(1)
give:vocation(1)
give:sex(PLAYERSEX_MALE)
give:town(1)
give:learnSpell("exori")
give:mount(1)
give:money(1000)
give:premium(30)
give:outfit(1211, 1212, 3)
give:items({{item = 2400, count = 1}, {item = 2509, count = 2}, {item = "crystal coin", count = 50}}, ITEM_SHOPPING_BAG, true)
]]
items:respond("Here are some sexy items for you!")

local weapons = greet:keyword({"weapons", "weapon"})
weapons:respond("I offer weapons, do you want to see them?")
-- this happens when the player says yes
local accept = weapons:keyword("yes")
-- this responds randomly with the messages provided
accept:respond({"Here are the fancy Weapons!", "Hope you find something you like!"})
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
local shields = greet:keyword("shields")
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

function shop2:callback(npc, player, handler, items, afterDiscount)
    local afterDiscount = afterDiscount or items
    for _, item in pairs(afterDiscount) do
        item.buy = item.buy * 2
    end
    return afterDiscount
end

--[[
    This is a test Traveler NPC that demonstrates the use of the NPC system.
    This example relies on the Travel Module to handle the travel functionality and requirement checks.
]]

local npc = Game.createNpcType("Traveler")
npc:speechBubble(SPEECHBUBBLE_COMPASS)
npc:outfit({lookType = 128, lookHead = 114, lookBody = 114, lookLegs = 114, lookFeet = 114})
npc:spawnRadius(2)
npc:walkInterval(2000)
npc:walkSpeed(100)
npc:defaultBehavior()

-- The NpcsHandler class is used to handle the NPC's responses to player interactions.
local handler = NpcsHandler(npc)

local greet = handler:keyword(handler.greetWords)
greet:setGreetResponse("Hello |PLAYERNAME| I can travel you to wherever you want, just tell me your {destination}")

local destinations = {
    ["temple"] =
    {
        position = Position(94, 129, 7)--[[,
        removeMoney = {100, "poor sucker"},
        storage = {9999, 10, ">", "You need to have more than 10 storage to travel to this destination"}, -- operators: <, >, <=, >=, ==, ~=
        -- storage = {9999, 10, ">", 50, "<", "You need to have more than 10 storage to travel to this destination", "You need to have less than 50 storage to travel to this destination"}, -- ranged operators (between something and something)
        level = {1, ">", "You need to be above level 1"}, -- operators: <, >, <=, >=, ==, ~=
        -- level = {10, ">", 50, "<", "You need to be above level 10", "You need to be lower than level 50"}, -- range operators (between something and something)
        premium = {true, "You need to be premium to travel to this destination"},
        items = {{{item = 2509, count = 1}, {item = 2510, count = 2}, {item = "crystal coin", count = 50}, {item = 2400, count = 1}}, true, "You need to have those items to travel to this destination"}, -- If true player needs all items, if false player does not have one of the items
        party = {true, "You need to be in a party to travel to this destination"},
        guild = {true, "You need to be in a guild to travel to this destination"},
        town = {1, true, "You need to be a citizen of town 1 to travel to this destination"},
        sex = {PLAYERSEX_MALE, true, "You need to be a male to travel to this destination"},
        mount = {1, true, "You need to have this mount to travel to this destination"},
        outfit = {1211, 1212, 3, true, "You need to have this outfit to travel to this destination"}, -- male/female, female/male, addon
        removeItems = {{{item = 2509, count = 1}, {item = 2510, count = 2}, {item = "crystal coin", count = 50}, {item = 2400, count = 1}}, "You need to have those items for me to remove"},
        learnedSpell = {"exori", false, "You need to know exori to travel to this destination"}
        ]]
    },
    ["depot"] =
    {
        position = Position(94, 129, 7)
    }
}

greet:travelTo(destinations)
