--[[
    >> Constants <<

    Description:
        - This file contains all the constants used in the NPC System.
]]

-- MESSAGE_TAGS
---@class MESSAGE_TAGS
---@type table<string, table<string, string|number|function>>
MESSAGE_TAGS = {
    playerName = { tag = "|PLAYERNAME|", func = function(params) return params.playerName or "" end },
    playerLevel = { tag = "|PLAYERLEVEL|", func = function(params) return params.playerLevel or "" end },
    playerLevel2 = { tag = "|PLAYERLEVEL2|", func = function(params) return params.playerLevel2 or "" end },
    itemCount = { tag = "|ITEMCOUNT|", func = function(params) return params.amount or "" end },
    totalCost = { tag = "|TOTALCOST|", func = function(params) return params.total or "" end },
    itemName = { tag = "|ITEMNAME|", func = function(params) return params.itemName or "" end },
    storageKey = { tag = "|STORAGEKEY|", func = function(params) return params.storage and params.storage.key or "" end },
    storageValue = { tag = "|STORAGEVALUE|", func = function(params) return params.storage and params.storage.value or "" end },
    vocationName = { tag = "|VOCATIONNAME|", func = function(params) return params.vocation or "" end }
}

-- MESSAGE_LIST
---@class MESSAGE_LIST
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
    storageRange = "You do not meet the storage requirement.",
    storageNotEqual = "You do not meet the storage requirement.",
    storageToLow = "You do not meet the storage requirement.",
    storageToHigh = "You do not meet the storage requirement.",
    level = "You need to be exactly level |PLAYERLEVEL|.",
    levelRange1 = "You need to be higher then level |PLAYERLEVEL| and below |PLAYERLEVEL2|.", -- > <
    levelRange2 = "You need to be level |PLAYERLEVEL| or higher and below |PLAYERLEVEL2|.", -- >= <
    levelRange3 = "You need to be higher then level |PLAYERLEVEL| and below or equal |PLAYERLEVEL2|.", -- > <=
    levelRange4 = "You need to be between level |PLAYERLEVEL| and |PLAYERLEVEL2|.", -- >= <=
    levelNotEqual = "You can't be exactly level |PLAYERLEVEL|.",
    levelToLow = "You need to be atleast level |PLAYERLEVEL|.",
    levelToHigh = "You need to be below level |PLAYERLEVEL|.",
    premium = "You need to be premium to do this.",
    vocation = "You need to be a |VOCATIONNAME| to do this.",
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
---@type table<string, any>
FOCUS = {
    -- how long the npc will focus the player in ms
    time = 60000,
    -- how far the player can step away until the npc loses focus
    distance = 5,
    -- how near the player has to be to greet the npc
    greetDistance = 3
}

-- TALK
---@class TALK
---@type table<string, any>
TALK = {
    -- how long in ms the npc will wait before responding
    defaultDelay = 1000
}

REQUIREMENTS = {
    storage = 1,
    level = 2,
    premium = 3,
    vocation = 4,
    money = 5,
    item = 6,
    inFight = 7,
    pzLocked = 8,
    removeItem = 9
}
