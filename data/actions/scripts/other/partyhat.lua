function onUse(cid, item, fromPosition, itemEx, toPosition, isHotkey)
	local player = Player(cid)
	local slot = player:getSlotItem(CONST_SLOT_HEAD)
	if slot and item.uid == slot:getUniqueId() then
		player:getPosition():sendMagicEffect(CONST_ME_GIFT_WRAPS)
		return true
	end

	return false
end
