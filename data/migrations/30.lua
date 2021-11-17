function onUpdateDatabase()
	print("> Updating database to version 30 (mount colors)")
	db.query([[
		ALTER TABLE `players`
			ADD COLUMN `lookmount` int(11) DEFAULT 0 NOT NULL AFTER `lookaddons`,
			ADD COLUMN `lookmounthead` int(11) DEFAULT 0 NOT NULL AFTER `lookmount`,
			ADD COLUMN `lookmountbody` int(11) DEFAULT 0 NOT NULL AFTER `lookmounthead`,
			ADD COLUMN `lookmountlegs` int(11) DEFAULT 0 NOT NULL AFTER `lookmountbody`,
			ADD COLUMN `lookmountfeet` int(11) DEFAULT 0 NOT NULL AFTER `lookmountlegs`;
	]])
	return true
end
