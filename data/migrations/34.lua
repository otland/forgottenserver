function onUpdateDatabase()
	print("> Updating database to version 34 (IPv6 support)")
	db.query("ALTER TABLE `players` MODIFY `lastip` varbinary(16) NOT NULL")
	db.query("ALTER TABLE `ip_bans` MODIFY `ip` varbinary(16) NOT NULL")
	db.query("UPDATE `players` SET `lastip` = INET6_ATON(INET_NTOA(`lastip`))")
	db.query("UPDATE `ip_bans` SET `ip` = INET6_ATON(INET_NTOA(`ip`))")
	return true
end
