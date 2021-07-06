function onUpdateDatabase()
	print("> Updating database to version 29 (account storages)")
	db.query([[
		CREATE TABLE IF NOT EXISTS `account_storage` (
		  `account_id` int NOT NULL,
		  `key` int unsigned NOT NULL,
		  `value` int NOT NULL,
		  PRIMARY KEY (`account_id`, `key`),
		  FOREIGN KEY (`account_id`) REFERENCES `accounts`(`id`) ON DELETE CASCADE
		) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;
	]])
	return true
end
