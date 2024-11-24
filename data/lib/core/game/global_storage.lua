do
	local globalStorageTable = {}

	function Game.getStorageValue(key)
		return globalStorageTable[key] or -1
	end

	function Game.setStorageValue(key, value)
		globalStorageTable[key] = value
	end
end
