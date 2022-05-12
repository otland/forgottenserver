local ec = EventCallback

ec.onLookInShop = function(self, itemType, count, description)
	local description = "You see " .. itemType:getItemDescription()
	if self:getGroup():getAccess() then
		description = string.format("%s\nItem ID: %d", description, itemType:getId())
		description = string.format("%s\nClient ID: %d", description, itemType:getClientId())

		local transformEquipId = itemType:getTransformEquipId()
		local transformDeEquipId = itemType:getTransformDeEquipId()
		if transformEquipId ~= 0 then
			description = string.format("%s\nTransforms to: %d (onEquip)", description, transformEquipId)
		elseif transformDeEquipId ~= 0 then
			description = string.format("%s\nTransforms to: %d (onDeEquip)", description, transformDeEquipId)
		end

		local decayId = itemType:getDecayId()
		if decayId ~= -1 then
			description = string.format("%s\nDecays to: %d", description, decayId)
		end
	end
	return description
end

ec:register()
