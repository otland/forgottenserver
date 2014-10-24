local deathListEnabled = true
local maxDeathRecords = 5

function onDeath(cid, corpse, killer, mostDamage, unjustified, mostDamage_unjustified)
	local player = Player(cid)

	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You are dead.")
	if not deathListEnabled then
		return
	end

	local byPlayer = 0
	local killerCreature = Creature(killer)
	if killerCreature == nil then
		killerName = "field item"
	else
		if killerCreature:isPlayer() then
			byPlayer = 1
		else
			local master = killerCreature:getMaster()
			if master and master ~= killerCreature and master:isPlayer() then
				killerCreature = master
				byPlayer = 1
			end
		end
		killerName = killerCreature:getName()
	end

	local byPlayerMostDamage = 0
	if mostDamage == 0 then
		mostDamageName = "field item"
	else
		local mostDamageKiller = Creature(mostDamage)
		if mostDamageKiller:isPlayer() then
			byPlayerMostDamage = 1
		else
			local master = mostDamageKiller:getMaster()
			if master and master ~= mostDamageKiller and master:isPlayer() then
				mostDamageKiller = master
				byPlayerMostDamage = 1
			end
		end
		mostDamageName = mostDamageKiller:getName()
	end

	local playerGuid = player:getGuid()
	db.query("INSERT INTO `player_deaths` (`player_id`, `time`, `level`, `killed_by`, `is_player`, `mostdamage_by`, `mostdamage_is_player`, `unjustified`, `mostdamage_unjustified`) VALUES (" .. playerGuid .. ", " .. os.time() .. ", " .. player:getLevel() .. ", " .. db.escapeString(killerName) .. ", " .. byPlayer .. ", " .. db.escapeString(mostDamageName) .. ", " .. byPlayerMostDamage .. ", " .. unjustified .. ", " .. mostDamage_unjustified .. ")")
	local resultId = db.storeQuery("SELECT `player_id` FROM `player_deaths` WHERE `player_id` = " .. playerGuid)

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
		db.query("DELETE FROM `player_deaths` WHERE `player_id` = " .. playerGuid .. " ORDER BY `time` LIMIT 1")
		deathRecords = deathRecords - 1
	end

	if byPlayer == 1 then
		local guild = player:getGuild()
		local targetGuild = guild and guild:getId()
		if targetGuild ~= 0 then
			local killerGuild = killerCreature:getGuild()
			killerGuild = killerGuild and killerGuild:getId() or 0
			if killerGuild ~= 0 and targetGuild ~= killerGuild and isInWar(cid, killerCreature) then
				local warId = false
				resultId = db.storeQuery("SELECT `id` FROM `guild_wars` WHERE `status` = 1 AND ((`guild1` = " .. killerGuild .. " AND `guild2` = " .. targetGuild .. ") OR (`guild1` = " .. targetGuild .. " AND `guild2` = " .. killerGuild .. "))")
				if resultId ~= false then
					warId = result.getDataInt(resultId, "id")
					result.free(resultId)
				end

				if warId ~= false then
					db.query("INSERT INTO `guildwar_kills` (`killer`, `target`, `killerguild`, `targetguild`, `time`, `warid`) VALUES (" .. db.escapeString(killerName) .. ", " .. db.escapeString(player:getName()) .. ", " .. killerGuild .. ", " .. targetGuild .. ", " .. os.time() .. ", " .. warId .. ")")
				end
			end
		end
	end
end
