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

local slotPositions = {
	[16] = SLOTP_RIGHT, -- 16
	[32] = SLOTP_LEFT, -- 32
	[49] = SLOTP_HEAD, -- 1
	[50] = SLOTP_NECKLACE, -- 2
	[52] = SLOTP_BACKPACK, -- 4
	[56] = SLOTP_ARMOR, -- 8
	[112] = SLOTP_LEGS, -- 64
	[176] = SLOTP_FEET, -- 128
	[304] = SLOTP_RING, -- 256
	[560] = SLOTP_AMMO, -- 512
	[2096] = SLOTP_TWO_HAND, -- 2048
}

function ItemType.usesSlot(self, slot)
	return bit.band(self:getSlotPosition(), slotBits[slot] or 0) ~= 0
end

function ItemType.getRealSlotPosition(self)
	return slotPositions[self:getSlotPosition()]
end
