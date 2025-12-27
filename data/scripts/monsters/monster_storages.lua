if not MonsterStorages then
	MonsterStorages = setmetatable({}, {
		__index = function(storage, cid)
			local self = Monster(cid)
			if not self then
				return
			end

			storage[cid] = {}
			return storage[cid]
		end
	})
end

function Monster:getStorageValue(key)
	return MonsterStorages[self:getId()][key] or -1
end

function Monster:setStorageValue(key, value)
	MonsterStorages[self:getId()][key] = value
	return true
end

local event = Event()

event.onCreatureDeath = function(self, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if self:isMonster() then
		MonsterStorages[self:getId()] = nil
	end
end

event:register()
