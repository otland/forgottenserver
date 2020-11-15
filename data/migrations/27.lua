function onUpdateDatabase()
	print("> Updating database to version 27 (guildhalls, guild banks #2213)")
	db.query("ALTER TABLE `houses` ADD `type` ENUM('HOUSE', 'GUILDHALL') NOT NULL DEFAULT 'HOUSE' AFTER `id`")
	db.query("ALTER TABLE `guilds` ADD `balance` bigint(20) UNSIGNED NOT NULL DEFAULT '0'")
	db.query([[
		CREATE TABLE IF NOT EXISTS `guild_transactions` (
			`id` int(11) NOT NULL AUTO_INCREMENT,
			`guild_id` int(11) NOT NULL,
			`guild_associated` int(11) DEFAULT NULL,
			`player_associated` int(11) DEFAULT NULL,
			`type` ENUM('DEPOSIT', 'WITHDRAW') NOT NULL,
			`category` ENUM ('OTHER', 'RENT', 'MATERIAL', 'SERVICES', 'REVENUE', 'CONTRIBUTION') NOT NULL DEFAULT 'OTHER',
			`balance` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
			`time` bigint(20) NOT NULL,
			PRIMARY KEY (`id`),
			FOREIGN KEY (`guild_id`) REFERENCES `guilds`(`id`) ON DELETE CASCADE,
			FOREIGN KEY (`guild_associated`) REFERENCES `guilds`(`id`) ON DELETE SET NULL,
			FOREIGN KEY (`player_associated`) REFERENCES `players`(`id`) ON DELETE SET NULL
		) ENGINE=InnoDB;
	]])
	return true
end
