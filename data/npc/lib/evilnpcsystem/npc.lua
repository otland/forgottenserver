--[[
    NPC System:
        - Version: 1.0

    Credits:
        - Evil Hero (https://otland.net/members/evil-hero.4280/)
        - EvilHero90 (https://github.com/EvilHero90)
        - evilhero90 discord

    Description:
        - This system is designed to make it easier to create NPCs with complex interactions with players.
        - It includes a handler for the NPCs, a focus system, a talk queue, and a shop system.
        - The system is modular and can be expanded with additional features as needed.

    Features:
        - NpcsHandler: Handles the keywords, responses, and shops for the NPCs.
        - NpcFocus: Manages the focus of NPCs on players.
        - NpcTalkQueue: Manages a queue of messages for NPCs to say to players.
        - NpcShop: Handles the items available in the shops for the NPCs.
        - NpcEvents: Handles the appearance, disappearance, thinking, and interaction with players for the NPCs.
        - NpcRequirements: Stores the requirements for an NPC interaction.
        - Constants: Contains the constants used by the system.

    Functions:
        - string:replaceTags(playerName: string, amount: string, total: string, itemName: string)
        - NpcType:defaultBehavior()
        - NpcType.onAppearCallback(creature)
        - NpcType.onMoveCallback(creature, oldPos, newPos)
        - NpcType.onPlayerCloseChannelCallback(creature)
        - NpcType.onPlayerEndTradeCallback(creature)
        - NpcType.onDisappearCallback(creature)
        - NpcType.onThinkCallback()
        - NpcType.onSayCallback(creature, messageType, message)

    Example:
        - An example of how to use the system is provided in the data/npc/lua/#test.lua file.
]]

---@alias NpcType.onAppearCallback fun(creature: Creature)
---@alias NpcType.onMoveCallback fun(creature: Creature, oldPos: Position, newPos: Position)
---@alias NpcType.onPlayerCloseChannelCallback fun(creature: Creature)
---@alias NpcType.onPlayerEndTradeCallback fun(creature: Creature)
---@alias NpcType.onDisappearCallback fun(creature: Creature)
---@alias NpcType.onThinkCallback fun()
---@alias NpcType.onSayCallback fun(creature: Creature, messageType: number, message: string)

-- Load all the necessary files to create the NPC system
dofile('data/npc/lib/evilnpcsystem/constants.lua')
dofile('data/npc/lib/evilnpcsystem/handler.lua')
dofile('data/npc/lib/evilnpcsystem/events.lua')
dofile('data/npc/lib/evilnpcsystem/focus.lua')
dofile('data/npc/lib/evilnpcsystem/shop.lua')
dofile('data/npc/lib/evilnpcsystem/talkqueue.lua')
dofile('data/npc/lib/evilnpcsystem/requirements.lua')
dofile('data/npc/lib/evilnpcsystem/modules.lua')
dofile('data/npc/lib/evilnpcsystem/voices.lua')

-- Replaces tags in a string with corresponding values.
---@param params table<string, number|string|table<string|number, string|number>> The parameters to replace the tags with.
---@return string stringLib The string with the tags replaced.
function string.replaceTags(string, params)
    local ret = string
    for _, handler in pairs(MESSAGE_TAGS) do
        ret = ret:gsub(handler.tag, handler.func(params))
    end
    return ret
end

-- This function assigns event handlers for the NPC's appearance, disappearance, thinking, and interaction with players.
function NpcType:defaultBehavior()
    -- The onAppear function is called when the NPC/Creature appears.
    self.onAppear = function(creature)
        NpcEvents.onAppear(Npc(getNpcCid()), creature)
        if self.onAppearCallback then
            self.onAppearCallback(Npc(getNpcCid()), creature)
        end
    end
    -- The onDisappear function is called when the NPC/Creature disappears.
    self.onDisappear = function(creature)
        NpcEvents.onDisappear(Npc(getNpcCid()), creature)
        if self.onDisappearCallback then
            self.onDisappearCallback(Npc(getNpcCid()), creature)
        end
    end
    -- The onThink function is called when the NPC thinks.
    self.onThink = function()
        NpcEvents.onThink(Npc(getNpcCid()))
        if self.onThinkCallback then
            self.onThinkCallback(Npc(getNpcCid()))
        end
        return true
    end
    -- The onSay function is called when a player says something to the NPC.
    self.onSay = function(creature, messageType, message)
        NpcEvents.onSay(Npc(getNpcCid()), creature, messageType, message)
        if self.onSayCallback then
            self.onSayCallback(Npc(getNpcCid()), creature, messageType, message)
        end
    end
    -- The onMove function is called when the NPC moves.
    self.onMove = function(creature, oldPos, newPos)
        NpcEvents.onMove(Npc(getNpcCid()), creature, oldPos, newPos)
        if self.onMoveCallback then
            self.onMoveCallback(Npc(getNpcCid()), creature, oldPos, newPos)
        end
    end
    -- The onPlayerCloseChannel function is called when a player closes the channel with the NPC.
    self.onPlayerCloseChannel = function(creature)
        NpcEvents.onPlayerCloseChannel(Npc(getNpcCid()), creature)
        if self.onPlayerCloseChannelCallback then
            self.onPlayerCloseChannelCallback(Npc(getNpcCid()), creature)
        end
    end
    -- The onPlayerEndTrade function is called when a player ends the trade with the NPC.
    self.onPlayerEndTrade = function(creature)
        NpcEvents.onPlayerEndTrade(Npc(getNpcCid()), creature)
        if self.onPlayerEndTradeCallback then
            self.onPlayerEndTradeCallback(Npc(getNpcCid()), creature)
        end
    end
end
