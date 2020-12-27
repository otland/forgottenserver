function onUpdateDatabase()
	print("> Updating database to version 27 (manaspent to bigint)")
	db.query("ALTER TABLE `players` CHANGE `manaspent` `manaspent` bigint unsigned NOT NULL DEFAULT '0'");
	return true
end
