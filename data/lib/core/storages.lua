--[[
Reserved storage ranges:
- 300000 to 301000+ reserved for achievements
- 20000 to 21000+ reserved for achievement progress
- 10000000 to 20000000 reserved for outfits and mounts on source
]]--

AccountStorageKeys = {
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

GlobalStorageKeys = {
	cobraBastionFlask = 30000,
}

-- Function to check for duplicate keys in a table
-- Receives the name of the table to be checked as argument
function checkDuplicatesStorages(varName)
    -- Retrieve the table to be checked
    local keys = _G[varName]
    -- Create a table to keep track of the keys already seen
    local seen = {}
    -- Iterate over the keys in the table
    for k, v in pairs(keys) do
        -- Check if a key has already been seen
        if seen[v] then
            -- If it has, return true and the duplicate key
            return true, "Duplicate key found: " .. v
        end
        -- If not, add the key to the seen table
        seen[v] = true
    end
    -- If no duplicates were found, return false and a message indicating that
    return false, "No duplicate keys found."
end

-- List of table names to be checked for duplicates
local variableNames = {"AccountStorageKeys", "PlayerStorageKeys", "GlobalStorageKeys"}
-- Loop through the list of table names
for _, variableName in ipairs(variableNames) do
    -- Call the checkDuplicatesStorages function for each table
    local hasDuplicates, message = checkDuplicatesStorages(variableName)
    -- Print the result of the check for each table
    print(">> Checking storages " .. variableName .. ": " .. message)
end
