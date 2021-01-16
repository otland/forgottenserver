function onUpdateDatabase()
	print('> Updating database to version 28 (Change storage values to 64bit signed integers)')
	db.query('ALTER TABLE `player_storage` MODIFY COLUMN `value` bigint NOT NULL DEFAULT 0')
	return true
end
