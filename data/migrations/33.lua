function onUpdateDatabase()
	print("> Updating database to version 33 (randomize mount)")
	db.query("ALTER TABLE `players` ADD COLUMN `randomizemount` tinyint DEFAULT 0 NOT NULL AFTER `lookmountfeet`;")
	return true
end
