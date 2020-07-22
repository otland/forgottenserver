local decayItems = {
	--[itemid] = {decayto, client_version}
	[1479] = {1480, 0}, -- street lamp
	[1480] = {1479, 0}, -- street lamp
	[1634] = {1635, 0}, -- table
	[1635] = {1634, 0}, -- table
	[1636] = {1637, 0}, -- table
	[1637] = {1636, 0}, -- table
	[1638] = {1639, 0}, -- table
	[1639] = {1638, 0}, -- table
	[1640] = {1641, 0}, -- table
	[1641] = {1640, 0}, -- table
	[1786] = {1787, 0}, -- oven
	[1787] = {1786, 0}, -- oven
	[1788] = {1789, 0}, -- oven
	[1789] = {1788, 0}, -- oven
	[1790] = {1791, 0}, -- oven
	[1791] = {1790, 0}, -- oven
	[1792] = {1793, 0}, -- oven
	[1793] = {1792, 0}, -- oven
	[1873] = {1874, 0}, -- cuckoo clock
	[1874] = {1873, 0}, -- cuckoo clock
	[1875] = {1876, 0}, -- cuckoo clock
	[1876] = {1875, 0}, -- cuckoo clock
	[1945] = {1946, 0}, -- lever
	[1946] = {1945, 0}, -- lever
	[2037] = {2038, 0}, -- wall lamp
	[2038] = {2037, 0}, -- lit wall lamp
	[2039] = {2040, 0}, -- wall lamp
	[2040] = {2039, 0}, -- lit wall lamp
	[2041] = {2042, 0}, -- candelabrum
	[2042] = {2041, 0}, -- lit candelabrum
	[2044] = {2045, 0}, -- lamp
	[2045] = {2044, 0}, -- lit lamp
	[2047] = {2048, 0}, -- candlestick
	[2048] = {2047, 0}, -- lit candlestick
	[2050] = {2051, 0}, -- torch
	[2051] = {2050, 0}, -- lit torch
	[2052] = {2053, 0}, -- torch
	[2053] = {2052, 0}, -- lit torch
	[2054] = {2055, 0}, -- torch
	[2055] = {2054, 0}, -- lit torch
	[2058] = {2059, 0}, -- torch bearer
	[2059] = {2058, 0}, -- lit torch bearer
	[2060] = {2061, 0}, -- torch bearer
	[2061] = {2060, 0}, -- lit torch bearer
	[2064] = {2065, 0}, -- table lamp
	[2065] = {2064, 0}, -- lit table lamp
	[2066] = {2067, 0}, -- wall lamp
	[2067] = {2066, 0}, -- lit wall lamp
	[2068] = {2069, 0}, -- wall lamp
	[2069] = {2068, 0}, -- lit wall lamp
	[2096] = {2097, 0}, -- pumpkinhead
	[2097] = {2096, 0}, -- pumpkinhead
	[2162] = {2163, 0}, -- magic light wand
	[2163] = {2162, 0}, -- magic light wand
	[2578] = {2579, 0}, -- closed trap
	[3697] = {3698, 740}, -- sacred statue
	[3698] = {3697, 740}, -- sacred statue
	[3743] = {4404, 750}, -- bamboo lamp
	[3947] = {3948, 750}, -- wall lamp
	[3948] = {3947, 750}, -- lit wall lamp
	[4404] = {3743, 750}, -- bamboo lamp
	[5812] = {5813, 780}, -- skull candle
	[5813] = {5812, 780}, -- skull candle
	[6489] = {6490, 790}, -- christmas branch
	[6490] = {6489, 790}, -- christmas branch
	[7058] = {7059, 800}, -- skull pillar
	[7059] = {7058, 800}, -- glowing skull pillar
	[7183] = {7184, 800}, -- baby seal doll
	[8684] = {8685, 820}, -- chimney
	[8685] = {8684, 820}, -- chimney
	[8686] = {8687, 820}, -- chimney
	[8687] = {8686, 820}, -- chimney
	[8688] = {8689, 820}, -- chimney
	[8689] = {8688, 820}, -- chimney
	[8690] = {8691, 820}, -- chimney
	[8691] = {8690, 820}, -- chimney
	[9575] = {9576, 840}, -- street lamp
	[9576] = {9575, 840}, -- street lamp
	[9577] = {9578, 840}, -- street lamp
	[9578] = {9577, 840}, -- street lamp
	[9579] = {9580, 840}, -- street lamp
	[9580] = {9579, 840}, -- street lamp
	[9581] = {9582, 840}, -- street lamp
	[9582] = {9581, 840}, -- street lamp
	[9747] = {9748, 840}, -- lit wall lamp
	[9748] = {9747, 840}, -- lit wall lamp
	[9749] = {9750, 840}, -- lit wall lamp
	[9750] = {9749, 840}, -- lit wall lamp
	[19691] = {19692, 980}, -- street lamp
	[19692] = {19691, 980}, -- street lamp
	[22868] = {22869, 1050}, -- street lamp
	[22869] = {22868, 1050}, -- street lamp
	[22870] = {22871, 1050}, -- street lamp
	[22871] = {22870, 1050}, -- street lamp
	[22872] = {22873, 1050}, -- candle
	[22873] = {22872, 1050}, -- candle
	[22874] = {22875, 1050}, -- candle
	[22875] = {22874, 1050}, -- candle
	[25545] = {25546, 1090}, -- shield of destiny
	[26098] = {26099, 1092} -- baby dragon
}
local decayto = Action()

function decayto.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	item:transform(decayItems[item.itemid][1])
	item:decay()
	return true
end

for i, d in pairs(decayItems) do
	if Game.getClientVersion().min >= d[2] then
		decayto:id(i)
	end
end
decayto:register()
