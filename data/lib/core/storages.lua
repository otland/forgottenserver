--[[
Reserved storage ranges:
- 300000 to 301000+ reserved for achievements
- 20000 to 21000+ reserved for achievement progress
- 10000000 to 20000000 reserved for outfits and mounts on source
]]--
PlayerStorageKeys = {
	annihilatorReward = 30015,
	promotion = 30018,
	delayLargeSeaShell = 30019,
	firstRod = 30020,
	delayWallMirror = 30021,
	madSheepSummon = 30023,
	crateUsable = 30024,
	afflictedOutfit = 30026,
	afflictedPlagueMask = 30027,
	afflictedPlagueBell = 30028,
	nailCaseUseCount = 30031,
	swampDigging = 30032,
	insectoidCell = 30033,
	vortexTamer = 30034,
	achievementsBase = 300000,
	achievementsCounter = 20000,
}

GlobalStorageKeys = {
}

-- Checking for duplicate storages:
local function extractValues(tab, ret)
	for _, v in pairs(tab) do
 		table.insert(ret, tab)
	end
end

local extraction = {}
extractValues(PlayerStorageKeys, extraction)
table.sort(extraction)

if #extraction > 1 then
	for i = 1, #extraction - 1 do
		if extraction[i] == extraction[i+1] then
			print("Duplicate storage value found: ".. extraction[i])
		end
	end
end
