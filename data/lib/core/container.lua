function Container.isContainer(self)
	return true
end

---Creates a loot item in the container.
---@param self Container
---@param item MonsterLootBlock
---@return boolean
function Container.createLootItem(self, item)
	if self:getEmptySlots() == 0 then
		return true
	end

	local itemCount = 0
	local randvalue = getLootRandom()
	local itemType = ItemType(item.itemId)

	if randvalue < item.chance then
		if itemType:isStackable() then
			itemCount = randvalue % item.maxCount + 1
		else
			itemCount = 1
		end
	end

	while itemCount > 0 do
		local count = math.min(ITEM_STACK_SIZE, itemCount)

		local subType = count
		if itemType:isFluidContainer() then
			subType = math.max(0, item.subType)
		end

		local tmpItem = Game.createItem(item.itemId, subType)
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

			if #item.childLoot > 0 and tmpItem:getSize() == 0 then
				tmpItem:remove()
				return true
			end
		end

		if item.subType ~= -1 then
			tmpItem:setAttribute(ITEM_ATTRIBUTE_CHARGES, item.subType)
		end

		if item.actionId ~= -1 then
			tmpItem:setActionId(item.actionId)
		end

		if item.text and item.text ~= "" then
			tmpItem:setText(item.text)
		end

		local ret = self:addItemEx(tmpItem)
		if ret ~= RETURNVALUE_NOERROR then
			tmpItem:remove()
		end

		itemCount = itemCount - count
	end
	return true
end

---Returns a string with all the loot that was dropped.
---@return table<number, string>|string
function Container:getContentDescription()
	local items = self:getItems()
	if items and #items > 0 then
		local loot = {}
		for i = 1, #items do
			loot[#loot + 1] = string.format("{%d|%s}", items[i]:getType():getClientId(), items[i]:getNameDescription(items[i]:getSubType(), true))
		end

		return table.concat(loot, ", ")
	end

	return "nothing"
end
