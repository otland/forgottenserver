function onStartup()
	db.query("DELETE FROM `guild_wars` WHERE `status` = 0")
end
