function onUpdateDatabase()
	print("> Updating database to version 37 (vipgroups)")
	db.query("CREATE TABLE IF NOT EXISTS `account_vipgroups` (`id` int NOT NULL AUTO_INCREMENT, `account_id` int NOT NULL, `name` varchar(128) NOT NULL DEFAULT '', `editable` tinyint NOT NULL DEFAULT '1', PRIMARY KEY (`id`), FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE) ENGINE=InnoDB;")
	db.query("ALTER TABLE `account_viplist` ADD `id` int NOT NULL AUTO_INCREMENT PRIMARY KEY FIRST")
	db.query("CREATE TABLE IF NOT EXISTS `account_vipgroup_entry` (`group_id` int NOT NULL, `entry_id` int NOT NULL, UNIQUE KEY `group_entry_index` (`group_id`, `entry_id`), FOREIGN KEY (`group_id`) REFERENCES `account_vipgroups` (`id`) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (`entry_id`) REFERENCES `account_viplist` (`id`) ON DELETE CASCADE ON UPDATE CASCADE) ENGINE=InnoDB;")

	local resultId = db.storeQuery("SELECT `accounts`.`id` AS `account_id` FROM `accounts`")
	if resultId ~= false then
		local stmt = "INSERT INTO `account_vipgroups` (`account_id`, `name`, `editable`) VALUES "
		repeat
			stmt = stmt .. "(" .. result.getNumber(resultId, "account_id") .. ", 'Enemies', 0),"
			stmt = stmt .. "(" .. result.getNumber(resultId, "account_id") .. ", 'Friends', 0),"
			stmt = stmt .. "(" .. result.getNumber(resultId, "account_id") .. ", 'Trading Partners', 0),"
		until not result.next(resultId)
		result.free(resultId)

		local stmtLen = string.len(stmt)
		if stmtLen > 74 then
			stmt = string.sub(stmt, 1, stmtLen - 1)
			db.query(stmt)
		end
	end

	db.query("CREATE TRIGGER `oncreate_accounts` AFTER INSERT ON `accounts` FOR EACH ROW BEGIN INSERT INTO `account_vipgroups` (`account_id`, `name`, `editable`) VALUES (NEW.id, 'Enemies', 0), (NEW.id, 'Friends', 0), (NEW.id, 'Trading Partners', 0); END")
	return true
end
