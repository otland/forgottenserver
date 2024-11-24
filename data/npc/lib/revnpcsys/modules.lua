--[[
    >> Modules <<

    Description:
        - This file contains the modules for the NPC system.
        - Modules are pre written NpcsHandler to make it easier to make certain types of NPCs.
        - You can write your own modules and add them to the system.

    Functions:
        - NpcsHandler:travelTo(params)
        
        - NpcModules:storage(key: number, value: number)
        - NpcModules:experience(exp: number)
        - NpcModules:money(money: number)
        - NpcModules:premium(days: number)
        - NpcModules:items(items: table<string, any>, container?: number|string, inbox?: boolean)
        - NpcModules:teleport(position: Position, magicEffectFromPos?: MagicEffect_t, magicEffectToPos?: MagicEffect_t)
        - NpcModules:blessing(blessing: number)
        - NpcModules:outfit(outfit1: number, outfit2: number, addon: number)
        - NpcModules:mount(mount: number)
        - NpcModules:vocation(vocation: string)
        - NpcModules:learnSpell(spell: string)
        - NpcModules:sex(sex: number)
        - NpcModules:town(town: number)

]]

---@alias travelTo fun(self: NpcsHandler, params: table<string, table>)

-- This Module enables fast and easy creation of Travel NPCs
---@class travelParams
---@field position Position
---@field money number
---@field level number
---@field premium boolean
---@field storage table<string, any>
---@field item table<string, number>
---@field removeItem table<string, any>
---@field isPzLocked boolean
---@field isInfight boolean
---@param params table<string, travelParams>
function NpcsHandler:travelTo(params)
    for name, dest in pairs(params) do
        local toDest = self:keyword(name)
        toDest:respond(string.format("Do you want to travel to {%s} for {%d} gold?", name, dest.money and dest.money[1] or 0))

        local destinations = self:keyword("destination")
        local words = {}
        for k, v in pairs(self:getKeywords()) do
            if k ~= "destination" then
                table.insert(words, "{".. k .."}")
            end
        end
        destinations:respond("Here are the destinations: " .. table.concat(words, ", "))

        local accept = toDest:keyword("yes")
        accept:respond("I will take you there!")
        local doPlayer = accept:player()
        doPlayer:teleport(dest.position)

        local require = accept:requirements()
        for func, parameters in pairs(dest) do
            if func ~= "position" then
                if require[func] then
                    require[func](require, unpack(parameters))
                else
                    print(debug.traceback("[Warning - NpcsHandler:travelTo] There is no requirement function with the name: ".. func.. "\n".. dump(dest), 1) .."\n")
                end
            end
        end

        local decline = toDest:keyword("no")
        decline:respond("Ok, maybe next time.")
    end
end

---@class NpcModules
---@field modStorage table<string, number>
---@field modExperience number
---@field modMoney number
---@field modPremium number
---@field modItems table<string, any>
---@field modTeleport table<string, any>
---@field modBlessing number
---@field modOutfit table<string, number>
---@field modMount number
---@field modVocation string
---@field modSpell string
---@field modSex number
---@field modTown number
---@field init fun(npc: Npc, player: Player)
---@field storage fun(key: number, value: number)
---@field experience fun(exp: number)
---@field money fun(money: number)
---@field premium fun(days: number)
---@field items fun(items: table<string, any>, container?: number|string, inbox?: boolean)
---@field teleport fun(position: Position, magicEffectFromPos?: MagicEffect_t, magicEffectToPos?: MagicEffect_t)
---@field blessing fun(blessing: number)
---@field outfit fun(outfit: number, addon?: number)
---@field mount fun(mount: number)
---@field vocation fun(vocation: string)
---@field learnSpell fun(spell: string)
---@field sex fun(sex: number)
---@field town fun(town: number)


NpcModules = {}

-- Sets the player the storage value
---@param key number The key to set the storage to.
---@param value number The value to set the storage to.
function NpcModules:storage(key, value)
    self.modStorage = {key = key, value = value}
end

-- Gives the player experience
---@param exp number The amount of experience to give the player.
function NpcModules:experience(exp)
    self.modExperience = exp
end

-- Gives the player money
---@param money number The amount of money to give the player.
function NpcModules:money(money)
    self.modMoney = money
end

-- Gives the player premium days
---@param days number The amount of days to give the player.
function NpcModules:premium(days)
    self.modPremium = days
end

-- This function adds items to the player, if the player does not have storage it goes into the players inbox.
---@param items table The items to add to the player.
---@param container number The container to add the items to.
---@param inbox boolean Whether to add the items to the player's inbox.
function NpcModules:items(items, container, inbox)
    self.modItems = {}
    self.modItems.items = items
    self.modItems.container = container or nil
    self.modItems.inbox = inbox or false
end

-- Sets the position to teleport the player to.
---@param position Position The position to teleport the player to.
---@param magicEffectFromPos MagicEffect_t The magic effect to show at the player's position.
---@param magicEffectToPos MagicEffect_t The magic effect to show at the destination position.
function NpcModules:teleport(position, magicEffectFromPos, magicEffectToPos)
    self.modTeleport = {}
    self.modTeleport.position = position
    self.modTeleport.magicEffectFromPos = magicEffectFromPos or CONST_ME_TELEPORT
    self.modTeleport.magicEffectToPos = magicEffectToPos or CONST_ME_TELEPORT
end

-- Sets the blessing to give the player
---@param blessing number The blessing to give the player.
function NpcModules:blessing(blessing)
    self.modBlessing = blessing
end

-- Gives the player the outfit
---@param outfit1 number The outfit to give the player (female,male).
---@param outfit2 number The second outfit to give the player (female,male).
---@param addon number The outfit addon to give the player.
function NpcModules:outfit(outfit1, outfit2, addon)
    self.modOutfit = {}
    self.modOutfit.outfit1 = outfit1
    self.modOutfit.outfit2 = outfit2
    self.modOutfit.addon = addon or 0
end

-- Gives the player the mount
---@param mount number The mount to give the player.
function NpcModules:mount(mount)
    self.modMount = mount
end

-- Sets the vocation to the player
---@param vocation string The vocation to set the player to.
function NpcModules:vocation(vocation)
    self.modVocation = vocation
end

-- Learns the spell to the player
---@param spell string The spell to learn to the player.
function NpcModules:learnSpell(spell)
    self.modSpell = spell
end

-- Gives the player the sex
---@param sex number The sex the player is set to.
function NpcModules:sex(sex)
    self.modSex = sex
end

-- Sets the town to the player
---@param town number The town id to set the player to.
function NpcModules:town(town)
    self.modTown = town
end

function NpcModules:init(npc, player)
    if self.modTown then
        player:setTown(self.modTown)
    end

    if self.modSex then
        player:setSex(self.modSex)
    end

    if self.modSpell then
        player:learnSpell(self.modSpell)
    end

    if self.modStorage then
        player:setStorageValue(self.modStorage.key, self.modStorage.value)
    end

    if self.modExperience then
        player:addExperience(self.modExperience)
    end

    if self.modMoney then
        if player:canCarryMoney(self.modMoney) then
            player:addMoney(self.modMoney)
        else
            player:addBankBalance(self.modMoney)
        end
    end

    if self.modPremium then
        player:addPremiumDays(self.modPremium)
    end

    if self.modItems then
        local weight = 0
        for _, item in pairs(self.modItems.items) do
            -- checking how much all items would weight
            weight = weight + ItemType(item.item):getWeight(item.count)
        end
        local backpack = player:getSlotItem(CONST_SLOT_BACKPACK) and Container(player:getSlotItem(CONST_SLOT_BACKPACK).uid) or nil
        -- checking if the player has enough capacity or has a backpack
        if player:getFreeCapacity() < weight or not backpack then
            local containerId = self.modItems.container or ITEM_SHOPPING_BAG
            player:sendInboxItems(self.modItems.items, containerId)
            local msg = "The items are to heavy for you to carry. I've sent them to your inbox."
            player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, msg)
        -- checking if the player has enough space in the backpack
        elseif backpack and backpack:getEmptySlots(true) < #self.modItems.items then
            local containerId = self.modItems.container or ITEM_SHOPPING_BAG
            player:sendInboxItems(self.modItems.items, containerId)
            local msg = "You don't have enough space in your backpack. I've sent the items to your inbox."
            player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, msg)
        -- checking if we should add the items by default into inbox
        elseif self.modItems.inbox then
            local containerId = self.modItems.container or ITEM_SHOPPING_BAG
            player:sendInboxItems(self.modItems.items, containerId)
            local msg = "Your items are waiting for you in your inbox."
            player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, msg)
        else
            if self.modItems.container then
                local container = Game.createItem(self.modItems.container, 1)
                for _, item in pairs(self.modItems.items) do
                    container:addItem(item.item, item.count)
                end
                container:moveTo(player)
            else
                for _, item in pairs(self.modItems.items) do
                    player:addItem(item.item, item.count)
                end
            end
        end
    end

    if self.modBlessing then
        player:addBlessing(self.modBlessing)
    end

    if self.modOutfit then
        if self.modOutfit.addon then
            player:addOutfitAddon(self.modOutfit.outfit1, self.modOutfit.addon)
            player:addOutfitAddon(self.modOutfit.outfit2, self.modOutfit.addon)
        else
            player:addOutfit(self.modOutfit.outfit1, self.modOutfit.addon)
            player:addOutfit(self.modOutfit.outfit2, self.modOutfit.addon)
        end
    end

    if self.modMount then
        player:addMount(self.modMount)
    end

    if self.modVocation then
        player:setVocation(self.modVocation)
    end

    if self.modTeleport then
        local handler = NpcsHandler(npc)
        local focus = NpcFocus(npc)
        focus:removeFocus(player)
        closeShopWindow(player)
        local msg = handler:getTalkState(player):getResponse():replaceTags({playerName = player:getName()})
        selfSay(msg, player)
        player:getPosition():sendMagicEffect(self.modTeleport.magicEffectFromPos)
        player:teleportTo(self.modTeleport.position)
        player:getPosition():sendMagicEffect(self.modTeleport.magicEffectToPos)
        handler:setTalkState(handler, player)
        handler:resetData(player)
        return false
    end
end
