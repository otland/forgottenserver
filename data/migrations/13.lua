function onUpdateDatabase()
	print("> Updating database to version 14 (account_bans, ip_bans and player_bans)")

	db.query("CREATE TABLE IF NOT EXISTS `account_bans` (`account_id` int NOT NULL, `reason` varchar(255) NOT NULL, `banned_at` bigint NOT NULL, `expires_at` bigint NOT NULL, `banned_by` int NOT NULL, PRIMARY KEY (`account_id`), KEY `banned_by` (`banned_by`), FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (`banned_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE) ENGINE=InnoDB")
	db.query("CREATE TABLE IF NOT EXISTS `account_ban_history` (`account_id` int NOT NULL, `reason` varchar(255) NOT NULL, `banned_at` bigint NOT NULL, `expired_at` bigint NOT NULL, `banned_by` int NOT NULL, PRIMARY KEY (`account_id`), KEY `banned_by` (`banned_by`), FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (`banned_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE) ENGINE=InnoDB")
	db.query("CREATE TABLE IF NOT EXISTS `ip_bans` (`ip` int unsigned NOT NULL, `reason` varchar(255) NOT NULL, `banned_at` bigint NOT NULL, `expires_at` bigint NOT NULL, `banned_by` int NOT NULL, PRIMARY KEY (`ip`), KEY `banned_by` (`banned_by`), FOREIGN KEY (`banned_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE) ENGINE=InnoDB")
	db.query("CREATE TABLE IF NOT EXISTS `player_namelocks` (`player_id` int NOT NULL, `reason` varchar(255) NOT NULL, `namelocked_at` bigint NOT NULL, `namelocked_by` int NOT NULL, PRIMARY KEY (`player_id`), KEY `namelocked_by` (`namelocked_by`), FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (`namelocked_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE) ENGINE=InnoDB")

	local resultId = db.storeQuery("SELECT `player`, `time` FROM `bans` WHERE `type` = 2")
	if resultId ~= false then
		local stmt = "INSERT INTO `player_namelocks` (`player_id`, `namelocked_at`, `namelocked_by`) VALUES "
		repeat
			stmt = stmt .. "(" .. result.getNumber(resultId, "player") .. "," .. result.getNumber(resultId, "time") .. "," .. result.getNumber(resultId, "player") .. "),"
		until not result.next(resultId)
		result.free(resultId)

		local stmtLen = string.len(stmt)
		if stmtLen > 86 then
			stmt = string.sub(stmt, 1, stmtLen - 1)
			db.query(stmt)
		end
	end

	db.query("DROP TRIGGER `ondelete_accounts`")
	db.query("DROP TRIGGER `ondelete_players`")
	db.query("ALTER TABLE `accounts` DROP `warnings`")

	db.query("DROP TABLE `bans`")

	print("Run this query in your database to create the ondelete_players trigger:")
	print("DELIMITER //")
	print("CREATE TRIGGER `ondelete_players` BEFORE DELETE ON `players`")
	print(" FOR EACH ROW BEGIN")
	print("  UPDATE `houses` SET `owner` = 0 WHERE `owner` = OLD.`id`;")
	print("END //")
	return true
end
