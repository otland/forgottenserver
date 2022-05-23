-- update cache only if last request was x seconds ago
local updateInterval = 10 * 60 -- 10 minutes

-- load/update cached info
-- NOT CONFIGURABLE
local goldenOutfitCache
local lastUpdated = 0
local function updateGoldenOutfitCache()
	if os.time() < lastUpdated + updateInterval then
		return
	end

	goldenOutfitCache = {[1] = {}, [2] = {}, [3] = {}}

	local resultId = db.storeQuery("SELECT `name`, `value` FROM `player_storage` INNER JOIN `players` as `p` ON `p`.`id` = `player_id` WHERE `key` = " .. PlayerStorageKeys.goldenOutfit .. " AND `value` >= 1;")
	if not resultId then
		result.free(resultId)
		lastUpdated = os.time()
		return
	end

	repeat
		local addons = result.getNumber(resultId, "value")
		local name = result.getString(resultId, "name")
		table.insert(goldenOutfitCache[addons], name)
	until not result.next(resultId)
	result.free(resultId)

	lastUpdated = os.time()
end

local function displayMemorialUI(player)
	-- response header
	local response = NetworkMessage()
	response:addByte(0xB0)

	-- golden outfit
	local price = 500000000
	response:addU32(price)

	price = price + 250000000
	response:addU32(price)

	price = price + 250000000
	response:addU32(price)

	for i = 1, 3 do
		response:addU16(#goldenOutfitCache[i])
		for j = 1, #goldenOutfitCache[i] do
			response:addString(goldenOutfitCache[i][j])
		end
	end

	-- royal costume
	for i = 1, 3 do
		response:addU16(0) -- price in silver tokens
		response:addU16(0) -- price in golden tokens
	end

	for i = 1, 3 do
		response:addU16(0) -- list of spenders
	end

	response:sendToPlayer(player)
end

local goldenOutfitMemorial = Action()
function goldenOutfitMemorial.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	updateGoldenOutfitCache()
	displayMemorialUI(player)
	return true
end

goldenOutfitMemorial:id(34174, 34175, 34176, 34177, 34178, 34179)
goldenOutfitMemorial:register()
