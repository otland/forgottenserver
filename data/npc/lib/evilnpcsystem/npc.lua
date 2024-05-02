--[[
    >> NPC System <<
    - Version: 1.0

    >> Credits <<
    - Evil Hero (https://otland.net/members/evil-hero.4280/)
    - EvilHero90 (https://github.com/EvilHero90)
    - evilhero90 discord

    >> Description <<
    This system is designed to make it easier to create NPCs with complex interactions with players.
    It includes a handler for the NPCs, a focus system, a talk queue, and a shop system.
    The system is modular and can be expanded with additional features as needed.

    >> Features <<
    - NpcsHandler: Handles the keywords, responses, and shops for the NPCs.
    - NpcFocus: Manages the focus of NPCs on players.
    - NpcTalkQueue: Manages a queue of messages for NPCs to say to players.
    - NpcShop: Handles the items available in the shops for the NPCs.
    - NpcEvents: Handles the appearance, disappearance, thinking, and interaction with players for the NPCs.
    - Constants: Contains the constants used by the system.

    >> Example <<
    an example of how to use the system is provided in the data/npc/lua/#test.lua file. 
]]

-- Load all the necessary files to create the NPC system
dofile('data/npc/lib/evilnpcsystem/constants.lua')
dofile('data/npc/lib/evilnpcsystem/handler.lua')
dofile('data/npc/lib/evilnpcsystem/events.lua')
dofile('data/npc/lib/evilnpcsystem/focus.lua')
dofile('data/npc/lib/evilnpcsystem/shop.lua')
dofile('data/npc/lib/evilnpcsystem/talkqueue.lua')

---@class Npc
---@class Player
---@class Creature
---@class Item
---@class NpcsHandler

-- Replaces tags in a string with corresponding values.
---@param playerName string The players name.
---@param amount string The amount.
---@param total string The total.
---@param itemName string The items name.
---@return string The string with replaced tags.
function string:replaceTags(playerName, amount, total, itemName)
    local playerName = playerName and playerName or ""
    local amount = amount and amount or ""
    local total = total and total or ""
    local itemName = itemName and itemName or ""
    local ret = self
    for _, handler in pairs(MESSAGE_TAGS) do
        ret = ret:gsub(handler.tag, handler.func(playerName, amount, total, itemName))
    end
    return ret
end

-- Responds to a message from a player.
---@param message string The message to respond to.
---@param player Player The player object.
---@param delay number (optional) The delay before responding. If not provided, the default delay will be used.
function Npc:respond(message, player, delay)
    delay = delay and delay or TALK.defaultDelay
    local talkQueue = NpcTalkQueue(self)
    talkQueue:addToQueue(player, message, delay)
end

-- This function assigns event handlers for the NPC's appearance, disappearance, thinking, and interaction with players.
function Npc:defaultBehavior()
    -- The onAppear function is called when the NPC/Creature appears.
    self.onAppear = function(creature)
        NpcEvents.onAppear(self, creature)
    end
    -- The onDisappear function is called when the NPC/Creature disappears.
    self.onDisappear = function(creature)
        NpcEvents.onDisappear(self, creature)
    end
    -- The onThink function is called when the NPC thinks.
    self.onThink = function()
        NpcEvents.onThink(self)
        return true
    end
    -- The onSay function is called when a player says something to the NPC.
    self.onSay = function(creature, messageType, message)
        NpcEvents.onSay(self, creature, messageType, message)
    end
end
