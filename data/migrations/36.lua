function onUpdateDatabase()
	print("> Updating database to version 37 (loyalty system)")
	db.query("ALTER TABLE `accounts` ADD COLUMN `loyalty_points` SMALLINT UNSIGNED NOT NULL DEFAULT 0 AFTER `premium_ends_at`")
	db.query("INSERT INTO `server_config` (`config`, `value`) VALUES ('loyalty_updated', '0')")
	return true
end
