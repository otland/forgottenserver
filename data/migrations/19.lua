function onUpdateDatabase()
	print("> Updating database to version 20 (IPv6 support)")
	db.query("ALTER TABLE `players` MODIFY `lastip` VARCHAR(45) NOT NULL")
	db.query("ALTER TABLE `ip_bans` MODIFY `ip` VARCHAR(45) NOT NULL")
	db.query("UPDATE `players` p  SET p.lastip=INET_NTOA(p.lastip)")
	db.query("UPDATE `ip_bans` b  SET b.ip=INET_NTOA(b.ip)")
	return true
end
