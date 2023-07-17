function onUpdateDatabase()
	print("> Updating database to version 35 (custom skills)")
	db.query("ALTER TABLE `players` ADD COLUMN `skill_runecraft` INT UNSIGNED NOT NULL DEFAULT 10, ADD COLUMN `skill_runecraft_tries` bigint UNSIGNED NOT NULL DEFAULT 0")
	return true
end
