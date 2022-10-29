function onUpdateDatabase()
	print("> Updating database to version 13 (house bidding system and additional columns to other tables)")

	db.query("ALTER TABLE `player_deaths` ADD `mostdamage_by` varchar(100) NOT NULL, ADD `mostdamage_is_player` tinyint NOT NULL DEFAULT '0', ADD `unjustified` tinyint NOT NULL DEFAULT '0', ADD `mostdamage_unjustified` tinyint NOT NULL DEFAULT '0', ADD KEY `killed_by` (`killed_by`), ADD KEY `mostdamage_by` (`mostdamage_by`)")
	db.query("ALTER TABLE `houses` ADD `name` varchar(255) NOT NULL, ADD `rent` int NOT NULL DEFAULT '0', ADD `town_id` int NOT NULL DEFAULT '0', ADD `bid` int NOT NULL DEFAULT '0', ADD `bid_end` int NOT NULL DEFAULT '0', ADD `last_bid` int NOT NULL DEFAULT '0', ADD `highest_bidder` int NOT NULL DEFAULT '0', ADD `size` int NOT NULL DEFAULT '0', ADD `beds` int NOT NULL DEFAULT '0', ADD KEY `owner` (`owner`), ADD KEY `town_id` (`town_id`)")
	db.query("ALTER TABLE `players` ADD `onlinetime` int NOT NULL DEFAULT '0', ADD `deletion` bigint NOT NULL DEFAULT '0'")
	db.query("ALTER TABLE `accounts` ADD `points` int NOT NULL DEFAULT '0', ADD `creation` int NOT NULL DEFAULT '0'")
	return true
end
