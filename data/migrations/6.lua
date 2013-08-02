function onUpdateDatabase()
	print("> Updating database to version 7 (offline training)")
	db.query("ALTER TABLE `players` ADD `offlinetraining_time` SMALLINT UNSIGNED NOT NULL DEFAULT 43200")
	db.query("ALTER TABLE `players` ADD `offlinetraining_skill` INT NOT NULL DEFAULT -1")
	return true
end
