SCREENSHOT_TYPE_ACHIEVEMENT = 1
SCREENSHOT_TYPE_BESTIARYENTRYCOMPLETED = 2
SCREENSHOT_TYPE_BESTIARYENTRYUNLOCKED = 3
SCREENSHOT_TYPE_BOSSDEFEATED = 4
SCREENSHOT_TYPE_DEATHPVE = 5
SCREENSHOT_TYPE_DEATHPVP = 6
SCREENSHOT_TYPE_LEVELUP = 7
SCREENSHOT_TYPE_PLAYERKILLASSIST = 8
SCREENSHOT_TYPE_PLAYERKILL = 9
SCREENSHOT_TYPE_PLAYERATTACKING = 10
SCREENSHOT_TYPE_TREASUREFOUND = 11
SCREENSHOT_TYPE_SKILLUP = 12

SCREENSHOT_TYPE_FIRST = SCREENSHOT_TYPE_ACHIEVEMENT
SCREENSHOT_TYPE_LAST = SCREENSHOT_TYPE_SKILLUP

function Player:takeScreenshot(eventType)
	if eventType and eventType >= SCREENSHOT_TYPE_FIRST and eventType < SCREENSHOT_TYPE_LAST then
		local m = NetworkMessage()
		m:addByte(0x75)
		m:addByte(eventType)
		m:sendToPlayer(self)
		return true
	end
	
	return false
end

function getCountdownString(duration)
	local days = math.floor(duration/86400)
	local hours = math.floor((duration % 86400)/3600)
	local minutes = math.floor((duration % 3600)/60)
	local seconds = math.floor(duration % 60)
	
	local response = {}
	if days > 0 then
		response[#response+1] = days .. "d"
	end
	
	response[#response+1] = hours .. "h"
	response[#response+1] = minutes .. "m"
	response[#response+1] = seconds .. "s"
	
	return table.concat(response, " ")
end

function NetworkMessage:addOutfit(outfit, addMount)
	self:addU16(outfit.lookType)
	if outfit.lookType == 0 then
		local itemType = ItemType(outfit.lookTypeEx)
		self:addU16(itemType and itemType:getClientId() or 0)
	else
		self:addByte(outfit.lookHead)
		self:addByte(outfit.lookBody)
		self:addByte(outfit.lookLegs)
		self:addByte(outfit.lookFeet)
		self:addByte(outfit.lookAddons)
	end

	if addMount then
		self:addU16(outfit.lookMount)
		if outfit.lookMount ~= 0 then
			self:addByte(outfit.lookMountHead or 0)
			self:addByte(outfit.lookMountBody or 0)
			self:addByte(outfit.lookMountLegs or 0)
			self:addByte(outfit.lookMountFeet or 0)
		end
	end
end

local function getDeathList(guid)
	local response = {}
	if not tonumber(guid) then
		return
	end
	
	local resultId = db.storeQuery(
		string.format(
			"SELECT `time`, `level`, `killed_by`, `is_player` FROM `player_deaths` WHERE `player_id` = %d ORDER BY `time` DESC",
			guid
		)
	)
	
	if resultId ~= false then
		repeat		
			response[#response + 1] = {
				msg = string.format("Died at level %d by %s", result.getNumber(resultId, "level"), result.getString(resultId, "killed_by")),
				at = result.getNumber(resultId, "time")
			}
		until not result.next(resultId)
		result.free(resultId)
	end
	return response
end

local function getFragList(guid)
	local response = {}
	if not tonumber(guid) then
		return
	end
	
	local player = Player(guid)
	if not player then
		return
	end
	
	local resultId = db.storeQuery(
		'SELECT `players`.`name`, `time`, `unjustified` FROM `player_deaths` LEFT JOIN `players` ON `players`.`id` = `player_deaths`.`player_id` WHERE `killed_by` = ' .. db.escapeString(player:getName()) .. ''
	)

	if resultId ~= false then
		repeat		
			response[#response + 1] = {
				victim = result.getString(resultId, "name"),
				at = result.getNumber(resultId, "time"),
				killType = result.getNumber(resultId, "unjustified")
			}
		until not result.next(resultId)
		result.free(resultId)
	end
	return response
end

local function getFragStatus(player)
	local response = {
		unjustified = 0,
		duration = 0,
		totalDuration = 0,
	}
	
	local fragTime = configManager.getNumber(configKeys.FRAG_TIME)
	if fragTime <= 0 then
		return response
	end
	
	local skullTime = player:getSkullTime()
	if skullTime <= 0 then
		return response
	end

	response.unjustified = math.ceil(skullTime / fragTime)
	response.duration = math.floor(skullTime % fragTime)
	response.totalDuration = skullTime
	return response
end

local function parseItem(item, response)
	local clientId = item:getType():getClientId()
	if not response[clientId] then
		response[clientId] = 0
	end
	response[clientId] = response[clientId] + item:getCount()
	
	if item:isContainer() then
		for containerIndex, containerItem in pairs(item:getItems()) do
			parseItem(containerItem, response)
		end
	end
end

function Player:getClientInventory(category)
	local response = {}
	local responseCount = 0
	
	if category == COMPENDIUM_PLAYERITEMS_EQUIPPED then
		for slot = CONST_SLOT_HEAD, CONST_SLOT_AMMO do
			local slotItem = self:getSlotItem(slot)
			if slotItem then
				parseItem(slotItem, response)
			end
		end
	elseif category == COMPENDIUM_PLAYERITEMS_PURSE then
		local slotItem = self:getSlotItem(CONST_SLOT_STORE_INBOX)
		if slotItem then
			parseItem(slotItem, response)
		end
	elseif category == COMPENDIUM_PLAYERITEMS_STASH then
		-- not implemented yet
	elseif category == COMPENDIUM_PLAYERITEMS_DEPOT then
		local towns = Game.getTowns()
		for _, town in pairs(towns) do
			local depotBox = self:getDepotChest(town:getId())
			if depotBox then
				for containerIndex, containerItem in pairs(depotBox:getItems()) do
					parseItem(containerItem, response)
				end
			end
		end
	elseif category == COMPENDIUM_PLAYERITEMS_MAILBOX then
		local inbox = self:getInbox()
		if inbox then
			for containerIndex, containerItem in pairs(inbox:getItems()) do
				parseItem(containerItem, response)
			end
		end
	end
	
	for _ in pairs(response) do
		responseCount = responseCount + 1
	end
	
	return response, responseCount
end


-- 800000-809999 - compendium tabs cooldown
COMPENDIUM_COOLDOWNS_BASE = 800000
COMPENDIUM_COOLDOWN_DURATION = 5 -- in seconds

-- compendium tabs
-- to do: verify
COMPENDIUM_TAB_ITEMS = 0
COMPENDIUM_TAB_BESTIARY = 1
COMPENDIUM_TAB_CHARMS = 2
COMPENDIUM_TAB_MAP = 3
COMPENDIUM_TAB_HOUSES = 4
COMPENDIUM_TAB_PLAYER = 0xDA

-- player info tabs
COMPENDIUM_PLAYER_BASEINFORMATION = 0
COMPENDIUM_PLAYER_GENERAL = 1
COMPENDIUM_PLAYER_COMBAT = 2
COMPENDIUM_PLAYER_DEATHS = 3
COMPENDIUM_PLAYER_PVPKILLS = 4
COMPENDIUM_PLAYER_ACHIEVEMENTS = 5
COMPENDIUM_PLAYER_INVENTORY = 6
COMPENDIUM_PLAYER_COSMETICS = 7
COMPENDIUM_PLAYER_STORE = 8
COMPENDIUM_PLAYER_INSPECTION = 9
COMPENDIUM_PLAYER_BADGES = 10
COMPENDIUM_PLAYER_TITLES = 11

-- player items tab
COMPENDIUM_PLAYERITEMS_EQUIPPED = 0
COMPENDIUM_PLAYERITEMS_PURSE = 1
COMPENDIUM_PLAYERITEMS_STASH = 2
COMPENDIUM_PLAYERITEMS_DEPOT = 3
COMPENDIUM_PLAYERITEMS_MAILBOX = 4

COMPENDIUM_PLAYERITEMS_FIRST = COMPENDIUM_PLAYERITEMS_EQUIPPED
COMPENDIUM_PLAYERITEMS_LAST = COMPENDIUM_PLAYERITEMS_MAILBOX

-- kill types
COMPENDIUM_KILLTYPE_JUSTIFIED = 0
COMPENDIUM_KILLTYPE_UNJUSTIFIED = 1
COMPENDIUM_KILLTYPE_GUILDWAR = 2
COMPENDIUM_KILLTYPE_ASSISTED = 3
COMPENDIUM_KILLTYPE_ARENA = 4

-- compendium response types
COMPENDIUM_RESPONSETYPE_OK = 0
COMPENDIUM_RESPONSETYPE_NODATA = 1
COMPENDIUM_RESPONSETYPE_ACCESSDENIED = 2
COMPENDIUM_RESPONSETYPE_INSPECTIONDENIED = 3

-- compendium skill ids
COMPENDIUM_SKILL_MAGIC = 1
COMPENDIUM_SKILL_FIST = 11
COMPENDIUM_SKILL_CLUB = 9
COMPENDIUM_SKILL_SWORD = 8
COMPENDIUM_SKILL_AXE = 10
COMPENDIUM_SKILL_DISTANCE = 7
COMPENDIUM_SKILL_SHIELDING = 6
COMPENDIUM_SKILL_FISHING = 13
	
compendiumSkillMap = {
	[SKILL_FIST] = COMPENDIUM_SKILL_FIST,
	[SKILL_CLUB] = COMPENDIUM_SKILL_CLUB,
	[SKILL_SWORD] = COMPENDIUM_SKILL_SWORD,
	[SKILL_AXE] = COMPENDIUM_SKILL_AXE,
	[SKILL_DISTANCE] = COMPENDIUM_SKILL_DISTANCE,
	[SKILL_SHIELD] = COMPENDIUM_SKILL_SHIELDING,
	[SKILL_FISHING] = COMPENDIUM_SKILL_FISHING
}

-- client combat types
CLIENT_COMBAT_PHYSICAL = 0
CLIENT_COMBAT_FIRE = 1
CLIENT_COMBAT_EARTH = 2
CLIENT_COMBAT_ENERGY = 3
CLIENT_COMBAT_ICE = 4
CLIENT_COMBAT_HOLY = 5
CLIENT_COMBAT_DEATH = 6
CLIENT_COMBAT_HEALING = 7
CLIENT_COMBAT_DROWN = 8
CLIENT_COMBAT_LIFEDRAIN = 9
CLIENT_COMBAT_MANADRAIN = 10

clientCombatMap = {
	CLIENT_COMBAT_PHYSICAL = COMBAT_PHYSICALDAMAGE,
	CLIENT_COMBAT_FIRE = COMBAT_FIREDAMAGE,
	CLIENT_COMBAT_EARTH = COMBAT_EARTHDAMAGE,
	CLIENT_COMBAT_ENERGY = COMBAT_ENERGYDAMAGE,
	CLIENT_COMBAT_ICE = COMBAT_ICEDAMAGE,
	CLIENT_COMBAT_HOLY = COMBAT_HOLYDAMAGE,
	CLIENT_COMBAT_DEATH = COMBAT_DEATHDAMAGE,
	CLIENT_COMBAT_HEALING = COMBAT_HEALING,
	CLIENT_COMBAT_DROWN = COMBAT_DROWN,
	CLIENT_COMBAT_LIFEDRAIN = COMBAT_LIFEDRAIN,
	CLIENT_COMBAT_MANADRAIN = COMBAT_MANADRAIN
}

function sendCompendiumError(player, infoType, errorCode)
	local response = NetworkMessage();
	response:addByte(COMPENDIUM_TAB_PLAYER)
	response:addByte(infoType)
	response:addByte(errorCode)
	response:sendToPlayer(player)
end

function sendCompendiumPlayerInfo(player, creatureId, infoType, entriesPerPage, page)
	-- to do: implement permission logic for inspecting other creatures
	-- to do: implement per-category cooldowns
	-- to do: allow updating static pages only on relog/data update/60 seconds
	-- to do: reduce sql queries by using cache? change frags/deathlist limit?
	-- to do: addEvent for responses
	if player:getId() ~= creatureId then
		sendCompendiumError(player, infoType, COMPENDIUM_RESPONSETYPE_ACCESSDENIED)
		return
	end

	local creature = Creature(creatureId)
	if not creature or creature:isRemoved() then
		sendCompendiumError(player, infoType, COMPENDIUM_RESPONSETYPE_NODATA)
		return
	end

	local isPlayer = creature:isPlayer()
	local creatureType = "unknown"
	if isPlayer then
		if creature:getGroup():getAccess() then
			creatureType = creature:getGroup():getName()
		else
			creatureType = creature:getVocation():getName()
		end
	elseif creature:isMonster() then
		creatureType = "monster"
	elseif creature:isNpc() then
		creatureType = "Non-Player Character"
	end
	
	local response = NetworkMessage()
	response:addByte(COMPENDIUM_TAB_PLAYER)
	response:addByte(infoType)
	response:addByte(COMPENDIUM_RESPONSETYPE_OK)
		
	if infoType == COMPENDIUM_PLAYER_BASEINFORMATION then
		response:addString(creature:getName())
		response:addString(creatureType:gsub("^%l", string.upper))
		response:addU16(isPlayer and creature:getLevel() or 1)
		response:addOutfit(creature:getOutfit(), false)
		response:addByte(0x00) -- hide stamina (hidden if 0x01)
		response:addByte(0x01) -- enable store summary and character titles
		response:addString("") -- character title
		response:sendToPlayer(player)
		return
	elseif infoType == COMPENDIUM_PLAYER_GENERAL then
		if not isPlayer then
			sendCompendiumError(player, infoType, COMPENDIUM_RESPONSETYPE_NODATA)
			return
		end
		
		local currentLevel = creature:getLevel()
		local currentLevelXP = Game.getExperienceForLevel(currentLevel)
		local currentXP = creature:getExperience()
		local goalXP = Game.getExperienceForLevel(currentLevel + 1) - currentLevelXP
		local progress = (currentXP - currentLevelXP) * 100 / goalXP
		response:addU64(creature:getExperience())
		response:addU16(creature:getLevel())
		response:addByte(progress)
	
		response:addU16(100) -- base XP rate
		response:addU32(0) -- tournament XP factor
		response:addU16(0) -- low level bonus
		response:addU16(0) -- XP boost
		response:addU16(100) -- stamina boost
		response:addU16(0) -- XP boost remaining time
		response:addByte(0) -- can buy XP boost

		local HP = creature:getHealth()
		local maxHP = creature:getMaxHealth()
		
		if maxHP > 65535 then
			HP = math.floor(HP * 100 / maxHP)
			maxHP = 100
		end

		response:addU16(math.min(HP, maxHP))
		response:addU16(maxHP)

		local MP = creature:getMana() or 0
		local maxMP = creature:getMaxMana() or 0
		
		if maxMP > 65535 then
			MP = math.floor(MP * 100 / maxMP)
			maxMP = 100
		end

		response:addU16(math.min(MP, maxMP))
		response:addU16(maxMP)

		response:addByte(creature:getSoul())
		response:addU16(creature:getStamina())
		
		local condition = creature:getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT)
		response:addU16(condition and condition:getTicks() / 1000 or 0)
		response:addU16(creature:getOfflineTrainingTime() / 60 / 1000)
		response:addU16(creature:getSpeed() / 2)
		response:addU16(creature:getBaseSpeed() / 2)
		response:addU32(creature:getCapacity())
		response:addU32(creature:getCapacity())
		response:addU32(creature:getFreeCapacity())

		-- skill amount
		response:addByte(8)
		-- u8: skill compendium id
		-- u16: bonus, base, loyalty, percent
		
		local baseML = creature:getBaseMagicLevel()
		local voc = creature:getVocation()
		local manaCost = voc:getRequiredManaSpent(baseML + 1) - voc:getRequiredManaSpent(baseML)		
		local manaSpent = creature:getManaSpent()
		local progress = manaSpent * 10000 / manaCost
		
		response:addByte(COMPENDIUM_SKILL_MAGIC)
		response:addU16(creature:getMagicLevel())
		response:addU16(baseML)
		response:addU16(baseML)
		response:addU16(progress)
		
		for skillId = SKILL_FIST, SKILL_FISHING do
			local baseSkill = creature:getSkillLevel(skillId)
		
			response:addByte(compendiumSkillMap[skillId])
			response:addU16(creature:getEffectiveSkillLevel(skillId))
			response:addU16(baseSkill)
			response:addU16(baseSkill) -- loyalty bonus
			response:addU16(creature:getSkillPercent(skillId)*100)
		end

		-- element magic level boosts
		response:addByte(0) -- elements count
		-- element structure:
		-- u8 element type
		-- u16 bonus magic levels
		
		response:sendToPlayer(player)
		return
	elseif infoType == COMPENDIUM_PLAYER_COMBAT then
		for skillId = SPECIALSKILL_CRITICALHITCHANCE, SPECIALSKILL_MANALEECHAMOUNT do
			response:addU16(isPlayer and creature:getSpecialSkill(skillId) or 0)
			response:addU16(0)
		end

		response:addU16(0) -- cleave (bonus percent damage to nearby enemies)
		response:addU16(0) -- bonus magic shield capacity (flat)
		response:addU16(0) -- bonus magic shield capacity (percent)

		-- perfect shot flat damage bonus at 1-5 range
		for i = 1, 5 do
			response:addU16(0)
		end

		response:addU16(0) -- damage reflection (flat, one value for all combat types)
		response:addByte(0) -- blesssings used
		response:addByte(8) -- blessings count

		-- weapon
		response:addU16(0) -- base max damage
		response:addByte(CLIENT_COMBAT_PHYSICAL) -- base element type
		response:addByte(0) -- percent damage conversion
		response:addByte(CLIENT_COMBAT_PHYSICAL) -- conversion element type

		response:addU16(0) -- armor
		response:addU16(0) -- defense

		-- element resistances count
		response:addByte(0)
		-- structure:
		-- u8 clientcombat
		-- u8 value
		
		-- active potions count
		response:addByte(0)
		-- structure:
		-- item clientId
		-- u16 duration
		
		response:sendToPlayer(player)
		return
	elseif infoType == COMPENDIUM_PLAYER_DEATHS then
		if not isPlayer then
			sendCompendiumError(player, infoType, COMPENDIUM_RESPONSETYPE_NODATA)
			return
		end
		
		local deathList = getDeathList(creature:getGuid())
		local pageCount = math.max(1, math.ceil(#deathList / entriesPerPage))
		if page > pageCount then
			sendCompendiumError(player, infoType, COMPENDIUM_RESPONSETYPE_NODATA)
			return
		end
		
		local responseCount = page < pageCount and entriesPerPage or #deathList - (page-1) * entriesPerPage
		
		response:addU16(page)
		response:addU16(pageCount)
		response:addU16(responseCount)
		for i = (page-1) * entriesPerPage + 1, math.min(page * entriesPerPage, #deathList) do
			response:addU32(deathList[i].at)
			response:addString(deathList[i].msg)
		end
		
		response:sendToPlayer(player)
		return
	elseif infoType == COMPENDIUM_PLAYER_PVPKILLS then
		if not isPlayer then
			sendCompendiumError(player, infoType, COMPENDIUM_RESPONSETYPE_NODATA)
			return
		end
		
		local fragList = getFragList(creature:getGuid())
		local pageCount = math.max(1, math.ceil(#fragList / entriesPerPage))
		if page > pageCount then
			sendCompendiumError(player, infoType, COMPENDIUM_RESPONSETYPE_NODATA)
			return
		end
		
		-- begin extra info
		local extraInfoCount = 2 -- unjustified count + separator
		local fragStatus = getFragStatus(creature)
		
		if fragStatus.duration > 0 then
			extraInfoCount = extraInfoCount + 1
		end
		if fragStatus.totalDuration > 0 then
			extraInfoCount = extraInfoCount + 1
		end
		
		local pzLockDuration = 0
		if creature:isPzLocked() then
			pzLockDuration = math.floor(creature:getCondition(CONDITION_INFIGHT, CONDITIONID_DEFAULT):getTicks() / 1000)
			extraInfoCount = extraInfoCount + 1
		end
		entriesPerPage = entriesPerPage - extraInfoCount
		-- end extra info
		
		local responseCount = page < pageCount and entriesPerPage or #fragList - (page-1) * entriesPerPage
		response:addU16(page)
		response:addU16(pageCount)
		response:addU16(responseCount + extraInfoCount) -- add extra info

		-- begin extra info (again)
		local ost = os.time()
		
		-- unjustified kills
		response:addU32(ost)
		response:addString(string.format("You have %d unjustified kill%s.", fragStatus.unjustified, (fragStatus.unjustified == 1 and "" or "s")))
		response:addByte(COMPENDIUM_KILLTYPE_ARENA)
		
		-- single frag duration
		if fragStatus.duration > 0 then
			response:addU32(ost)
			response:addString(string.format("Next kill expires in: %s", getCountdownString(fragStatus.duration)))
			response:addByte(COMPENDIUM_KILLTYPE_ARENA)
		end
	
		-- total frag duration
		if fragStatus.totalDuration > 0 then
			response:addU32(ost)
			response:addString(string.format("All kills expire in: %s", getCountdownString(fragStatus.totalDuration)))
			response:addByte(COMPENDIUM_KILLTYPE_ARENA)
		end
		
		if pzLockDuration > 0 then
			response:addU32(ost)
			response:addString(string.format("PZ lock expires in: %s", getCountdownString(pzLockDuration)))
			response:addByte(COMPENDIUM_KILLTYPE_ARENA)
		end
		
		response:addU32(ost)
		response:addString("----------------")
		response:addByte(COMPENDIUM_KILLTYPE_ARENA)
		-- end extra info (again)

		-- display kills on page
		for i = (page-1) * entriesPerPage + 1, math.min(page * entriesPerPage, #fragList) do
			response:addU32(fragList[i].at)
			response:addString(fragList[i].victim)
			response:addByte(fragList[i].killType)
		end
		
		response:sendToPlayer(player)
		return
	elseif infoType == COMPENDIUM_PLAYER_ACHIEVEMENTS then
		local achievements = {
			--{id = 2, extended = false, unlockedAt = os.time()},
			--{id = 3, extended = true, grade = 9, title = "Crashed!", description = "After sleepless night and countless client crashes you got what you wanted. No client tab is safe from you!"}
		}

		response:addU16(0) -- achievement points
		response:addU16(0) -- secret achievements amount
		
		response:addU16(#achievements) -- achievements count
		for i = 1, #achievements do
			local extended = achievements[i].extended
			
			response:addU16(achievements[i].id) -- achievement id (will show hardcoded values if byte after timestamp is disabled)
			response:addU32(achievements[i].unlockedAt or 0) -- unlock timestamp
			response:addByte(extended and 1 or 0)

			if extended then
				response:addString(achievements[i].title)
				response:addString(achievements[i].description) -- achievement description
				response:addByte(achievements[i].grade) -- achievement grade (1-?)
			end
		end
		response:sendToPlayer(player)
		return
	elseif infoType == COMPENDIUM_PLAYER_INVENTORY then
		-- to do: show loot if inspecting monster?
		-- shoplist if npc?
		if not isPlayer then
			sendCompendiumError(player, infoType, COMPENDIUM_RESPONSETYPE_NODATA)
			return
		end
		
		-- send each category
		for categoryType = COMPENDIUM_PLAYERITEMS_FIRST, COMPENDIUM_PLAYERITEMS_LAST do
			local categoryItems, categoryCount = creature:getClientInventory(categoryType)
			response:addU16(categoryCount) -- items to send
			for clientId, count in pairs(categoryItems) do
				response:addU16(clientId) -- item clientId
				response:addU32(count) -- item amount
			end
		end
		
		response:sendToPlayer(player)
		return
	elseif infoType == COMPENDIUM_PLAYER_COSMETICS then
		return
	elseif infoType == COMPENDIUM_PLAYER_STORE then
		return
	elseif infoType == COMPENDIUM_PLAYER_INSPECTION then
		return
	elseif infoType == COMPENDIUM_PLAYER_BADGES then
		return
	elseif infoType == COMPENDIUM_PLAYER_TITLES then
		return
	end

	sendCompendiumError(player, infoType, COMPENDIUM_RESPONSETYPE_NODATA)
end

local callback = function(player, recvbyte, networkMessage)
	local playerId = networkMessage:getU32()
	if playerId == 0 then
		playerId = player:getId()
	end
	
	local infoType = networkMessage:getByte()
	local entriesPerPage = 5
	local currentPage = 1
	
	if (infoType == COMPENDIUM_PLAYER_DEATHS or infoType == COMPENDIUM_PLAYER_PVPKILLS) then
		entriesPerPage = math.min(30, math.max(5, networkMessage:getU16()))
		currentPage = math.max(1, networkMessage:getU16());
	end
	
	sendCompendiumPlayerInfo(player, playerId, infoType, entriesPerPage, currentPage)
	return true
end

setPacketEvent(0xE5, callback)