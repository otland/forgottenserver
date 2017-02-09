function Creature.getClosestFreePosition(self, position, maxRadius)
	maxRadius = maxRadius or 1

	-- backward compatability (extended)
	if maxRadius == true then
		maxRadius = 2
	end

	for radius = 0, maxRadius do
		for x = -radius, radius do
			for y = -radius, radius do
				if math.abs(x) == radius or math.abs(y) == radius then
					local checkPosition = Position(position.x + x, position.y + y, position.z)
					local tile = Tile(checkPosition)
					if tile:getCreatureCount() == 0 and not tile:hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) and
						(not mustBeReachable or self:getPathTo(checkPosition)) then
						return checkPosition
					end
				end
			end
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
