function onUpdateDatabase()
	print("> Updating database to version 24 (Guildhall support)")
	db.query("ALTER TABLE `guilds` ADD `house_id` int(11) NOT NULL DEFAULT '0'")
	db.query("ALTER TABLE `houses` ADD `guild_id` int(11) NOT NULL DEFAULT '0'")
	db.query("ALTER TABLE `houses` ADD `type` tinyint(3) NOT NULL DEFAULT '0'")
	return true
end
