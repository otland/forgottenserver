function onUpdateDatabase()
	print("> Updating database to version 18 (optimize account password field)")
	db.query("DELETE FROM `server_config` WHERE `config` = 'encryption'")
	db.query("ALTER TABLE `accounts` CHANGE `password` `password` CHAR(40) NOT NULL")
	return true
end
