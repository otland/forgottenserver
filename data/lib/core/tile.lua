function Tile.isCreature(self)
	return false
end

function Tile.isItem(self)
	return false
end

function Tile.isTile(self)
	return true
end

function Tile.isContainer(self)
	return false
end

function Tile.relocateTo(self, toPosition)
	if self:getPosition() == toPosition or not Tile(toPosition) then
		return false
	end

	for i = self:getThingCount() - 1, 0, -1 do
		local thing = self:getThing(i)
		if thing then
			if thing:isItem() then
				if thing:getFluidType() ~= 0 then
					thing:remove()
				elseif ItemType(thing:getId()):isMovable() then
					thing:moveTo(toPosition)
				end
			elseif thing:isCreature() then
				thing:teleportTo(toPosition)
			end
		end
	end

	return true
end

function Tile.isWalkable(self)
	local ground = self:getGround()
	if not ground or ground:hasProperty(CONST_PROP_BLOCKSOLID) then
		return false
	end

	local items = self:getItems()
	for i = 1, self:getItemCount() do
		local item = items[i]
		local itemType = item:getType()
		if itemType:getType() ~= ITEM_TYPE_MAGICFIELD and not itemType:isMovable() and item:hasProperty(CONST_PROP_BLOCKSOLID) then
			return false
		end
	end
	return true
end
