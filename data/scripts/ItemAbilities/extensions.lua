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

function Item.getRawAbilities(self)
	local abilities = {}
	for line in io.lines('data/scripts/ItemAbilities/1_enums.lua') do
		local key = line:match(" = '([%a_]*)'")
		abilities[#abilities + 1] = {key = key, value = self:getAbility(key)}
	end
	return abilities
end

function Item.getAbilities(self)
	local raw = self:getRawAbilities()
	local default = self:getType():getAbilities()
	local abilities = {}
	-- remap for easy indexing
	for _, ability in ipairs(raw) do
		abilities[ability.key] = ability.value
	end
	local function fallback(x)
		return x == 0 and 'default' or x
	end 
	local ret = {
		conditionImmunities = default.conditionImmunities,
		conditionSuppressions = default.conditionSuppressions,
		elementDamage = default.elementDamage,
		elementType = default.elementType,
		regeneration = default.regeneration,
		healthGain = fallback(abilities.IA_HEALTHGAIN),
		healthTicks = fallback(abilities.IA_HEALTHTICKS),
		manaGain = fallback(abilities.IA_MANAGAIN),
		manaTicks = fallback(abilities.IA_MANATICKS),
		speed = fallback(abilities.IA_SPEED),
		dodge = fallback(abilities.IA_DODGE),
		skills = {
			[1] = fallback(abilities.IA_SKILLFIST),
			[2] = fallback(abilities.IA_SKILLCLUB),
			[3] = fallback(abilities.IA_SKILLSWORD),
			[4] = fallback(abilities.IA_SKILLAXE),
			[5] = fallback(abilities.IA_SKILLDISTANCE),
			[6] = fallback(abilities.IA_SKILLSHIELD),
			[7] = fallback(abilities.IA_SKILLFISHING),
		},
		stats = {
			[1] = fallback(abilities.IA_MAXHITPOINTS),
			[2] = fallback(abilities.IA_MAXMANAPOINTS),
			[3] = 'default',
			[4] = fallback(abilities.IA_MAGICPOINTS)
		},
		statsPercent = {
			[1] = fallback(abilities.IA_MAXHITPOINTSPERCENT),
			[2] = fallback(abilities.IA_MAXMANAPOINTSPERCENT),
			[3] = 'default',
			[4] = fallback(abilities.IA_MAGICPOINTSPERCENT)
		},
		absorbPercent = {
			[1] = fallback(abilities.IA_ABSORBPHYSICAL),
			[2] = fallback(abilities.IA_ABSORBENERGY),
			[3] = fallback(abilities.IA_ABSORBEARTH),
			[4] = fallback(abilities.IA_ABSORBFIRE),
			[5] = 'default',
			[6] = 'default',
			[7] = 'default',
			[8] = 'default',
			[9] = fallback(abilities.IA_ABSORBWATER),
			[10] = fallback(abilities.IA_ABSORBICE),
			[11] = fallback(abilities.IA_ABSORBHOLY),
			[12] = fallback(abilities.IA_ABSORBDEATH),
		},
		specialSkills = {
			[1] = fallback(abilities.IA_CRITICALHITCHANCE),
			[2] = fallback(abilities.IA_CRITICALHITDAMAGE),
			[3] = fallback(abilities.IA_LIFELEECHCHANCE),
			[4] = fallback(abilities.IA_LIEFLEECHAMOUNT),
			[5] = fallback(abilities.IA_MANALEECHCHANCE),
			[6] = fallback(abilities.IA_MANALEECHAMOUNT)
		},
		fieldAbsorbPercent = {}
	}
	-- use default ability value from itemType if dynamic one is 0 
	for k, v in pairs(ret) do
		if v == 'default' then
			ret[k] = default[k] or 0
		elseif type(v) == 'table' then
			for _k, _v in pairs(v) do
				if _v == 'default' then
					ret[k][_k] = default[k][_k] or 0
				end
			end
		end
	end
	for i = 1, 12 do
		ret.fieldAbsorbPercent[i] = default.fieldAbsorbPercent[i]
	end
	return ret
end

function Player.getAbilities(self, keymap)
	local abilities = {}
	for slot = 1, 10 do
		local item = self:getSlotItem(slot)
		if item then
			local slotTypes = item:getSlotTypes()
			if table.contains(slotTypes, slot) then
				for _, ability in ipairs(item:getRawAbilities()) do
					abilities[ability.key] = (abilities[ability.key] or 0) + ability.value
				end
			end
		end
	end
	local temp = {}
	for key, value in pairs(abilities) do
		if keymap then
			temp[key] = value
		else
			temp[#temp+1] = {key = key, value = value}
		end
	end
	return temp
end

function Player.getAbilityValue(self, ability)
	return self:getAbilities(true)[ability] or 0
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