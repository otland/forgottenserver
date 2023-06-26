function onUpdateDatabase()
	print("> Updating database to version 36 (blessings update)")

	-- skip BLESSING_ADVENTURERS_BLESSING & BLESSING_TWIST_OF_FATE
	-- start at 10003001 + BLESSING_WISDOM_OF_SOLITUDE (2)
	local storageRange = 10003003

	local resultId = db.storeQuery("SELECT `id`, `blessings` FROM `players`")
	if resultId then
		local rows = {}
		repeat
			local blessings = result.getNumber(resultId, "blessings")
			local playerId = result.getNumber(resultId, "id")
			for id = 0, 4 do
				if bit.band(blessings, 2 ^ id) ~= 0 then
					rows[#rows + 1] = "(" .. playerId .. ", " .. (storageRange + id) .. ", 1)"
				end
			end
		until not result.next(resultId)
		result.free(resultId)
		
		local stmt = "INSERT INTO `player_storage` (`player_id`, `key`, `value`) VALUES "
		if #rows > 0 then
			db.query(stmt .. table.concat(rows, ","))
		end
	end

	db.query([[
		CREATE TABLE IF NOT EXISTS `blessings_history` (
			`id` int unsigned NOT NULL AUTO_INCREMENT,
			`player_id` int NOT NULL,
			`type` tinyint NOT NULL DEFAULT '0',
			`event` varchar(255) NOT NULL,
			`created_at` bigint unsigned NOT NULL,
			PRIMARY KEY (`id`),
			FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
		) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;
	]])
	db.query("ALTER TABLE `players` DROP COLUMN `blessings`")
	return true
end
