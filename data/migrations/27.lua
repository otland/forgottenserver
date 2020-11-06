function onUpdateDatabase()
	print("> Updating database to version 27 (change type of `player_storage`.`value` from int to string)")
	db.query("ALTER TABLE `player_storage` CHANGE `value` `value` VARCHAR(255) NOT NULL DEFAULT '0'")
	return true
end
