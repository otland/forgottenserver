function onUpdateDatabase()
	print("> Updating database to version 26 (Better premium end handling)")
	db.query("ALTER TABLE `accounts` DROP COLUMN `lastday`")
	db.query("ALTER TABLE `accounts` CHANGE COLUMN `premdays` `premend` int(11) NOT NULL DEFAULT 0");
	return true
end