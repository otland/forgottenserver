do
	VIPList = {}

	local function clear(self)
		VIPList[self.guid] = nil
	end

	local function getList(self)
		return self.vips
	end

	local function add(self, vip)
		self.vips[vip] = vip
	end

	local function remove(self, vip)
		self.vips[vip] = nil
	end

	local function hasVIP(self, vip)
		return self.vips[vip] ~= nil
	end

	local function canAdd(self)
		local player = Player(self.guid)
		if not player then
			debugPrint("[Error - Player::canAdd] null player (" .. self.guid .. ") when check if can add vip.")
			return 0
		end

		return #self.vips < player:getMaxVIPEntries()
	end

	function VIP(guid)
		if not VIPList[guid] then
			VIPList[guid] = {
				guid = guid,
				clear = clear,
				getList = getList,
				add = add,
				remove = remove,
				hasVIP = hasVIP,
				canAdd = canAdd,
				vips = {}
			}
		end
		return VIPList[guid]
	end
end
