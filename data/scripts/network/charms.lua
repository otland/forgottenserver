local config = {
    faccLimit = 2,
    premiumLimit = 6,
    premiumDiscount = 0.75, -- percent multipler
    charmDataSize = 0,
    charmCostPerLevel = 100
}

local ACTION_TYPE = {
    UNLOCK_RUNE = 0,
    SET_CREATURE = 1,
    REMOVE_CREATURE = 2
}

CHARMS_TYPE = {
    WOUND = 0,
    ENFLAME = 1,
    POISON = 2,
    FREEZE = 3,
    ZAP = 4,
    CURSE = 5,
    CRIPPLE = 6,
    PARRY = 7,
    DODGE = 8,
    ADRENALINE_BOOST = 9,
    NUMB = 10,
    CLEANS = 11,
    BLSEE = 12,
    SCAVANGE = 13,
    GUT = 14,
    LOW_BLOW = 15,
    DIVINE_WRATH = 16,
    VAMPIRIC_EMBRACE = 17,
    VOID_CALL = 18,

    LAST = 18
}

CHARMS_DATA = {
    [CHARMS_TYPE.WOUND] = {
        id = CHARMS_TYPE.WOUND,
        name = "Wound",
        description = "Triggers on a creature with a certain chance and deals 5% of its initial hit points as Physical Damage once.",
        percentage = 5,
        chance = 10,
        effect = CONST_ME_HITAREA,
        cost = 600,
        damageType = COMBAT_PHYSICALDAMAGE
    },
    [CHARMS_TYPE.ENFLAME] = {
        id = CHARMS_TYPE.ENFLAME,
        name = "Enflame",
        description = "Triggers on a creature with a certain chance and deals 5% of its initial hit points as Fire Damage once.",
        percentage = 5,
        chance = 10,
        effect = CONST_ME_HITBYFIRE,
        cost = 1000,
        damageType = COMBAT_FIREDAMAGE
    },
    [CHARMS_TYPE.POISON] = {
        id = CHARMS_TYPE.POISON,
        name = "Poison",
        description = "Triggers on a creature with a certain chance and deals 5% of its initial hit points as Earth Damage once.",
        percentage = 5,
        chance = 10,
        effect = CONST_ME_GREEN_RINGS,
        cost = 600,
        damageType = COMBAT_EARTHDAMAGE
    },
    [CHARMS_TYPE.FREEZE] = {
        id = CHARMS_TYPE.FREEZE,
        name = "Freez",
        description = "Triggers on a creature with a certain chance and deals 5% of its initial hit points as Ice Damage once.",
        percentage = 5,
        chance = 10,
        effect = CONST_ME_ICEATTACK,
        cost = 800,
        damageType = COMBAT_ICEDAMAGE
    },
    [CHARMS_TYPE.ZAP] = {
        id = CHARMS_TYPE.ZAP,
        name = "Zap",
        description = "Triggers on a creature with a certain chance and deals 5% of its initial hit points as Energy Damage once.",
        percentage = 5,
        chance = 10,
        effect = CONST_ME_ENERGYHIT,
        cost = 800,
        damageType = COMBAT_ENERGYDAMAGE
    },
    [CHARMS_TYPE.CURSE] = {
        id = CHARMS_TYPE.CURSE,
        name = "Curse",
        description = "Triggers on a creature with a certain chance and deals 5% of its initial hit points as Death Damage once.",
        percentage = 5,
        chance = 10,
        effect = CONST_ME_SMALLCLOUDS,
        cost = 900,
        damageType = COMBAT_DEATHDAMAGE
    },
    [CHARMS_TYPE.DIVINE_WRATH] = {
        id = CHARMS_TYPE.DIVINE_WRATH,
        name = "Divine Wrath",
        description = "Triggers on a creature with a certain chance and deals 5% of its initial hit points as Holy Damage once.",
        percentage = 5,
        chance = 10,
        effect = CONST_ME_HOLYDAMAGE,
        cost = 1500,
        damageType = COMBAT_HOLYDAMAGE
    },
    [CHARMS_TYPE.DODGE] = {
        id = CHARMS_TYPE.DODGE,
        name = "Dodge",
        description = "Dodges an attack with a certain chance without taking any damage at all.",
        chance = 10,
        effect = 231,
        cost = 1000
    },
}

local function getCharmRemoveCost(player)
    if player:isPremium() then
        return config.charmCostPerLevel * player:getLevel() * config.premiumDiscount
    end
    return config.charmCostPerLevel * player:getLevel()
end

local function getCompletedBestiary(player)
    local bestiaryClasses = Game.getBestiary()
    local unlockedRaces = {}
    for _, class in pairs(bestiaryClasses) do
        for _, monsterType in ipairs(class.monsterTypes) do
            local info = monsterType:getBestiaryInfo()
            if info then
                if info.raceId and player:getStorageValue(PlayerStorageKeys.bestiaryKillsBase + info.raceId) >=
                    info.mastery then
                    table.insert(unlockedRaces, info.raceId)
                end
            end
        end
    end
    return unlockedRaces
end

local function getTableLength(tbl)
    local getN = 0
    for n in pairs(tbl) do
        getN = getN + 1
    end
    return getN
end

do 
    config.charmDataSize = getTableLength(CHARMS_DATA)
end

local function getMaxCharmCreatures(player)
    if player:isPremium() then
        return config.premiumLimit
    else
        return config.faccLimit
    end
end

function sendCharmData(player)
    local msg = NetworkMessage()
    msg:addByte(0xD8)
    msg:addU32(player:getCharmPoints()) -- charm points

    msg:addByte(config.charmDataSize) -- charm count
    -- charm block
    for i = 0, CHARMS_TYPE.LAST, 1 do
        local charmData = CHARMS_DATA[i]
        if charmData then
            msg:addByte(charmData.id) -- charmId
            msg:addString(charmData.name)
            msg:addString(charmData.description)
            msg:addByte(2) -- charm level (0-2)
            msg:addU16(charmData.cost) -- cost in charm points
            msg:addByte(player:isCharmUnlocked(charmData.id)) -- is unlocked
            local raceIdAssigned = player:getCharmMonster(charmData.id)
            if raceIdAssigned == 0 then
                msg:addByte(0) -- is monster assigned
            else
                msg:addByte(1) -- is monster assigned
                msg:addU16(raceIdAssigned) -- raceId
                msg:addU32(getCharmRemoveCost(player)) -- remove cost
            end
        end
    end

    msg:addByte(getMaxCharmCreatures(player)) -- amount of charms you can assign (0-254), 255 - unlimited

    -- list of monsters you can assign your charms to
    local typeIds = getCompletedBestiary(player)
    msg:addU16(#typeIds) -- creatures count
    for _, value in ipairs(typeIds) do
        msg:addU16(value) -- raceId (shows on the list)
    end
    msg:sendToPlayer(player)
    msg:delete()
end

local function buyCharmRune(player, runeId)
    if not CHARMS_DATA[runeId] then
        return false
    end

    if CHARMS_DATA[runeId].cost <= player:getCharmPoints() then
        player:unlockCharm(runeId)
        player:popupFYI("You successfully unlocked '" .. CHARMS_DATA[runeId].name .. "' for " ..
                            CHARMS_DATA[runeId].cost .. " charm points.")
        player:removeCharmPoints(CHARMS_DATA[runeId].cost)
    else
        player:popupFYI("You don't have enough charm points to unlock this rune")
    end

    return true
end

local function isCharmedCreaturesLimitReached(player, count)
    if getMaxCharmCreatures(player) > count then
        return false
    end

    return true
end

local function isCharmedMonsterAlreadyCharmed(player, raceId)
    for i = 0, CHARMS_TYPE.LAST, 1 do
        if CHARMS_DATA[i] then
            if player:getCharmMonster(CHARMS_DATA[i].id) == raceId then
                return true
            end
        end
    end

    return false
end

local function setCharmCreature(player, runeId, raceId)
    local count = 0
    for i = 0, CHARMS_TYPE.LAST, 1 do
        if CHARMS_DATA[i] then
            if player:getCharmMonster(CHARMS_DATA[i].id) > 0 then
                count = count + 1
            end
        end
    end

    if not CHARMS_DATA[runeId] or not MonsterType(raceId) then
        print(">> setCharmCreature: invalid request")
        return
    end

    if CHARMS_DATA[runeId].id == CHARMS_TYPE.SCAVANGE then
        player:popupFYI("This charm is always active, monster not required.")
        return
    end

    if isCharmedCreaturesLimitReached(player, count) then
        player:popupFYI("You don't have any charm slots available.")
        return
    end

    if isCharmedMonsterAlreadyCharmed(player, raceId) then
        player:popupFYI("Charm failed, " .. MonsterType(raceId):getName() .. " is already charmed.")
        return
    end

    player:setCharmMonster(runeId, raceId)
    player:popupFYI("Charm '" .. CHARMS_DATA[runeId].name .. "' has been successfully set to " ..
                        MonsterType(raceId):getName())
end

local function removeCharmCreature(player, runeId)
    if not CHARMS_DATA[runeId] then
        print(">> removeCharmCreature: invalid request")
        return
    end

    if player:getTotalMoney() < getCharmRemoveCost(player) then
        player:popupFYI("You don't have enough money.")
        return
    end

    player:popupFYI("Charm '" .. CHARMS_DATA[runeId].name .. "' has been successfully cleared.")
    player:removeTotalMoney(getCharmRemoveCost(player))
    player:setCharmMonster(runeId, 0)
end

local handlerBuy = PacketHandler(0xE4)
function handlerBuy.onReceive(player, msg)
    local runeId = msg:getByte()
    local action = msg:getByte()
    local raceId = msg:getU16()

    if action == ACTION_TYPE.UNLOCK_RUNE then
        buyCharmRune(player, runeId)
    elseif action == ACTION_TYPE.SET_CREATURE then
        setCharmCreature(player, runeId, raceId)
    elseif action == ACTION_TYPE.REMOVE_CREATURE then
        removeCharmCreature(player, runeId)
    end
    sendCharmData(player)
    player:sendResourceBalance(RESOURCE_BANK_BALANCE, player:getBankBalance())
    player:sendResourceBalance(RESOURCE_GOLD_EQUIPPED, player:getMoney())
    player:sendResourceBalance(RESOURCE_CHARM_POINTS, player:getCharmPoints())
end
handlerBuy:register()

local combatCharms = {CHARMS_TYPE.WOUND, CHARMS_TYPE.ENFLAME, CHARMS_TYPE.POISON, CHARMS_TYPE.FREEZE, CHARMS_TYPE.ZAP,
                      CHARMS_TYPE.CURSE, CHARMS_TYPE.DIVINE_WRATH}

local charmCombat = Combat()
charmCombat:setParameter(COMBAT_PARAM_USECHARGES, 1)

local isCharmTrigerred = false

local charmDamage = CreatureEvent("CharmMonsterDamageTrigger")
function charmDamage.onHealthChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType,
    origin)

    if isCharmTrigerred == true then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    if not attacker or not creature then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    if not (creature:isMonster() and attacker:isPlayer()) then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    local bestiaryInfo = creature:getType():getBestiaryInfo()
    if not bestiaryInfo then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    if bestiaryInfo.raceId == 0 then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    local charm = nil
    for index, value in ipairs(combatCharms) do
        if attacker:getCharmMonster(CHARMS_DATA[value].id) == bestiaryInfo.raceId then
            charm = CHARMS_DATA[value]
            break
        end
    end

    if not charm then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    if math.random(0, 100) <= charm.chance then
        isCharmTrigerred = true
        local damage = math.floor(creature:getMaxHealth() / 100 * charm.percentage)
        charmCombat:setParameter(COMBAT_PARAM_TYPE, charm.damageType)
        charmCombat:setParameter(COMBAT_PARAM_EFFECT, charm.effect)
        charmCombat:setFormula(COMBAT_FORMULA_DAMAGE, -damage, 0, -damage, 0)
        charmCombat:execute(attacker, Variant(creature:getId()))
        isCharmTrigerred = false
    end
    return primaryDamage, primaryType, secondaryDamage, secondaryType
end
charmDamage:register()

local charmDamage = CreatureEvent("CharmPlayerDamageReceived")
function charmDamage.onHealthChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
    if not attacker then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    if not (creature:isPlayer() and attacker:isMonster()) then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    local bestiaryInfo = attacker:getType():getBestiaryInfo()
    if not bestiaryInfo then
        return primaryDamage, primaryType, secondaryDamage, secondaryType
    end

    local charm = CHARMS_DATA[CHARMS_TYPE.DODGE]

    if math.random(0, 100) <= charm.chance and creature:getCharmMonster(charm.id) == bestiaryInfo.raceId and bestiaryInfo.raceId ~= 0 then
        creature:getPosition():sendMagicEffect(charm.effect)
        creature:sendTextMessage(MESSAGE_DAMAGE_RECEIVED, "You blocked damage from " .. attacker:getDescription())
        return 0, primaryType, 0, secondaryType
    end

    return primaryDamage, primaryType, secondaryDamage, secondaryType
end
charmDamage:register()

local playerCharm = CreatureEvent("CharmPlayerDamageReceivedLogin")
function playerCharm.onLogin(player)
    player:registerEvent("CharmPlayerDamageReceived")
    return true
end
playerCharm:register()

local ec = EventCallback
ec.onSpawn = function(monster, position, startup, artificial)
	monster:registerEvent("CharmMonsterDamageTrigger")
    return true
end
ec:register()