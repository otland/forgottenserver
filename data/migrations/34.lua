function onUpdateDatabase()
	print("> Updating database to version 34 (re work on outfits(addons) & mount saving/loading)")
	db.query("CREATE TABLE IF NOT EXISTS `player_outfits` (`player_id` int NOT NULL DEFAULT '0',`outfit_id` smallint unsigned NOT NULL DEFAULT '0',`addons` tinyint unsigned NOT NULL DEFAULT '0',PRIMARY KEY (`outfit_id`),FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;")
	db.query("CREATE TABLE IF NOT EXISTS `player_mounts` (`player_id` int NOT NULL DEFAULT '0',`mount_id` smallint unsigned NOT NULL DEFAULT '0',PRIMARY KEY (`mount_id`),FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;")
	db.query("ALTER TABLE players ADD currentmount smallint unsigned NOT NULL DEFAULT '0';")

	local outfitRange = 10001000
	local mountRange = 10002001

	local resultId = db.storeQuery(string.format("SELECT `player_id`, `value` FROM `player_storage` WHERE `key` >= %d AND `key` <= %d", outfitRange, outfitRange + 500))
	if resultId then
		repeat
			local playerId = result.getNumber(resultId, "player_id")
			local outfitId = bit.rshift(result.getNumber(resultId, "value"), 16)
			local addons = bit.band(result.getNumber(resultId, "value"), 0xFF)
			
			db.query(string.format("INSERT INTO `player_outfits` (`player_id`, `outfit_id`, `addons`) VALUES (%d, %d, %d)", playerId, outfitId, addons))
		until not result.next(resultId)
		result.free(resultId)
	end

	-- this equals to our current id range from mounts
	for i = 1, 200 do
		local key = mountRange + ((i-1) / 31)
		local resultId = db.storeQuery(string.format("SELECT `player_id`, `key`, `value` FROM `player_storage` WHERE `key` = %d", key))
		if resultId then
			repeat
				local playerId = result.getNumber(resultId, "player_id")
				local lshift = bit.lshift(1, ((i-1) % 31))
				local mount = bit.band(lshift, result.getNumber(resultId, "value"))

				if mount ~= 0 then
					db.query(string.format("INSERT INTO `player_mounts` (`player_id`, `mount_id`) VALUES (%d, %d)", playerId, i))
				end
			until not result.next(resultId)
			result.free(resultId)
		end
	end

	-- deleting all outfit & mount storages at once
	db.asyncQuery(string.format("DELETE FROM `player_storage` WHERE `key` >= %d AND `key` <= %d AND `key` >= %d AND `key` <= %d", outfitRange, outfitRange + 500, mountRange, mountRange + 10))

	return true
end
