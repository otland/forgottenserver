function onUpdateDatabase()
	print("> Updating database to version 20 (setting default cap to 400)")
	db.query("ALTER TABLE `players` CHANGE `cap` `cap` int(11) NOT NULL DEFAULT '400'")
	return true
end
