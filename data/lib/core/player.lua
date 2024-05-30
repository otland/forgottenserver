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

function Player.getPremiumTime(self)
	return math.max(0, self:getPremiumEndsAt() - os.time())
end

function Player.setPremiumTime(self, seconds)
	self:setPremiumEndsAt(os.time() + seconds)
	return true
end

function Player.addPremiumTime(self, seconds)
	self:setPremiumTime(self:getPremiumTime() + seconds)
	return true
end

function Player.removePremiumTime(self, seconds)
	local currentTime = self:getPremiumTime()
	if currentTime < seconds then
		return false
	end

	self:setPremiumTime(currentTime - seconds)
	return true
end

function Player.getPremiumDays(self)
	return math.floor(self:getPremiumTime() / 86400)
end

function Player.addPremiumDays(self, days)
	return self:addPremiumTime(days * 86400)
end

function Player.removePremiumDays(self, days)
	return self:removePremiumTime(days * 86400)
end

function Player.isPremium(self)
	return self:getPremiumTime() > 0 or configManager.getBoolean(configKeys.FREE_PREMIUM) or self:hasFlag(PlayerFlag_IsAlwaysPremium)
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

-- Always pass the number through the isValidMoney function first before using the transferMoneyTo
function Player.transferMoneyTo(self, target, amount)
	if not target then
		return false
	end

	-- See if you can afford this transfer
	local balance = self:getBankBalance()
	if amount > balance then
		return false
	end

	-- See if player is online
	local targetPlayer = Player(target.guid)
	if targetPlayer then
		targetPlayer:setBankBalance(targetPlayer:getBankBalance() + amount)
		db.query("UPDATE `players` SET `balance` = " .. targetPlayer:getBankBalance() .. " WHERE `id` = '" .. targetPlayer:getGuid() .. "'")
	else
		db.query("UPDATE `players` SET `balance` = `balance` + " .. amount .. " WHERE `id` = '" .. target.guid .. "'")
	end

	self:setBankBalance(self:getBankBalance() - amount)
	-- incase server crashes that we do not duplicate money
	self:save()
	return true
end

function Player.canCarryMoney(self, amount)
	-- Anyone can carry as much imaginary money as they desire
	if amount == 0 then
		return true
	end

	-- The 3 below loops will populate these local variables
	local totalWeight = 0
	local inventorySlots = 0

	local currencyItems = Game.getCurrencyItems()
	for index = #currencyItems, 1, -1 do
		local currency = currencyItems[index]
		-- Add currency coins to totalWeight and inventorySlots
		local worth = currency:getWorth()
		local currencyCoins = math.floor(amount / worth)
		if currencyCoins > 0 then
			amount = amount - (currencyCoins * worth)
			while currencyCoins > 0 do
				local count = math.min(100, currencyCoins)
				totalWeight = totalWeight + currency:getWeight(count)
				currencyCoins = currencyCoins - count
				inventorySlots = inventorySlots + 1
			end
		end
	end

	-- If player don't have enough capacity to carry this money
	if self:getFreeCapacity() < totalWeight then
		return false
	end

	-- If player don't have enough available inventory slots to carry this money
	local backpack = self:getSlotItem(CONST_SLOT_BACKPACK)
	if not backpack or backpack:getEmptySlots(true) < inventorySlots then
		return false
	end
	return true
end

function Player.withdrawMoney(self, amount)
	local balance = self:getBankBalance()
	if amount > balance or not self:addMoney(amount) then
		return false
	end

	self:setBankBalance(balance - amount)
	return true
end

function Player.depositMoney(self, amount)
	if not self:removeMoney(amount) then
		return false
	end

	self:setBankBalance(self:getBankBalance() + amount)
	return true
end

function Player.removeTotalMoney(self, amount)
	local moneyCount = self:getMoney()
	local bankCount = self:getBankBalance()
	if amount <= moneyCount then
		self:removeMoney(amount)
		return true
	elseif amount <= (moneyCount + bankCount) then
		if moneyCount ~= 0 then
			self:removeMoney(moneyCount)
			local remains = amount - moneyCount
			self:setBankBalance(bankCount - remains)
			self:sendTextMessage(MESSAGE_INFO_DESCR, ("Paid %d from inventory and %d gold from bank account. Your account balance is now %d gold."):format(moneyCount, amount - moneyCount, self:getBankBalance()))
			return true
		end

		self:setBankBalance(bankCount - amount)
		self:sendTextMessage(MESSAGE_INFO_DESCR, ("Paid %d gold from bank account. Your account balance is now %d gold."):format(amount, self:getBankBalance()))
		return true
	end
	return false
end

function Player.addLevel(self, amount, round)
	round = round or false
	local level, amount = self:getLevel(), amount or 1
	if amount > 0 then
		return self:addExperience(Game.getExperienceForLevel(level + amount) - (round and self:getExperience() or Game.getExperienceForLevel(level)))
	end
	return self:removeExperience(((round and self:getExperience() or Game.getExperienceForLevel(level)) - Game.getExperienceForLevel(level + amount)))
end

function Player.addMagicLevel(self, value)
	local currentMagLevel = self:getBaseMagicLevel()
	local sum = 0

	if value > 0 then
		while value > 0 do
			sum = sum + self:getVocation():getRequiredManaSpent(currentMagLevel + value)
			value = value - 1
		end

		return self:addManaSpent(sum - self:getManaSpent())
	end

	value = math.min(currentMagLevel, math.abs(value))
	while value > 0 do
		sum = sum + self:getVocation():getRequiredManaSpent(currentMagLevel - value + 1)
		value = value - 1
	end

	return self:removeManaSpent(sum + self:getManaSpent())
end

function Player.addSkillLevel(self, skillId, value)
	local currentSkillLevel = self:getSkillLevel(skillId)
	local sum = 0

	if value > 0 then
		while value > 0 do
			sum = sum + self:getVocation():getRequiredSkillTries(skillId, currentSkillLevel + value)
			value = value - 1
		end

		return self:addSkillTries(skillId, sum - self:getSkillTries(skillId))
	end

	value = math.min(currentSkillLevel, math.abs(value))
	while value > 0 do
		sum = sum + self:getVocation():getRequiredSkillTries(skillId, currentSkillLevel - value + 1)
		value = value - 1
	end

	return self:removeSkillTries(skillId, sum + self:getSkillTries(skillId), true)
end

function Player.addSkill(self, skillId, value, round)
	if skillId == SKILL_LEVEL then
		return self:addLevel(value, round or false)
	elseif skillId == SKILL_MAGLEVEL then
		return self:addMagicLevel(value)
	end
	return self:addSkillLevel(skillId, value)
end

function Player.getWeaponType(self)
	local weapon = self:getSlotItem(CONST_SLOT_LEFT)
	if weapon then
		return weapon:getType():getWeaponType()
	end
	return WEAPON_NONE
end

function Player.updateKillTracker(self, monster, corpse)
	local monsterType = monster:getType()
	if not monsterType then
		return false
	end

	local msg = NetworkMessage()
	msg:addByte(0xD1)
	msg:addString(monster:getName())

	local monsterOutfit = monsterType:getOutfit()
	msg:addU16(monsterOutfit.lookType or 19)
	msg:addByte(monsterOutfit.lookHead)
	msg:addByte(monsterOutfit.lookBody)
	msg:addByte(monsterOutfit.lookLegs)
	msg:addByte(monsterOutfit.lookFeet)
	msg:addByte(monsterOutfit.lookAddons)

	local corpseSize = corpse:getSize()
	msg:addByte(corpseSize)
	for index = corpseSize - 1, 0, -1 do
		msg:addItem(corpse:getItem(index))
	end

	local party = self:getParty()
	if party then
		local members = party:getMembers()
		members[#members + 1] = party:getLeader()

		for _, member in ipairs(members) do
			msg:sendToPlayer(member)
		end
	else
		msg:sendToPlayer(self)
	end

	msg:delete()
	return true
end

function Player.getTotalMoney(self)
	return self:getMoney() + self:getBankBalance()
end

function Player.addAddonToAllOutfits(self, addon)
	for sex = 0, 1 do
		local outfits = Game.getOutfits(sex)
		for outfit = 1, #outfits do
			self:addOutfitAddon(outfits[outfit].lookType, addon)
		end
	end
end

function Player.addAllMounts(self)
	local mounts = Game.getMounts()
	for mount = 1, #mounts do
		self:addMount(mounts[mount].id)
	end
end

function Player.setSpecialContainersAvailable(self, available)
	local msg = NetworkMessage()
	msg:addByte(0x2A)

	msg:addByte(0x00) -- stash
	msg:addByte(available and 0x01 or 0x00) -- market

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.addBankBalance(self, amount)
	self:setBankBalance(self:getBankBalance() + amount)
end

function Player.isPromoted(self)
	local vocation = self:getVocation()
	local fromVocId = vocation:getDemotion():getId()
	return vocation:getId() ~= fromVocId
end

function Player.getMaxTrackedQuests(self)
	return configManager.getNumber(self:isPremium() and configKeys.QUEST_TRACKER_PREMIUM_LIMIT or configKeys.QUEST_TRACKER_FREE_LIMIT)
end

function Player.getQuests(self)
	local quests = {}
	for _, quest in pairs(Game.getQuests()) do
		if quest:isStarted(self) then
			quests[#quests + 1] = quest
		end
	end
	return quests
end

function Player.sendQuestLog(self)
	local msg = NetworkMessage()
	msg:addByte(0xF0)
	local quests = self:getQuests()
	msg:addU16(#quests)

	for _, quest in pairs(quests) do
		msg:addU16(quest.id)
		msg:addString(quest.name)
		msg:addByte(quest:isCompleted(self) and 0x01 or 0x00)
	end

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.sendQuestLine(self, quest)
	local msg = NetworkMessage()
	msg:addByte(0xF1)
	msg:addU16(quest.id)
	local missions = quest:getMissions(self)
	msg:addByte(#missions)

	for _, mission in pairs(missions) do
		msg:addU16(mission.id)
		msg:addString(mission:getName(self))
		msg:addString(mission:getDescription(self))
	end

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.getTrackedQuests(self, missionsId)
	local playerId = self:getId()
	local maxTrackedQuests = self:getMaxTrackedQuests()
	local trackedQuests = {}
	Game.getTrackedQuests()[playerId] = trackedQuests
	local quests = Game.getQuests()
	local missions = Game.getMissions()
	local trackeds = 0
	for _, missionId in pairs(missionsId) do
		local mission = missions[missionId]
		if mission and mission:isStarted(self) then
			trackedQuests[mission] = quests[mission.questId]
			trackeds = trackeds + 1
			if trackeds >= maxTrackedQuests then
				break
			end
		end
	end
	return trackedQuests, trackeds
end

function Player.sendQuestTracker(self, missionsId)
	local msg = NetworkMessage()
	msg:addByte(0xD0)
	msg:addByte(1)

	local trackedQuests, trackeds = self:getTrackedQuests(missionsId)
	msg:addByte(self:getMaxTrackedQuests() - trackeds)
	msg:addByte(trackeds)

	for mission, quest in pairs(trackedQuests or {}) do
		msg:addU16(mission.id)
		msg:addString(quest.name)
		msg:addString(mission:getName(self))
		msg:addString(mission:getDescription(self))
	end

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.sendUpdateQuestTracker(self, mission)
	local msg = NetworkMessage()
	msg:addByte(0xD0)
	msg:addByte(0)

	msg:addU16(mission.id)
	msg:addString(mission:getName(self))
	msg:addString(mission:getDescription(self))

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.getBestiaryKills(self, raceId)
	return math.max(0, self:getStorageValue(PlayerStorageKeys.bestiaryKillsBase + raceId))
end

function Player.setBestiaryKills(self, raceId, value)
	return self:setStorageValue(PlayerStorageKeys.bestiaryKillsBase + raceId, value)
end

function Player.addBestiaryKills(self, raceId)
	local monsterType = MonsterType(raceId)
	if not monsterType then
		return false
	end

	local kills = self:getBestiaryKills(raceId)
	local newKills = kills + 1
	local bestiaryInfo = monsterType:getBestiaryInfo()
	for _, totalKills in pairs({bestiaryInfo.prowess, bestiaryInfo.expertise, bestiaryInfo.mastery}) do
		if kills == 0 or (kills < totalKills and newKills >= totalKills) then
			self:sendTextMessage(MESSAGE_EVENT_DEFAULT, string.format("You unlocked details for the creature %s.", monsterType:getName()))
			self:sendBestiaryMilestoneReached(raceId)
			break
		end
	end
	return self:setBestiaryKills(raceId, newKills)
end

function Player.sendBestiaryMilestoneReached(self, raceId)
	local msg = NetworkMessage()
	msg:addByte(0xD9)
	msg:addU16(raceId)
	msg:sendToPlayer(self)
	msg:delete()
	return true
end

local function getStaminaBonus(staminaMinutes)
	if staminaMinutes > 2340 then
		return 150
	elseif staminaMinutes < 840 then
		return 50
	else
		return 100
	end
end

function Player.calculateLowLevelBonus(self, level)
	if level > 1 and level <= 50 then
		local expBonus = {minlevel = 2, maxlevel = 50, bonus = 1}
		local bonusPercentage = (expBonus.maxlevel - level) / (expBonus.maxlevel - expBonus.minlevel)
		return expBonus.bonus * 100 * bonusPercentage
	else
		return 0
	end
end

function Player.updateClientExpDisplay(self)
	local level = self:getLevel()

	-- Experience bonus (includes server rates)
	local expGainRate = 100 * Game.getExperienceStage(level)
	self:setClientExpDisplay(expGainRate)

	-- Stamina bonus
	local staminaMinutes = self:getStamina()
	local staminaBonus = getStaminaBonus(staminaMinutes)
	self:setClientStaminaBonusDisplay(staminaBonus)

	-- Low level bonus
	local levelBonus = self:calculateLowLevelBonus(level)
	self:setClientLowLevelBonusDisplay(levelBonus)
	return true
end

function Player.sendHighscores(self, entries, params)
	local msg = NetworkMessage()
	msg:addByte(0xB1)

	msg:addByte(params.action)

	msg:addByte(0x01)
	msg:addString(configManager.getString(configKeys.SERVER_NAME))
	msg:addString(params.world)

	msg:addByte(params.worldType)
	msg:addByte(params.battlEye)

	local vocations = Game.getUnpromotedVocations()
	msg:addByte(#vocations + 1)

	msg:addU32(0xFFFFFFFF)
	msg:addString("All vocations")

	for _, vocation in ipairs(vocations) do
		msg:addU32(vocation:getId())
		msg:addString(vocation:getName())
	end

	msg:addU32(params.vocation)

	msg:addByte(#HIGHSCORES_CATEGORIES)
	for id, category in ipairs(HIGHSCORES_CATEGORIES) do
		msg:addByte(id)
		msg:addString(category.name)
	end

	msg:addByte(params.category)

	msg:addU16(params.page)
	msg:addU16(params.totalPages)

	msg:addByte(#entries.data)
	for _, entry in pairs(entries.data) do
		msg:addU32(entry.rank)
		msg:addString(entry.name)
		msg:addString(entry.title)
		msg:addByte(entry.vocation)
		msg:addString(entry.world)
		msg:addU16(entry.level)
		msg:addByte(self:getGuid() == entry.id and 0x01 or 0x00)
		msg:addU64(entry.points)
	end

	msg:addByte(0xFF) -- unknown
	msg:addByte(0x00) -- display loyalty title column
	msg:addByte(HIGHSCORES_CATEGORIES[params.category].type or 0x00)

	msg:addU32(entries.ts)

	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.takeScreenshot(self, screenshotType, ignoreConfig)
	if not ignoreConfig and (screenshotType < SCREENSHOT_TYPE_FIRST or screenshotType > SCREENSHOT_TYPE_LAST) then
		return false
	end

	local msg = NetworkMessage()
	msg:addByte(0x75)
	msg:addByte(screenshotType)
	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.getBlessings(self)
	local blessings = 0
	for i = 1, 6 do
		if self:hasBlessing(i) then
			blessings = blessings + 1
		end
	end
	return blessings
end

local slots = {
	CONST_SLOT_RIGHT,
	CONST_SLOT_LEFT,
	CONST_SLOT_HEAD,
	CONST_SLOT_NECKLACE,
	CONST_SLOT_ARMOR,
	CONST_SLOT_LEGS,
	CONST_SLOT_FEET,
	CONST_SLOT_RING
}

function Player.getTotalArmor(self)
	local total = 0
	local item
	for i = 1, #slots do
		item = self:getSlotItem(slots[i])
		if item then
			total = total + item:getType():getArmor()
		end
	end
	return total
end

function Player.getTotalDefense(self)
	local total = 0
	local item
	for i = 1, #slots do
		item = self:getSlotItem(slots[i])
		if item then
			total = total + item:getType():getDefense()
		end
	end
	return total
end

function Player.setAccountStorageValue(self, key, value)
	return Game.setAccountStorageValue(self:getAccountId(), key, value)
end

function Player.getAccountStorageValue(self, key)
	return Game.getAccountStorageValue(self:getAccountId(), key)
end

function Player.sendWorldLight(self, color, level)
	local msg = NetworkMessage()
	msg:addByte(0x82)
	msg:addByte(self:getGroup():getAccess() and 0xFF or level)
	msg:addByte(color)
	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.sendWorldTime(self, time)
	local msg = NetworkMessage()
	msg:addByte(0xEF)
	msg:addByte(time / 60) -- hour
	msg:addByte(time % 60) -- min
	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.sendHotkeyPreset(self)
	local msg = NetworkMessage()
	msg:addByte(0x9D)
	msg:addU32(self:getVocation():getClientId())
	msg:sendToPlayer(self)
	return true
end

function Player.disableLoginMusic(self)
	local msg = NetworkMessage()
	msg:addByte(0x85)
	msg:addByte(0x01)
	msg:addByte(0x00)
	msg:addByte(0x00)
	msg:sendToPlayer(self)
	msg:delete()
	return true
end

function Player.sendInboxItems(self, items, containerId)
	local inbox = self:getInbox()
	local container = Game.createItem(containerId, 1)
	for _, item in pairs(items) do
		container:addItem(item.item, item.count)
	end
	container:moveTo(inbox)
end
