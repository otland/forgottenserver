ItemAbilities = {}

function ItemAbilities.getEnums()
	local enums = {}
	for line in io.lines('data/scripts/ItemAbilities/1_enums.lua') do
		local enum = line:match("([%a_]*) = ")
		if enum then
			enums[#enums+1] = enum
		end
	end
	return enums
end

function ItemAbilities.internalInventoryUpdate(player, item, slot, equip)
	local slotTypes = item:getSlotTypes()
	if table.contains(slotTypes, slot) then
		for _, ability in ipairs(player:getAbilities()) do
			local def = ItemAbilities.getCondition(ability.key, ability.value)
			if def then
				if equip then
					player:addCondition(def.condition)
				else
					player:removeCondition(def.conditionType, CONDITIONID_DEFAULT, def.subId)
				end
			end
		end
	end
end

function ItemAbilities.getCondition(key, value)
	local def = ABILITY_CONDITIONS[key]
	if def then
		-- update condition value before updating player
		def.condition:setParameter(def.key, value)
	end
	return def
end