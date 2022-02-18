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

function ItemType.usesSlot(self, slot)
	return bit.band(self:getSlotPosition(), slotBits[slot] or 0) ~= 0
end

function ItemType:isHelmet()
	return self:usesSlot(CONST_SLOT_HEAD)
end

function ItemType:isArmor()
	return self:usesSlot(CONST_SLOT_ARMOR)
end

function ItemType:isLegs()
	return self:usesSlot(CONST_SLOT_LEGS)
end

function ItemType:isBoots()
	return self:usesSlot(CONST_SLOT_FEET)
end

local notWeapons = {WEAPON_NONE, WEAPON_SHIELD, WEAPON_AMMO}
function ItemType:isWeapon()
	return not table.contains(notWeapons, self:getWeaponType())
end

function ItemType:isTwoHanded()
	return bit.band(self:getSlotPosition(), SLOTP_TWO_HAND) ~= 0
end

function ItemType:isBow()
	local ammoType = self:getAmmoType()
	return self:getWeaponType() == WEAPON_DISTANCE and (ammoType == AMMO_ARROW or ammoType == AMMO_BOLT)
end

function ItemType:isMissile()
	local ammoType = self:getAmmoType()
	return self:getWeaponType() == WEAPON_DISTANCE and ammoType ~= AMMO_ARROW and ammoType ~= AMMO_BOLT
end

function ItemType:isWand()
	return self:getWeaponType() == WEAPON_WAND
end

function ItemType:isShield()
	return self:getWeaponType() == WEAPON_SHIELD
end

function ItemType:isBackpack()
	return self:usesSlot(CONST_SLOT_BACKPACK)
end

function ItemType:isNecklace()
	return self:usesSlot(CONST_SLOT_NECKLACE)
end

function ItemType:isRing()
	return self:usesSlot(CONST_SLOT_RING)
end

function ItemType:isAmmo()
	return self:getWeaponType() == WEAPON_AMMO
end

function ItemType:isTrinket()
	return self:usesSlot(CONST_SLOT_AMMO) and self:getWeaponType() == WEAPON_NONE
end

function ItemType:isKey()
	return self:getType() == ITEM_TYPE_KEY
end

function ItemType:isBed()
	return self:getType() == ITEM_TYPE_BED
end
