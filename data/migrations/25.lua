function onUpdateDatabase()
	print("> Updating database to version 26 (Better premium time handling)")
	db.asyncQuery("ALTER TABLE `accounts` DROP COLUMN `lastday`")

	local resultId = db.storeQuery("SELECT `premdays` FROM `accounts`")

	db.query("ALTER TABLE `accounts` DROP COLUMN `premdays`")
	db.query("ALTER TABLE `accounts` ADD COLUMN `premium_ends_at` datetime NOT NULL DEFAULT NOW() AFTER `type`")

	if resultId then
		repeat
			local premDays = result.getNumber(resultId, "premdays")
			db.asyncQuery("UPDATE `accounts` SET `premium_ends_at` = FROM_UNIXTIME("  .. premDays .. " * 86400 + " .. os.time() .. ")")
		until not result.next(resultId)
	end
	return true
end
