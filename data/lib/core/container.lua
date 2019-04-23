function Container.isContainer(self)
	return true
end

function Container.createLootItem(self, item)
	local itemCount = 0
	local randvalue = getLootRandom()
	if (randvalue < item.chance) then
		if (ItemType(item):isStackable()) then
			itemCount = randvalue % item.countmax + 1
		else 
			itemCount = 1
		end
	end
	
	while (itemCount > 0) do
		local n = math.min(itemCount, 100);
		local tmpItem = self:addItem(item.itemId, n)
		if not tmpItem then
			return false
		end

		if tmpItem:isContainer() then
			for i = 1, #item.childLoot do
				if not tmpItem:createLootItem(item.childLoot[i]) then
					tmpItem:remove()
					return false
				end
			end
		end

		itemCount = itemCount - n
		if (item.subType ~= -1) then
			tmpItem:setAttribute(ITEM_ATTRIBUTE_CHARGES, item.subType)
		end

		if (item.actionId ~= -1) then
			tmpItem:setActionId(item.actionId)
		end

		if (item.text and item.text ~= "") then
			tmpItem:setText(item.text)
		end
	end
	return true
end