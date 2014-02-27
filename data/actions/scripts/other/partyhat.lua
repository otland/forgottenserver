function onUse(cid, item, fromPosition, itemEx, toPosition)
	local player = Player(cid)

	if item.uid == player:getSlotItem(CONST_SLOT_HEAD):getUniqueId() then
		player:getPosition():sendMagicEffect(CONST_ME_GIFT_WRAPS)
		return true
	end

	return false
end
