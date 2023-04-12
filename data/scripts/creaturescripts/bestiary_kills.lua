local PARTY_MEMBERS_ENABLED = false

local function kill(player, monsterName, raceId, unlockLimits)
    local oldKills = player:getBestiaryKills(raceId)
    player:addBestiaryKills(raceId)
    local newKills = player:getBestiaryKills(raceId)
    for _, limit in pairs(unlockLimits) do
        if oldKills < limit and newKills >= limit then
            player:sendTextMessage(MESSAGE_EVENT_DEFAULT, string.format("You unlocked details for the creature %s.", monsterName))
            player:sendBestiaryMilestoneReached(raceId)
            break
        end
    end
end

local creatureEvent = CreatureEvent("BestiaryKills")

function creatureEvent.onKill(player, target)
    local monster = target:getMonster()
    if not monster then
        return true
    end

    local info = monster:getType():getBestiaryInfo()
    if info.raceId == 0 then
        return true
    end

    local monsterName = monster:getName()
    local unlockLimits = {info.prowess, info.expertise, info.mastery}
    if PARTY_MEMBERS_ENABLED then
        local party = player:getParty()
        if party then
            for _, member in pairs(party:getMembers()) do
                kill(member, monsterName, info.raceId, unlockLimits)
            end

            kill(party:getLeader(), monsterName, info.raceId, unlockLimits)
            return true
        end
    end

    kill(player, monsterName, info.raceId, unlockLimits)
    return true
end

creatureEvent:register()
