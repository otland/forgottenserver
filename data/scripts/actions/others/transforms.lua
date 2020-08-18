local transformItems = {
	[1479] = 1480, [1480] = 1479, -- street lamp
	[1634] = 1635, [1635] = 1634, -- table
	[1636] = 1637, [1637] = 1636, -- table
	[1638] = 1639, [1639] = 1638, -- table
	[1640] = 1641, [1641] = 1640, -- table
	[1786] = 1787, [1787] = 1786, -- oven
	[1788] = 1789, [1789] = 1788, -- oven
	[1790] = 1791, [1791] = 1790, -- oven
	[1792] = 1793, [1793] = 1792, -- oven
	[1945] = 1946, [1946] = 1945, -- lever
	[2037] = 2038, [2038] = 2037, -- wall lamp
	[2039] = 2040, [2040] = 2039, -- wall lamp
	[2058] = 2059, [2059] = 2058, -- torch bearer
	[2060] = 2061, [2061] = 2060, -- torch bearer
	[2064] = 2065, [2065] = 2064, -- table lamp
	[2066] = 2067, [2067] = 2066, -- wall lamp
	[2068] = 2069, [2069] = 2068, -- wall lamp
	[2096] = 2097, [2097] = 2096, -- pumpkinhead
	[2578] = 2579, -- trap
	[3697] = 3698, [3698] = 3697, -- sacred statue
	[3699] = 3700, [3700] = 3699, -- sacred statue
	[3743] = 4404, [4404] = 3743, -- bamboo lamp
	[3943] = 3944, [3944] = 3943, -- torch bearer
	[3945] = 3946, [3946] = 3945, -- torch bearer
	[3947] = 3948, [3948] = 3947, -- wall lamp
	[3949] = 3950, [3950] = 3949, -- wall lamp
	[6489] = 6490, [6490] = 6489, -- christmas branch
	[7058] = 7059, [7059] = 7058, -- skull pillar
	[8684] = 8685, [8685] = 8684, -- chimney
	[8686] = 8687, [8687] = 8686, -- chimney
	[8688] = 8689, [8689] = 8688, -- chimney
	[8690] = 8691, [8691] = 8690, -- chimney
	[9575] = 9576, [9576] = 9575, -- street lamp (yalahar)
	[9577] = 9578, [9578] = 9577, -- street lamp (yalahar)
	[9579] = 9580, [9580] = 9579, -- street lamp (yalahar)
	[9581] = 9582, [9582] = 9581, -- street lamp (yalahar)
	[9747] = 9748, [9748] = 9747, -- wall lamp
	[9749] = 9750, [9750] = 9749, -- wall lamp
	[9825] = 9826, [9826] = 9825, -- lever
	[9827] = 9828, [9828] = 9827, -- lever
	[9838] = 9839, [9839] = 9838, -- wall lamp
	[9840] = 9841, [9841] = 9840, -- wall lamp
	[9842] = 9843, [9843] = 9842, -- wall lamp
	[9844] = 9845, [9845] = 9844, -- wall lamp
	[9976] = 9977, -- crystal pedestal
	[9977] = 9978, -- crystal pedestal
	[9978] = 9979, -- crystal pedestal
	[9979] = 9976, -- crystal pedestal
	[10029] = 10030, [10030] = 10029, -- lever
	[10044] = 10045, [10045] = 10044, -- lever
	[10971] = 10972, [10972] = 10971, -- wall lamp
	[10973] = 10974, [10974] = 10973, -- wall lamp
	[10999] = 11000, [11000] = 10999, -- dragon statue (lamp)
	[11001] = 11002, [11002] = 11001, -- dragon statue (lamp)
	[11915] = 11916, [11916] = 11915, -- dragon basin
	[11995] = 11996, [11996] = 11995, -- torch
	[11997] = 11998, [11998] = 11997, -- torch
	[11999] = 12000, [12000] = 11999, -- dragon statue (lamp)
	[12001] = 12002, [12002] = 12001, -- dragon statue (lamp)
	[12006] = 12005, [12005] = 12006, -- jade basin
	[12007] = 12008, [12008] = 12007, -- jade basin
	[12014] = 12016, [12016] = 12014, -- mystic floor lamp
	[12015] = 12017, [12017] = 12015, -- mystic floor lamp
	[12191] = 12192, [12192] = 12191, -- wall torch
	[12200] = 12201, [12201] = 12200, -- wall torch
	[12208] = 12209, [12209] = 12208, -- wall lamp
	[12210] = 12211, [12211] = 12210, -- wall lamp
	[13688] = 13689, [13689] = 13688, -- lamp
	[13690] = 13691, [13691] = 13690, -- lamp
	[19689] = 19690, [19690] = 19689, -- street lamp (venore)
	[19691] = 19692, [19692] = 19691, -- street lamp (venore)
	[22868] = 22869, [22869] = 22868, -- street lamp (oramond)
	[22870] = 22871, [22871] = 22870, -- street lamp (oramond)
	[22872] = 22873, [22873] = 22872, -- candle (oramond)
	[22874] = 22875, [22875] = 22874, -- candle (oramond)
	[23496] = 23500, [23500] = 23496, -- lever (red)
	[23497] = 23501, [23501] = 23497, -- lever (yellow)
	[23498] = 23502, [23502] = 23498, -- lever (green)
	[23499] = 23503, [23503] = 23499, -- lever (violet)
	[26090] = 26092, [26092] = 26090, -- predator lamp
	[26091] = 26093, [26093] = 26091, -- predator lamp
	[26096] = 26094, [26094] = 26096, -- protectress lamp
	[26097] = 26095, [26095] = 26097, -- protectress lamp
}

local transformTo = Action()

function transformTo.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local transformIds = transformItems[item:getId()]
	if not transformIds then
		return false
	end

	item:transform(transformIds)
	return true
end

for i, v in pairs(transformItems) do
	transformTo:id(i)
end

transformTo:register()
