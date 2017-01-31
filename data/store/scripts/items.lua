function onRender(player, offer)
	return true
end

function onBuy(player, offer)
	local itemType = ItemType(offer:getName())
	if itemType:getId() == 0 then
		player:sendStoreError(STORE_ERROR_PURCHASE, "Item not found. Please contact an administrator.")
		return false
	end

	local inbox = player:getSlotItem(CONST_SLOT_STORE_INBOX)
	if inbox and inbox:getEmptySlots() >= 1 then
		inbox:addItem(itemType:getId(), 1)
	else
		player:sendStoreError(STORE_ERROR_PURCHASE, "Please make sure you have free slots in your store inbox.")
		return false
	end

	return true
end
