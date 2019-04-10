function Creature.getClosestFreePosition(self, position, maxRadius, mustBeReachable)
	maxRadius = maxRadius or 1

	-- backward compatability (extended)
	if maxRadius == true then
		maxRadius = 2
	end

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
			if tile:getCreatureCount() == 0 and not tile:hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) and
				(not mustBeReachable or self:getPathTo(checkPosition)) then
				return checkPosition
			end
		end
	end
	return Position()
end

function Creature.getPlayer(self)
	return self:isPlayer() and self or nil
end

function Creature.isContainer(self)
	return false
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

function Creature.isTeleport(self)
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

	if self:isPlayer() and not (self:hasFlag(PlayerFlag_CanIllusionAll) or monsterType:isIllusionable()) then
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

local conditionToCombat = {
	[CONDITION_FIRE] = COMBAT_FIREDAMAGE,
	[CONDITION_ENERGY] = COMBAT_ENERGYDAMAGE,
	[CONDITION_POISON] = COMBAT_EARTHDAMAGE
}

function Creature.addDamageCondition(self, target, type, list, damage, period, rounds, field)
	if not target or target:isImmune(type) or not field and damage <= 0 then
		return false
	end

	if field then
		if damage.step and damage.step <= 0 and damage.periodic and damage.periodic <= 0 then
			return false
		end

		if self then
			if Game.getWorldType() == WORLD_TYPE_NO_PVP or field:getTile():hasFlag(TILESTATE_NOPVPZONE) then
				local master = self:getMaster()
				if self:isPlayer() or master and master:isPlayer() then
					local targetMaster = target:getMaster()
					if target:isPlayer() or targetMaster and targetMaster:isPlayer() then
						return false
					end
				end
			end

			if self:isPlayer() and target:isPlayer() and target:isProtected(self) then
				return false
			end
		end

		if target:isPlayer() and self and self:isPlayer() then
			if damage.step then
				damage.step = math.floor(damage.step / 2 + 0.5)
			end

			if damage.periodic then
				damage.periodic = math.floor(damage.periodic / 2 + 0.5)
			end
		end

		if damage.step then
			doTargetCombatHealth(0, target, conditionToCombat[type], -damage.step, -damage.step)

			if not damage.periodic then
				return true
			end
		end
	end

	local condition = Condition(type)
	if self then
		condition:setParameter(CONDITION_PARAM_OWNER, self:getId())
	end

	condition:setParameter(CONDITION_PARAM_DELAYED, true)
	condition:createDamageList(list, field and damage.periodic or damage, period, rounds)
	target:addCondition(condition)
	return true
end
