function onUpdateDatabase()
	print("> Updating database to version 19 (optimize health/healthmax to unsigned int)")
	db.query("ALTER TABLE `players` CHANGE `health` `health` INT(11) UNSIGNED NOT NULL DEFAULT '150'")
	db.query("ALTER TABLE `players` CHANGE `healthmax` `healthmax` INT(11) UNSIGNED NOT NULL DEFAULT '150'")
	return true
end
