function onUpdateDatabase()
	print("> Updating database to version 31 (64 bit market prices)")
	db.query("ALTER TABLE `market_offers` MODIFY `price` BIGINT UNSIGNED;")
	db.query("ALTER TABLE `market_history` MODIFY `price` BIGINT UNSIGNED;")
	return true
end
