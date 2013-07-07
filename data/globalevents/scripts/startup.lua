function onStartup()
	db.query("TRUNCATE TABLE `players_online`")
	db.query("DELETE FROM `guild_wars` WHERE `status` = 0")
end
