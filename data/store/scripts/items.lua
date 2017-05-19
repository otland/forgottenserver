function onRender(player, offer)
	local itemType = ItemType(offer:getName())
	if itemType:getId() == 0 then
		return false, "Item not found. Please contact an administrator."
	end

	return true
end

function onBuy(player, offer)
	local itemType = ItemType(offer:getName())
	local inbox = player:getSlotItem(CONST_SLOT_STORE_INBOX)
	if inbox and inbox:getEmptySlots() >= 1 then
		inbox:addItem(itemType:getId(), 1)
	else
		player:sendStoreError(STORE_ERROR_PURCHASE, "Please make sure you have free slots in your store inbox.")
		return false
	end

	return true
end
