do
	VIPs = {}

	local function clear(self)
		VIPs[self.guid] = nil
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

	local function has(self, vip)
		return self.vips[vip] ~= nil
	end

	local function canAdd(self)
		local player = Player(self.guid)
		if not player then
			debugPrint("[Error - VIP::canAdd] null player (" .. self.guid .. ") when check if can add vip.")
			return 0
		end

		return #self.vips < player:getMaxVIPEntries()
	end

	function VIP(guid)
		if not VIPs[guid] then
			VIPs[guid] = {
				guid = guid,
				clear = clear,
				getList = getList,
				add = add,
				remove = remove,
				has = has,
				canAdd = canAdd,
				vips = {}
			}
		end
		return VIPs[guid]
	end
end
