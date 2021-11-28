function NetworkMessage:addOutfit(outfit, addMount)
	self:addU16(outfit.lookType)
	if outfit.lookType == 0 then
		local itemType = ItemType(outfit.lookTypeEx)
		self:addU16(itemType and itemType:getClientId() or 0)
	else
		self:addByte(outfit.lookHead)
		self:addByte(outfit.lookBody)
		self:addByte(outfit.lookLegs)
		self:addByte(outfit.lookFeet)
		self:addByte(outfit.lookAddons)
	end

	if addMount then
		self:addU16(outfit.lookMount)
		if outfit.lookMount ~= 0 then
			self:addByte(outfit.lookMountHead or 0)
			self:addByte(outfit.lookMountBody or 0)
			self:addByte(outfit.lookMountLegs or 0)
			self:addByte(outfit.lookMountFeet or 0)
		end
	end
end

function NetworkMessage:addItemType(itemType)
	if tonumber(item) then
		itemType = ItemType(item)
	end
	
	if not itemType then
		return false
	end
	
	self:addU16(itemType:getClientId())
	if itemType:isStackable() then
		self:addByte(1) -- count
	elseif itemType:isSplash() or itemType:isFluidContainer() then
		self:addByte(0) -- splash color (fluid client id)
	elseif itemType:isContainer() then
		self:addByte(0x00) -- has loot container icon (bool), requires u32 id? if true
		self:addByte(0x00) -- is quiver (bool), requires u32 ammo count if true
	elseif itemType:isPodium() then
		self:addU16(0) -- lookType
		self:addU16(0) -- lookMount
		self:addByte(0) -- direction
		self:addByte(0x01) -- isVisible
	end
	
	return true
end
