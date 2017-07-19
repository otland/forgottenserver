function onUpdateDatabase()
	print("> Updating database to version 20 (live casting support)") 
	db.query("ALTER TABLE `players_online` ADD COLUMN `cast_on` tinyint(1) default '0' NOT NULL, ADD COLUMN `cast_password` varchar(40) default NULL, ADD COLUMN `cast_spectators` int(5) default '0' NOT NULL;")
	return true
end
