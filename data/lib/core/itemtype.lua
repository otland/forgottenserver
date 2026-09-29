---@return boolean
function ItemType:isItemType()
	return true
end

do
	local slotBits = {
		[CONST_SLOT_HEAD] = SLOTP_HEAD,
		[CONST_SLOT_NECKLACE] = SLOTP_NECKLACE,
		[CONST_SLOT_BACKPACK] = SLOTP_BACKPACK,
		[CONST_SLOT_ARMOR] = SLOTP_ARMOR,
		[CONST_SLOT_RIGHT] = SLOTP_RIGHT,
		[CONST_SLOT_LEFT] = SLOTP_LEFT,
		[CONST_SLOT_LEGS] = SLOTP_LEGS,
		[CONST_SLOT_FEET] = SLOTP_FEET,
		[CONST_SLOT_RING] = SLOTP_RING,
		[CONST_SLOT_AMMO] = SLOTP_AMMO
	}

	---Verifies if an item type uses a specific slot.
	---@param slot PlayerEquipmentSlots
	---@return boolean
	function ItemType:usesSlot(slot)
		return bit.band(self:getSlotPosition(), slotBits[slot] or 0) ~= 0
	end
end

---Checks if an item type goes into the head slot.
---@return boolean
function ItemType:isHelmet()
	return self:usesSlot(CONST_SLOT_HEAD)
end

---Checks if an item type goes into the armor slot.
---@return boolean
function ItemType:isArmor()
	return self:usesSlot(CONST_SLOT_ARMOR)
end

---checks if an item type goes into the legs slot.
---@return boolean
function ItemType:isLegs()
	return self:usesSlot(CONST_SLOT_LEGS)
end

---Checks if an item type goes into the feet slot.
---@return boolean
function ItemType:isBoots()
	return self:usesSlot(CONST_SLOT_FEET)
end

local notWeapons = {WEAPON_NONE, WEAPON_SHIELD, WEAPON_AMMO}
---Checks if an item type is a weapon.
---@return boolean
function ItemType:isWeapon()
	return not table.contains(notWeapons, self:getWeaponType())
end

---Checks if an item uses both hands
---@return boolean
function ItemType:isTwoHanded()
	return bit.band(self:getSlotPosition(), SLOTP_TWO_HAND) ~= 0
end

---Checks if an item type is a bow
---@return boolean
function ItemType:isBow()
	local ammoType = self:getAmmoType()
	return self:getWeaponType() == WEAPON_DISTANCE and (ammoType == AMMO_ARROW or ammoType == AMMO_BOLT)
end

---Checks if an item type is a missile (throwing star, spear, etc.)
---@return boolean
function ItemType:isMissile()
	local ammoType = self:getAmmoType()
	return self:getWeaponType() == WEAPON_DISTANCE and ammoType ~= AMMO_ARROW and ammoType ~= AMMO_BOLT
end

---Checks if an item type is a quiver
---@return boolean
function ItemType:isQuiver()
	return self:getWeaponType() == WEAPON_QUIVER
end

---Checks if an item type is a wand
---@return boolean
function ItemType:isWand()
	return self:getWeaponType() == WEAPON_WAND
end

---Checks if an item type is a shield
---@return boolean
function ItemType:isShield()
	return self:getWeaponType() == WEAPON_SHIELD
end

---Checks if an item type is a backpack
---@return boolean
function ItemType:isBackpack()
	return self:usesSlot(CONST_SLOT_BACKPACK)
end

---Checks if an item type is a necklace
---@return boolean
function ItemType:isNecklace()
	return self:usesSlot(CONST_SLOT_NECKLACE)
end

---Checks if an item type is a ring
---@return boolean
function ItemType:isRing()
	return self:usesSlot(CONST_SLOT_RING)
end

---Checks if an item type is ammunition
---@return boolean
function ItemType:isAmmo()
	return self:getWeaponType() == WEAPON_AMMO
end

---Checks if an item type is a trinket
---@return boolean
function ItemType:isTrinket()
	return self:usesSlot(CONST_SLOT_AMMO) and self:getWeaponType() == WEAPON_NONE
end

---Checks if an item type is a key
---@return boolean
function ItemType:isKey()
	return self:getType() == ITEM_TYPE_KEY
end

---Checks if an item type is a bed
---@return boolean
function ItemType:isBed()
	return self:getType() == ITEM_TYPE_BED
end

---Checks if the item type is a splash
---@return boolean
function ItemType:isSplash()
	return self:getGroup() == ITEM_GROUP_SPLASH
end

---Checks if the item type is a podium
---@return boolean
function ItemType:isPodium()
	return self:getGroup() == ITEM_GROUP_PODIUM
end

---Returns the weapon string of the item type.
---@return string
function ItemType:getWeaponString()
	local weaponType = self:getWeaponType()
	local weaponString = "unknown"

	if weaponType == WEAPON_CLUB then
		weaponString = "blunt instrument"
	elseif weaponType == WEAPON_SWORD then
		weaponString = "stabbing weapon"
	elseif weaponType == WEAPON_AXE then
		weaponString = "cutting weapon"
	elseif weaponType == WEAPON_DISTANCE then
		weaponString = self:isBow() and "firearm" or "missile"
	elseif weaponType == WEAPON_WAND then
		weaponString = "wand/rod"
	elseif weaponType == WEAPON_QUIVER then
		weaponString = "quiver"
	end

	if self:isTwoHanded() then
		weaponString = string.format("%s, two-handed", weaponString)
	end

	return weaponString
end
