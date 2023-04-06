local REQUEST_TYPE = 
{
    BASE_INFORMATION = 0,
	GENERAL_STATS = 1,
	COMBAT_STATS = 2,
	RECENT_DEATHS = 3,
	RECENT_PVP_KILLS = 4,
	ACHIEVEMENTS = 5,
	ITEMS = 6,
	OUTFITS_AND_MOUNTS = 7,
	STORE = 8,
	INSPECTION = 9,
	BADGES = 10,
	TITLES = 11
}

local function sendBaseStats(player)
    local msg = NetworkMessage()
	msg:addByte(0xDA)
	msg:addByte(REQUEST_TYPE.BASE_INFORMATION)
	msg:addByte(0) -- ??
	msg:addString(player:getName())
	msg:addString(player:getVocation():getName())
	msg:addU16(player:getLevel())

    local playerOutfit = player:getOutfit()
    if playerOutfit.lookType ~= 0 then
        msg:addU16(playerOutfit.lookType)
        msg:addByte(playerOutfit.lookHead)
        msg:addByte(playerOutfit.lookBody)
        msg:addByte(playerOutfit.lookLegs)
        msg:addByte(playerOutfit.lookFeet)
        msg:addByte(playerOutfit.lookAddons)
    else 
        msg:addU16(playerOutfit.lookTypeEx)
    end

    msg:addByte(0) -- hide stamina
    msg:addByte(0) -- enable store summary & character titles
    msg:addString("") -- character title

    msg:sendToPlayer(player)
    msg:delete()
end

-- hardcoded in client
local generalStatsSkillsId = {
    [0] = 11,
    [1] = 9,
    [2] = 8,
    [3] = 10,
    [4] = 7,
    [5] = 6,
    [6] = 13
}
local function getLevelPercent(player)
    local currentLevelExp = Game.getExperienceForLevel(player:getLevel())
    local nextLvlExp = Game.getExperienceForLevel(player:getLevel() +1)
    local playerExp = player:getExperience()
    return (playerExp - currentLevelExp) * 100 / (nextLvlExp - currentLevelExp)
end

local function sendGeneralStats(player)
    local msg = NetworkMessage()
	msg:addByte(0xDA)
	msg:addByte(REQUEST_TYPE.GENERAL_STATS)
	msg:addByte(0x00)

	msg:addU64(player:getExperience())
	msg:addU16(player:getLevel())
	msg:addByte(getLevelPercent(player)) -- level percent

	msg:addU16(100 * Game.getExperienceStage(player:getLevel())) -- base exp rate, should exp rate 
	msg:addU32(0); -- turnament exp (deprecated) 
	msg:addU16(0); -- bonus for lov level
	msg:addU16(0) -- XPBoost
	msg:addU16(100) -- 100% bonus of base exp bonus 
	msg:addU16(100) -- stamina boost(100=x1.0)
	msg:addByte(0x00) -- can buy exp boost


	msg:addU16(player:getHealth())
	msg:addU16(player:getMaxHealth())
	msg:addU16(player:getMana())
	msg:addU16(player:getMaxMana())
	msg:addByte(player:getSoul())
	msg:addU16(player:getStamina())

    local foodTime = 0
	local condition = player:getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT) 
    if condition then
        foodTime = condition:getTicks() /  1000
        print(foodTime)
    end
	msg:addU16(foodTime) --health tick??
	msg:addU16(player:getOfflineTrainingTime() / 60 / 1000)
	msg:addU16(player:getSpeed() / 2)
	msg:addU16(player:getBaseSpeed() / 2)
	msg:addU32(player:getCapacity()) -- cap + boost
	msg:addU32(player:getCapacity())
	msg:addU32(player:getFreeCapacity())

	msg:addByte(8) -- ??
	msg:addByte(1) -- ??
	msg:addU16(player:getMagicLevel())
	msg:addU16(player:getBaseMagicLevel())
	-- loyalty bonus
	msg:addU16(player:getBaseMagicLevel())
	msg:addU16(player:getSkillPercent(SKILL_MAGLEVEL))

    for i = SKILL_FIST, SKILL_FISHING, 1 do
        msg:addByte(generalStatsSkillsId[i])
		msg:addU16(player:getEffectiveSkillLevel(i))
		msg:addU16(player:getSkillLevel(i))
		-- loyalty bonus
		msg:addU16(player:getSkillLevel(i))
		msg:addU16(player:getSkillPercent(i) * 100)
    end

	msg:addByte(0x00);
	msg:sendToPlayer(player)
    msg:delete()
end

local function sendCombatStats(player)

end

local function sendRecentDeaths(player)

end

local function sendPvPKills(player)

end

local function sendAchievements(player)

end

local function sendItems(player) -- or better name

end

local function sendOutfitsAndMounts(player)

end

local function sendStore(player) -- or better name

end

local function sendInspection(player)

end

local function sendBadgets(player)

end

local function sendTitles(player)

end

local handler = PacketHandler(0xE5)

function handler.onReceive(player, msg)

    local playerId = msg:getU32() -- player id, not used ??
    local cyclopediaInfoType = msg:getByte()

    if cyclopediaInfoType == REQUEST_TYPE.BASE_INFORMATION then
        sendBaseStats(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.GENERAL_STATS then
        sendGeneralStats(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.COMBAT_STATS then
        sendCombatStats(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.RECENT_DEATHS then
        sendRecentDeaths(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.RECENT_PVP_KILLS then
        sendPvPKills(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.ACHIEVEMENTS then
        sendAchievements(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.ITEMS then
        sendItems(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.OUTFITS_AND_MOUNTS then
        sendOutfitsAndMounts(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.STORE then
        sendStore(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.INSPECTION then
        sendInspection(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.BADGES then
        sendBadgets(player)
    elseif cyclopediaInfoType == REQUEST_TYPE.TITLES then
        sendTitles(player)
    end
end

handler:register()
