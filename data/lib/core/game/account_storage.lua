do
	local accountsStorage = {}
---Returns the accounts storage table.
---@return table
	function Game.getAccountsStorage() return accountsStorage end

	function Game.clearAccountStorageValue(accountId, key)
		local accountStorage = accountsStorage[accountId]
		if accountStorage then
			accountStorage[key] = nil
		end
	end
--- Returns the value of the specified key in the account storage.
---@param accountId number
---@param key any
---@return any
	function Game.getAccountStorageValue(accountId, key)
		local accountStorage = accountsStorage[accountId]
		return accountStorage and accountStorage[key] or nil
	end
--- Sets the value of the specified key in the account storage.
---@param accountId number
---@param key any
---@param value any
	function Game.setAccountStorageValue(accountId, key, value)
		if not accountsStorage[accountId] then
			accountsStorage[accountId] = {}
		end
		accountsStorage[accountId][key] = value
	end

--- Saves the current account storage table to the database.
---@return boolean
	function Game.saveAccountsStorage()
		local transaction = DBTransaction()
		if not transaction:begin() then
			return false
		end

		local result = db.query("DELETE FROM `account_storage`")
		if not result then
			return false
		end

		local accountsStorage = Game.getAccountsStorage()
		for accountId, accountStorage in pairs(accountsStorage) do
			local query = DBInsert("INSERT INTO `account_storage` (`account_id`, `key`, `value`) VALUES")
			for key, value in pairs(accountStorage) do
				local success = query:addRow(accountId .. ", " .. key .. ", " .. value)
				if not success then
					return false
				end
			end

			if not query:execute() then
				return false
			end
		end

		return transaction:commit()
	end
end
