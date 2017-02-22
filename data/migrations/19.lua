function onUpdateDatabase()
	print("> Updating database to version 20 (IPv6 support)")
	db.query("ALTER TABLE `players` MODIFY `lastip` VARBINARY(16) NOT NULL")
	db.query("ALTER TABLE `ip_bans` MODIFY `ip` VARBINARY(16) NOT NULL")
	db.query("UPDATE `players`  SET `lastip` = INET_ATON6(INET_NTOA(p.lastip))")
	db.query("UPDATE `ip_bans`  SET `ip` = INET_ATON6(INET_NTOA(b.ip))")
	return true
end
