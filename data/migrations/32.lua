function onUpdateDatabase()
	print("> Updating database to version 32 (vipgroups)")
	db.query("CREATE TABLE IF NOT EXISTS `account_vipgroups` (`id` int NOT NULL AUTO_INCREMENT, `account_id` int NOT NULL, `name` varchar(128) NOT NULL DEFAULT '', `editable` tinyint NOT NULL DEFAULT '1', PRIMARY KEY (`id`), FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE) ENGINE=InnoDB;")
	db.query("ALTER TABLE `account_viplist` ADD `id` int NOT NULL AUTO_INCREMENT PRIMARY KEY FIRST")
	db.query("ALTER TABLE `account_viplist` RENAME `account_vipentries`")
	db.query("CREATE TABLE IF NOT EXISTS `vipgroup_vipentry` (`group_id` int NOT NULL, `entry_id` int NOT NULL, UNIQUE KEY `group_entry_index` (`group_id`, `entry_id`), FOREIGN KEY (`group_id`) REFERENCES `account_vipgroups` (`id`) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (`entry_id`) REFERENCES `account_vipentries` (`id`) ON DELETE CASCADE ON UPDATE CASCADE) ENGINE=InnoDB;")

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

	print("Run this query in your database to create the oncreate_accounts trigger:")
	print("DELIMITER //")
	print("CREATE TRIGGER `oncreate_accounts` AFTER INSERT ON `accounts`")
	print(" FOR EACH ROW BEGIN")
	print("  INSERT INTO `account_vipgroups` (`account_id`, `name`, `editable`) VALUES (NEW.id, 'Enemies', 0), (NEW.id, 'Friends', 0), (NEW.id, 'Trading Partners', 0);")
	print("END //")

	return true
end
