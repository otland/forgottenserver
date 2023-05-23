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

local BASIC_INFO = 0x00

--[[
	local GENERAL_STATS = 0x01
	local COMBAT_STATS = 0x02
	local RECENT_DEATHS = 0x03
	local RECENT_PVP_KILLS = 0x04
	local ACHIEVEMENTS = 0x05
	local ITEM_SUMMARY = 0x06
	local APPEARANCES = 0x07
	local STORE = 0x08
	local INSPECTION = 0x09
	local BADGES = 0x0A
	local TITLES = 0x0B
]]--

local handlers = {
	[BASIC_INFO] = sendBasicInfo,
	--[[
		[GENERAL_STATS] = "sendGeneralStats",
		[COMBAT_STATS] = "sendCombatStats",
		[RECENT_DEATHS] = "sendRecentDeaths",
		[RECENT_PVP_KILLS] = "sendRecentPvpKills",
		[ACHIEVEMENTS] = "sendAchievements",
		[ITEM_SUMMARY] = "sendItemSummary",
		[APPEARANCES] = "sendAppearances",
		[STORE] = "sendStore",
		[INSPECTION] = "sendInspection",
		[BADGES] = "sendBadges",
		[TITLES] = "sendTitles"
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
