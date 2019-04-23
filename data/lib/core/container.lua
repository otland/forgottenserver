function Container.isContainer(self)
	return true
end

function Container.createLootContainer(self, lootblock)
	if #lootblock == 0 then
		return true
	end
	for i = 1, #lootblock do
		if self:getSize() < self:getCapacity() then
			local loot = lootblock[i]
			local itemList = createLootItem(loot)
			if (#itemList > 0) then
				for i = 1, #itemList do
					local item = itemList[i]
					-- check containers
					if item:isContainer() then
						if not item:createLootContainer(loot.childLoot) then
							print('[Warning] DropLoot:', 'Could not create loot container inside container.')
							item = nil
						end
					end
					if item then
						if self:addItemEx(item) ~= RETURNVALUE_NOERROR then
							print('[Warning] DropLoot:', 'Could not add loot item to container.')
						end
					else
						print('[Warning] DropLoot:', 'Item is nil.')
					end
				end
			end
		end
	end
	return(not (#self:getItems() == 0))
end
