function onUpdateDatabase()
	print("> Updating database to version 31 (biger price support)")
	db.query("ALTER TABLE `market_history` CHANGE `price` `price` bigint unsigned NOT NULL DEFAULT '0'");
	db.query("ALTER TABLE `market_offers` CHANGE `price` `price` bigint unsigned NOT NULL DEFAULT '0'");
	return true
end
