function Container.isContainer(self)
	return true
end

function Container.createLootItem(self, item)
	if self:getEmptySlots() == 0 then
		return true
	end

	local itemCount = 0
	local maxCount = math.min(item.maxCount, 100)
	if ItemType(item.itemId):isStackable() then
		local stacks = math.ceil(item.maxCount / 100)
		for i = 1, stacks do
			local randvalue = getLootRandom()
			if randvalue < item.chance then
				itemCount = itemCount + (randvalue % maxCount + 1)
				item.maxCount = item.maxCount - maxCount
				maxCount = math.min(item.maxCount, 100)
			end
		end
	elseif getLootRandom(player, bonus) < item.chance then
		itemCount = 1
	end

	repeat
		local addCount = math.min(itemCount, 100)
		local itm = self:addItem(item.itemId, addCount)
		if itm then
			if itm:isContainer() then
				for i = 1, #item.childLoot do
					if not itm:createLootItem(item.childLoot[i]) then
						itm:remove()
						return false
					end
				end
			end

			if item.subType ~= -1 then
				itm:setAttribute(ITEM_ATTRIBUTE_CHARGES, item.subType)
			end

			if item.actionId ~= -1 then
				itm:setActionId(item.actionId)
			end

			if item.text and item.text ~= "" then
				itm:setAttribute(ITEM_ATTRIBUTE_TEXT, item.text)
			end
		end
		itemCount = itemCount - addCount
	until itemCount < 1
	return true
end
