function onUpdateDatabase()
	print("> Updating database to version 21 (store towns in database)")
	db.query("CREATE TABLE IF NOT EXISTS `towns` (`id` int NOT NULL AUTO_INCREMENT, `name` varchar(255) NOT NULL, `posx` int NOT NULL DEFAULT '0', `posy` int NOT NULL DEFAULT '0', `posz` int NOT NULL DEFAULT '0', PRIMARY KEY (`id`), UNIQUE KEY `name` (`name`)) ENGINE=InnoDB")

	return true
end
