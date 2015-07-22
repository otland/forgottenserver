local function alterTables()
	-- creating columns
	db.query("ALTER TABLE `account_ban_history` ADD COLUMN `banned_at_new` TIMESTAMP NOT NULL AFTER `banned_at`, ADD COLUMN `expired_at_new` TIMESTAMP NOT NULL AFTER `expired_at`")
	db.query("ALTER TABLE `account_bans` ADD COLUMN `banned_at_new` TIMESTAMP NOT NULL AFTER `banned_at`, ADD COLUMN `expires_at_new` TIMESTAMP NOT NULL AFTER `expires_at`")
	db.query("ALTER TABLE `accounts` ADD COLUMN `last_login` TIMESTAMP NOT NULL AFTER `lastday`, ADD COLUMN `created_at` TIMESTAMP NOT NULL AFTER `creation`")
	db.query("ALTER TABLE `guilds` ADD COLUMN `created_at` TIMESTAMP NOT NULL AFTER `creationdata`")
	db.query("ALTER TABLE `guildwar_kills` ADD COLUMN `killed_at` TIMESTAMP NOT NULL AFTER `time`")
	db.query("ALTER TABLE `guild_wars` ADD COLUMN `started_at` TIMESTAMP NOT NULL AFTER `started`, ADD COLUMN `ended_at` TIMESTAMP NOT NULL AFTER `ended`")
	db.query("ALTER TABLE `houses` ADD COLUMN `bid_end_at` TIMESTAMP NULL AFTER `bid_end`")
	db.query("ALTER TABLE `ip_bans` ADD COLUMN `banned_at_new` TIMESTAMP NOT NULL AFTER `banned_at`, ADD COLUMN `expires_at_new` TIMESTAMP NOT NULL AFTER `expires_at`")
	db.query("ALTER TABLE `market_history` ADD COLUMN `created_at` TIMESTAMP NOT NULL AFTER `inserted`, ADD COLUMN `expired_at` TIMESTAMP NOT NULL AFTER `expires_at`")
	db.query("ALTER TABLE `market_offers` ADD COLUMN `created_at` TIMESTAMP NOT NULL AFTER `created`")
	db.query("ALTER TABLE `player_deaths` ADD COLUMN `killed_at` TIMESTAMP NOT NULL AFTER `time`")
	db.query("ALTER TABLE `player_namelocks` ADD COLUMN `namelocked_at_new` TIMESTAMP NOT NULL AFTER `namelocked_at`")
	db.query("ALTER TABLE `players` ADD COLUMN `deleted_at` TIMESTAMP NOT NULL AFTER `deletion`, ADD COLUMN `last_login` TIMESTAMP NOT NULL AFTER `lastlogin`, ADD COLUMN `last_logout` TIMESTAMP NOT NULL AFTER `lastlogout`")
end

local function copyData()
	-- copying old data
	db.query("UPDATE `account_ban_history` SET `banned_at_new` = FROM_UNIXTIME(`banned_at`), `expired_at_new` = FROM_UNIXTIME(`expired_at`)")
	db.query("UPDATE `account_bans` SET `banned_at_new` = FROM_UNIXTIME(`banned_at`), `expires_at_new` = FROM_UNIXTIME(`expires_at`)")
	db.query("UPDATE `accounts` SET `last_login` = FROM_UNIXTIME(`lastday`), `created_at` = FROM_UNIXTIME(`creation`)")
	db.query("UPDATE `guilds` SET `created_at` = FROM_UNIXTIME(`creationdata`)")
	db.query("UPDATE `guildwar_kills` SET `killed_at` = FROM_UNIXTIME(`time`)")
	db.query("UPDATE `guild_wars` SET `started_at` = FROM_UNIXTIME(`started`), `ended_at` = FROM_UNIXTIME(`ended`))
	db.query("UPDATE `houses` SET `bid_end_at` = FROM_UNIXTIME(`bid_end`)")
	db.query("UPDATE `ip_bans` SET `banned_at_new` = FROM_UNIXTIME(`banned_at`), `expires_at_new` = FROM_UNIXTIME(`expires_at`)")
	db.query("UPDATE `market_history` SET `created_at` = FROM_UNIXTIME(`inserted`), `expired_at` = FROM_UNIXTIME(`expires_at`)")
	db.query("UPDATE `market_offers` SET `created_at` = FROM_UNIXTIME(`created`)")
	db.query("UPDATE `player_deaths` SET `killed_at` = FROM_UNIXTIME(`time`)")
	db.query("UPDATE `player_namelocks` SET `namelocked_at_new` = FROM_UNIXTIME(`namelocked_at`)")
	db.query("UPDATE `players` SET `deleted_at` = FROM_UNIXTIME(`deletion`), `last_login` = FROM_UNIXTIME(`lastlogin`), `last_logout` = FROM_UNIXTIME(`lastlogout`)")
end

local function dropColumns()
	-- removing old columns
	db.query("ALTER TABLE `account_ban_history` DROP COLUMN `banned_at`, DROP COLUMN `expired_at`");
	db.query("ALTER TABLE `account_bans` DROP COLUMN `banned_at`, DROP COLUMN `expires_at`");
	db.query("ALTER TABLE `accounts` DROP COLUMN `creation`")
	db.query("ALTER TABLE `guilds` DROP COLUMN `creationdata`")
	db.query("ALTER TABLE `guildwar_kills` DROP COLUMN `time`")
	db.query("ALTER TABLE `guild_wars` DROP COLUMN `started`, `ended`")
	db.query("ALTER TABLE `houses` DROP COLUMN `bid_end`")
	db.query("ALTER TABLE `ip_bans` DROP COLUMN `banned_at`, DROP COLUMN `expires_at`");
	db.query("ALTER TABLE `market_history` DROP COLUMN `inserted`, DROP COLUMN `expires_at`");
	db.query("ALTER TABLE `market_offers` DROP COLUMN `created`");
	db.query("ALTER TABLE `player_deaths` DROP COLUMN `time`")
	db.query("ALTER TABLE `player_namelocks` DROP COLUMN `namelocked_at`")
	db.query("ALTER TABLE `players` DROP COLUMN `deletion`, DROP COLUMN `lastlogin`, DROP COLUMN `lastlogout`");
end

local function renameColumns()
	-- changing temporary names
	db.query("ALTER TABLE `account_ban_history` CHANGE COLUMN `banned_at_new` `banned_at` TIMESTAMP NOT NULL, CHANGE COLUMN `expired_at_new` `expired_at` TIMESTAMP NOT NULL")
	db.query("ALTER TABLE `account_bans` CHANGE COLUMN `banned_at_new` `banned_at` TIMESTAMP NOT NULL, CHANGE COLUMN `expires_at_new` `expires_at` TIMESTAMP NOT NULL")
	db.query("ALTER TABLE `ip_bans` CHANGE COLUMN `banned_at_new` `banned_at` TIMESTAMP NOT NULL, CHANGE COLUMN `expires_at_new` `expires_at` TIMESTAMP NOT NULL")
	db.query("ALTER TABLE `player_namelocks` CHANGE COLUMN `namelocked_at_new` `namelocked_at` TIMESTAMP NOT NULL")
end

function onUpdateDatabase()
	print("> Updating database to version 19 (timestamps improvement)")
	alterTables()
	copyData()
	dropColumns()
	renameColumns()
	return true
end
