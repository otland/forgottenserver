function onUpdateDatabase()
	print("> Updating database to version 24 (Add player direction)")
	db.query("ALTER TABLE `players` ADD `direction` int unsigned NOT NULL DEFAULT 2")
	return true
end
