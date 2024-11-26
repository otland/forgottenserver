do
	-- VIP limits
	VIP_FREE_LIMIT = 20
	VIP_PREMIUM_LIMIT = 100

	Vips = {}

	local function clear(self)
		Vips[self.guid] = nil
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
			debugPrint("[Error - Vip::canAdd] null player (" .. self.guid .. ") when check if can add Vip.")
			return 0
		end

		return #self.vips < player:getMaxVipEntries()
	end

	function Vip(guid)
		if not Vips[guid] then
			Vips[guid] = {
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
		return Vips[guid]
	end
end
