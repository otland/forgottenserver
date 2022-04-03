function onUpdateDatabase()
	print("> Updating database to version 32 (removed MOTD)")
	db.query("DELETE FROM `server_config` WHERE `config` = 'motd_num'")
	db.query("DELETE FROM `server_config` WHERE `config` = 'motd_hash'")
	return true
end
