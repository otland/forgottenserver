---Returns a random position within the given radius of the given position. Verifies that the position does not already have a creature on it, it is reachable, and the tile does not have CONST_PROP_IMMOVABLEBLOCKSOLID property.
---@param self Creature
---@param position Position
---@param maxRadius number
---@param mustBeReachable boolean
---@return Position
function Creature.getClosestFreePosition(self, position, maxRadius, mustBeReachable)
	maxRadius = maxRadius or 1

	-- backward compatability (extended)
	if maxRadius then
		maxRadius = 2
	end

	---@type Position
	local checkPosition = Position(position)
	for radius = 0, maxRadius do
		checkPosition.x = checkPosition.x - math.min(1, radius)
		checkPosition.y = checkPosition.y + math.min(1, radius)

		local total = math.max(1, radius * 8)
		for i = 1, total do
			if radius > 0 then
				local direction = math.floor((i - 1) / (radius * 2))
				checkPosition:getNextPosition(direction)
			end

			local tile = Tile(checkPosition)
			if tile and tile:getCreatureCount() == 0 and not tile:hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) and
				(not mustBeReachable or self:getPathTo(checkPosition)) then
				return checkPosition
			end
		end
	end
	return Position()
end

---Returns the creature if a creature is a player, or nil if not
---@param self Creature
---@return Player|nil
function Creature.getPlayer(self)
	return self:isPlayer() and self or nil
end

---Returns false as creatures cannot be containers
---@param self Creature
---@return boolean
function Creature.isContainer(self)
	return false
end

---Retruns false as creatures cannot be items
---@param self Creature
---@return boolean
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

function Creature.isTeleport(self)
	return false
end

function Creature.isTile(self)
	return false
end

---Sets the outfit of a creature to a specific monster type for a specific amount of time.
---@param monster string
---@param time number
---@return boolean
function Creature:setMonsterOutfit(monster, time)
	local monsterType = MonsterType(monster)
	if not monsterType then
		return false
	end

	---@type Condition
	local condition = Condition(CONDITION_OUTFIT)
	condition:setOutfit(monsterType:getOutfit())
	condition:setTicks(time)
	self:addCondition(condition)

	return true
end

---Sets a creature's outfit to a specific item type, for a specific amount of time.
---@param item number
---@param time number
---@return boolean
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

---Adds a summon to a creature.
---@param monster Monster
---@return boolean
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

	if self:isPlayer() then
		summon:getPosition():notifySummonAppear(summon)
	end

	return true
end

---Removes a summon from a creature.
---@param monster Monster
---@return boolean
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

---Adds a damage condition to a target creature.
---@param target any
---@param type ConditionType
---@param list number
---@param damage number
---@param period number
---@param rounds number
---@return boolean
function Creature:addDamageCondition(target, type, list, damage, period, rounds)
	if damage <= 0 or not target or target:isImmune(type) then
		return false
	end

	---@type Condition
	local condition = Condition(type)
	condition:setParameter(CONDITION_PARAM_OWNER, self:getId())
	condition:setParameter(CONDITION_PARAM_DELAYED, true)

	if list == DAMAGELIST_EXPONENTIAL_DAMAGE then
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
	elseif list == DAMAGELIST_LOGARITHMIC_DAMAGE then
		local n, value = 0, damage
		while value > 0 do
			value = math.floor(damage * math.pow(2.718281828459, -0.05 * n) + 0.5)
			if value ~= 0 then
				condition:addDamage(1, period or 4000, -value)
				n = n + 1
			end
		end
	elseif list == DAMAGELIST_VARYING_PERIOD then
		for _ = 1, rounds do
			condition:addDamage(1, math.random(period[1], period[2]) * 1000, -damage)
		end
	elseif list == DAMAGELIST_CONSTANT_PERIOD then
		condition:addDamage(rounds, period * 1000, -damage)
	end

	target:addCondition(condition)
	return true
end

---Returns true if the creature is a player and can access the PZ (protection zone).
---@return boolean
function Creature:canAccessPz()
	if self:isMonster() or (self:isPlayer() and self:isPzLocked()) then
		return false
	end
	return true
end

---Returns itself if the creature is a monster, or nil if not.
---@param self Creature
---@return Monster|nil
function Creature.getMonster(self)
	return self:isMonster() and self or nil
end

---Returns a table of all creatures that have dealt damage to the given creature. If onlyPlayers is true, it will only return players.
---@param self Creature
---@param onlyPlayers boolean
---@return table<number, Player|Monster>
function Creature.getKillers(self, onlyPlayers)
	local killers = {}
	local inFightTicks = configManager.getNumber(configKeys.PZ_LOCKED)
	local timeNow = os.mtime()
	local getCreature = onlyPlayers and Player or Creature
	for cid, cb in pairs(self:getDamageMap()) do
		local creature = getCreature(cid)
		if creature and creature ~= self and (timeNow - cb.ticks) <= inFightTicks then
			killers[#killers + 1] = {
				creature = creature,
				damage = cb.total
			}
		end
	end

	table.sort(killers, function(a, b) return a.damage > b.damage end)
	for i, killer in pairs(killers) do
		killers[i] = killer.creature
	end
	return killers
end

---Removes a storage value from the creature.
---@param self Creature
---@param key number
---@return boolean
function Creature.removeStorageValue(self, key)
	return self:setStorageValue(key, nil)
end
