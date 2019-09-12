function onUpdateDatabase()
	print("> Updating database to version 26 (Binary player items)")
	db.query("CREATE TABLE IF NOT EXISTS `player_binary_items` (`player_id` int(11) NOT NULL, `type` int(11) NOT NULL, `items` longblob NOT NULL) ENGINE=InnoDB DEFAULT CHARSET=utf8")
	db.query("ALTER TABLE `player_binary_items` ADD UNIQUE KEY `player_id_2` (`player_id`,`type`)")
	db.query("ALTER TABLE `player_binary_items` ADD CONSTRAINT `player_binary_items_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE")
	return true
end
