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

local function sendCombatStats(self, msg)
	for skillId = SPECIALSKILL_CRITICALHITCHANCE, SPECIALSKILL_MANALEECHAMOUNT do
		msg:addU16(self:getSpecialSkill(skillId))
		msg:addU16(0) -- base special skill
	end

	-- fatal, dodge, momentum
	for i = 1, 3 do
		msg:addU16(0)
		msg:addU16(0)
	end

	msg:addU16(0) -- cleave (bonus percent damage to nearby enemies)
	msg:addU16(0) -- bonus magic shield capacity (flat)
	msg:addU16(0) -- bonus magic shield capacity (percent)

	-- perfect shot flat damage bonus at 1-5 range
	for i = 1, 5 do
		msg:addU16(0)
	end

	msg:addU16(0) -- damage reflection (flat, one value for all combat types)
	msg:addByte(self:getBlessings())
	msg:addByte(8) -- blessings count

	-- weapon
	msg:addU16(0) -- base max damage
	msg:addByte(0) -- base element type
	msg:addByte(0) -- percent damage conversion
	msg:addByte(0) -- conversion element type

	msg:addU16(self:getTotalArmor())
	msg:addU16(self:getTotalDefense())

	msg:addDouble(0) -- damage mitigation (in %)

	-- element resistances count
	msg:addByte(0)
	-- structure:
	-- u8 clientcombat
	-- u8 value

	-- active potions count
	msg:addByte(0)
	-- structure:
	-- item clientId
	-- u16 duration

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

local clientSkillsId = {
	[0] = CYCLOPEDIA_SKILL_FIST,
	[1] = CYCLOPEDIA_SKILL_CLUB,
	[2] = CYCLOPEDIA_SKILL_SWORD,
	[3] = CYCLOPEDIA_SKILL_AXE,
	[4] = CYCLOPEDIA_SKILL_DISTANCE,
	[5] = CYCLOPEDIA_SKILL_SHIELDING,
	[6] = CYCLOPEDIA_SKILL_FISHING
}

local function sendGeneralStats(self, msg)
	msg:addU64(self:getExperience())
	msg:addU16(self:getLevel())
	msg:addByte(self:getLevelPercent())

	msg:addU16(self:getClientExpDisplay())
	msg:addU32(0) -- tournament exp (deprecated)
	msg:addU16(self:getClientLowLevelBonusDisplay())
	msg:addU16(0) -- store exp bonus
	msg:addU16(self:getClientStaminaBonusDisplay())
	msg:addU16(0) -- exp boost remaining time
	msg:addByte(0) -- enable exp boost store button

	msg:addU32(self:getHealth())
	msg:addU32(self:getMaxHealth())
	msg:addU32(self:getMana())
	msg:addU32(self:getMaxMana())
	msg:addByte(self:getSoul())
	msg:addU16(self:getStamina())

	local foodTime = 0
	local condition = self:getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT)
	if condition then
		foodTime = condition:getTicks() / 1000
	end

	msg:addU16(foodTime)
	msg:addU16(self:getOfflineTrainingTime() / 60 / 1000)
	msg:addU16(self:getSpeed() / 2)
	msg:addU16(self:getBaseSpeed() / 2)

	local infiniteCapacity = self:hasFlag(PlayerFlag_HasInfiniteCapacity) and 1000000
	msg:addU32(infiniteCapacity or self:getCapacity()) -- base + bonus capacity
	msg:addU32(infiniteCapacity or self:getCapacity())
	msg:addU32(infiniteCapacity or self:getFreeCapacity())

	msg:addByte(CYCLOPEDIA_SKILL_AMOUNT)

	msg:addByte(CYCLOPEDIA_SKILL_MAGIC)
	msg:addU16(self:getMagicLevel())
	msg:addU16(self:getBaseMagicLevel())
	msg:addU16(self:getBaseMagicLevel()) -- base + loyalty bonus
	msg:addU16(self:getMagicLevelPercent() * 100)

	for i = SKILL_FIST, SKILL_FISHING, 1 do
		msg:addByte(clientSkillsId[i])
		msg:addU16(self:getEffectiveSkillLevel(i))
		msg:addU16(self:getSkillLevel(i))

		-- base + loyalty bonus
		msg:addU16(self:getSkillLevel(i))
		msg:addU16(self:getSkillPercent(i) * 100)
	end

	msg:addByte(0) -- magic boost (element and value)

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

local function sendBadges(self, msg)
	msg:addByte(0x01) -- show account info
	msg:addByte(0x01) -- account online
	msg:addByte(self:isPremium() and 1 or 0)
	msg:addString("") -- loyalty title

	msg:addByte(0) -- badges count
	-- structure:
	-- u32 badge id
	-- string badge name

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

local BASIC_INFO = 0x00
local GENERAL_STATS = 0x01
local COMBAT_STATS = 0x02
local ACHIEVEMENTS = 0x05
local BADGES = 0x0A

--[[
	local RECENT_DEATHS = 0x03
	local RECENT_PVP_KILLS = 0x04
	local ITEM_SUMMARY = 0x06
	local APPEARANCES = 0x07
	local STORE = 0x08
	local INSPECTION = 0x09
	local TITLES = 0x0B
]]--

local handlers = {
	[BASIC_INFO] = sendBasicInfo,
	[GENERAL_STATS] = sendGeneralStats,
	[COMBAT_STATS] = sendCombatStats,
	[ACHIEVEMENTS] = sendAchievements,
	[BADGES] = sendBadges,

	--[[
		[RECENT_DEATHS] = sendRecentDeaths,
		[RECENT_PVP_KILLS] = sendRecentPvpKills,
		[ITEM_SUMMARY] = sendItemSummary,
		[APPEARANCES] = sendAppearances,
		[STORE] = sendStore,
		[INSPECTION] = sendInspection,
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
