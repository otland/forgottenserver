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
		if ItemType(item.itemId):isStackable() then
			itemCount = randvalue % item.maxCount + 1
		else
			itemCount = 1
		end
	end

	if itemCount > 0 then
		local tmpItem = {}
		while itemCount > 100 do
			local itm = self:addItem(item.itemId, 100)
			if itm then
				tmpItem[#tmpItem + 1] = itm
			end
			itemCount = itemCount - 100
		end

		if itemCount > 0 then
			local itm = self:addItem(item.itemId, itemCount)
			if itm then
				tmpItem[#tmpItem + 1] = itm
			end
		end

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
