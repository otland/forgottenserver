--[[
    >> NpcRequirements <<

    Description:
        This table stores the requirements for an NPC interaction.
        It provides functions to set various requirements such as storage, level, premium status, money, items, infight status, and PZ lock status.
        The init function checks if the player meets all the requirements specified and returns true if all requirements are met, otherwise false.

    Functions:
        NpcRequirements:storage(key, value, operator, failResponse)
        NpcRequirements:storage(key, value, operator, value2, operator2, failResponse, failResponse2)
        NpcRequirements:level(level, operator, failureResponse)
        NpcRequirements:level(level, operator, level2, operator2, failureResponse, failureResponse2)
        NpcRequirements:premium(premium, failResponse)
        NpcRequirements:vocation(vocation, check, failResponse)
        NpcRequirements:money(amount, check, failResponse)
        NpcRequirements:removeMoney(amount, failResponse)
        NpcRequirements:item(item, check, failResponse)
        NpcRequirements:item(item, count, check, failResponse)
        NpcRequirements:item(item, count, subType, check, failResponse)
        NpcRequirements:items(table, check, failResponse)
        NpcRequirements:removeItem(item, count, failResponse)
        NpcRequirements:removeItem(item, count, subType, failResponse)
        NpcRequirements:removeItem(item, count, subType, ignoreEquipped, failResponse)
        NpcRequirements:removeItems(table, failResponse)
        NpcRequirements:isInfight(infight, failResponse)
        NpcRequirements:isPzLocked(pzLocked, failResponse)
        NpcRequirements:outfit(outfit1, outfit2, addon, check, failResponse)
        NpcRequirements:mount(mount, check, failResponse)
        NpcRequirements:learnedSpell(spell, check, failResponse)
        NpcRequirements:blessing(blessing, check, failResponse)
        NpcRequirements:sex(sex, check, failResponse)
        NpcRequirements:party(party, check, failResponse)
        NpcRequirements:guild(guild, check, failResponse)
        NpcRequirements:town(town, check, failResponse)
        NpcRequirements:checkStorageValue(player)
        NpcRequirements:checkLevel(player)

        NpcRequirements:init(player)
]]

---@class NpcRequirements
---@field requireStorage table<string, any>
---@field requireLevel table<string, any>
---@field requirePremium boolean
---@field requireMoney number
---@field requireRemoveMoney number
---@field requireItem table<string, any>
---@field requireRemoveItem table<string, any>
---@field requireInfight boolean
---@field requirePzLocked boolean
---@field requireOutfit table<string, any>
---@field requireMount table<string, any>
---@field requireLearnedSpell table<string, any>
---@field requireBlessing table<string, any>
---@field requireSex table<string, any>
---@field requireParty table<string, any>
---@field requireGuild table<string, any>
---@field requireTown table<string, any>
---@field requireVocation table<string, any>
---@field init fun(player: Player): boolean, string?
---@field storage fun(self: NpcRequirements, key: number, value: number, operator: string, failResponse: string): boolean
---@field storage fun(self: NpcRequirements, key: number, value: number, operator: string, value2: number, operator2: string, failResponse: string, failResponse2: string): boolean
---@field level fun(self: NpcRequirements, level: number, operator: string, failureResponse: string): boolean
---@field level fun(self: NpcRequirements, level: number, operator: string, level2: number, operator2: string, failureResponse: string, failureResponse2: string): boolean
---@field premium fun(self: NpcRequirements, premium: boolean, failResponse: string)
---@field vocation fun(self: NpcRequirements, vocation: string|table<number, string>, check: boolean, failResponse: string)
---@field money fun(self: NpcRequirements, amount: number, check: boolean, failResponse: string)
---@field removeMoney fun(self: NpcRequirements, amount: number, failResponse: string)
---@field item fun(self: NpcRequirements, item: number, check: boolean, failResponse: string)
---@field item fun(self: NpcRequirements, item: number, count: number, check: boolean, failResponse: string)
---@field item fun(self: NpcRequirements, item: number, count: number, subType: number, check: boolean, failResponse: string)
---@field items fun(self: NpcRequirements, table: table<number, table>, check: boolean, failResponse: string)
---@field removeItem fun(self: NpcRequirements, item: number, count: number, failResponse: string)
---@field removeItem fun(self: NpcRequirements, item: number, count: number, subType: number, failResponse: string)
---@field removeItem fun(self: NpcRequirements, item: number, count: number, subType: number, ignoreEquipped: boolean, failResponse: string)
---@field removeItems fun(self: NpcRequirements, table: table<number, table>, failResponse: string)
---@field isInfight fun(self: NpcRequirements, infight: boolean, failResponse: string)
---@field isPzLocked fun(self: NpcRequirements, pzLocked: boolean, failResponse: string)
---@field outfit fun(self: NpcRequirements, outfit1: number, outfit2: number, addon: number, check: boolean, failResponse: string)
---@field mount fun(self: NpcRequirements, mount: number, check: boolean, failResponse: string)
---@field learnedSpell fun(self: NpcRequirements, spell: string, check: boolean, failResponse: string)
---@field blessing fun(self: NpcRequirements, blessing: number, check: boolean, failResponse: string)
---@field sex fun(self: NpcRequirements, sex: number, check: boolean, failResponse: string)
---@field party fun(self: NpcRequirements, check: boolean, failResponse: string)
---@field guild fun(self: NpcRequirements, check: boolean, failResponse: string)
---@field town fun(self: NpcRequirements, town: number, check: boolean, failResponse: string)
---@field checkStorageValue fun(self: NpcRequirements, player: Player): boolean, string|nil
---@field checkLevel fun(self: NpcRequirements, player: Player): boolean, string|nil
---@type table<string, NpcRequirements>

NpcRequirements = {}

---@alias storage1 fun(self: NpcRequirements, key: number, value: number, operator: string, failResponse: string)
local function storage1(self, key, value, operator, failResponse)
    self.requireStorage = {key = key, value = value, operator = operator, failResponse = failResponse}
end

---@alias storage2 fun(self: NpcRequirements, key: number, value: number, operator: string, value2: number, operator2: string, failResponse: string, failResponse2: string)
local function storage2(self, key, value, operator, value2, operator2, failResponse, failResponse2)
    self.requireStorage = {key = key, value = value, operator = operator, value2 = value2, operator2 = operator2, failResponse = failResponse, failResponse2 = failResponse2}
end

-- Sets the storage requirement for a keyword.
---@class storageParams
---@field key number
---@field value number
---@field operator string
---@field value2 number
---@field operator2 string
---@field failResponse string
---@field failResponse2 string
---@param ... storageParams
---@return boolean
---@see storage1
---@see storage2
function NpcRequirements:storage(...)
    local args = #{...}
    if args == 4 then
        storage1(self, ...)
    elseif args == 7 then
        storage2(self, ...)
    else
        print(debug.traceback("[Warning - NpcRequirements:storage] invalid number of arguments: [".. args .."] Function expected 4 or 7 arguments.", 1) .."\n")
        return false
    end
    return true
end

---@alias level1 fun(self: NpcRequirements, level: number, operator: string, failResponse: string)
local function level1(self, level, operator, failResponse)
    self.requireLevel = {level = level, operator = operator, failResponse = failResponse}
end

---@alias level2 fun(self: NpcRequirements, level: number, operator: string, level2: number, operator2: string, failResponse: string, failResponse2: string)
local function level2(self, level, operator, level2, operator2, failResponse, failResponse2)
    self.requireLevel = {level = level, operator = operator, level2 = level2, operator2 = operator2, failResponse = failResponse, failResponse2 = failResponse2}
end

-- Sets the level requirement for a keyword.
---@class levelParams
---@field level number
---@field operator string
---@field level2 number
---@field operator2 string
---@field failResponse string
---@field failResponse2 string
---@param ... levelParams
---@return boolean
function NpcRequirements:level(...)
    local args = #{...}
    if args == 3 then
        level1(self, ...)
    elseif args == 6 then
        level2(self, ...)
    else
        print(debug.traceback("[Warning - NpcRequirements:level] invalid number of arguments: [".. args .."] Function expected 3 or 6 arguments.", 1) .."\n")
        return false
    end
    return true
end

-- Sets the premium requirement for a keyword.
---@param premium boolean: If true, the player must be premium to advance the talk state.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:premium(premium, failResponse)
    self.requirePremium = {premium = premium, failResponse = failResponse}
end

-- Sets the vocation requirement for a keyword.
---@param vocation string|table<number, string> The required vocation name.
---@param check boolean If true, the player must have one of the vocations. If false, the player is not allowed to have one of the vocations.
function NpcRequirements:vocation(vocation, check, failResponse)
    if type(vocation) == "string" then
        vocation = {vocation}
    end
    self.requireVocation = {vocation = vocation, check = check, failResponse = failResponse}
end

-- Sets the money requirement for a keyword.
---@param amount number: The required amount of money.
---@param check boolean If true, the player must have at least the amount of money. If false, the player must have less than the amount of money.
function NpcRequirements:money(amount, check, failResponse)
    self.requireMoney = {amount = amount, check = check, failResponse = failResponse}
end

-- Sets the money to remove for a keyword.
---@param amount number: The amount of money to remove.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:removeMoney(amount, failResponse)
    self.requireRemoveMoney = {amount = amount, failResponse = failResponse}
end

local function item1(self, item, check, failResponse)
    self.requireItem = {items = {item = item, count = 1, subType = -1}, check = check, failResponse = failResponse}
end

local function item2(self, item, count, check, failResponse)
    self.requireItem = {items = {item = item, count = count, subType = -1}, check = check, failResponse = failResponse}
end

local function item3(self, item, count, subType, check, failResponse)
    self.requireItem = {items = {item = item, count = count, subType = subType}, check = check, failResponse = failResponse}
end

-- Sets the item requirement for a keyword.
function NpcRequirements:item(...)
    local args = #{...}
    if args == 3 then
        item1(self, ...)
    elseif args == 4 then
        item2(self, ...)
    elseif args == 5 then
        item3(self, ...)
    else
        print(debug.traceback("[Warning - NpcRequirements:item] invalid number of arguments: [".. args .."] Function expected 3, 4 or 5 arguments.", 1) .."\n")
        return false
    end
    return true
end

-- Sets the items requirement for a keyword.
---@param table table<number, table>: The table of items to require.
---@param check boolean: If true, the player must have all the items. If false, the player must not have any of the items.
function NpcRequirements:items(table, check, failResponse)
    self.requireItem = {items = table, check = check, failResponse = failResponse}
end

local function removeItem1(self, item, count, failResponse)
    self.requireRemoveItem = {items = {item = item, count = count, subType = -1, ignoreEquipped = true}, failResponse = failResponse}
end

local function removeItem2(self, item, count, subType, failResponse)
    self.requireRemoveItem = {items = {item = item, count = count, subType = subType, ignoreEquipped = true}, failResponse = failResponse}
end

local function removeItem3(self, item, count, subType, ignoreEquipped, failResponse)
    self.requireRemoveItem = {items ={item = item, count = count, subType = subType, ignoreEquipped = ignoreEquipped}, failResponse = failResponse}
end

-- Sets the item to remove for a keyword.
function NpcRequirements:removeItem(...)
    local args = #{...}
    if args == 3 then
        removeItem1(self, ...)
    elseif args == 4 then
        removeItem2(self, ...)
    elseif args == 5 then
        removeItem3(self, ...)
    else
        print(debug.traceback("[Warning - NpcRequirements:removeItem] invalid number of arguments: [".. args .."] Function expected 3, 4 or 5 arguments.", 1) .."\n")
        return false
    end
    return true
end

-- Sets the items to remove for a keyword.
---@param table table<number, table>: The table of items to remove.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:removeItems(table, failResponse)
    self.requireRemoveItem = {items = table, failResponse = failResponse}
    for _, item in pairs(self.requireRemoveItem.items) do
        item.ignoreEquipped = item.ignoreEquipped or true
    end
end

-- Sets the infight requirement for a keyword.
---@param infight boolean: If true, the player must be in a fight.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:isInfight(infight, failResponse)
    self.requireInfight = {infight = infight, failResponse = failResponse}
end

-- Sets the PZ lock requirement for a keyword.
---@param pzLocked boolean: If true, the player must be PZ locked.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:isPzLocked(pzLocked, failResponse)
    self.requirePzLocked = {pzLocked = pzLocked, failResponse = failResponse}
end

-- Sets the outfit requirement for a keyword.
---@param outfit1 number: The outfit ID to require (male,female).
---@param outfit2 number: The second outfit ID to require (male,female).
---@param addon number: The addon to require. 0 for no addon, 1 for first addon, 2 for second addon, 3 for both addons.
---@param check boolean: If true, the player must have the outfit. If false, the player must not have the outfit.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:outfit(outfit1, outfit2, addon, check, failResponse)
    self.requireOutfit = {outfit1 = outfit1, outfit2 = outfit2, addon = addon, check = check, failResponse = failResponse}
end

-- Sets the mount requirement for a keyword.
---@param mount number: The mount ID to require.
---@param check boolean: If true, the player must have the mount. If false, the player must not have the mount.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:mount(mount, check, failResponse)
    self.requireMount = {mount = mount, check = check, failResponse = failResponse}
end

-- Sets the learnedSpell requirement for a keyword.
---@param spell string: The spell name to require.
---@param check boolean: If true, the player must have the spell. If false, the player must not have the spell.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:learnedSpell(spell, check, failResponse)
    self.requireLearnedSpell = {spell = spell, check = check, failResponse = failResponse}
end

-- Sets the blessing requirement for a keyword.
---@param blessing number: The blessing ID to require.
---@param check boolean: If true, the player must have the blessing. If false, the player must not have the blessing.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:blessing(blessing, check, failResponse)
    self.requireBlessing = {blessing = blessing, check = check, failResponse = failResponse}
end

-- Sets the player sex requirement for a keyword.
---@param sex number: The sex of the player to require.
---@param check boolean: If true, the player
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:sex(sex, check, failResponse)
    self.requireSex = {sex = sex, check = check, failResponse = failResponse}
end

-- Sets the party requirement for a keyword.
---@param check boolean: If true, the player must be in a party. If false, the player must not be in a party.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:party(check, failResponse)
    self.requireParty = {check = check, failResponse = failResponse}
end

-- Sets the guild requirement for a keyword.
---@param check boolean: If true, the player must have a guild. If false, the player must not have a guild.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:guild(check, failResponse)
    self.requireGuild = {check = check, failResponse = failResponse}
end

-- Sets the town requirement for a keyword.
---@param town number: The town ID to require.
---@param check boolean: If true, the player must be in the town. If false, the player must not be in the town.
---@param failResponse string: The message to send to the player if the requirements are not met.
function NpcRequirements:town(town, check, failResponse)
    self.requireTown = {town = town, check = check, failResponse = failResponse}
end

-- Uses the correct operator on storage values, depending on string.
---@param player Player The player to check the storage value for.
---@return boolean The result of the storage value check.
---@return nil|string The message to send to the player if the requirements are not met.
function NpcRequirements:checkStorageValue(player)
    local storage = self.requireStorage
    if storage.operator == "==" then
        return player:getStorageValue(storage.key) == storage.value, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
    elseif storage.operator == "~=" then
        return player:getStorageValue(storage.key) ~= storage.value, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
    elseif storage.operator == "<" then
        return player:getStorageValue(storage.key) < storage.value, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
    elseif storage.operator == ">" then
        if storage.value2 and storage.operator2 == "<" then
            if player:getStorageValue(storage.key) <= storage.value then
                print(storage.value)
                return false, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
            end
            if player:getStorageValue(storage.key) >= storage.value2 then
                print(storage.value2)
                return false, storage.failResponse2:replaceTags({storage = {key = storage.key, value = storage.value2}})
            end
            return true
        elseif storage.value2 and storage.operator2 == "<=" then
            if player:getStorageValue(storage.key) <= storage.value then
                return false, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
            end
            if player:getStorageValue(storage.key) > storage.value2 then
                return false, storage.failResponse2:replaceTags({storage = {key = storage.key, value = storage.value2}})
            end
            return true
        end
        return player:getStorageValue(storage.key) > storage.value, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
    elseif storage.operator == "<=" then
        return player:getStorageValue(storage.key) <= storage.value, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
    elseif storage.operator == ">=" then
        if storage.value2 and storage.operator2 == "<" then
            if player:getStorageValue(storage.key) < storage.value then
                return false, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
            end
            if player:getStorageValue(storage.key) >= storage.value2 then
                return false, storage.failResponse2:replaceTags({storage = {key = storage.key, value = storage.value2}})
            end
            return true
        elseif storage.value2 and storage.operator2 == "<=" then
            if player:getStorageValue(storage.key) < storage.value then
                return false, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
            end
            if player:getStorageValue(storage.key) > storage.value2 then
                return false, storage.failResponse2:replaceTags({storage = {key = storage.key, value = storage.value2}})
            end
            return true
        end
        return player:getStorageValue(storage.key) >= storage.value, storage.failResponse:replaceTags({storage = {key = storage.key, value = storage.value}})
    end
    print(debug.traceback("[Warning - NpcRequirements:checkStorageValue] Npc: ".. Npc(getNpcCid()):getName() .." has operator: ".. storage.operator .." which does not exist.", 1) .."\n")
    return false
end

-- Uses the correct operator on level, depending on string.
---@param player Player The player to check the storage value for.
---@return boolean The result of the storage value check.
---@return nil|string The message to send to the player if the requirements are not met.
function NpcRequirements:checkLevel(player)
    local params = self.requireLevel
    if params.operator == "==" then
        return player:getLevel() == params.level, params.failResponse:replaceTags({playerLevel = params.level})
    elseif params.operator == "~=" then
        return player:getLevel() ~= params.level, params.failResponse:replaceTags({playerLevel = params.level})
    elseif params.operator == "<" then
        return player:getLevel() < params.level, params.failResponse:replaceTags({playerLevel = params.level})
    elseif params.operator == ">" then
        if params.level2 and params.operator2 == "<" then
            if player:getLevel() <= params.level then
                return false, params.failResponse:replaceTags({playerLevel = params.level})
            end
            if player:getLevel() >= params.level2 then
                return false, params.failResponse2:replaceTags({playerLevel = params.level2})
            end
            return true
        elseif params.level2 and params.operator2 == "<=" then
            if player:getLevel() <= params.level then
                return false, params.failResponse:replaceTags({playerLevel = params.level})
            end
            if player:getLevel() > params.level2 then
                return false, params.failResponse2:replaceTags({playerLevel = params.level2})
            end
            return true
        end
        return player:getLevel() > params.level, params.failResponse:replaceTags({playerLevel = params.level})
    elseif params.operator == "<=" then
        return player:getLevel() <= params.level, params.failResponse:replaceTags({playerLevel = params.level})
    elseif params.operator == ">=" then
        if params.level2 and params.operator2 == "<" then
            if player:getLevel() < params.level then
                return false, params.failResponse:replaceTags({playerLevel = params.level})
            end
            if player:getLevel() >= params.level2 then
                return false, params.failResponse2:replaceTags({playerLevel = params.level2})
            end
            return true
        elseif params.level2 and params.operator2 == "<=" then
            if player:getLevel() < params.level then
                return false, params.failResponse:replaceTags({playerLevel = params.level})
            end
            if player:getLevel() > params.level2 then
                return false, params.failResponse2:replaceTags({playerLevel = params.level2})
            end
            return true
        end
        return player:getLevel() >= params.level, params.failResponse:replaceTags({playerLevel = params.level})
    end
    print(debug.traceback("[Warning - NpcRequirements:checkLevel] Npc: ".. Npc(getNpcCid()):getName() .." has operator: ".. params.operator .." which does not exist.", 1) .."\n")
    return false
end

-- Initializes the requirements for a player.
---@param player Player: The player to check the requirements for.
---@return boolean: True if the player meets all requirements, false otherwise.
---@return nil|string (optional): The message to send to the player if the requirements are not met.
function NpcRequirements:init(player)
    if self.requireOutfit then
        local outfit = self.requireOutfit
        if outfit.check and not player:hasOutfit(outfit.outfit1, outfit.addon) and not player:hasOutfit(outfit.outfit2, outfit.addon) then
            return false, outfit.failResponse
        elseif not outfit.check and player:hasOutfit(outfit.outfit1, outfit.addon) and player:hasOutfit(outfit.outfit2, outfit.addon) then
            return false, outfit.failResponse
        end
    end

    if self.requireMount then
        local mount = self.requireMount
        if mount.check and not player:hasMount(mount.mount) then
            return false, mount.failResponse
        elseif not mount.check and player:hasMount(mount.mount) then
            return false, mount.failResponse
        end
    end

    if self.requireLearnedSpell then
        local spell = self.requireLearnedSpell
        if spell.check and not player:hasLearnedSpell(spell.spell) then
            return false, spell.failResponse
        elseif not spell.check and player:hasLearnedSpell(spell.spell) then
            return false, spell.failResponse
        end
    end

    if self.requireBlessing then
        local blessing = self.requireBlessing
        if blessing.check and not player:hasBlessing(blessing.blessing) then
            return false, blessing.failResponse
        elseif not blessing.check and player:hasBlessing(blessing.blessing) then
            return false, blessing.failResponse
        end
    end

    if self.requireSex then
        local sex = self.requireSex
        if sex.check and player:getSex() ~= sex.sex then
            return false, sex.failResponse
        elseif not sex.check and player:getSex() == sex.sex then
            return false, sex.failResponse
        end
    end

    if self.requireParty then
        local party = self.requireParty
        if party.check and not player:getParty() then
            return false, party.failResponse
        elseif not party.check and player:getParty() then
            return false, party.failResponse
        end
    end

    if self.requireGuild then
        local guild = self.requireGuild
        if guild.check and not player:getGuild() then
            return false, guild.failResponse
        elseif not guild.check and player:getGuild() then
            return false, guild.failResponse
        end
    end

    if self.requireTown then
        local town = self.requireTown
        if town.check and not player:getTown():getId() == town.town then
            return false, town.failResponse
        elseif not town.check and player:getTown():getId() == town.town then
            return false, town.failResponse
        end
    end

    if self.requireStorage then
        local ret, msg = self:checkStorageValue(player)
        if not ret then
            return false, msg
        end
    end

    if self.requireLevel then
        local ret, msg = self:checkLevel(player)
        if not ret then
            return false, msg
        end
    end

    if self.requirePremium then
        if self.requirePremium.premium and not player:isPremium() then
            return false, self.requirePremium.failResponse
        elseif not self.requirePremium.premium and player:isPremium() then
            return false, self.requirePremium.failResponse
        end
    end

    if self.requireVocation then
        if self.requireVocation.check and not table.contains(self.requireVocation.vocation, player:getVocation():getName():lower()) and not table.contains(self.requireVocation.vocation, player:getVocation():getBase():getName():lower()) then
            return false, self.requireVocation.failResponse:replaceTags({vocation = table.concat(self.requireVocation.vocation, ", ")})
        elseif not self.requireVocation.check and table.contains(self.requireVocation.vocation, player:getVocation():getName():lower()) and table.contains(self.requireVocation.vocation, player:getVocation():getBase():getName():lower()) then
            return false, self.requireVocation.failResponse:replaceTags({vocation = table.concat(self.requireVocation.vocation, ", ")})
        end
    end

    if self.requireMoney then
        if self.requireMoney.check and player:getMoney() < self.requireMoney.amount then
            return false, self.requireMoney.failResponse:replaceTags({total = self.requireMoney.amount})
        elseif not self.requireMoney.check and player:getMoney() >= self.requireMoney.amount then
            return false, self.requireMoney.failResponse:replaceTags({total = self.requireMoney.amount})
        end
    end

    if self.requireItem then
        for i = 1, #self.requireItem.items do
            local index = self.requireItem.items[i]
            local id = ItemType(index.item):getId()
            local name = ItemType(index.item):getName()
            if self.requireItem.check and player:getItemCount(id, index.subType, index.ignoreEquipped) < index.count then
                return false, self.requireItem.failResponse:replaceTags({itemName = name, amount = index.count})
            elseif not self.requireItem.check and player:getItemCount(id, index.subType, index.ignoreEquipped) >= index.count then
                return false, self.requireItem.failResponse:replaceTags({itemName = name, amount = index.count})
            end
        end
    end

    if self.requireInfight then
        if self.requireInfight.infight and not player:getCondition(CONDITION_INFIGHT, CONDITIONID_DEFAULT) and not player:getCondition(CONDITION_INFIGHT, CONDITIONID_COMBAT) then
            return false, self.requireInfight.failResponse
        end
        if not self.requireInfight.infight and player:getCondition(CONDITION_INFIGHT, CONDITIONID_DEFAULT) and player:getCondition(CONDITION_INFIGHT, CONDITIONID_COMBAT) then
            return false, self.requireInfight.failResponse
        end
    end

    if self.requirePzLocked then
        if self.requirePzLocked.pzLocked and not player:isPzLocked() then
            return false, self.requirePzLocked.failResponse
        end
        if not self.requirePzLocked.pzLocked and player:isPzLocked() then
            return false, self.requirePzLocked.failResponse
        end
    end

    if self.requireRemoveMoney and self.requireRemoveItem then
        for i = 1, #self.requireRemoveItem.items do
            local index = self.requireRemoveItem.items[i]
            local id = ItemType(index.item):getId()
            local name = ItemType(index.item):getName()
            if player:getItemCount(id, index.subType, index.ignoreEquipped) < index.count then
                return false, self.requireRemoveItem.failResponse:replaceTags({itemName = name, amount = index.count})
            end
        end
        if player:getMoney() < self.requireRemoveMoney.amount then
            return false, self.requireRemoveMoney.failResponse:replaceTags({total = self.requireMoney.amount})
        end
    end

    if self.requireRemoveMoney then
        if not player:removeMoney(self.requireRemoveMoney.amount) then
            return false, self.requireRemoveMoney.failResponse:replaceTags({total = self.requireRemoveMoney.amount})
        end
    end

    if self.requireRemoveItem then
        for i = 1, #self.requireRemoveItem.items do
            local index = self.requireRemoveItem.items[i]
            local id = ItemType(index.item):getId()
            local name = ItemType(index.item):getName()
            if player:getItemCount(id, index.subType, index.ignoreEquipped) < index.count then
                return false, self.requireRemoveItem.failResponse:replaceTags({itemName = name, amount = index.count})
            end
        end
        for i = 1, #self.requireRemoveItem.items do
            local index = self.requireRemoveItem.items[i]
            local id = ItemType(index.item):getId()
            local name = ItemType(index.item):getName()
            if not player:removeItem(id, index.count, index.subType, index.ignoreEquipped) then
                return false, self.requireRemoveItem.failResponse:replaceTags({itemName = name, amount = index.count})
            end
        end
    end
    return true
end
