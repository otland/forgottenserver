function onUpdateDatabase()
	print("> Updating database to version 5 (black skull & guild wars)")
	db.query("ALTER TABLE `players` CHANGE `redskull` `skull` TINYINT(1) NOT NULL DEFAULT '0', CHANGE `redskulltime` `skulltime` INT(11) NOT NULL DEFAULT '0'")
	db.query("CREATE TABLE IF NOT EXISTS `guild_wars` ( `id` int(11) NOT NULL AUTO_INCREMENT, `guild1` int(11) NOT NULL DEFAULT '0', `guild2` int(11) NOT NULL DEFAULT '0', `name1` varchar(255) NOT NULL, `name2` varchar(255) NOT NULL, `status` tinyint(2) NOT NULL DEFAULT '0', `started` bigint(15) NOT NULL DEFAULT '0', `ended` bigint(15) NOT NULL DEFAULT '0', PRIMARY KEY (`id`), KEY `guild1` (`guild1`), KEY `guild2` (`guild2`)) ENGINE=InnoDB")
	db.query("CREATE TABLE IF NOT EXISTS `guildwar_kills` (`id` int(11) NOT NULL AUTO_INCREMENT, `killer` varchar(50) NOT NULL, `target` varchar(50) NOT NULL, `killerguild` int(11) NOT NULL DEFAULT '0', `targetguild` int(11) NOT NULL DEFAULT '0', `warid` int(11) NOT NULL DEFAULT '0', `time` bigint(15) NOT NULL, PRIMARY KEY (`id`), KEY `warid` (`warid`), FOREIGN KEY (`warid`) REFERENCES `guild_wars`(`id`) ON DELETE CASCADE) ENGINE=InnoDB")
	return true
end
