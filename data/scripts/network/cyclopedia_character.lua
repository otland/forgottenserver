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

local methods = {
	[BASIC_INFO] = "sendBasicInfo",
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
	local method = Player[methods[type]]
	if method then
		local response = NetworkMessage()
		response:addByte(0xDA)
		response:addByte(type)
		response:addByte(0x00)
		method(player, response)
	end
end

handler:register()
