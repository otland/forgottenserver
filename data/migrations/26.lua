function onUpdateDatabase()
	print("> Updating database to version 26 (Better premium time handling)")
	db.query("ALTER TABLE `accounts` DROP COLUMN `lastday`")
	db.query("ALTER TABLE `accounts` CHANGE COLUMN `premdays` `premium_ends_at` int unsigned NOT NULL DEFAULT 0");
	db.query("UPDATE `accounts` SET `premium_ends_at` = `premium_ends_at` * 86400 + " .. os.time())
	return true
end
