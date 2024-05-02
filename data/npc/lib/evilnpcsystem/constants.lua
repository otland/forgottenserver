--[[
    >> Constants <<
    This file contains all the constants used in the NPC System.
]]

-- MESSAGE_TAGS
---@field tag string
---@field func function(playerName, amount, total, itemName)
MESSAGE_TAGS = {
    playerName = { tag = "|PLAYERNAME|", func = function(playerName, amount, total, itemName) return playerName end },
    itemCount = { tag = "|ITEMCOUNT|", func = function(playerName, amount, total, itemName) return amount end },
    totalCost = { tag = "|TOTALCOST|", func = function(playerName, amount, total, itemName) return total end },
    itemName = { tag = "|ITEMNAME|", func = function(playerName, amount, total, itemName) return itemName end }
}

-- MESSAGE_LIST
---@field needMoney string
---@field needSpace string
---@field needMoreSpace string
---@field bought string
---@field sold string
MESSAGE_LIST = {
    needMoney = "You need more money",
    needSpace = "You do not have enough capacity.",
    needMoreSpace = "You do not have enough capacity for all items.",
    bought = "You bought |ITEMCOUNT| |ITEMNAME|(s) for |TOTALCOST| gold",
    sold = "You sold |ITEMCOUNT| |ITEMNAME|(s) for |TOTALCOST| gold"
}

-- KEYWORDS_GREET
---@type string[]
KEYWORDS_GREET = {
    "hi",
    "hello",
    "hey",
    "greetings"
}

-- MESSAGES_GREET
---@type string[]
MESSAGES_GREET = {
    "Hello |PLAYERNAME| how can I help you?",
    "Greetings |PLAYERNAME|, what can I do for you?",
    "Hi |PLAYERNAME|, what's your desire today?"
}

-- KEYWORDS_FAREWELL
---@type string[]
KEYWORDS_FAREWELL = {
    "bye",
    "goodbye",
    "farewell",
    "cya",
    "ciao"
}

-- MESSAGES_FAREWELL
---@type string[]
MESSAGES_FAREWELL = {
    "Goodbye |PLAYERNAME|, have a nice day!",
    "Farewell |PLAYERNAME|, see you soon!",
    "See you later |PLAYERNAME|, take care!"
}

-- FOCUS
---@field time number
---@field distance number
---@field greetDistance number
FOCUS = {
    -- how long the npc will focus the player in seconds
    time = 60,
    -- how far the player can step away until the npc loses focus
    distance = 5,
    -- how near the player has to be to greet the npc
    greetDistance = 3
}

-- TALK
---@field defaultDelay number
TALK = {
    -- how long in ms the npc will wait before responding
    defaultDelay = 1000
}
