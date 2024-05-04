--[[
    >> Constants <<

    Description:
        - This file contains all the constants used in the NPC System.
]]

-- MESSAGE_TAGS
---@class MESSAGE_TAGS
---@type table<string, table<string|number, string|number|function>>
MESSAGE_TAGS = {
    playerName = { tag = "|PLAYERNAME|", func = function(params) return params.playerName or "" end },
    playerLevel = { tag = "|PLAYERLEVEL|", func = function(params) return params.playerLevel or "" end },
    itemCount = { tag = "|ITEMCOUNT|", func = function(params) return params.amount or "" end },
    totalCost = { tag = "|TOTALCOST|", func = function(params) return params.total or "" end },
    itemName = { tag = "|ITEMNAME|", func = function(params) return params.itemName or "" end },
    storageKey = { tag = "|STORAGEKEY|", func = function(params) return params.storage and params.storage.key or "" end },
    storageValue = { tag = "|STORAGEVALUE|", func = function(params) return params.storage and params.storage.value or "" end }
}

-- MESSAGE_LIST
---@class MESSAGE_LIST
---@field needMoney string
---@field needSpace string
---@field needMoreSpace string
---@field bought string
---@field sold string
---@field storage string
---@field storageToLow string
---@field storageToHigh string
---@field level string
---@field levelToLow string
---@field levelToHigh string
---@field premium string
---@field money string
---@field item string
---@field infight string
---@field notInfight string
---@field pzLocked string
---@field notPzLocked string
---@type table<string, string>
MESSAGE_LIST = {
    -- shop messages
    needMoney = "You need more money",
    needSpace = "You do not have enough capacity.",
    needMoreSpace = "You do not have enough capacity for all items.",
    bought = "You bought |ITEMCOUNT| |ITEMNAME|(s) for |TOTALCOST| gold.",
    sold = "You sold |ITEMCOUNT| |ITEMNAME|(s) for |TOTALCOST| gold.",
    -- requirement cancel messages
    storage = "You do not meet the storage requirement.",
    storageToLow = "You do not meet the storage requirement.",
    storageToHigh = "You do not meet the storage requirement.",
    level = "You need to be exactly level |PLAYERLEVEL|.",
    levelToLow = "You need to be atleast level |PLAYERLEVEL|.",
    levelToHigh = "You need to be under level |PLAYERLEVEL|.",
    premium = "You need to be premium to do this.",
    money = "You do not have enough money, it costs |TOTALCOST| gold.",
    item = "You do not have atleast |ITEMCOUNT| |ITEMNAME|(s).",
    infight = "You need to be in fight.",
    notInfight = "You have to be out of fight.",
    pzLocked = "You need to be pz locked",
    notPzLocked = "You are not allowed to be pz locked"
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
---@class FOCUS
---@field time number
---@field distance number
---@field greetDistance number
---@type table<string, number|string|table|boolean>
FOCUS = {
    -- how long the npc will focus the player in seconds
    time = 60,
    -- how far the player can step away until the npc loses focus
    distance = 5,
    -- how near the player has to be to greet the npc
    greetDistance = 3
}

-- TALK
---@class TALK
---@field defaultDelay number
---@type table<string, number|string|table|boolean>
TALK = {
    -- how long in ms the npc will wait before responding
    defaultDelay = 1000
}
