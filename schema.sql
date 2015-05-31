CREATE TABLE IF NOT EXISTS `accounts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL,
  `password` char(40) NOT NULL,
  `type` int(11) NOT NULL DEFAULT '1',
  `premdays` int(11) NOT NULL DEFAULT '0',
  `lastday` int(10) unsigned NOT NULL DEFAULT '0',
  `email` varchar(255) NOT NULL DEFAULT '',
  `creation` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `players` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `group_id` int(11) NOT NULL DEFAULT '1',
  `account_id` int(11) NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '1',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `health` int(11) NOT NULL DEFAULT '150',
  `healthmax` int(11) NOT NULL DEFAULT '150',
  `experience` bigint(20) NOT NULL DEFAULT '0',
  `lookbody` int(11) NOT NULL DEFAULT '0',
  `lookfeet` int(11) NOT NULL DEFAULT '0',
  `lookhead` int(11) NOT NULL DEFAULT '0',
  `looklegs` int(11) NOT NULL DEFAULT '0',
  `looktype` int(11) NOT NULL DEFAULT '136',
  `lookaddons` int(11) NOT NULL DEFAULT '0',
  `maglevel` int(11) NOT NULL DEFAULT '0',
  `mana` int(11) NOT NULL DEFAULT '0',
  `manamax` int(11) NOT NULL DEFAULT '0',
  `manaspent` int(11) unsigned NOT NULL DEFAULT '0',
  `soul` int(10) unsigned NOT NULL DEFAULT '0',
  `town_id` int(11) NOT NULL DEFAULT '0',
  `posx` int(11) NOT NULL DEFAULT '0',
  `posy` int(11) NOT NULL DEFAULT '0',
  `posz` int(11) NOT NULL DEFAULT '0',
  `conditions` blob NOT NULL,
  `cap` int(11) NOT NULL DEFAULT '0',
  `sex` int(11) NOT NULL DEFAULT '0',
  `lastlogin` bigint(20) unsigned NOT NULL DEFAULT '0',
  `lastip` int(10) unsigned NOT NULL DEFAULT '0',
  `save` tinyint(1) NOT NULL DEFAULT '1',
  `skull` tinyint(1) NOT NULL DEFAULT '0',
  `skulltime` int(11) NOT NULL DEFAULT '0',
  `lastlogout` bigint(20) unsigned NOT NULL DEFAULT '0',
  `blessings` tinyint(2) NOT NULL DEFAULT '0',
  `onlinetime` int(11) NOT NULL DEFAULT '0',
  `deletion` bigint(15) NOT NULL DEFAULT '0',
  `balance` bigint(20) unsigned NOT NULL DEFAULT '0',
  `offlinetraining_time` smallint(5) unsigned NOT NULL DEFAULT '43200',
  `offlinetraining_skill` int(11) NOT NULL DEFAULT '-1',
  `stamina` smallint(5) unsigned NOT NULL DEFAULT '2520',
  `skill_fist` int(10) unsigned NOT NULL DEFAULT 10,
  `skill_fist_tries` bigint(20) unsigned NOT NULL DEFAULT 0,
  `skill_club` int(10) unsigned NOT NULL DEFAULT 10,
  `skill_club_tries` bigint(20) unsigned NOT NULL DEFAULT 0,
  `skill_sword` int(10) unsigned NOT NULL DEFAULT 10,
  `skill_sword_tries` bigint(20) unsigned NOT NULL DEFAULT 0,
  `skill_axe` int(10) unsigned NOT NULL DEFAULT 10,
  `skill_axe_tries` bigint(20) unsigned NOT NULL DEFAULT 0,
  `skill_dist` int(10) unsigned NOT NULL DEFAULT 10,
  `skill_dist_tries` bigint(20) unsigned NOT NULL DEFAULT 0,
  `skill_shielding` int(10) unsigned NOT NULL DEFAULT 10,
  `skill_shielding_tries` bigint(20) unsigned NOT NULL DEFAULT 0,
  `skill_fishing` int(10) unsigned NOT NULL DEFAULT 10,
  `skill_fishing_tries` bigint(20) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE,
  KEY `vocation` (`vocation`)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `account_bans` (
  `account_id` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL,
  `banned_at` bigint(20) NOT NULL,
  `expires_at` bigint(20) NOT NULL,
  `banned_by` int(11) NOT NULL,
  PRIMARY KEY (`account_id`),
  FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (`banned_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `account_ban_history` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL,
  `banned_at` bigint(20) NOT NULL,
  `expired_at` bigint(20) NOT NULL,
  `banned_by` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (`banned_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `ip_bans` (
  `ip` int(10) unsigned NOT NULL,
  `reason` varchar(255) NOT NULL,
  `banned_at` bigint(20) NOT NULL,
  `expires_at` bigint(20) NOT NULL,
  `banned_by` int(11) NOT NULL,
  PRIMARY KEY (`ip`),
  FOREIGN KEY (`banned_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `player_namelocks` (
  `player_id` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL,
  `namelocked_at` bigint(20) NOT NULL,
  `namelocked_by` int(11) NOT NULL,
  PRIMARY KEY (`player_id`),
  FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (`namelocked_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `account_viplist` (
  `account_id` int(11) NOT NULL COMMENT 'id of account whose viplist entry it is',
  `player_id` int(11) NOT NULL COMMENT 'id of target player of viplist entry',
  `description` varchar(128) NOT NULL DEFAULT '',
  `icon` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `notify` tinyint(1) NOT NULL DEFAULT '0',
  UNIQUE KEY `account_player_index` (`account_id`,`player_id`),
  FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE,
  FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `guilds` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `ownerid` int(11) NOT NULL,
  `creationdata` int(11) NOT NULL,
  `motd` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE KEY (`name`),
  UNIQUE KEY (`ownerid`),
  FOREIGN KEY (`ownerid`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `guild_invites` (
  `player_id` int(11) NOT NULL DEFAULT '0',
  `guild_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`player_id`,`guild_id`),
  FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE,
  FOREIGN KEY (`guild_id`) REFERENCES `guilds` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `guild_ranks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `guild_id` int(11) NOT NULL COMMENT 'guild',
  `name` varchar(255) NOT NULL COMMENT 'rank name',
  `level` int(11) NOT NULL COMMENT 'rank level - leader, vice, member, maybe something else',
  PRIMARY KEY (`id`),
  FOREIGN KEY (`guild_id`) REFERENCES `guilds` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `guild_membership` (
  `player_id` int(11) NOT NULL,
  `guild_id` int(11) NOT NULL,
  `rank_id` int(11) NOT NULL,
  `nick` varchar(15) NOT NULL DEFAULT '',
  PRIMARY KEY (`player_id`),
  FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (`guild_id`) REFERENCES `guilds` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (`rank_id`) REFERENCES `guild_ranks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `guild_wars` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `guild1` int(11) NOT NULL DEFAULT '0',
  `guild2` int(11) NOT NULL DEFAULT '0',
  `name1` varchar(255) NOT NULL,
  `name2` varchar(255) NOT NULL,
  `status` tinyint(2) NOT NULL DEFAULT '0',
  `started` bigint(15) NOT NULL DEFAULT '0',
  `ended` bigint(15) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `guild1` (`guild1`),
  KEY `guild2` (`guild2`)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `guildwar_kills` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `killer` varchar(50) NOT NULL,
  `target` varchar(50) NOT NULL,
  `killerguild` int(11) NOT NULL DEFAULT '0',
  `targetguild` int(11) NOT NULL DEFAULT '0',
  `warid` int(11) NOT NULL DEFAULT '0',
  `time` bigint(15) NOT NULL,
  PRIMARY KEY (`id`),
  FOREIGN KEY (`warid`) REFERENCES `guild_wars` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `houses` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner` int(11) NOT NULL,
  `paid` int(10) unsigned NOT NULL DEFAULT '0',
  `warnings` int(11) NOT NULL DEFAULT '0',
  `name` varchar(255) NOT NULL,
  `rent` int(11) NOT NULL DEFAULT '0',
  `town_id` int(11) NOT NULL DEFAULT '0',
  `bid` int(11) NOT NULL DEFAULT '0',
  `bid_end` int(11) NOT NULL DEFAULT '0',
  `last_bid` int(11) NOT NULL DEFAULT '0',
  `highest_bidder` int(11) NOT NULL DEFAULT '0',
  `size` int(11) NOT NULL DEFAULT '0',
  `beds` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `owner` (`owner`),
  KEY `town_id` (`town_id`)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `house_lists` (
  `house_id` int(11) NOT NULL,
  `listid` int(11) NOT NULL,
  `list` text NOT NULL,
  FOREIGN KEY (`house_id`) REFERENCES `houses` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `market_history` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `player_id` int(11) NOT NULL,
  `sale` tinyint(1) NOT NULL DEFAULT '0',
  `itemtype` int(10) unsigned NOT NULL,
  `amount` smallint(5) unsigned NOT NULL,
  `price` int(10) unsigned NOT NULL DEFAULT '0',
  `expires_at` bigint(20) unsigned NOT NULL,
  `inserted` bigint(20) unsigned NOT NULL,
  `state` tinyint(1) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `player_id` (`player_id`, `sale`),
  FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `market_offers` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `player_id` int(11) NOT NULL,
  `sale` tinyint(1) NOT NULL DEFAULT '0',
  `itemtype` int(10) unsigned NOT NULL,
  `amount` smallint(5) unsigned NOT NULL,
  `created` bigint(20) unsigned NOT NULL,
  `anonymous` tinyint(1) NOT NULL DEFAULT '0',
  `price` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `sale` (`sale`,`itemtype`),
  KEY `created` (`created`),
  FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `players_online` (
  `player_id` int(11) NOT NULL,
  PRIMARY KEY (`player_id`)
) ENGINE=MEMORY;

CREATE TABLE IF NOT EXISTS `player_deaths` (
  `player_id` int(11) NOT NULL,
  `time` bigint(20) unsigned NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '1',
  `killed_by` varchar(255) NOT NULL,
  `is_player` tinyint(1) NOT NULL DEFAULT '1',
  `mostdamage_by` varchar(100) NOT NULL,
  `mostdamage_is_player` tinyint(1) NOT NULL DEFAULT '0',
  `unjustified` tinyint(1) NOT NULL DEFAULT '0',
  `mostdamage_unjustified` tinyint(1) NOT NULL DEFAULT '0',
  FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE,
  KEY `killed_by` (`killed_by`),
  KEY `mostdamage_by` (`mostdamage_by`)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `player_depotitems` (
  `player_id` int(11) NOT NULL,
  `sid` int(11) NOT NULL COMMENT 'any given range eg 0-100 will be reserved for depot lockers and all > 100 will be then normal items inside depots',
  `pid` int(11) NOT NULL DEFAULT '0',
  `itemtype` smallint(6) NOT NULL,
  `count` smallint(5) NOT NULL DEFAULT '0',
  `attributes` blob NOT NULL,
  UNIQUE KEY `player_id_2` (`player_id`, `sid`),
  FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `player_inboxitems` (
  `player_id` int(11) NOT NULL,
  `sid` int(11) NOT NULL,
  `pid` int(11) NOT NULL DEFAULT '0',
  `itemtype` smallint(6) NOT NULL,
  `count` smallint(5) NOT NULL DEFAULT '0',
  `attributes` blob NOT NULL,
  UNIQUE KEY `player_id_2` (`player_id`, `sid`),
  FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `player_items` (
  `player_id` int(11) NOT NULL DEFAULT '0',
  `pid` int(11) NOT NULL DEFAULT '0',
  `sid` int(11) NOT NULL DEFAULT '0',
  `itemtype` smallint(6) NOT NULL DEFAULT '0',
  `count` smallint(5) NOT NULL DEFAULT '0',
  `attributes` blob NOT NULL,
  FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE,
  KEY `sid` (`sid`)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `player_spells` (
  `player_id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `player_storage` (
  `player_id` int(11) NOT NULL DEFAULT '0',
  `key` int(10) unsigned NOT NULL DEFAULT '0',
  `value` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`player_id`,`key`),
  FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `server_config` (
  `config` varchar(50) NOT NULL,
  `value` varchar(256) NOT NULL DEFAULT '',
  PRIMARY KEY `config` (`config`)
) ENGINE=InnoDB;

INSERT INTO `server_config` (`config`, `value`) VALUES ('db_version', '18'), ('motd_hash', ''), ('motd_num', '0'), ('players_record', '0');

CREATE TABLE IF NOT EXISTS `tile_store` (
  `house_id` int(11) NOT NULL,
  `data` longblob NOT NULL,
  FOREIGN KEY (`house_id`) REFERENCES `houses` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

DROP TRIGGER IF EXISTS `ondelete_players`;
DROP TRIGGER IF EXISTS `oncreate_guilds`;

DELIMITER //
CREATE TRIGGER `ondelete_players` BEFORE DELETE ON `players`
 FOR EACH ROW BEGIN
    UPDATE `houses` SET `owner` = 0 WHERE `owner` = OLD.`id`;
END
//
CREATE TRIGGER `oncreate_guilds` AFTER INSERT ON `guilds`
 FOR EACH ROW BEGIN
    INSERT INTO `guild_ranks` (`name`, `level`, `guild_id`) VALUES ('the Leader', 3, NEW.`id`);
    INSERT INTO `guild_ranks` (`name`, `level`, `guild_id`) VALUES ('a Vice-Leader', 2, NEW.`id`);
    INSERT INTO `guild_ranks` (`name`, `level`, `guild_id`) VALUES ('a Member', 1, NEW.`id`);
END
//
DELIMITER ;
