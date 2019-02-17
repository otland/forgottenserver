function onUpdateDatabase()
	print("> Updating database to version 23 (Guild houses changes)")
	db.query("ALTER TABLE `guilds` ADD `house_id` int(11) NOT NULL;")
	return true
end
