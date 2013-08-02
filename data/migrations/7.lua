function onUpdateDatabase()
	print("> Updating database to version 8 (account viplist with description, icon and notify server side)")
	db.query("RENAME TABLE `player_viplist` TO `account_viplist`")
	db.query("ALTER TABLE `account_viplist` DROP FOREIGN KEY `account_viplist_ibfk_1`")
	db.query("UPDATE `account_viplist` SET `player_id` = (SELECT `account_id` FROM `players` WHERE `id` = `player_id`)")
	db.query("ALTER TABLE `account_viplist` CHANGE `player_id` `account_id` INT( 11 ) NOT NULL COMMENT 'id of account whose viplist entry it is'")
	db.query("ALTER TABLE `account_viplist` DROP FOREIGN KEY `account_viplist_ibfk_2`")
	db.query("ALTER TABLE `account_viplist` CHANGE `vip_id` `player_id` INT( 11 ) NOT NULL COMMENT 'id of target player of viplist entry'")
	db.query("ALTER TABLE `account_viplist` DROP INDEX `player_id`, ADD INDEX `account_id` (`account_id`)")
	db.query("ALTER TABLE `account_viplist` DROP INDEX `vip_id`, ADD INDEX `player_id` (`player_id`)")
	db.query("ALTER TABLE `account_viplist` ADD FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE")
	db.query("ALTER TABLE `account_viplist` ADD FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE")
	db.query("ALTER TABLE `account_viplist` ADD `description` VARCHAR(128) NOT NULL DEFAULT '', ADD `icon` TINYINT( 2 ) UNSIGNED NOT NULL DEFAULT '0', ADD `notify` TINYINT( 1 ) NOT NULL DEFAULT '0'")

	-- Remove duplicates
	local resultId = db.storeQuery("SELECT `account_id`, `player_id`, COUNT(*) AS `count` FROM `account_viplist` GROUP BY `account_id`, `player_id` HAVING COUNT(*) > 1")
	if resultId ~= false then
		repeat
			db.query("DELETE FROM `account_viplist` WHERE `account_id` = " .. result.getDataInt(resultId, "account_id") .. " AND `player_id` = " .. result.getDataInt(resultId, "player_id") .. " LIMIT " .. (result.getDataInt(resultId, "count") - 1))
		until not result.next(resultId)
		result.free(resultId)
	end

	-- Remove if an account has over 200 entries
	resultId = db.storeQuery("SELECT `account_id`, COUNT(*) AS `count` FROM `account_viplist` GROUP BY `account_id` HAVING COUNT(*) > 200")
	if resultId ~= false then
		repeat
			db.query("DELETE FROM `account_viplist` WHERE `account_id` = " .. result.getDataInt(resultId, "account_id") .. " LIMIT " .. (result.getDataInt(resultId, "count") - 200))
		until not result.next(resultId)
		result.free(resultId)
	end

	db.query("ALTER TABLE `account_viplist` ADD UNIQUE `account_player_index` (`account_id`, `player_id`)")
	return true
end
