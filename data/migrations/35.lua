function onUpdateDatabase()
	print("> Updating database to version 36 (client debug asserts)")
	db.query([[
		CREATE TABLE IF NOT EXISTS `player_debugasserts` (
			`id` int unsigned NOT NULL AUTO_INCREMENT,
			`player_id` int NOT NULL,
			`assert_line` varchar(5000),
			`date` varchar(5000),
			`description` varchar(5000),
			`comment` varchar(5000),
			PRIMARY KEY (`id`),
			FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
		) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8;
	]])
	return false
end
