local action = Action()

local skillChoices = {{
    text = "Sword Fighting and Shielding",
    skill = SKILL_SWORD
}, {
    text = "Axe Fighting and Shielding",
    skill = SKILL_AXE
}, {
    text = "Club Fighting and Shielding",
    skill = SKILL_CLUB
}, {
    text = "Distance Fighting and Shielding",
    skill = SKILL_DISTANCE
}, {
    text = "Magic Level and Shielding",
    skill = SKILL_MAGLEVEL
}}

local function canUse(player, bed)
    if player:getZone() ~= ZONE_PROTECTION then
        return false
    end

    if Beds.RequiresPremium then
        local premiumEnds = player:getPremiumEndsAt()
        if premiumEnds <= os.time() then
            return false
        end
    end

    local tile = bed:getTile()
    if not tile then
        return false
    end

    local house = tile:getHouse()
    if not house then
        return false
    end

    return true
end

local function sleep(player, headboardPos)
    local tile = Tile(headboardPos)
    if not tile then
        return false
    end

    local headboard = tile:getHeadboard()
    if not headboard then
        return false
    end

    local footboard = headboard:getPartnerBed()
    if not footboard or not footboard:isFootboard() then
        return false
    end

    headboardPos:sendMagicEffect(CONST_ME_SLEEP)

    if not player:teleportTo(headboardPos, true) then
        return false
    end

    headboard:setSleeper(player)
    footboard:setSleeper(player)

    addEvent(function(pid)
        local player = Player(pid)
        if player then
            player:remove()
        end
    end, SCHEDULER_MINTICKS, player:getId())
    return true
end

local function abortOfflineTraining(player)
    player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Offline training aborted.")
end

local function sendOfflineTrainingModal(player, headboardPos)
    local offlineTrainingModal = ModalWindow {
        title = "Choose a Skill",
        message = "Please choose a skill:",
        priority = true
    }

    offlineTrainingModal:setDefaultEnterButton("Okay")
    offlineTrainingModal:setDefaultEscapeButton("Cancel")

    local choiceSkillById = {}
    for _, entry in ipairs(skillChoices) do
        local choice = offlineTrainingModal:addChoice(entry.text)
        choiceSkillById[choice.id] = entry.skill
    end

    offlineTrainingModal:addButton("Okay", function(player, button, choice)
        local selectedSkill = choiceSkillById[choice.id]
        if not selectedSkill then
            abortOfflineTraining(player)
            return true
        end

        player:setOfflineTrainingSkill(selectedSkill)
        return sleep(player, headboardPos)
    end)

    offlineTrainingModal:addButton("Cancel", function(player, button, choice)
        abortOfflineTraining(player)
        return true
    end)

    offlineTrainingModal:sendToPlayer(player)
end

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    local bed = item:getBed()
    if not bed then
        return true
    end

    local headboard, footboard = bed:getBedParts()
    if not headboard or not footboard then
        -- invalid bed parts placement
        return true
    end

    if not canUse(player, headboard) then
        item:getPosition():sendMagicEffect(CONST_ME_POFF)
        return true
    end

    if Beds.OfflineTrainingEnabled then
        sendOfflineTrainingModal(player, headboard:getPosition())
        return true
    end

    return sleep(player, headboard:getPosition())
end

local function getFreeHeadboardIds()
    local headboards = {}
    for id, bed in pairs(Game.getBeds()) do
        if not bed.occupied and (bed.partnerDirection == DIRECTION_SOUTH or bed.partnerDirection == DIRECTION_EAST) then
            table.insert(headboards, id)
        end
    end
    return headboards
end

action:id(table.unpack(getFreeHeadboardIds()))
action:register()
