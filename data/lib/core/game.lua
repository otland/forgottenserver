function Game.broadcastMessage(message, messageType)
	if not messageType then
		messageType = MESSAGE_STATUS_WARNING
	end

	for _, player in ipairs(Game.getPlayers()) do
		player:sendTextMessage(messageType, message)
	end
end

function Game.getReverseDirection(direction)
	if direction == WEST then
		return EAST
	elseif direction == EAST then
		return WEST
	elseif direction == NORTH then
		return SOUTH
	elseif direction == SOUTH then
		return NORTH
	elseif direction == NORTHWEST then
		return SOUTHEAST
	elseif direction == NORTHEAST then
		return SOUTHWEST
	elseif direction == SOUTHWEST then
		return NORTHEAST
	elseif direction == SOUTHEAST then
		return NORTHWEST
	end
	return NORTH
end

function Game.getSkillType(weaponType)
	if weaponType == WEAPON_CLUB then
		return SKILL_CLUB
	elseif weaponType == WEAPON_SWORD then
		return SKILL_SWORD
	elseif weaponType == WEAPON_AXE then
		return SKILL_AXE
	elseif weaponType == WEAPON_DISTANCE then
		return SKILL_DISTANCE
	elseif weaponType == WEAPON_SHIELD then
		return SKILL_SHIELD
	end
	return SKILL_FIST
end

do
	local cdShort = {"d", "h", "m", "s"}
	local cdLong = {" day", " hour", " minute", " second"}
	local function getTimeUnitGrammar(amount, unitID, isLong)
		return isLong and string.format("%s%s", cdLong[unitID], amount ~= 1 and "s" or "") or cdShort[unitID]
	end

	function Game.getCountdownString(duration, longVersion, hideZero)
		if duration < 0 then
			return "expired"
		end

		local days = math.floor(duration / 86400)
		local hours = math.floor((duration % 86400) / 3600)
		local minutes = math.floor((duration % 3600) / 60)
		local seconds = math.floor(duration % 60)

		local response = {}
		if hideZero then
			if days > 0 then
				response[#response + 1] = days .. getTimeUnitGrammar(days, 1, longVersion)
			end

			if hours > 0 then
				response[#response + 1] = hours .. getTimeUnitGrammar(hours, 2, longVersion)
			end

			if minutes > 0 then
				response[#response + 1] = minutes .. getTimeUnitGrammar(minutes, 3, longVersion)
			end

			if seconds > 0 then
				response[#response + 1] = seconds .. getTimeUnitGrammar(seconds, 4, longVersion)
			end
		else
			if days > 0 then
				response[#response + 1] = days .. getTimeUnitGrammar(days, 1, longVersion)
				response[#response + 1] = hours .. getTimeUnitGrammar(hours, 2, longVersion)
				response[#response + 1] = minutes .. getTimeUnitGrammar(minutes, 3, longVersion)
				response[#response + 1] = seconds .. getTimeUnitGrammar(seconds, 4, longVersion)
			elseif hours > 0 then
				response[#response + 1] = hours .. getTimeUnitGrammar(hours, 2, longVersion)
				response[#response + 1] = minutes .. getTimeUnitGrammar(minutes, 3, longVersion)
				response[#response + 1] = seconds .. getTimeUnitGrammar(seconds, 4, longVersion)
			elseif minutes > 0 then
				response[#response + 1] = minutes .. getTimeUnitGrammar(minutes, 3, longVersion)
				response[#response + 1] = seconds .. getTimeUnitGrammar(seconds, 4, longVersion)
			elseif seconds >= 0 then
				response[#response + 1] = seconds .. getTimeUnitGrammar(seconds, 4, longVersion)
			end
		end

		return table.concat(response, " ")
	end
end

if not globalStorageTable then
	globalStorageTable = {}
end

function Game.getStorageValue(key)
	return globalStorageTable[key] or -1
end

function Game.setStorageValue(key, value)
	globalStorageTable[key] = value
end

do
	local quests = {}
	local missions = {}
	local trackedQuests = {}

	function Game.getQuests() return quests end
	function Game.getMissions() return missions end
	function Game.getTrackedQuests() return trackedQuests end

	function Game.getQuestById(id) return quests[id] end
	function Game.getMissionById(id) return missions[id] end

	function Game.clearQuests()
		quests = {}
		missions = {}
		for playerId, _ in pairs(trackedQuests) do
			local player = Player(playerId)
			if player then
				player:sendQuestTracker({})
			end
		end
		trackedQuests = {}
		return true
	end

	function Game.createQuest(name, quest)
		if not isScriptsInterface() then
			return
		end

		if type(quest) == "table" then
			setmetatable(quest, Quest)
			quest.id = -1
			quest.name = name
			if type(quest.missions) ~= "table" then
				quest.missions = {}
			end

			return quest
		end

		quest = setmetatable({}, Quest)
		quest.id = -1
		quest.name = name
		quest.storageId = 0
		quest.storageValue = 0
		quest.missions = {}
		return quest
	end

	function Game.isQuestStorage(key, value, oldValue)
		for _, quest in pairs(quests) do
			if quest.storageId == key and quest.storageValue == value then
				return true
			end
		end

		for _, mission in pairs(missions) do
			if mission.storageId == key and value >= mission.startValue and value <= mission.endValue then
				return not mission.description or oldValue < mission.startValue or oldValue > mission.endValue
			end
		end
		return false
	end
end

function Game.getUnpromotedVocations()
	local vocations = {}
	for _, vocation in ipairs(Game.getVocations()) do
		if vocation == vocation:getFromVocation() then
			vocations[#vocations + 1] = vocation
		end
	end
	return vocations
end

local function getHousePaidUntil(rentPeriod)
	local day = 24 * 60 * 60
	if rentPeriod == RENTPERIOD_DAILY then
		return day
	elseif rentPeriod == RENTPERIOD_WEEKLY then
		return day * 7
	elseif rentPeriod == RENTPERIOD_MONTHLY then
		return day * 30
	elseif rentPeriod == RENTPERIOD_YEARLY then
		return day * 365
	end
	return 0
end

do
	function Game.getNameRentPeriodHouse(rentPeriod)
		if rentPeriod == RENTPERIOD_DAILY then
			return "daily"
		elseif rentPeriod == RENTPERIOD_WEEKLY then
			return "weekly"
		elseif rentPeriod == RENTPERIOD_MONTHLY then
			return "monthly"
		elseif rentPeriod == RENTPERIOD_YEARLY then
			return "annual"
		end
		return nil
	end

	function Game.getRentPeriodHouse(s)
		if s == "yearly" then
			return RENTPERIOD_YEARLY
		elseif s == "weekly" then
			return RENTPERIOD_WEEKLY
		elseif s == "monthly" then
			return RENTPERIOD_MONTHLY
		elseif s == "daily" then
			return RENTPERIOD_DAILY
		end
		return RENTPERIOD_NEVER
	end

	function Game.payHouses(rentPeriod)
		if rentPeriod == RENTPERIOD_NEVER then
			return
		end

		local currentTime = os.time()
		local paidUntil = currentTime + getHousePaidUntil(rentPeriod)
		local nameRentPeriod = getHouseRentPeriod(rentPeriod)

		for _, house in ipairs(Game.getHouses()) do
			local ownerGuid = house:getOwnerGuid()
			if ownerGuid == 0 then
				goto next
			end

			local rent = house:getRent()
			if rent == 0 or house:getPaidUntil() > currentTime then
				goto next
			end

			if not house:getTown() then
				goto next
			end

			local payRentWarnings = house:getPayRentWarnings()
			local houseName = house:getName()

			local offlinePlayer = false
			local player = Player(ownerGuid)
			if not player then
				offlinePlayer = true
				player = Game.loadPlayer(ownerGuid)
				if not player then
					house:setOwnerGuid(0)
					goto next
				end
			end

			local playerBalance = player:getBankBalance()
			if playerBalance >= rent then
				player:setBankBalance(playerBalance - rent)
				house:setPaidUntil(paidUntil)
				house:setPayRentWarnings(0)
			elseif payRentWarnings < 7 then
				local daysLeft = 7 - payRentWarnings
				
				local stampedLetter = Item(ITEM_LETTER_STAMPED)
				stampedLetter:setAttribute(ITEM_ATTRIBUTE_TEXT, "Warning! \nThe " .. nameRentPeriod .. " rent of " .. rent .. " gold for your house \"" .. houseName .. "\" is payable. Have it within " .. daysLeft .. " days or you will lose this house.")
				
				local playerInbox = player:getInbox()
				playerInbox:addItemEx(stampedLetter, INDEX_WHEREEVER, FLAG_NOLIMIT)

				house:setPayRentWarnings(payRentWarnings + 1);
			else
				house:setOwnerGuid(0)
			end

			player:save()
			if offlinePlayer then
				Game.unloadPlayer(player)
			end

			::next::
		end
	end
end

do
	local worldLightLevel = 0
	local worldLightColor = 0
	
	function Game.getWorldLight()
		return worldLightLevel, worldLightColor
	end
	
	function Game.setWorldLight(color, level)
		if not configManager.getBoolean(configKeys.DEFAULT_WORLD_LIGHT) then
			return
		end

		local previousColor = worldLightColor
		local previousLevel = worldLightLevel
		worldLightColor = color
		worldLightLevel = level

		if worldLightColor ~= previousColor or worldLightLevel ~= previousLevel then
			for _, player in ipairs(Game.getPlayers()) do
				player:sendWorldLight(worldLightColor, worldLightLevel)
			end
		end
	end
end

do
	local worldTime = 0

	function Game.getWorldTime()
		return worldTime
	end

	function Game.setWorldTime(time)
		worldTime = time

		-- quarter-hourly update to client clock near the minimap
		if worldTime % 15 == 0 then
			for _, player in ipairs(Game.getPlayers()) do
				player:sendWorldTime(worldTime)
			end
		end
	end
end
