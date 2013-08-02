function onUpdateDatabase()
	print("> Updating database to version 11 (improved guild and players online structure)")
	db.query("CREATE TABLE IF NOT EXISTS `guild_membership` (`player_id` int(11) NOT NULL, `guild_id` int(11) NOT NULL, `rank_id` int(11) NOT NULL, `nick` varchar(15) NOT NULL DEFAULT '', PRIMARY KEY (`player_id`), KEY `guild_id` (`guild_id`), KEY `rank_id` (`rank_id`)) ENGINE=InnoDB")
	db.query("ALTER TABLE `guild_membership` ADD CONSTRAINT `guild_membership_ibfk_3` FOREIGN KEY (`rank_id`) REFERENCES `guild_ranks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE, ADD CONSTRAINT `guild_membership_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE, ADD CONSTRAINT `guild_membership_ibfk_2` FOREIGN KEY (`guild_id`) REFERENCES `guilds` (`id`) ON DELETE CASCADE ON UPDATE CASCADE")
	db.query("ALTER TABLE `guild_invites` ADD PRIMARY KEY (`player_id`, `guild_id`)")
	db.query("ALTER TABLE `player_skills` ADD PRIMARY KEY (`player_id`, `skillid`)")
	db.query("ALTER TABLE `player_storage` ADD PRIMARY KEY (`player_id`, `key`)")

	local resultId = db.storeQuery("SELECT `players`.`id` AS `player_id`, `players`.`rank_id` AS `rank_id`, `players`.`guildnick` AS `guild_nick`, `guild_ranks`.`guild_id` AS `guild_id` FROM `guild_ranks` INNER JOIN `players` ON `guild_ranks`.`id` = `players`.`rank_id`")
	if resultId ~= false then
		local stmt = "INSERT INTO `guild_membership` (`player_id`, `guild_id`, `rank_id`, `nick`) VALUES "
		repeat
			stmt = stmt .. "(" .. result.getDataInt(resultId, "player_id") .. "," .. result.getDataInt(resultId, "guild_id") .. "," .. result.getDataInt(resultId, "rank_id") .. "," .. db.escapeString(result.getDataString(resultId, "guild_nick")) .. "),"
		until not result.next(resultId)
		result.free(resultId)

		local stmtLen = string.len(stmt)
		if stmtLen > 83 then
			stmt = string.sub(stmt, 1, stmtLen - 1)
			db.query(stmt)
		end
	end

	db.query("ALTER TABLE `players` DROP `rank_id`, DROP `guildnick`, DROP `direction`, DROP `loss_experience`, DROP `loss_mana`, DROP `loss_skills`, DROP `premend`, DROP `online`")
	db.query("DROP TRIGGER IF EXISTS `ondelete_guilds`")
	db.query("CREATE TABLE IF NOT EXISTS `players_online` (`player_id` int(11) NOT NULL, PRIMARY KEY (`player_id`)) ENGINE=MEMORY")
	return true
end
