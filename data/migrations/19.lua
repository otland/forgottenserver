function onUpdateDatabase()
	print("> Updating database to version 20 (guildhalls, guild banks #2213)")
	db.query("ALTER TABLE `houses` ADD `type` TINYINT(3) NOT NULL DEFAULT '0'")
	db.query("ALTER TABLE `guilds` ADD `balance` bigint(20) unsigned NOT NULL DEFAULT '0'")
	db.query("CREATE TABLE IF NOT EXISTS `guild_transactions` (`id` int(11) NOT NULL AUTO_INCREMENT, `guild_id` int(11) NOT NULL, `to_guild_id` int(11) DEFAULT NULL, `player_id` int(11) DEFAULT NULL, `type` TINYINT(3) NOT NULL, `balance` bigint(20) unsigned NOT NULL DEFAULT '0', `time` bigint(20) NOT NULL, PRIMARY KEY (`id`), FOREIGN KEY (`guild_id`) REFERENCES `guilds`(`id`) ON DELETE CASCADE) ENGINE=InnoDB")
	return true
end
