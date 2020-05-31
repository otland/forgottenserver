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

-- Values extraction function
local function extractValues(tab, ret)
	if type(tab) == "number" then
		table.insert(ret, tab)
	else
		for _, v in pairs(tab) do
			extractValues(v, ret)
		end
	end
end

local benchmark = os.clock()
local extraction = {}
extractValues(PlayerStorageKeys, extraction)  -- Call function
table.sort(extraction) -- Sort the table
-- The choice of sorting is due to the fact that sorting is very cheap O (n log2 (n)) and then we can simply compare one by one the elements finding duplicates in O(n)

-- Scroll through the extracted table for duplicates
if #extraction > 1 then
	for i = 1, #extraction - 1 do
		if extraction[i] == extraction[i+1] then
			print("Duplicate storage value found: ".. extraction[i])
		end
	end

	print(string.format("Processed in %.4f(s)", os.clock() - benchmark))
end
