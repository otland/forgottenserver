function Item.setAbility(self, key, value)
	self:setCustomAttribute(key, value)
	local parent = self:getParent()
	local pos = self:getPosition()
	if parent:isPlayer() then
		for slot = 1, 10 do
			local slotItem = parent:getSlotItem(slot)
			if slotItem and self == slotItem then
				-- reload abilities
				ItemAbilities.internalInventoryUpdate(parent, self, slot, false)
				ItemAbilities.internalInventoryUpdate(parent, self, slot, true)
				break
			end
		end
	end
end

function Item.getAbility(self, key)
	return self:getCustomAttribute(key) or 0
end

function Item.getAbilities(self)
	local abilities = {}
	for line in io.lines('data/scripts/ItemAbilities/1_enums.lua') do
		local key = line:match(" = '([%a_]*)'")
		abilities[#abilities + 1] = {key = key, value = self:getAbility(key)}
	end
	return abilities
end

function Player.getAbilities(self)
	local abilities = {}
	for slot = 1, 10 do
		local item = self:getSlotItem(slot)
		if item then
			local slotTypes = item:getSlotTypes()
			if table.contains(slotTypes, slot) then
				for _, ability in ipairs(item:getAbilities()) do
					abilities[ability.key] = (abilities[ability.key] or 0) + ability.value
				end
			end
		end
	end
	local temp = {}
	for key, value in pairs(abilities) do
		temp[#temp+1] = {key = key, value = value}
	end
	return temp
end

do
	local slots = {CONST_SLOT_HEAD, CONST_SLOT_ARMOR, CONST_SLOT_LEGS, CONST_SLOT_FEET, CONST_SLOT_AMMO, CONST_SLOT_RING, CONST_SLOT_NECKLACE}

	function Item.getSlotTypes(self)
		local iType = self:getType()
		for _, slot in pairs(slots) do
			if iType:usesSlot(slot) then
				return {slot}
			end
		end
		if self:getType():getWeaponType() ~= WEAPON_NONE then
			return {CONST_SLOT_LEFT, CONST_SLOT_RIGHT}
		end
		return {}
	end
end