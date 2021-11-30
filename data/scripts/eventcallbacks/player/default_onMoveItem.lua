local function isGold(item)
	local itemId = item:getId()
	return itemId == ITEM_GOLD_COIN or itemId == ITEM_PLATINUM_COIN or itemId == ITEM_CRYSTAL_COIN
end

local ec = EventCallback

ec.onMoveItem = function(self, item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	if item:getAttribute("wrapid") ~= 0 then
		local tile = Tile(toPosition)
		if (fromPosition.x ~= CONTAINER_POSITION and toPosition.x ~= CONTAINER_POSITION) or tile and not tile:getHouse() then
			if tile and not tile:getHouse() then
				self:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
				return false
			end
		end
	end

	if toPosition.x ~= CONTAINER_POSITION then
		return true
	end

	local itemId = item:getId()
	if toCylinder:isItem() then
		local parent = toCylinder:getParent()
		if parent:isItem() and parent:getId() == ITEM_STORE_INBOX then
			if toCylinder:getId() == ITEM_GOLD_POUCH then
				if not isGold(item) then
					self:sendTextMessage(MESSAGE_STATUS_SMALL, "You can move only money to this container.")
					return false
				end
			else
				self:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
				return false
			end
		elseif toCylinder:getId() == ITEM_STORE_INBOX then
			local targetItem = toCylinder:getItem(toPosition.z)
			if targetItem and targetItem:isContainer() then
				if targetItem:getId() == ITEM_GOLD_POUCH then
					if not isGold(item) then
						self:sendTextMessage(MESSAGE_STATUS_SMALL, "You can move only money to this container.")
						return false
					end
				else
					self:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
					return false
				end
			end
		end
	end

	if item:getTopParent() == self and bit.band(toPosition.y, 0x40) == 0 then
		local itemType, moveItem = ItemType(item:getId())
		if bit.band(itemType:getSlotPosition(), SLOTP_TWO_HAND) ~= 0 and toPosition.y == CONST_SLOT_LEFT then
			moveItem = self:getSlotItem(CONST_SLOT_RIGHT)
		elseif itemType:getWeaponType() == WEAPON_SHIELD and toPosition.y == CONST_SLOT_RIGHT then
			moveItem = self:getSlotItem(CONST_SLOT_LEFT)
			if moveItem and bit.band(ItemType(moveItem:getId()):getSlotPosition(), SLOTP_TWO_HAND) == 0 then
				return true
			end
		end

		if moveItem then
			local parent = item:getParent()
			if parent:isContainer() and parent:getSize() == parent:getCapacity() then
				self:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_CONTAINERNOTENOUGHROOM))
				return false
			else
				return moveItem:moveTo(parent)
			end
		end
	end

	return true
end

ec:register()
