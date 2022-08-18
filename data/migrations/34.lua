function onUpdateDatabase()
	print("> Updating database to version 34 (re work on outfits(addons) & mount saving/loading)")
	db.query(
	--[[
	CREATE TABLE IF NOT EXISTS `player_outfits` (
	`player_id` int NOT NULL DEFAULT '0',
	`outfit_id` smallint unsigned NOT NULL DEFAULT '0',
	`addons` tinyint unsigned NOT NULL DEFAULT '0',
	PRIMARY KEY (`player_id`,`key`),
	FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
	) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;

	CREATE TABLE IF NOT EXISTS `player_mounts` (
	`player_id` int NOT NULL DEFAULT '0',
	`mount_id` smallint unsigned NOT NULL DEFAULT '0',
	PRIMARY KEY (`player_id`,`key`),
	FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
	) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;

	ALTER TABLE players
	ADD currentmount smallint unsigned NOT NULL DEFAULT '0';
	]])

	local outfitRange = 10001000
	local mountRange = 10002001

	local resultId = db.storeQuery(string.format("SELECT `player_id`, `key`, `value` FROM `player_storage` WHERE `key` >= %d AND `key` <= %d", outfitRange, outfitRange + 500))
	if resultId then
		repeat
			local playerId = result.getNumber(resultId, "player_id"))
			local outfitId = bit.rshift(result.getNumber(resultId, "value"), 16)
			local addons = bit.band(result.getNumber(resultId, "value"), 0xFF)
			local key = result.getNumber(resultId, "key")
			
			db.query(string.format("INSERT INTO `player_outfits` (`player_id`, `outfit_id`, `addons`) VALUES (%d, %d, %d)", playerId, outfitId, addons))
			db.asyncQuery(string.format("DELETE FROM `player_storage` WHERE `player_id` = %d AND `key` = %d", playerId, key))			
		until not result.next(resultId)
		result.free(resultId)
	end

	local resultId = db.storeQuery(string.format("SELECT `player_id`, `key`, `value` FROM `player_storage` WHERE `key` >= %d AND `key` <= %d", mountRange, mountRange + 10))
	if resultId then
		repeat
			local playerId = result.getNumber(resultId, "player_id"))
			local key = result.getNumber(resultId, "key")
			local mounts = {}

			for i = 1, 300 do
				if key == mountRange + (i % 31) then
					table.insert(mounts, i-1)
				end
			end

			for k, v in pairs(mounts) do
				local lshift = bit.lshift(1, (v % 31))
				local mount = bit.band(lshift, result.getNumber(resultId, "value"))
				if mount then
					db.query(string.format("INSERT INTO `player_mounts` (`player_id`, `mount_id`) VALUES (%d, %d)", playerId, v+1))
				end
			end

			db.asyncQuery(string.format("DELETE FROM `player_storage` WHERE `player_id` = %d AND `key` = %d", playerId, key))			
		until not result.next(resultId)
		result.free(resultId)
	end
	return true
end
