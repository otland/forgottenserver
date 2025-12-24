local event = Event()

event.onPlayerLookInTrade = function(self, partner, item, distance)
	local description = "You see " .. item:getDescription(distance)
	if self:getGroup():getAccess() then
		description = string.format("%s\nItem ID: %d", description, item:getId())

		local actionId = item:getActionId()
		if actionId ~= 0 then
			description = string.format("%s, Action ID: %d", description, actionId)
		end

		local uniqueId = item:getAttribute(ITEM_ATTRIBUTE_UNIQUEID)
		if uniqueId > 0 and uniqueId < 65536 then
			description = string.format("%s, Unique ID: %d", description, uniqueId)
		end

		local itemType = item:getType()

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
	self:sendTextMessage(MESSAGE_INFO_DESCR, description)
end

event:register()
