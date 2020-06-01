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
	achievementsBase = 300000, -- range 300000 to 301000+ reserved for achievements
	achievementsCounter = 20000, -- range 20000 to 21000+ reserved for achievement progress
}

GlobalStorageKeys = {
}

-- Checking for duplicate storages:
local function extractValues(tab, ret)
	if type(tab) == "number" then
		table.insert(ret, tab)
	else
		for _, v in pairs(tab) do
			extractValues(v, ret)
		end
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
