local invalidTypes = {
	1, 135, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
	174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
	189, 190, 191, 411, 415, 424, 439, 440, 468, 469, 474, 475, 476, 477, 478,
	479, 480, 481, 482, 483, 484, 485, 501, 518, 519, 520, 524, 525, 536, 543,
	549, 576, 581, 582, 597, 616, 623, 625, 638, 639, 640, 641, 642, 643, 645,
	646, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661, 662, 663, 678, 700,
	701, 702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 713, 715, 718, 719,
	722, 723, 737, 741, 742, 743, 744, 748, 751, 752, 753, 754, 755, 756, 757,
	758, 764, 765, 766, 767, 768, 769, 770, 771, 772, 773, 774, 775, 776, 777,
	778, 779, 780, 781, 782, 783, 784, 785, 786, 787, 788, 789, 790, 791, 792,
	793, 794, 795, 796, 797, 798, 799, 800, 801, 802, 803, 804, 805, 806, 807,
	808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 821, 822,
	823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837,
	838, 839, 840, 841, 847, 861, 863, 864, 865, 866, 867, 871, 872, 880, 891,
	892, 893, 894, 895, 896, 897, 898, 911, 912, 917, 930, 941, 942, 946, 953,
	954, 983, 995, 996, 997, 998, 999, 1000, 1001, 1002, 1003, 1004, 1005, 1006,
	1007, 1008, 1009, 1010, 1012, 1014, 1015, 1022, 1028, 1074, 1075, 1080, 1081,
	1082, 1083, 1084, 1085, 1086, 1087, 1089, 1090, 1096, 1097, 1098, 1099, 1100,
	1141, 1145, 1153, 1154, 1155, 1156, 1160, 1170, 1171, 1172, 1176, 1177, 1178,
	1182, 1192, 1193, 1194, 1198, 1215, 1216, 1225, 1226, 1227, 1228, 1235, 1236,
	1237, 1238, 1239, 1240, 1241, 1242, 1250, 1254, 1263, 1267, 1273, 1274, 1287,
	1302, 1318, 1319, 1320, 1327, 1328, 1329, 1330, 1340, 1343, 1345, 1347, 1348,
	1349, 1350, 1351, 1352, 1353, 1354, 1355, 1356, 1357, 1358, 1359, 1360, 1361,
	1362, 1368, 1369, 1370, 1374, 1375, 1376, 1388, 1392, 1395, 1400, 1402, 1404,
	1409, 1410, 1411, 1420, 1421, 1427, 1428, 1429, 1432, 1433, 1434, 1435, 1438,
	1442, 1443, 1451, 1452, 1458, 1462, 1470, 1471, 1472, 1473, 1474, 1475, 1476,
	1477, 1478, 1479, 1480, 1481, 1482, 1483, 1484, 1485, 1486, 1487, 1488, 1494,
	1495, 1496, 1497, 1498, 1499, 1502, 1503, 1504, 1505, 1506, 1507, 1508, 1509,
	1510, 1511, 1512, 1513, 1514, 1515, 1516, 1517, 1518, 1519, 1520, 1521, 1522,
	1523, 1524, 1525, 1529, 1530, 1531, 1532, 1544, 1545, 1563, 1564, 1571, 1572,
	1573, 1574
}

local function sendCraftedOutfitWindow(player, param)	
	local currentOutfit = player:getOutfit()
	local m = NetworkMessage()
	m:addByte(0xC8) -- outfit window packet header
	
	-- current outfit
	m:addU16(currentOutfit.lookType)
	m:addByte(currentOutfit.lookHead)
	m:addByte(currentOutfit.lookBody)
	m:addByte(currentOutfit.lookLegs)
	m:addByte(currentOutfit.lookFeet)
	m:addByte(currentOutfit.lookAddons)
	
	-- current mount
	m:addU16(0)
	m:addByte(0)
	m:addByte(0)
	m:addByte(0)
	m:addByte(0)
	
	-- current familiar
	m:addU16(0)
	
	local lookType = tonumber(param)
	if lookType then
		if not table.contains(invalidTypes, lookType) then
			m:addU16(1)
			m:addU16(lookType)
			m:addString("Selected outfit")
			m:addByte(3)
			m:addByte(0)
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "This looktype will crash your client.")
			return
		end
	else
		local listSize = 1575
		local realCount = listSize - #invalidTypes
		
		-- outfits block
		m:addU16(realCount)
		for lookType = 1, listSize do
			if not table.contains(invalidTypes, lookType) then
				m:addU16(lookType)
				m:addString(tostring(lookType))
				m:addByte(3) -- unlocked addons
				m:addByte(0) -- unlock mode (0 - available, 1 - store (requires u32 offerId after that), 2 - golden outfit tooltip, 3 - royal costume tooltip
			end
		end
	end
	
	-- mounts block
	m:addU16(0)
	
	-- familiars block
	m:addU16(0)
	
	-- flags
	m:addByte(0)
	m:addByte(0)
	
	-- randomize
	m:addByte(0)
	m:sendToPlayer(player)
	return
end

function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end
	
	sendCraftedOutfitWindow(player, param)
	return false
end
