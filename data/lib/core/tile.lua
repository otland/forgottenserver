function Tile.isCreature(self)
	return false
end

function Tile.isItem(self)
	return false
end

function Tile.isTile(self)
	return true
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

function Tile.isWalkable(self, check)
	local ground = self:getGround()
	if not ground or ground:hasProperty(CONST_PROP_BLOCKSOLID) or
		check and (check.actionId == true and ground:getActionId() ~= 0 or check.actionId == ground:getActionId() or
		check.uniqueId == true and ground:getUniqueId() ~= 0 or check.uniqueId == ground:getUniqueId()) then
		return false
	end

	if check then
		if check.protection and self:hasFlag(TILESTATE_PROTECTIONZONE) or
			check.floorChange and self:hasFlag(TILESTATE_FLOORCHANGE) or
			check.house and self:getHouse() or
			check.tileCondition and check.tileCondition(self) then
			return false
		elseif check.creatures or check.players or check.monsters or check.npcs then
			local creatures = self:getCreatures()
			for i = 1, self:getCreatureCount() do
				local creature = creatures[i]
				if check.creatures and creature:isCreature() or
					check.players and creature:isPlayer() or
					check.monsters and creature:isMonster() or
					check.npcs and creature:isNpc() then
					return false
				end
			end
		end
	end

	local items = self:getItems()
	for i = 1, self:getItemCount() do
		local item = items[i]
		local itemType = item:getType()
		if item:hasProperty(CONST_PROP_BLOCKSOLID) or
			check and (check.teleport and itemType:getType() == ITEM_TYPE_TELEPORT or
			check.magicField and itemType:getType() == ITEM_TYPE_MAGICFIELD or
			check.actionId == true and item:getActionId() ~= 0 or check.actionId == item:getActionId() or
			check.uniqueId == true and item:getUniqueId() ~= 0 or check.uniqueId == item:getUniqueId() or
			check.itemCondition and check.itemCondition(item)) then
			return false
		end
	end
	return true
end
