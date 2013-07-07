dofile("./config.lua")

function onDeath(cid, corpse, killer)
	doPlayerSendTextMessage(cid, MESSAGE_EVENT_ADVANCE, "You are dead.")
	if deathListEnabled == "yes" then
		local byPlayer = 0
		if killer == 0 then
			killerName = "field item"
		else
			if isPlayer(killer) == TRUE then
				byPlayer = 1
			end
			killerName = getCreatureName(killer)
		end

		db.query("INSERT INTO `player_deaths` (`player_id`, `time`, `level`, `killed_by`, `is_player`) VALUES (" .. getPlayerGUID(cid) .. ", " .. os.time() .. ", " .. getPlayerLevel(cid) .. ", '" .. escapeString(killerName) .. "', " .. byPlayer .. ");")
		local resultId = db.storeQuery("SELECT `player_id` FROM `player_deaths` WHERE `player_id` = " .. getPlayerGUID(cid) .. ";")

		local deathRecords = 0
		while tmpResultId ~= false do
			tmpResultId = result.next(resultId)
			deathRecords = deathRecords + 1
		end

		if resultId ~= false then
			if sqlType == "mysql" then
				while deathRecords > maxDeathRecords do
					db.query("DELETE FROM `player_deaths` WHERE `player_id` = " .. getPlayerGUID(cid) .. " ORDER BY `time` LIMIT 1;")
					deathRecords = deathRecords - 1
				end
			else
				while deathRecords > maxDeathRecords do
					db.query("DELETE FROM `player_deaths` WHERE `rowid` = (SELECT `rowid` FROM `player_deaths` WHERE `player_id` = " .. getPlayerGUID(cid) .. " ORDER BY `time` LIMIT 1);")
					deathRecords = deathRecords - 1
				end
			end
		end

		if byPlayer == 1 then
			local targetGuild = getPlayerGuildId(cid)
			if targetGuild ~= 0 then
				local killerGuild = getPlayerGuildId(killer)
				if killerGuild ~= 0 and targetGuild ~= killerGuild and isInWar(cid, killer) == TRUE then
					local warId = false
					resultId = db.storeQuery("SELECT `id` FROM `guild_wars` WHERE `status` = 1 AND ((`guild1` = " .. killerGuild .. " AND `guild2` = " .. targetGuild .. ") OR (`guild1` = " .. targetGuild .. " AND `guild2` = " .. killerGuild .. "));")
					if resultId ~= false then
						warId = result.getDataInt(resultId, "id")
						result.free(resultId)
					end

					if warId ~= false then
						db.query("INSERT INTO `guildwar_kills` (`killer`, `target`, `killerguild`, `targetguild`, `time`, `warid`) VALUES (" .. db.escapeString(getCreatureName(killer)) .. ", " .. db.escapeString(getCreatureName(cid)) .. ", " .. killerGuild .. ", " .. targetGuild .. ", " .. os.time() .. ", " .. warId .. ");")
					end
				end
			end
		end
	end
end
