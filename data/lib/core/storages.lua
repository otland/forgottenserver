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
	goldenOutfit = 30016,
	-- empty: 30017
	promotion = 30018,
	delayLargeSeaShell = 30019,
	firstRod = 30020,
	delayWallMirror = 30021,
	-- empty: 30022
	madSheepSummon = 30023,
	crateUsable = 30024,
	-- empty: 30025
	afflictedOutfit = 30026,
	afflictedPlagueMask = 30027,
	afflictedPlagueBell = 30028,
	-- empty: 30029
	-- empty: 30030
	nailCaseUseCount = 30031,
	swampDigging = 30032,
	insectoidCell = 30033,
	vortexTamer = 30034,
	mutatedPumpkin = 30035,

	achievementsBase = 300000,
	achievementsCounter = 20000,
}

-- Checking for duplicate storages:
local keys = {AccountStorageKeys, GlobalStorageKeys, PlayerStorageKeys}
local storage_map = {}
for _, key in pairs(keys) do
	if storage_map[key] == nil then
		storage_map[key] = 1
	else
		print(key .. " is duplicated")
	end
end

