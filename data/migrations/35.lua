function onUpdateDatabase()
	print("> Updating database to version 35 (bye bye relational items)")
	db.query([[
		ALTER TABLE `players`
			ADD COLUMN `inventory_items` LONGBLOB NOT NULL AFTER `skill_fishing_tries`,
			ADD COLUMN `depot_items` LONGBLOB NOT NULL AFTER `inventory_items`,
			ADD COLUMN `inbox_items` LONGBLOB NOT NULL AFTER `depot_items`,
			ADD COLUMN `storeinbox_items` LONGBLOB NOT NULL AFTER `inbox_items`;
	]])
	return true
end
