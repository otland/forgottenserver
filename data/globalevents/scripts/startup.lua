function onStartup()
	db.query("TRUNCATE TABLE `players_online`")
	db.query("DELETE FROM `guild_wars` WHERE `status` = 0")
	db.query("DELETE FROM `ip_bans` WHERE `expires_at` != 0 AND `expires_at` <= " .. os.time())
	
	local resultId = db.storeQuery("SELECT * FROM `account_bans` WHERE `expires_at` != 0 AND `expires_at` <= " .. os.time())
	if resultId ~= false then
		repeat
			db.query("INSERT INTO `account_ban_history` (`account_id`, `reason`, `banned_at`, `expired_at`, `banned_by`) VALUES (" .. result.getDataInt(resultId, "account_id") .. ", " .. db.escapeString(result.getDataString(resultId, "reason")) .. ", " .. result.getDataLong(resultId, "banned_at") .. ", " .. result.getDataLong(resultId, "expires_at") .. ", " .. result.getDataInt(resultId, "banned_by") .. ")")
			db.query("DELETE FROM `account_bans WHERE `account_id` = " .. accountId);
		until not result.next(resultId)
		result.free(resultId)
	end
end
