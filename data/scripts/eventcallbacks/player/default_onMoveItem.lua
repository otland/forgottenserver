local ec = EventCallback

ec.onMoveItem = function(self, item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	if item:getAttribute("wrapid") ~= 0 then
		local tile = Tile(toPosition)
		if (fromPosition.x ~= CONTAINER_POSITION and toPosition.x ~= CONTAINER_POSITION) or tile and not tile:getHouse() then
			if tile and not tile:getHouse() then
				return RETURNVALUE_NOTPOSSIBLE
			end
		end
	end

	if toPosition.x ~= CONTAINER_POSITION then
		return RETURNVALUE_NOERROR
	end

	if bit.band(toPosition.y, 0x40) == 0 then
		local itemType, moveItem = ItemType(item:getId())
		if bit.band(itemType:getSlotPosition(), SLOTP_TWO_HAND) ~= 0 and toPosition.y == CONST_SLOT_LEFT then
			local rightItem = self:getSlotItem(CONST_SLOT_RIGHT)
			if rightItem and not(itemType:isBow() and rightItem:getType():getWeaponType() == WEAPON_QUIVER) then
				moveItem = self:getSlotItem(CONST_SLOT_RIGHT)
			end
		elseif itemType:getWeaponType() == WEAPON_SHIELD and toPosition.y == CONST_SLOT_RIGHT then
			moveItem = self:getSlotItem(CONST_SLOT_LEFT)
			if moveItem and bit.band(ItemType(moveItem:getId()):getSlotPosition(), SLOTP_TWO_HAND) == 0 then
				return RETURNVALUE_NOERROR
			end
		end

		if moveItem then
			local parent = item:getParent()
			local topParent = item:getTopParent()
			if parent:isContainer() and parent:getSize() == parent:getCapacity() then
				return RETURNVALUE_CONTAINERNOTENOUGHROOM
			end
			if Player(topParent) then
				return moveItem:moveTo(parent) and RETURNVALUE_NOERROR or RETURNVALUE_NOTPOSSIBLE
			else
				return RETURNVALUE_BOTHHANDSNEEDTOBEFREE
			end
		end
	end

	return RETURNVALUE_NOERROR
end

ec:register()
