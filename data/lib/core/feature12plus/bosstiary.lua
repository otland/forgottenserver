local CONST = {
	SLOT = {
		FIRST = 1,
		SECOND = 2,
		THIRD = 3,

		FIRST_STORAGE = PlayerStorageKeys.bosstiarySlot1,
		SECOND_STORAGE = PlayerStorageKeys.bosstiarySlot2
	},
	BOSS_TYPE = {
		BANE = 0,
		ARCHFOE = 1,
		NEMESIS = 2
	},
	KILLS = {
		BANE = {
			PROWESS = 25,
			EXPERTISE = 100,
			MASTERY = 300
		},
		ARCHFOE = {
			PROWESS = 5,
			EXPERTISE = 20,
			MASTERY = 60
		},
		NEMESIS = {
			PROWESS = 1,
			EXPERTISE = 3,
			MASTERY = 5
		}
	},
	POINTS = {
		BANE = {
			PROWESS = 5,
			EXPERTISE = 15,
			MASTERY = 30
		},
		ARCHFOE = {
			PROWESS = 10,
			EXPERTISE = 30,
			MASTERY = 60
		},
		NEMESIS = {
			PROWESS = 10,
			EXPERTISE = 30,
			MASTERY = 60
		}
	},
	SECOND_SLOOT_POINTS = 1500,
	TODAY_BOOSTED_BOSS_KILL_COUNT = 3,
	BOOSTED_BOSS_KILL_BONUS = 25,
	TODAY_BOOSTED_BOSS_KILL_BONUS = 250,
	REMOVE_BOSS_COST_BASE = 100000,
	REMOVE_BOSS_COST_STEP = 300000,
}

-- For optimization to avoid iteration over all boss records
-- Array holds information if players reached first milestone on any boss record
local firstMilestoneUnlocked = {}

local bossCount = 0

local bossesByName = {
	["Abyssador"] = { id = 887, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Alptramun"] = { id = 1698, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Ancient Spawn of Morgathla"] = { id = 1551, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Annihilon"] = { id = 418, bossType = CONST.BOSS_TYPE.BANE },
	["Anomaly"] = { id = 1219, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Arachir The Ancient One"] = { id = 478, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Ashmunrah"] = { id = 91, bossType = CONST.BOSS_TYPE.BANE },
	["Barbaria"] = { id = 440, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Battlemaster Zunzu"] = { id = 635, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Bibby Bloodbath"] = { id = 900, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Big Boss Trolliver"] = { id = 432, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Black Knight"] = { id = 46, bossType = CONST.BOSS_TYPE.BANE },
	["Black Vixen"] = { id = 1559, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Bloodback"] = { id = 1560, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Boogey"] = { id = 567, bossType = CONST.BOSS_TYPE.BANE },
	["Bragrumol"] = { id = 1828, bossType = CONST.BOSS_TYPE.BANE },
	["Brokul"] = { id = 1645, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Bullwark"] = { id = 1060, bossType = CONST.BOSS_TYPE.BANE },
	["Captain Jones"] = { id = 430, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Chikhaton"] = { id = 647, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Chizzoron The Distorter"] = { id = 629, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Count Vlarkorth"] = { id = 1753, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Countess Sorrow"] = { id = 306, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Custodian"] = { id = 1770, bossType = CONST.BOSS_TYPE.BANE },
	["Darkfang"] = { id = 1558, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Dazed Leaf Golem"] = { id = 1578, bossType = CONST.BOSS_TYPE.BANE },
	["Death Priest Shargon"] = { id = 1047, bossType = CONST.BOSS_TYPE.BANE },
	["Deathstrike"] = { id = 892, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Deep Terror"] = { id = 1087, bossType = CONST.BOSS_TYPE.BANE },
	["Dharalion"] = { id = 203, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Diblis The Fair"] = { id = 477, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Dipthrah"] = { id = 87, bossType = CONST.BOSS_TYPE.BANE },
	["Dirtbeard"] = { id = 565, bossType = CONST.BOSS_TYPE.BANE },
	["Diseased Bill"] = { id = 485, bossType = CONST.BOSS_TYPE.BANE },
	["Diseased Dan"] = { id = 486, bossType = CONST.BOSS_TYPE.BANE },
	["Diseased Fred"] = { id = 484, bossType = CONST.BOSS_TYPE.BANE },
	["Doctor Perhaps"] = { id = 564, bossType = CONST.BOSS_TYPE.BANE },
	["Dracola"] = { id = 302, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Dreadmaw"] = { id = 639, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Drume"] = { id = 1957, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Duke Krule"] = { id = 1758, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Earl Osam"] = { id = 1757, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Ekatrix"] = { id = 1140, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Eradicator"] = { id = 1225, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Essence Of Malice"] = { id = 1487, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Evil Mastermind"] = { id = 569, bossType = CONST.BOSS_TYPE.BANE },
	["Faceless Bane"] = { id = 1727, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Feroxa"] = { id = 1149, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Ferumbras"] = { id = 231, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Ferumbras Mortal Shell"] = { id = 1206, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Fleabringer"] = { id = 640, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Fleshslicer"] = { id = 858, bossType = CONST.BOSS_TYPE.BANE },
	["Foreman Kneebiter"] = { id = 424, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Furyosa"] = { id = 987, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Gaffir"] = { id = 1778, bossType = CONST.BOSS_TYPE.BANE },
	["Gaz'Haragoth"] = { id = 1003, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Gelidrazah the Frozen"] = { id = 1379, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["General Murius"] = { id = 207, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Ghazbaran"] = { id = 312, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Ghulosh"] = { id = 1608, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Glitterscale"] = { id = 670, bossType = CONST.BOSS_TYPE.BANE },
	["Glooth Fairy"] = { id = 1058, bossType = CONST.BOSS_TYPE.BANE },
	["Gnomevil"] = { id = 893, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Golgordan"] = { id = 416, bossType = CONST.BOSS_TYPE.BANE },
	["Gorzindel"] = { id = 1591, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Grand Canon Dominus"] = { id = 1584, bossType = CONST.BOSS_TYPE.BANE },
	["Grand Chaplain Gaunder"] = { id = 1579, bossType = CONST.BOSS_TYPE.BANE },
	["Grand Commander Soeren"] = { id = 1582, bossType = CONST.BOSS_TYPE.BANE },
	["Grand Master Oberon"] = { id = 1576, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Grand Mother Foulscale"] = { id = 642, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Grandfather Tridian"] = { id = 431, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Gravelord Oshuran"] = { id = 426, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Groam"] = { id = 736, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Grorlam"] = { id = 205, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Guard Captain Quaid"] = { id = 1791, bossType = CONST.BOSS_TYPE.BANE },
	["Hairman The Huge"] = { id = 425, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Hellgorak"] = { id = 403, bossType = CONST.BOSS_TYPE.BANE },
	["Heoni"] = { id = 671, bossType = CONST.BOSS_TYPE.BANE },
	["Hirintror"] = { id = 967, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Horestis"] = { id = 713, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Irgix The Flimsy"] = { id = 1890, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Izcandar the Banished"] = { id = 1699, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Jailer"] = { id = 1577, bossType = CONST.BOSS_TYPE.BANE },
	["Jaul"] = { id = 773, bossType = CONST.BOSS_TYPE.BANE },
	["Jesse The Wicked"] = { id = 763, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Kalyassa"] = { id = 1389, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["King Zelos"] = { id = 1784, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Kroazur"] = { id = 1515, bossType = CONST.BOSS_TYPE.BANE },
	["Lady Tenebris"] = { id = 1315, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Latrivan"] = { id = 417, bossType = CONST.BOSS_TYPE.BANE },
	["Lisa"] = { id = 1059, bossType = CONST.BOSS_TYPE.BANE },
	["Lloyd"] = { id = 1329, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Lokathmor"] = { id = 1574, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Lord Azaram"] = { id = 1756, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Lord of the Elements"] = { id = 454, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Mad Mage"] = { id = 703, bossType = CONST.BOSS_TYPE.BANE },
	["Madareth"] = { id = 414, bossType = CONST.BOSS_TYPE.BANE },
	["Mahrdis"] = { id = 86, bossType = CONST.BOSS_TYPE.BANE },
	["Malofur Mangrinder"] = { id = 1696, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Man In The Cave"] = { id = 338, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Massacre"] = { id = 305, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Mawhawk"] = { id = 1028, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Maxxenius"] = { id = 1697, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Mazoran"] = { id = 1186, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Mazzinor"] = { id = 1605, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Melting Frozen Horror"] = { id = 1336, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Mephiles"] = { id = 566, bossType = CONST.BOSS_TYPE.BANE },
	["Monstor"] = { id = 568, bossType = CONST.BOSS_TYPE.BANE },
	["Morgaroth"] = { id = 229, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Morguthis"] = { id = 84, bossType = CONST.BOSS_TYPE.BANE },
	["Mozradek"] = { id = 1829, bossType = CONST.BOSS_TYPE.BANE },
	["Mr. Punish"] = { id = 303, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Obujos"] = { id = 774, bossType = CONST.BOSS_TYPE.BANE },
	["Ocyakao"] = { id = 970, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Omrafir"] = { id = 1011, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Omruc"] = { id = 90, bossType = CONST.BOSS_TYPE.BANE },
	["Orshabaal"] = { id = 201, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Outburst"] = { id = 1227, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Plagirath"] = { id = 1199, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Plagueroot"] = { id = 1695, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Preceptor Lazare"] = { id = 1583, bossType = CONST.BOSS_TYPE.BANE },
	["Professor Maxxen"] = { id = 1093, bossType = CONST.BOSS_TYPE.BANE },
	["Ragiaz"] = { id = 1180, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Raging mage"] = { id = 718, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Rahemos"] = { id = 88, bossType = CONST.BOSS_TYPE.BANE },
	["Ravenous Hunger"] = { id = 1427, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Raxias"] = { id = 1624, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Razzagorn"] = { id = 1177, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Realityquake"] = { id = 1218, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Robby The Reckless"] = { id = 764, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Rotworm Queen"] = { id = 459, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Rukor Zad"] = { id = 435, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Rupture"] = { id = 1225, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Scarlett Etzel"] = { id = 1801, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Shadowpelt"] = { id = 1561, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Sharpclaw"] = { id = 1562, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Shulgrax"] = { id = 1191, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Sir Baeloc"] = { id = 1755, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Sir Nictros"] = { id = 1754, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Sir Valorcrest"] = { id = 476, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Smuggler Baron Silvertoe"] = { id = 436, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Soul of Dragonking Zyrtarch"] = { id = 1289, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Tanjis"] = { id = 775, bossType = CONST.BOSS_TYPE.BANE },
	["Tarbaz"] = { id = 1188, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Tazhadur"] = { id = 1390, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Thalas"] = { id = 89, bossType = CONST.BOSS_TYPE.BANE },
	["Thawing Dragon Lord"] = { id = 1585, bossType = CONST.BOSS_TYPE.BANE },
	["The Abomination"] = { id = 373, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Armored Voidborn"] = { id = 1406, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Baron From Below"] = { id = 1518, bossType = CONST.BOSS_TYPE.BANE },
	["The Blazing Rose"] = { id = 1600, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Blightfather"] = { id = 638, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Count Of The Core"] = { id = 1519, bossType = CONST.BOSS_TYPE.BANE },
	["The Diamond Blossom"] = { id = 1598, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Duke Of The Depths"] = { id = 1520, bossType = CONST.BOSS_TYPE.BANE },
	["The Enraged Thorn Knight"] = { id = 1297, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Evil Eye"] = { id = 210, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The False God"] = { id = 1409, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Fear Feaster"] = { id = 1873, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Frog Prince"] = { id = 420, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Handmaiden"] = { id = 301, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Imperor"] = { id = 304, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Last Lore Keeper"] = { id = 1304, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Lily of Night"] = { id = 1602, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Lord of the Lice"] = { id = 1179, bossType = CONST.BOSS_TYPE.BANE },
	["The Mutated Pumpkin"] = { id = 466, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Nightmare Beast"] = { id = 1718, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Pale Count"] = { id = 972, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Percht Queen"] = { id = 1744, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Plasmother"] = { id = 300, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Ravager"] = { id = 1035, bossType = CONST.BOSS_TYPE.BANE },
	["The Sandking"] = { id = 1444, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Scourge Of Oblivion"] = { id = 1642, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Shatterer"] = { id = 1165, bossType = CONST.BOSS_TYPE.BANE },
	["The Souldespoiler"] = { id = 1422, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Source Of Corruption"] = { id = 1500, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Time Guardian"] = { id = 1290, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Unarmored Voidborn"] = { id = 1406, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["The Voice of Ruin"] = { id = 636, bossType = CONST.BOSS_TYPE.NEMESIS },
	["The Welter"] = { id = 964, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Tyrn"] = { id = 966, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Unaz the Mean"] = { id = 1891, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Urmahlullu the Weakened"] = { id = 1811, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Ushuriel"] = { id = 415, bossType = CONST.BOSS_TYPE.BANE },
	["Vashresamun"] = { id = 85, bossType = CONST.BOSS_TYPE.BANE },
	["Vok The Freakish"] = { id = 1892, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Warlord Ruzad"] = { id = 419, bossType = CONST.BOSS_TYPE.NEMESIS },
	["White Pale"] = { id = 965, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Willi Wasp"] = { id = 955, bossType = CONST.BOSS_TYPE.NEMESIS },
	["World Devourer"] = { id = 1228, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Xenia"] = { id = 428, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Xogixath"] = { id = 1827, bossType = CONST.BOSS_TYPE.BANE },
	["Yaga The Crone"] = { id = 427, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Yakchal"] = { id = 336, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Zamulosh"] = { id = 1181, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Zarabustor"] = { id = 421, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Zevelon Duskbringer"] = { id = 475, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Zomba"] = { id = 956, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Zorvorax"] = { id = 1375, bossType = CONST.BOSS_TYPE.ARCHFOE },
	["Zugurosh"] = { id = 434, bossType = CONST.BOSS_TYPE.BANE },
	["Zulazza The Corruptor"] = { id = 628, bossType = CONST.BOSS_TYPE.NEMESIS },
	["Zushuka"] = { id = 969, bossType = CONST.BOSS_TYPE.NEMESIS },
}

local bossesById = {}
do
	for _, bossData in pairs(bossesByName) do
		bossesById[bossData.id] = {
			name = _,
			bossType = bossData.bossType
		}
		bossCount = bossCount + 1
	end
end

local todayResetCostTime
do
	local now = os.time()
	local date = os.date("*t", now)
	date.hour = 0
	date.min = 0
	date.sec = 0

	local start_of_day = os.time(date)

	-- add 10h according to Shutdown event -> globalevent name="Server Save" time="09:55:00" script="server_save.lua"
	todayResetCostTime = start_of_day + (10 * 3600)
end

-- PRIVATE API --

local function rollBoostedBoss()
	local keys = {}
	for key in pairs(bossesByName) do
		table.insert(keys, key)
	end

	local randomKey
	repeat
		randomKey = keys[math.random(#keys)]
	until randomKey.id ~= 0 -- 0 is for remove slot, is not valid boss
	return bossesByName[randomKey]
end

local function isBossUnlocked(bossId, kills)
	local type = bossesById[bossId].bossType

	if type == CONST.BOSS_TYPE.BANE then
		if kills >= CONST.KILLS.BANE.PROWESS then
			return true
		end
	elseif type == CONST.BOSS_TYPE.ARCHFOE then
		if kills >= CONST.KILLS.ARCHFOE.PROWESS then
			return true
		end
	elseif type == CONST.BOSS_TYPE.NEMESIS then
		if kills >= CONST.KILLS.NEMESIS.PROWESS then
			return true
		end
	end
	return false
end

local todayBoostedBoss = rollBoostedBoss()

local function isValidSlot(slot)
	if slot ~= CONST.SLOT.FIRST and slot ~= CONST.SLOT.SECOND then
		return false
	end
	return true
end

local function isValidBoss(boss)
	if boss == 0 then
		return true
	end

	if bossesByName[boss] ~= nil then
		return true
	end

	if bossesById[boss] ~= nil then
		return true
	end

	return false
end


local function getPointsForNextLevel(level)
	if level <= 25 then
		return 10 * level
	elseif level <= 75 then
		return 250 + (level - 25) * 20
	else
		local total_points = 1250
		local increment = 25
		for lvl = 76, level do
			total_points = total_points + increment
			increment = increment + 5
		end
		return total_points
	end
end

 local function getCurrentLevelFromPoints(points)
	if points < 250 then
		return math.floor(points / 10)
	elseif points < 1250 then
		return 25 + math.floor((points - 250) / 20)
	else
		local level = 75
		local remaining_points = points - 1250
		local increment = 25
		while remaining_points > 0 do
			remaining_points = remaining_points - increment
			if remaining_points >= 0 then
				level = level + 1
				increment = increment + 5
			end
		end
		return level
	end
end

local function isFirstSlotUnlocked(player, bossId, bossType)
	local countToUnlock = 0
	if bossType == CONST.BOSS_TYPE.BANE then
		countToUnlock = CONST.KILLS.BANE.PROWESS
	elseif bossType == CONST.BOSS_TYPE.ARCHFOE then
		countToUnlock = CONST.KILLS.ARCHFOE.PROWESS
	elseif bossType == CONST.BOSS_TYPE.NEMESIS then
		countToUnlock = CONST.KILLS.NEMESIS.PROWESS
	end

	local kills = player:getBosstiaryKills(bossId)
	if kills >= countToUnlock then
		return true
	end

	return false
end

-- PUBLIC API --

Bosstiary = {}

Bosstiary.getConst = function()
	return CONST
end

Bosstiary.getBossesByName = function()
	return bossesByName
end

Bosstiary.getBossesCount = function()
	return bossCount
end

Bosstiary.getBossesById = function()
	return bossesById
end

Bosstiary.getTodayBoostedBoss = function()
	return todayBoostedBoss
end

Bosstiary.getBosstiaryRemoveCostResetTime = function()
	return todayResetCostTime
end

function Bosstiary.getBossById(bossId)
	if bossesById[bossId] ~= nil then
		return bossesById[bossId]
	end

	return 0
end

function Bosstiary.getBossByName(bossName)
	if bossesByName[bossName] ~= nil then
		return bossesByName[bossName]
	end

	return ""
end

-- POINTS
function Player.getBosstiaryPoints(self)
	return math.max(0, self:getStorageValue(PlayerStorageKeys.bosstiaryPoints) or 0)
end

function Player.getBosstiaryNextLevelPoints(self)
	return getPointsForNextLevel(getCurrentLevelFromPoints(math.max(0, self:getStorageValue(PlayerStorageKeys.bosstiaryPoints) or 0)) + 1)
end

function Player.addBosstiaryPoints(self, points)
	return self:setStorageValue(PlayerStorageKeys.bosstiaryPoints,
		math.max(0, (self:getStorageValue(PlayerStorageKeys.bosstiaryPoints) or 0) + points))
end

function Player.setBosstiaryPoints(self, points)
	return self:setStorageValue(PlayerStorageKeys.bosstiaryPoints, math.max(0, points))
end

-- SLOTS
-- "boss" can be name or id
function Player.setBosstiarySlotBoss(self, slot, boss)
	if not isValidSlot(slot) then
		return false
	end

	if not isValidBoss(boss) then
		return false
	end

	if not isNumber(boss) then
		boss = Bosstiary.getBossByName(boss).id
	end

	if slot == CONST.SLOT.FIRST then
		self:setStorageValue(CONST.SLOT.FIRST_STORAGE, boss)
	else
		self:setStorageValue(CONST.SLOT.SECOND_STORAGE, boss)
	end

	return true
end

function Player.getBosstiarySlotBoss(self, slot)
	if not isValidSlot(slot) then
		return 0
	end

	if slot == CONST.SLOT.FIRST then
		return math.max(0, self:getStorageValue(CONST.SLOT.FIRST_STORAGE) or 0)
	else
		return math.max(0, self:getStorageValue(CONST.SLOT.SECOND_STORAGE) or 0)
	end
end

function Player.isValidBoostiaryBoss(self, boss)
	if boss == 0 then
		return true
	end

	if todayBoostedBoss then
		if todayBoostedBoss.id == boss then
			return false
		end
	end

	if self:getBosstiarySlotBoss(CONST.SLOT.FIRST) == boss then
		return false
	end

	if self:getBosstiarySlotBoss(CONST.SLOT.SECOND) == boss then
		return false
	end

	return true
end

-- KILLS
-- "boss" can be name or id
function Player.getBosstiaryKills(self, boss)
	if not isValidBoss(boss) then
		return 0
	end

	if not isNumber(boss) then
		boss = Bosstiary.getBossByName(boss).id
	end

	if self:getGroup():getAccess() then
		return 1000
	end
	return math.max(0, self:getStorageValue(PlayerStorageKeys.bosstiaryKillsBase + boss) or 0)
end

-- "boss" can be name or id
function Player.setBosstiaryKills(self, boss, points)
	if not isValidBoss(boss) then
		return false
	end

	if not isNumber(boss) then
		boss = Bosstiary.getBossByName(boss).id
	end

	return self:setStorageValue(PlayerStorageKeys.bosstiaryKillsBase + boss, math.max(0, points))
end

function Player.addBosstiaryKills(self, boss, points)
	if not isValidBoss(boss) then
		return false
	end

	if not isNumber(boss) then
		boss = Bosstiary.getBossByName(boss).id
	end

	return self:setStorageValue(PlayerStorageKeys.bosstiaryKillsBase + boss,
		self:getBosstiaryKills(boss) + math.max(0, points))
end

-- BONUS
function Player.getBosstiaryBonusValue(self, slot)
	if slot == CONST.SLOT.FIRST or slot == CONST.SLOT.SECOND then
		return CONST.BOOSTED_BOSS_KILL_BONUS + getCurrentLevelFromPoints(self:getBosstiaryPoints())
	elseif slot == CONST.SLOT.THIRD then
		return CONST.TODAY_BOOSTED_BOSS_KILL_BONUS
	end

	return getCurrentLevelFromPoints(self:getBosstiaryPoints())
end

function Player.getBosstiaryNextLevelBonusValue(self)
	return getCurrentLevelFromPoints(self:getBosstiaryPoints()) + 1
end

-- REMOVE COST
function Player.getBosstiaryRemoveCost(self)
	local currentCount = self:getBosstiaryTodayRemoveCount()
	if currentCount <= 1 then
		return 0
	end

	return (math.max(0, self:getBosstiaryTodayRemoveCount() - 2) * CONST.REMOVE_BOSS_COST_STEP) + CONST.REMOVE_BOSS_COST_BASE
end

-- REMOVE COUNT
function Player.getBosstiaryTodayRemoveCount(self)
	return math.max(1, self:getStorageValue(PlayerStorageKeys.bosstiaryTodayRemoveCount) or 1)
end

function Player.setBosstiaryTodayRemoveCount(self, value)
	return self:setStorageValue(PlayerStorageKeys.bosstiaryTodayRemoveCount, value)
end

function Player.incrementBosstiaryRemoveCounter(self)
	return self:setBosstiaryTodayRemoveCount(self:getBosstiaryTodayRemoveCount() + 1)
end

function Player.resetBestiaryRemoveCountIfNeeded(self)
	if self:getBosstiaryLastRemoveDate() ~= Bosstiary.getBosstiaryRemoveCostResetTime() then
		return self:setBosstiaryTodayRemoveCount(0)
	end
end

-- REMOVE TIME
function Player.setBosstiaryLastRemoveDate(self)
	return self:setStorageValue(PlayerStorageKeys.bosstiaryTodayRemoveDate, Bosstiary.getBosstiaryRemoveCostResetTime())
end

function Player.getBosstiaryLastRemoveDate(self)
	return self:getStorageValue(PlayerStorageKeys.bosstiaryTodayRemoveDate)
end

-- SLOT UNLOCKED
function Player.isBosstiarySlotUnlocked(self, slot)
	if not isValidSlot(slot) then
		return false
	end

	if slot == CONST.SLOT.FIRST then
		if firstMilestoneUnlocked[self:getGuid()] == true then
			return true
		else
			for _, bossData in pairs(Bosstiary.getBossesByName()) do
				if isFirstSlotUnlocked(self, bossData.id, bossData.bossType) then
					firstMilestoneUnlocked[self:getGuid()] = true
					return true
				end
			end
		end
	else
		if self:getBosstiaryPoints() >= CONST.SECOND_SLOOT_POINTS then
			return true
		end
	end
	return false
end

-- NETWORK MESSAGE
function Player.sendBosstiaryList(self)
	local msg = NetworkMessage()
	msg:addByte(0x73)
	msg:addU16(Bosstiary.getBossesCount())
	for _, boss in pairs(bossesByName) do
		msg:addU32(boss.id)
		msg:addByte(boss.bossType)
		msg:addU32(self:getBosstiaryKills(boss.id))
		msg:addByte(0) -- unknown
	end

	msg:sendToPlayer(self)
	msg:delete()
end

function Player.sendBoostiaryMilestonesData(self)
	local msg = NetworkMessage()
	msg:addByte(0x61)

	-- BANE kills milestone
	msg:addU16(CONST.KILLS.BANE.PROWESS)
	msg:addU16(CONST.KILLS.BANE.EXPERTISE)
	msg:addU16(CONST.KILLS.BANE.MASTERY)

	-- ARCHFOE kills milestone
	msg:addU16(CONST.KILLS.ARCHFOE.PROWESS)
	msg:addU16(CONST.KILLS.ARCHFOE.EXPERTISE)
	msg:addU16(CONST.KILLS.ARCHFOE.MASTERY)

	-- NEMESIS kills milestone
	msg:addU16(CONST.KILLS.NEMESIS.PROWESS)
	msg:addU16(CONST.KILLS.NEMESIS.EXPERTISE)
	msg:addU16(CONST.KILLS.NEMESIS.MASTERY)

	-- BANE milestone points
	msg:addU16(CONST.POINTS.BANE.PROWESS)
	msg:addU16(CONST.POINTS.BANE.EXPERTISE)
	msg:addU16(CONST.POINTS.BANE.MASTERY)

	-- ARCHFOE milestone points
	msg:addU16(CONST.POINTS.ARCHFOE.PROWESS)
	msg:addU16(CONST.POINTS.ARCHFOE.EXPERTISE)
	msg:addU16(CONST.POINTS.ARCHFOE.MASTERY)

	-- NEMESIS milestone points
	msg:addU16(CONST.POINTS.NEMESIS.PROWESS)
	msg:addU16(CONST.POINTS.NEMESIS.EXPERTISE)
	msg:addU16(CONST.POINTS.NEMESIS.MASTERY)

	msg:sendToPlayer(self)
	msg:delete()
end

-- Private api but require some additional Player methods to be loaded already
local function addDailyBoostedBossToNetworkMessage(player, msg)
	if not todayBoostedBoss then
		msg:addByte(0)
		msg:addU32(0)
		return true
	end

	msg:addByte(1)
	msg:addU32(todayBoostedBoss.id)
	msg:addByte(todayBoostedBoss.bossType)
	msg:addU32(player:getBosstiaryKills(todayBoostedBoss.id))
	msg:addU16(player:getBosstiaryBonusValue(CONST.SLOT.THIRD))
	msg:addByte(CONST.TODAY_BOOSTED_BOSS_KILL_COUNT)
	msg:addByte(todayBoostedBoss.bossType)
	msg:addU32(0) -- we can't remove boss from this slot, remove cost equal 0
	msg:addByte(0)
	return true
end

-- Private api but require some additional Player methods to be loaded already
local function addSlotDataToNetworkMessage(player, msg, slot)
	if not isValidSlot(slot) then
		return false
	end

	if not player:isBosstiarySlotUnlocked(slot) then
		msg:addByte(0)
		msg:addU32(0)
		return true
	end

	local bossId = player:getBosstiarySlotBoss(slot)
	msg:addByte(1)
	msg:addU32(bossId)
	if bossId ~= 0 then
		local bossData = Bosstiary.getBossById(bossId)
		msg:addByte(bossData.bossType)
		msg:addU32(player:getBosstiaryKills(bossId))
		msg:addU16(player:getBosstiaryBonusValue(slot))
		msg:addByte(0) -- kill bonus, not used
		msg:addByte(bossData.bossType)
		msg:addU32(player:getBosstiaryRemoveCost())
		local isDailyBoosted = todayBoostedBoss.id == bossId
		-- to ensure we push 0 or 1 to msg
		msg:addByte(isDailyBoosted and 1 or 0)
		return true
	end

	return true
end

-- Private api but require some additional Player methods to be loaded already
local function addUnlockedBossesToNetworkMessage(player, msg)
	local bufferPos = msg:len()
	msg:skipBytes(2)
	local unlockedCount = 0
	for _, bossData in pairs(bossesByName) do
		local isUnlocked = isBossUnlocked(bossData.id, player:getBosstiaryKills(bossData.id))
		if isUnlocked then
			unlockedCount = unlockedCount + 1
			msg:addU32(bossData.id)
			msg:addByte(bossData.bossType)
		end
	end
	msg:seek(bufferPos)
	msg:addU16(unlockedCount)

	return true
end

function Player.sendBoostiarySlotsData(self)
	local msg = NetworkMessage()
	msg:addByte(0x62)
	msg:addU32(self:getBosstiaryPoints())
	msg:addU32(self:getBosstiaryNextLevelPoints())
	msg:addU16(self:getBosstiaryBonusValue(-1)) -- current bonus excluding boss bonus
	msg:addU16(self:getBosstiaryNextLevelBonusValue())

	-- Left slot
	local result = addSlotDataToNetworkMessage(self, msg, CONST.SLOT.FIRST)
	if result == false then
		msg:delete()
		return
	end

	-- Right slot
	result = addSlotDataToNetworkMessage(self, msg, CONST.SLOT.SECOND)
	if result == false then
		msg:delete()
		return
	end

	-- Mid slot
	result = addDailyBoostedBossToNetworkMessage(self, msg)
	if result == false then
		msg:delete()
		return
	end

	msg:addByte(1)
	addUnlockedBossesToNetworkMessage(self, msg)
	msg:sendToPlayer(self)
	msg:delete()
end
