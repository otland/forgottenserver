local foodCondition = Condition(CONDITION_REGENERATION, CONDITIONID_DEFAULT)

function Player.feed(self, food)
	local condition = self:getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT)
	if condition then
		condition:setTicks(condition:getTicks() + (food * 1000))
	else
		local vocation = self:getVocation()
		if not vocation then
			return nil
		end

		foodCondition:setTicks(food * 1000)
		foodCondition:setParameter(CONDITION_PARAM_HEALTHGAIN, vocation:getHealthGainAmount())
		foodCondition:setParameter(CONDITION_PARAM_HEALTHTICKS, vocation:getHealthGainTicks() * 1000)
		foodCondition:setParameter(CONDITION_PARAM_MANAGAIN, vocation:getManaGainAmount())
		foodCondition:setParameter(CONDITION_PARAM_MANATICKS, vocation:getManaGainTicks() * 1000)

		self:addCondition(foodCondition)
	end
	return true
end

function Player.getClosestFreePosition(self, position, extended)
	if self:getGroup():getAccess() and self:getAccountType() >= ACCOUNT_TYPE_GOD then
		return position
	end
	return Creature.getClosestFreePosition(self, position, extended)
end

function Player.getDepotItems(self, depotId)
	return self:getDepotChest(depotId, true):getItemHoldingCount()
end

function Player.hasFlag(self, flag)
	return self:getGroup():hasFlag(flag)
end

function Player.getLossPercent(self)
	local blessings = 0
	local lossPercent = {
		[0] = 100,
		[1] = 70,
		[2] = 45,
		[3] = 25,
		[4] = 10,
		[5] = 0
	}

	for i = 1, 5 do
		if self:hasBlessing(i) then
			blessings = blessings + 1
		end
	end
	return lossPercent[blessings]
end

function Player.isPremium(self)
	return self:getPremiumDays() > 0 or configManager.getBoolean(configKeys.FREE_PREMIUM)
end

function Player.sendCancelMessage(self, message)
	if type(message) == "number" then
		message = Game.getReturnMessage(message)
	end
	return self:sendTextMessage(MESSAGE_STATUS_SMALL, message)
end

function Player.isUsingOtClient(self)
	return self:getClient().os >= CLIENTOS_OTCLIENT_LINUX
end

function Player.sendExtendedOpcode(self, opcode, buffer)
	if not self:isUsingOtClient() then
		return false
	end

	local networkMessage = NetworkMessage()
	networkMessage:addByte(0x32)
	networkMessage:addByte(opcode)
	networkMessage:addString(buffer)
	networkMessage:sendToPlayer(self)
	networkMessage:delete()
	return true
end

APPLY_SKILL_MULTIPLIER = true
local addSkillTriesFunc = Player.addSkillTries
function Player.addSkillTries(...)
	APPLY_SKILL_MULTIPLIER = false
	local ret = addSkillTriesFunc(...)
	APPLY_SKILL_MULTIPLIER = true
	return ret
end

local addManaSpentFunc = Player.addManaSpent
function Player.addManaSpent(...)
	APPLY_SKILL_MULTIPLIER = false
	local ret = addManaSpentFunc(...)
	APPLY_SKILL_MULTIPLIER = true
	return ret
end

function Player:getWeaponItemType(...)
	for slot = CONST_SLOT_LEFT, configManager.getBoolean(configKeys.CLASSIC_EQUIPMENT_SLOTS) and CONST_SLOT_RIGHT or CONST_SLOT_LEFT, -1 do
		local item = self:getSlotItem(slot)
		if item then
			local itemType = item:getType()
			for _, weaponType in ipairs({...}) do
				if weaponType == itemType:getWeaponType() then
					return itemType
				end
			end
		end
	end
end

function Player:getHighestSkillLevel(...)
	local highestSkillLevel = 0
	for _, skillType in ipairs({...}) do
		local currentSkillLevel = self:getEffectiveSkillLevel(skillType)
		if highestSkillLevel < currentSkillLevel then
			highestSkillLevel = currentSkillLevel
		end
	end
	return highestSkillLevel
end

function Player:getFormulaValues(a, b, c, d, e, f, formula, combat, variant, g, h)
	local skill
	if formula == FORMULA_MAGIC then
		skill = self:getMagicLevel()
	elseif formula == FORMULA_MELEE then
		local weapon = self:getWeaponItemType(WEAPON_SWORD, WEAPON_CLUB, WEAPON_AXE)
		skill = weapon:getAttack() * self:getEffectiveSkillLevel(weapon:getSkillType())
	elseif formula == FORMULA_DISTANCE then
		skill = self:getEffectiveSkillLevel(SKILL_DISTANCE)
	elseif formula == FORMULA_MELEE_HIGHEST_SKILL then
		skill = self:getHighestSkillLevel(SKILL_CLUB, SKILL_SWORD, SKILL_AXE)
	end

	if h then
		skill = math.min(h, skill)
	end

	local level = self:getLevel()
	if g then
		level = math.min(g, level)
	end

	local minimum = math.floor(level / a) + math.floor(skill * b + c)
	local maximum = math.floor(level / d) + math.floor(skill * e + f)

	if combat and variant then
		if combat:aggressive() then
			minimum, maximum = -minimum, -maximum
		end

		combat:setFormula(COMBAT_FORMULA_DAMAGE, minimum, 0, maximum, 0)
		return combat:execute(self, variant)
	end
	return minimum, maximum
end
