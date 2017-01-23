function onUpdateDatabase()
	print("> Updating database to version 20 (renaming abbreviated fields)")
	for _, table in ipairs({"player_depotitems", "player_inboxitems", "player_items"}) do
		db.query(string.format("ALTER TABLE `%s` CHANGE COLUMN `pid` `parent_id` INT NOT NULL DEFAULT '0', CHANGE COLUMN `sid` `slot_id` INT NOT NULL", table))
	end
	return true
end
