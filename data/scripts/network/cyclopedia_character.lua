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
local COMBAT_STATS = 0x02
local ACHIEVEMENTS = 0x05

--[[
	local GENERAL_STATS = 0x01
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
	[COMBAT_STATS] = sendCombatStats,
	[ACHIEVEMENTS] = sendAchievements,

	--[[
		[GENERAL_STATS] = sendGeneralStats,
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
