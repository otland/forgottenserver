--[[
Reserved storage ranges:
- 300000 to 301000+ reserved for achievements
- 20000 to 21000+ reserved for achievement progress
- 10000000 to 20000000 reserved for outfits and mounts on source
]]--

AccountStorageKeys = {
}

GlobalStorageKeys = {
}

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
	mutatedPumpkin = 30035,
	achievementsBase = 300000,
	achievementsCounter = 20000,
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

local keys = (AccountStorageKeys, GlobalStorageKeys, PlayerStorageKeys)
local extraction = {}
extractValues(keys, extraction)
table.sort(extraction)

if #extraction > 1 then
	for i = 1, #extraction - 1 do
		if extraction[i] == extraction[i+1] then
			print("Duplicate storage value found: ".. extraction[i])
		end
	end
end
