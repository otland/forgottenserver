
function Player:onLook(thing, position, distance)
	local description = "You see " .. thing:getDescription(distance)
	if self:getGroup():getAccess() then
		if thing:isItem() then
			description = description .. string.format("\nItemID: [%d]", thing:getId())

			local actionId = thing:getActionId()
			if actionId ~= 0 then
				description = description .. string.format(", ActionID: [%d]", actionId)
			end
			
			local uniqueId = thing:getAttribute(ITEM_ATTRIBUTE_UNIQUEID)
			if uniqueId > 0 and uniqueId < 65536 then
				description = description .. string.format(", UniqueId: [%d]", uniqueId)
			end
			
			description = description .. "."
			local itemType = thing:getType()
			
			local transformEquipId = itemType:getTransformEquipId()
			local transformDeEquipId = itemType:getTransformDeEquipId()
			if transformEquipId ~= 0 then
				description = description .. string.format("\nTransformTo: [%d] (onEquip).", transformEquipId)
			elseif transformDeEquipId ~= 0 then
				description = description .. string.format("\nTransformTo: [%d] (onDeEquip).", transformDeEquipId)
			end

			local decayId = itemType:getDecayId()
			if decayId ~= -1 then
				description = description .. string.format("\nDecayTo: [%d]", decayId)
			end
		elseif thing:isCreature() then
			local str = "\nHealth: [%d / %d]"
			if thing:getMaxMana() > 0 then
				str = str .. string.format(", Mana: [%d / %d]", thing:getMana(), thing:getMaxMana())
			end
			description = description .. string.format(str, thing:getHealth(), thing:getMaxHealth()) .. "."
		end
		
		local position = thing:getPosition()
		description = description .. string.format(
			"\nPosition: [X: %d] [Y: %d] [Z: %d].",
			position.x, position.y, position.z
		)
	end
	self:sendTextMessage(MESSAGE_INFO_DESCR, description)
end

function Player:onLookInBattleList(creature, distance)
	local description = "You see " .. creature:getDescription(distance)
	if self:getGroup():getAccess() then
		local str = "\nHealth: [%d / %d]"
		if creature:getMaxMana() > 0 then
			str = str .. string.format(", Mana: [%d / %d]", creature:getMana(), creature:getMaxMana())
		end
		
		local position = creature:getPosition()
		description = description .. string.format(
			str .. ".\nPosition: [X: %d] [Y: %d] [Z: %d].",
			creature:getHealth(), creature:getMaxHealth(),
			position.x, position.y, position.z
		)
	end
	self:sendTextMessage(MESSAGE_INFO_DESCR, description)
end

function Player:onLookInTrade(partner, item, distance)
	self:sendTextMessage(MESSAGE_INFO_DESCR, "You see " .. item:getDescription(distance))
end

function Player:onLookInShop(itemType, count)
	return true
end

function Player:onMoveItem(item, count, fromPosition, toPosition)
	return true
end

function Player:onMoveCreature(creature, fromPosition, toPosition)
	return true
end
