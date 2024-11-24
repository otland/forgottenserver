local deathListEnabled = true
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

function onDeath(player, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	local playerId = player:getId()
	if nextUseStaminaTime[playerId] then
		nextUseStaminaTime[playerId] = nil
	end

	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You are dead.")

	local byPlayer, killerName = getKiller(killer)
	local byPlayerMostDamage, killerNameMostDamage = getKiller(mostDamageKiller)

	player:takeScreenshot(byPlayer and SCREENSHOT_TYPE_DEATHPVP or SCREENSHOT_TYPE_DEATHPVE)

	if byPlayer then
		killer:takeScreenshot(SCREENSHOT_TYPE_PLAYERKILL)
	end

	if mostDamageKiller and mostDamageKiller:isPlayer() then
		mostDamageKiller:takeScreenshot(SCREENSHOT_TYPE_PLAYERKILL)
	end

	if not deathListEnabled then
		return
	end

	local playerGuid = player:getGuid()
	db.query("INSERT INTO `player_deaths` (`player_id`, `time`, `level`, `killed_by`, `is_player`, `mostdamage_by`, `mostdamage_is_player`, `unjustified`, `mostdamage_unjustified`) VALUES (" .. playerGuid .. ", " .. os.time() .. ", " .. player:getLevel() .. ", " .. db.escapeString(killerName) .. ", " .. (byPlayer and 1 or 0) .. ", " .. db.escapeString(killerNameMostDamage) .. ", " .. (byPlayerMostDamage and 1 or 0) .. ", " .. (lastHitUnjustified and 1 or 0) .. ", " .. (mostDamageUnjustified and 1 or 0) .. ")")
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
	if limit > 0 then
		db.asyncQuery("DELETE FROM `player_deaths` WHERE `player_id` = " .. playerGuid .. " ORDER BY `time` LIMIT " .. limit)
	end

	if byPlayer then
		local targetGuild = player:getGuild()
		targetGuild = targetGuild and targetGuild:getId() or 0
		if targetGuild ~= 0 then
			local killerGuild = killer:getGuild()
			killerGuild = killerGuild and killerGuild:getId() or 0
			if killerGuild ~= 0 and targetGuild ~= killerGuild and isInWar(playerId, killer:getId()) then
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
		end
	end
end
