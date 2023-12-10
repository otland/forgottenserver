-- load accounts storages
local load = GlobalEvent("LoadAccountStorage")

function load.onStartup()
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

load:register()

-- save accounts storages
local function trySaveAccountsStorage()
	local transaction = DBTransaction()
	if not transaction.begin() then
		return false
	end

	local result = db.query("DELETE FROM `account_storage`")
	if not result then
		return false
	end

	local accountsStorage = Game.getAccountsStorage()
	for _, accountStorage in pairs(accountsStorage) do
		
	end
end

local save = GlobalEvent("SaveAccountStorage")

function save.onSave()
	local success = trySaveAccountsStorage()
	if not success then
		print("Failed to save account-level storage values.")
	end
	return true
end

save:register()
