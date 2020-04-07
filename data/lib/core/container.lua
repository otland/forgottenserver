function Container.isContainer(self)
	return true
end

function Container.createLootItem(self, item)
	if self:getEmptySlots() == 0 then
		return true
	end

	local itemCount = 0
	local randvalue = getLootRandom()
	if randvalue < item.chance then
		local maxCount = math.min(item.maxCount, 100)
		local isStackable = ItemType(item.itemId):isStackable()
		for i = 1, math.ceil(item.maxCount / 100) do
			if isStackable then
				itemCount = itemCount + (randvalue % maxCount + 1)
				item.maxCount = item.maxCount - maxCount
				maxCount = math.min(item.maxCount, 100)
			else
				itemCount = itemCount + 1
			end
		end
	end

	if itemCount > 0 then
		local tmpItem = {}
		repeat
			local addCount = math.min(itemCount, 100)
			local itm = self:addItem(item.itemId, addCount)
			if itm then
				tmpItem[#tmpItem + 1] = itm
			end
			itemCount = itemCount - addCount
		until itemCount < 1
		
		if #tmpItem == 0 then
			return false
		end

		for index, tmp in pairs(tmpItem) do
			if tmp:isContainer() then
				for i = 1, #item.childLoot do
					if not tmp:createLootItem(item.childLoot[i]) then
						tmp:remove()
						return false
					end
				end
			end

			if item.subType ~= -1 then
				tmp:setAttribute(ITEM_ATTRIBUTE_CHARGES, item.subType)
			end

			if item.actionId ~= -1 then
				tmp:setActionId(item.actionId)
			end

			if item.text and item.text ~= "" then
				tmp:setText(item.text)
			end
		end
	end
	return true
end
