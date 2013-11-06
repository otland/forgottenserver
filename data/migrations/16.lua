function onUpdateDatabase()
	print("> Updating database to version 17 (fixing primary key in account ban history)")
	db.query("ALTER TABLE `account_ban_history` DROP FOREIGN KEY `account_ban_history_ibfk_1`")
	db.query("ALTER TABLE `account_ban_history` DROP PRIMARY KEY")
	db.query("ALTER TABLE `account_ban_history` ADD `id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY FIRST")
	db.query("ALTER TABLE `account_ban_history` ADD INDEX (`account_id`)")
	db.query("ALTER TABLE `account_ban_history` ADD FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE")
	return true
end
