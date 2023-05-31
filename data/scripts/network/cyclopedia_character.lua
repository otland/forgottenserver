local function sendBasicInfo(self, msg)
	msg:addString(self:getName())
	msg:addString(self:getVocation():getName())
	msg:addU16(self:getLevel())

	local outfit = self:getOutfit()
	if outfit.lookType ~= 0 then
		msg:addU16(outfit.lookType)
		msg:addByte(outfit.lookHead)
		msg:addByte(outfit.lookBody)
		msg:addByte(outfit.lookLegs)
		msg:addByte(outfit.lookFeet)
		msg:addByte(outfit.lookAddons)
	else
		msg:addU16(outfit.lookTypeEx)
	end

	msg:addByte(0) -- hide stamina
	msg:addByte(1) -- enable store summary & character titles
	msg:addString("") -- character title

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

local function getLevelPercent(player)
	local currentLevelExp = Game.getExperienceForLevel(player:getLevel())
	local nextLvlExp = Game.getExperienceForLevel(player:getLevel() +1)
	local playerExp = player:getExperience()
	return (playerExp - currentLevelExp) * 100 / (nextLvlExp - currentLevelExp)
end

local function sendGeneralStats(self, msg)
	msg:addU64(self:getExperience())
	msg:addU16(self:getLevel())
	msg:addByte(math.floor(getLevelPercent(self)))

	msg:addU16(self:getClientExpDisplay())
	msg:addU32(0) -- turnament exp (deprecated)
	msg:addU16(0) -- low level bonus
	msg:addU16(0) -- store exp bonus
	msg:addU16(self:getClientStaminaBonusDisplay())
	msg:addU16(0) -- exp boost remaining time
	msg:addByte(0) -- enable exp boost store button

	msg:addU16(self:getHealth())
	msg:addU16(self:getMaxHealth())
	msg:addU16(self:getMana())
	msg:addU16(self:getMaxMana())
	msg:addByte(self:getSoul())
	msg:addU16(self:getStamina())

	local foodTime = 0
	local condition = self:getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT)
	if condition then
		foodTime = condition:getTicks() /  1000
	end

	msg:addU16(foodTime)
	msg:addU16(self:getOfflineTrainingTime() / 60 / 1000)
	msg:addU16(self:getSpeed() / 2)
	msg:addU16(self:getBaseSpeed() / 2)
	msg:addU32(self:getCapacity()) -- cap + boost
	msg:addU32(self:getCapacity())
	msg:addU32(self:hasFlag(PlayerFlag_HasInfiniteCapacity) and 1000000 or self:getFreeCapacity())

	msg:addByte(8) -- ??
	msg:addByte(1) -- ??

	msg:addU16(self:getMagicLevel())
	msg:addU16(self:getBaseMagicLevel())

	-- loyalty bonus
	msg:addU16(self:getBaseMagicLevel())
	msg:addU16(self:getSkillPercent(SKILL_MAGLEVEL))

	for i = SKILL_FIST, SKILL_FISHING, 1 do
		msg:addByte(cyclopediaSkillMap[skillId])
		msg:addU16(self:getEffectiveSkillLevel(i))
		msg:addU16(self:getSkillLevel(i))

		-- loyalty bonus
		msg:addU16(self:getSkillLevel(i))
		msg:addU16(self:getSkillPercent(i) * 100)
	end

	msg:addByte(0)

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

local function sendAchievements(self, msg)
	local achievementIds = self:getAchievements()
	msg:addU16(self:getAchievementPoints())
	msg:addU16(#getSecretAchievements())
	msg:addU16(#achievementIds)

	for _, id in pairs(achievementIds) do
		local achievement = achievements[id]
		local secret = achievement.secret
		msg:addU16(achievement.clientId)
		msg:addU32(self:getStorageValue(PlayerStorageKeys.achievementsBase + id))
		msg:addByte(secret and 0x01 or 0x00)

		if secret then
			msg:addString(achievement.name)
			msg:addString(achievement.description)
			msg:addByte(achievement.grade)
		end
	end

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

local BASIC_INFO = 0x00
local GENERAL_STATS = 0x01
local ACHIEVEMENTS = 0x05

--[[
	local COMBAT_STATS = 0x02
	local RECENT_DEATHS = 0x03
	local RECENT_PVP_KILLS = 0x04
	local ITEM_SUMMARY = 0x06
	local APPEARANCES = 0x07
	local STORE = 0x08
	local INSPECTION = 0x09
	local BADGES = 0x0A
	local TITLES = 0x0B
]]--

local handlers = {
	[BASIC_INFO] = sendBasicInfo,
	[GENERAL_STATS] = sendGeneralStats,
	[ACHIEVEMENTS] = sendAchievements,

	--[[
		[COMBAT_STATS] = sendCombatStats,
		[RECENT_DEATHS] = sendRecentDeaths,
		[RECENT_PVP_KILLS] = sendRecentPvpKills,
		[ITEM_SUMMARY] = sendItemSummary,
		[APPEARANCES] = sendAppearances,
		[STORE] = sendStore,
		[INSPECTION] = sendInspection,
		[BADGES] = sendBadges,
		[TITLES] = sendTitles
	]]--
}

local handler = PacketHandler(0xE5)

function handler.onReceive(player, msg)
	msg:skipBytes(4)
	local type = msg:getByte()
	local method = handlers[type]
	if method then
		local msg = NetworkMessage()
		msg:addByte(0xDA)
		msg:addByte(type)
		msg:addByte(0x00)
		method(player, msg)
	end
end

handler:register()
