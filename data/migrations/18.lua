function onUpdateDatabase()
	print("> Updating database to version 19 (authenticator token support)")
	db.query("ALTER TABLE `accounts` ADD COLUMN `secret` CHAR(16) NULL AFTER `password`")
	return true
end
