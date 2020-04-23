function onUpdateDatabase()
	print('> Updating database to version 26 (Change storage values to 64bit signed integers)')
	db.query('ALTER TABLE `player_storage` MODIFY COLUMN `value` bigint(20) NOT NULL DEFAULT 0')
	return true
end
