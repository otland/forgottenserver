local event = GlobalEvent("AccountStorage")

function event.onStartup()
	local resultId = db.asyncStoreQuery("SELECT `account_id`, `key`, `value` FROM `account_storage`")
	if not resultId then
		return true
	end

	repeat
		local accountId = result.getNumber(resultId, "account_id")
        local key = result.getNumber(resultId, "key")
        local value = result.getNumber(resultId, "value")
        Game.setAccountStorageValue(accountId, key, value)
    until not result.next(resultId)
	result.free(resultId)

	return true
end

event:register()
