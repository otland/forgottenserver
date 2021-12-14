function onUpdateDatabase()
	print("> Updating database to version 32 (Magic Shield Spell)")
	db.query("ALTER TABLE `players` ADD `manashield` SMALLINT UNSIGNED NOT NULL DEFAULT '0' AFTER `manamax`")
	db.query("ALTER TABLE `players` ADD `maxmanashield` SMALLINT UNSIGNED NOT NULL DEFAULT '0' AFTER `manashield`")
	return false
end
