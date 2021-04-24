function onUpdateDatabase()
	print("> Updating database to version 29 (setting default outfit colors)")
	db.query("ALTER TABLE `players` CHANGE `lookbody` `lookbody` int NOT NULL DEFAULT '69'")
	db.query("ALTER TABLE `players` CHANGE `lookfeet` `lookfeet` int NOT NULL DEFAULT '76'")
	db.query("ALTER TABLE `players` CHANGE `lookhead` `lookhead` int NOT NULL DEFAULT '78'")
	db.query("ALTER TABLE `players` CHANGE `looklegs` `looklegs` int NOT NULL DEFAULT '58'")
	db.query("ALTER TABLE `players` CHANGE `looktype` `looktype` int NOT NULL DEFAULT '128'")
	return true
end