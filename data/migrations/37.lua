function onUpdateDatabase()
	print("> Updating database to version 38 (player stash)")

	db.query([[ 
		CREATE TABLE IF NOT EXISTS `player_stash` (
			`player_id` INT NOT NULL,
			`item_id` SMALLINT UNSIGNED NOT NULL,
			`item_count` INT UNSIGNED NOT NULL,
			PRIMARY KEY (`player_id`, `item_id`),
			FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
		) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Stores the items stashed by players';
	]])

	return true
end