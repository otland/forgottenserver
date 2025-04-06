do
--- Global Storage Table is a non-persistent table that stores key-value pairs
	local globalStorageTable = {}
--- Gets the value of a global storage key.
---@param key any
---@return any
	function Game.getStorageValue(key)
		return globalStorageTable[key] or -1
	end

--- Sets the value of a global storage key.
---@param key any
---@param value any
	function Game.setStorageValue(key, value)
		globalStorageTable[key] = value
	end
end
