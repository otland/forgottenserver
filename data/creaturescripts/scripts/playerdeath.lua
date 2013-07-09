dofile("./config.lua")

function onDeath(cid, corpse, killer)
	doPlayerSendTextMessage(cid, MESSAGE_EVENT_ADVANCE, "You are dead.")
	if deathListEnabled == "yes" then
		local byPlayer = 0
		if killer == 0 then
			killerName = "field item"
		else
			if isPlayer(killer) then
				byPlayer = 1
			else
				local master = getCreatureMaster(killer)
				if master ~= killer and isPlayer(master) then
					killer = master
					byPlayer = 1
				end
			end
			killerName = getCreatureName(killer)
		end

		local byPlayerMostDamage = 0
		if mostDamage == 0 then
			mostDamageName = "field item"
		else
			if isPlayer(mostDamage) == TRUE then
				byPlayerMostDamage = 1
			else
				local master = getCreatureMaster(mostDamage)
				if master ~= mostDamage and isPlayer(master) then
					mostDamage = master
					byPlayerMostDamage = 1
				end
			end
			mostDamageName = getCreatureName(mostDamage)
		end

		db.query("INSERT INTO `player_deaths` (`player_id`, `time`, `level`, `killed_by`, `is_player`, `mostdamage_by`, `mostdamage_is_player`, `unjustified`, `mostdamage_unjustified`) VALUES (" .. getPlayerGUID(cid) .. ", " .. os.time() .. ", " .. getPlayerLevel(cid) .. ", " .. db.escapeString(killerName) .. ", " .. byPlayer .. ", " .. db.escapeString(mostDamageName) .. ", " .. byPlayerMostDamage .. ", " .. unjustified .. ", " .. mostDamage_unjustified .. ")")
		local resultId = db.storeQuery("SELECT `player_id` FROM `player_deaths` WHERE `player_id` = " .. getPlayerGUID(cid))

		local deathRecords = 0
		local tmpResultId = resultId
		while tmpResultId ~= false do
			tmpResultId = result.next(resultId)
			deathRecords = deathRecords + 1
		end

		if resultId ~= false then
			result.free(resultId)
		end

		while deathRecords > maxDeathRecords do
			db.query("DELETE FROM `player_deaths` WHERE `player_id` = " .. getPlayerGUID(cid) .. " ORDER BY `time` LIMIT 1")
			deathRecords = deathRecords - 1
		end

		if byPlayer == 1 then
			local targetGuild = getPlayerGuildId(cid)
			if targetGuild ~= 0 then
				local killerGuild = getPlayerGuildId(killer)
				if killerGuild ~= 0 and targetGuild ~= killerGuild and isInWar(cid, killer) == TRUE then
					local warId = false
					resultId = db.storeQuery("SELECT `id` FROM `guild_wars` WHERE `status` = 1 AND ((`guild1` = " .. killerGuild .. " AND `guild2` = " .. targetGuild .. ") OR (`guild1` = " .. targetGuild .. " AND `guild2` = " .. killerGuild .. "))")
					if resultId ~= false then
						warId = result.getDataInt(resultId, "id")
						result.free(resultId)
					end

					if warId ~= false then
						db.query("INSERT INTO `guildwar_kills` (`killer`, `target`, `killerguild`, `targetguild`, `time`, `warid`) VALUES (" .. db.escapeString(getCreatureName(killer)) .. ", " .. db.escapeString(getCreatureName(cid)) .. ", " .. killerGuild .. ", " .. targetGuild .. ", " .. os.time() .. ", " .. warId .. ")")
					end
				end
			end
		end
	end
end
