function onUpdateDatabase()
	print("> Updating database to version 38 (vipgroups)")

	db.query([[
		CREATE TABLE IF NOT EXISTS `account_vipgroups` (
			`id` int NOT NULL AUTO_INCREMENT,
			`account_id` int NOT NULL,
			`name` varchar(128) NOT NULL DEFAULT '',
			`editable` tinyint NOT NULL DEFAULT '1',
			PRIMARY KEY (`id`),
			FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
		) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;
	]])

	db.query("ALTER TABLE `account_viplist` ADD `id` int NOT NULL AUTO_INCREMENT PRIMARY KEY FIRST")

	db.query([[
		CREATE TABLE IF NOT EXISTS `account_vipgroup_entry` (
			`group_id` int NOT NULL,
			`entry_id` int NOT NULL,
			UNIQUE KEY `group_entry_index` (`group_id`, `entry_id`),
			FOREIGN KEY (`group_id`) REFERENCES `account_vipgroups` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
			FOREIGN KEY (`entry_id`) REFERENCES `account_viplist` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
		) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;
	]])

	local resultId = db.storeQuery("SELECT `id` FROM `accounts`")
	if resultId ~= false then
		local values = {}
		repeat
			local accountId = result.getNumber(resultId, "id")
			values[#values + 1] = string.format("(%d, 'Enemies', 0), (%d, 'Friends', 0), (%d, 'Trading Partners', 0)", accountId, accountId, accountId)
		until not result.next(resultId)
		result.free(resultId)

		if #values > 0 then
			db.query("INSERT INTO `account_vipgroups` (`account_id`, `name`, `editable`) VALUES " .. table.concat(values, ","))
		end
	end

	db.query([[
		CREATE TRIGGER `oncreate_accounts` AFTER INSERT ON `accounts`
		FOR EACH ROW BEGIN
			INSERT INTO `account_vipgroups` (`account_id`, `name`, `editable`) VALUES (NEW.`id`, 'Enemies', 0);
			INSERT INTO `account_vipgroups` (`account_id`, `name`, `editable`) VALUES (NEW.`id`, 'Friends', 0);
			INSERT INTO `account_vipgroups` (`account_id`, `name`, `editable`) VALUES (NEW.`id`, 'Trading Partners', 0);
		END
	]])
	return true
end
