function onUpdateDatabase()
	print("> Updating database to version 28 (data type mismatch 2)")
	db.query("ALTER TABLE `market_history` CHANGE `itemtype` `itemtype` smallint unsigned NOT NULL");
	db.query("ALTER TABLE `market_offers` CHANGE `itemtype` `itemtype` smallint unsigned NOT NULL");
	db.query("ALTER TABLE `player_depotitems` CHANGE `itemtype` `itemtype` smallint unsigned NOT NULL");
	db.query("ALTER TABLE `player_inboxitems` CHANGE `itemtype` `itemtype` smallint unsigned NOT NULL");
	db.query("ALTER TABLE `player_storeinboxitems` CHANGE `itemtype` `itemtype` smallint unsigned NOT NULL");
	db.query("ALTER TABLE `player_items` CHANGE `itemtype` `itemtype` smallint unsigned NOT NULL");
	return true
end
