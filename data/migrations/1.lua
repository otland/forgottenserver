function onUpdateDatabase()
	print("> Updating database to version 2 (market offers)")
	db.query("CREATE TABLE `market_offers` (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT, `player_id` INT NOT NULL, `sale` TINYINT NOT NULL DEFAULT 0, `itemtype` INT UNSIGNED NOT NULL, `amount` SMALLINT UNSIGNED NOT NULL, `created` BIGINT UNSIGNED NOT NULL, `anonymous` TINYINT(1) NOT NULL DEFAULT 0, `price` INT UNSIGNED NOT NULL DEFAULT 0, PRIMARY KEY (`id`), KEY(`sale`, `itemtype`), KEY(`created`), FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE) ENGINE = InnoDB")
	return true
end
