function Container.isContainer(self)
	return true
end

gemAmount = 0

function Container.getGems()
	local gemStorage = gemAmount
	
	return gemStorage
end

function Container.resetGems()
	gemAmount = 0
end

function Container.createLootItem(self, item)
	if self:getEmptySlots() == 0 then
		
		return true
	end

	local gemid = 18419
	local itemCount = 0
	local randvalue = getLootRandom()

	if item.itemId == gemid then
		gemChance = item.chance
		gemMaxCount = item.maxCount
		gemCount = math.random(0, gemMaxCount)

		if randvalue < gemChance then
			gemAmount = gemCount
		end
	end
		
	if randvalue < item.chance then
		if ItemType(item.itemId):isStackable() then
			itemCount = randvalue % item.maxCount + 1
		else
			itemCount = 1
		end
	end

	if itemCount > 0 then
		if item.itemId ~= gemid then
			local tmpItem = Game.createItem(item.itemId, math.min(itemCount, 100))
		if not tmpItem then
			return false
		end

		if tmpItem:isContainer() then
			for i = 1, #item.childLoot do
				print(item.childLoot)
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
	end
	return true
end
end

