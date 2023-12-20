local deathListEnabled = true
local deleteRecords = true
local maxDeathRecords = 5

local function getKiller(killer)
	if not killer then
		return false, "field item"
	end

	if killer:isPlayer() then
		return true, killer:getName()
	end

	local master = killer:getMaster()
	if master and master ~= killer and master:isPlayer() then
		return true, master:getName()
	end

	return false, killer:getName()
end

local function takeScreenShot(player, killer, killedByPlayer, mostDamageKiller)
	player:takeScreenshot(killedByPlayer and SCREENSHOT_TYPE_DEATHPVP or SCREENSHOT_TYPE_DEATHPVE)

	if killedByPlayer then
		killer:takeScreenshot(SCREENSHOT_TYPE_PLAYERKILL)
	end

	if mostDamageKiller and mostDamageKiller:isPlayer() then
		mostDamageKiller:takeScreenshot(SCREENSHOT_TYPE_PLAYERKILL)
	end
end

local function updateStamina(player)
	local playerId = player:getId()
	if nextUseStaminaTime[playerId] then
		nextUseStaminaTime[playerId] = nil
	end
end

local function updateDeathList(player, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified, killedByPlayer, killerName, killedByPlayerMostDamage, killerNameMostDamage)
	if not deathListEnabled then
		return
	end

	local playerGuid = player:getGuid()
	db.query("INSERT INTO `player_deaths` (`player_id`, `time`, `level`, `killed_by`, `is_player`, `mostdamage_by`, `mostdamage_is_player`, `unjustified`, `mostdamage_unjustified`) VALUES (" .. playerGuid .. ", " .. os.time() .. ", " .. player:getLevel() .. ", " .. db.escapeString(killerName) .. ", " .. (killedByPlayer and 1 or 0) .. ", " .. db.escapeString(killerNameMostDamage) .. ", " .. (killedByPlayerMostDamage and 1 or 0) .. ", " .. (lastHitUnjustified and 1 or 0) .. ", " .. (mostDamageUnjustified and 1 or 0) .. ")")
	local resultId = db.storeQuery("SELECT `player_id` FROM `player_deaths` WHERE `player_id` = " .. playerGuid)

	local deathRecords = 0
	local tmpResultId = resultId
	while tmpResultId do
		tmpResultId = result.next(resultId)
		deathRecords = deathRecords + 1
	end

	if resultId then
		result.free(resultId)
	end

	local limit = deathRecords - maxDeathRecords
	if deleteRecords and limit > 0 then
		db.asyncQuery("DELETE FROM `player_deaths` WHERE `player_id` = " .. playerGuid .. " ORDER BY `time` LIMIT " .. limit)
	end

	if not killedByPlayer then
		return
	end

	local targetGuild = player:getGuild()
	targetGuild = targetGuild and targetGuild:getId() or 0
	if targetGuild == 0 then
		return
	end

	local killerGuild = killer:getGuild()
	killerGuild = killerGuild and killerGuild:getId() or 0
	if killerGuild ~= 0 then
		return
	end

	if targetGuild == killerGuild then
		return
	end

	if not player:isAtWarAgainst(killer) then
		return
	end

	local warId = false
	resultId = db.storeQuery("SELECT `id` FROM `guild_wars` WHERE `status` = 1 AND ((`guild1` = " .. killerGuild .. " AND `guild2` = " .. targetGuild .. ") OR (`guild1` = " .. targetGuild .. " AND `guild2` = " .. killerGuild .. "))")
	if resultId then
		warId = result.getNumber(resultId, "id")
		result.free(resultId)
	end

	if warId then
		db.asyncQuery("INSERT INTO `guildwar_kills` (`killer`, `target`, `killerguild`, `targetguild`, `time`, `warid`) VALUES (" .. db.escapeString(killerName) .. ", " .. db.escapeString(player:getName()) .. ", " .. killerGuild .. ", " .. targetGuild .. ", " .. os.time() .. ", " .. warId .. ")")
	end
end

function onDeath(player, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You are dead.")

	local killedByPlayer, killerName = getKiller(killer)
	local killedByPlayerMostDamage, killerNameMostDamage = getKiller(mostDamageKiller)

	takeScreenShot(player, killer, killedByPlayer, mostDamageKiller)

	updateStamina(player)
	updateDeathList(player, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified, killedByPlayer, killerName, byPlayerMostDamage, killerNameMostDamage)
end
