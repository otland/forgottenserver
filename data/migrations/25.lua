function onUpdateDatabase()
	print("> Updating database to version 26 (store player storages and learned spells in binary)")
	db.query("DROP TABLE `player_storage`;")
	db.query("DROP TABLE `player_spells`;")
	db.query("ALTER TABLE `players` ADD `spells` blob DEFAULT NULL AFTER `conditions`;")
	db.query("ALTER TABLE `players` ADD `storages` mediumblob DEFAULT NULL AFTER `spells`;")
	return true
end