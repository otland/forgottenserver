function Creature.getClosestFreePosition(self, position, extended)
	local usePosition = Position(position)
	local tiles = { Tile(usePosition) }
	local length = extended and 2 or 1

	local tile
	for y = -length, length do
		for x = -length, length do
			if x ~= 0 or y ~= 0 then
				usePosition.x = position.x + x
				usePosition.y = position.y + y

				tile = Tile(usePosition)
				if tile then
					tiles[#tiles + 1] = tile
				end
			end
		end
	end

	for i = 1, #tiles do
		tile = tiles[i]
		if tile:getCreatureCount() == 0 and not tile:hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) then
			return tile:getPosition()
		end
	end
	return Position()
end

function Creature.getPlayer(self)
	return self:isPlayer() and self or nil
end

function Creature.isItem(self)
	return false
end

function Creature.isMonster(self)
	return false
end

function Creature.isNpc(self)
	return false
end

function Creature.isPlayer(self)
	return false
end

function Creature.isTile(self)
	return false
end

function Creature:setMonsterOutfit(monster, time)
	local monsterType = MonsterType(monster)
	if not monsterType then
		return false
	end

	if self:isPlayer() and not (getPlayerFlagValue(self, PlayerFlag_CanIllusionAll) or monsterType:isIllusionable()) then
		return false
	end

	local condition = Condition(CONDITION_OUTFIT)
	condition:setOutfit(monsterType:getOutfit())
	condition:setTicks(time)
	self:addCondition(condition)

	return true
end

function Creature:setItemOutfit(item, time)
	local itemType = ItemType(item)
	if not itemType then
		return false
	end

	local condition = Condition(CONDITION_OUTFIT)
	condition:setOutfit({
		lookTypeEx = itemType:getId()
	})
	condition:setTicks(time)
	self:addCondition(condition)

	return true
end
