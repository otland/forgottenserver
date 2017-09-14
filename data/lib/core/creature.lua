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

function Creature:addSummon(monster)
	local summon = Monster(monster)
	if not summon then
		return false
	end

	summon:setTarget(nil)
	summon:setFollowCreature(nil)
	summon:setDropLoot(false)
	summon:setSkillLoss(false)
	summon:setMaster(self)

	return true
end

function Creature:removeSummon(monster)
	local summon = Monster(monster)
	if not summon or summon:getMaster() ~= self then
		return false
	end

	summon:setTarget(nil)
	summon:setFollowCreature(nil)
	summon:setDropLoot(true)
	summon:setSkillLoss(true)
	summon:setMaster(nil)

	return true
end

function Creature:addDamageCondition(target, conditionType, listType, damage, period, rounds)
	if damage <= 0 or not target or target:isImmune(conditionType) then
		return false
	end

	local condition = Condition(conditionType)
	condition:setParameter(CONDITION_PARAM_OWNER, self:getId())
	condition:setParameter(CONDITION_PARAM_DELAYED, true)

	if listType == 0 then
		local exponent, value = -10, 0
		while value < damage do
			value = math.floor(10 * math.pow(1.2, exponent) + 0.5)
			condition:addDamage(1, period or 4000, -value)

			if value >= damage then
				local permille = math.random(10, 1200) / 1000
				condition:addDamage(1, period or 4000, -math.max(1, math.floor(value * permille + 0.5)))
			else
				exponent = exponent + 1
			end
		end
	elseif listType == 1 then
		local n, value = 0, damage
		while value > 0 do
			value = math.floor(damage * math.pow(2.718281828459, -0.05 * n) + 0.5)
			if value ~= 0 then
				condition:addDamage(1, period or 4000, -value)
				n = n + 1
			end
		end
	elseif listType == 2 then
		for _ = 1, rounds do
			condition:addDamage(1, math.random(period[1], period[2]) * 1000, -damage)
		end
	elseif listType == 3 then
		condition:addDamage(rounds, period * 1000, -damage)
	end

	target:addCondition(condition)
	return true
end
