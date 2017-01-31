function onUpdateDatabase()
	print("> Updating database to version 19 (store system)")

	db.query("ALTER TABLE `accounts` ADD COLUMN `coins` int(11) NOT NULL DEFAULT '0'")

	db.query("CREATE TABLE IF NOT EXISTS `store_history` ( `id` int(10) unsigned NOT NULL AUTO_INCREMENT, `account_id` int(11) NOT NULL, `coins` int(10) NOT NULL DEFAULT '0', `description` varchar(256) NOT NULL DEFAULT '', `timestamp` bigint(20) unsigned NOT NULL, PRIMARY KEY (`id`), FOREIGN KEY (`account_id`) REFERENCES `accounts`(`id`) ON DELETE CASCADE ) ENGINE=InnoDB;")

	return true
end
