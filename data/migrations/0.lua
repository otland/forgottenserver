function onUpdateDatabase()
	print("> Updating database to version 1 (account names)")
	db.query("ALTER TABLE `accounts` ADD `name` VARCHAR(32) NOT NULL AFTER `id`")
	db.query("UPDATE `accounts` SET `name` = `id`")
	db.query("ALTER TABLE `accounts` ADD UNIQUE (`name`)")
	return true
end
