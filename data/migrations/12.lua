function onUpdateDatabase()
	print("> Updating database to version 13 (house bidding system and additional columns to other tables)")

	db.query("ALTER TABLE `player_deaths` ADD `mostdamage_by` varchar(100) NOT NULL, ADD `mostdamage_is_player` tinyint(1) NOT NULL DEFAULT '0', ADD `unjustified` tinyint(1) NOT NULL DEFAULT '0', ADD `mostdamage_unjustified` tinyint(1) NOT NULL DEFAULT '0', ADD KEY `killed_by` (`killed_by`), ADD KEY `mostdamage_by` (`mostdamage_by`)")
	db.query("ALTER TABLE `houses` ADD `name` varchar(255) NOT NULL, ADD `rent` int(11) NOT NULL DEFAULT '0', ADD `town_id` int(11) NOT NULL DEFAULT '0', ADD `bid` int(11) NOT NULL DEFAULT '0', ADD `bid_end` int(11) NOT NULL DEFAULT '0', ADD `last_bid` int(11) NOT NULL DEFAULT '0', ADD `highest_bidder` int(11) NOT NULL DEFAULT '0', ADD `size` int(11) NOT NULL DEFAULT '0', ADD `beds` int(11) NOT NULL DEFAULT '0', ADD KEY `owner` (`owner`), ADD KEY `town_id` (`town_id`)")
	db.query("ALTER TABLE `players` ADD `onlinetime` int(11) NOT NULL DEFAULT '0', ADD `deletion` bigint(15) NOT NULL DEFAULT '0'")
	db.query("ALTER TABLE `accounts` ADD `points` int(11) NOT NULL DEFAULT '0', ADD `creation` int(11) NOT NULL DEFAULT '0'")
	return true
end
