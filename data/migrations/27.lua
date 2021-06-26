function onUpdateDatabase()
	print("> Updating database to version 27 (data type mismatch)")
	db.query("ALTER TABLE `players` CHANGE `manaspent` `manaspent` bigint unsigned NOT NULL DEFAULT '0'");
	db.query("ALTER TABLE `players` CHANGE `experience` `experience` bigint unsigned NOT NULL DEFAULT '0'");
	db.query("ALTER TABLE `players` CHANGE `onlinetime` `onlinetime` bigint NOT NULL DEFAULT '0'");
	return true
end
