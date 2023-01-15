Quest = {}
Quest.__index = Quest
Quest.__list = {}
Quest.__count = 0

Mission = {}
Mission.__index = Mission
Mission.__list = {}
Mission.__count = 0

TrackedQuests = {}
TrackedQuests.__index = TrackedQuests
TrackedQuests.__list = {}

function Quest.getById(id) return Quest.__list[id] end
function Mission.getById(id) return Mission.__list[id] end
function TrackedQuests.getByMissionsId(player, missionsId)
	local playerId = player:getId()
	local maxTrackedQuests = player:getMaxTrackedQuests()
	TrackedQuests.__list[playerId] = {}
	local trackeds = 0
	local trackedQuests = TrackedQuests.__list[playerId]
	for _, missionId in pairs(missionsId or {}) do
		local mission = Mission(missionId)
		if mission and mission:isStarted(player) then
			trackeds = trackeds + 1
			trackedQuests[mission] = Quest(mission.questId)
			if trackeds >= maxTrackedQuests then
				break
			end
		end
	end
	return trackedQuests, trackeds
end

setmetatable(Quest, {__call = function(_, id) return Quest.getById(id) end})
setmetatable(Mission, {__call = function(_, id) return Mission.getById(id) end})
setmetatable(TrackedQuests, {__call = function(_, player, missionsId) return TrackedQuests.getByMissionsId(player, missionsId) end})

function Quest.clear()
	Quest.__list = {}
	Quest.__count = 0
	Mission.__list = {}
	Mission.__count = 0
	for key, _ in pairs(TrackedQuests.__list) do
		local player = Player(key)
		if player then
			player:sendQuestTracker(TrackedQuests(player))
		end
	end
	TrackedQuests.__list = {}
end

function Game.createQuest(name, quest)
	if not isScriptsInterface() then
		return
	elseif type(quest) == "table" then
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

function Quest:register()
	self.id = Quest.__count
	Quest.__count = Quest.__count + 1
	for _, mission in pairs(self.missions) do
		mission.id = Mission.__count
		Mission.__count = Mission.__count + 1
		mission.questId = self.id
		Mission.__list[mission.id] = setmetatable(mission, Mission)
	end
	Quest.__list[self.id] = self
	return true
end

function Quest:isStarted(player)
	return player:getStorageValue(self.storageId) >= self.storageValue
end

function Mission:isStarted(player)
	local value = player:getStorageValue(self.storageId)
	if value < self.startValue then
		return false
	end

	if not self.ignoreEndValue and value > self.endValue then
		return false
	end

	return true
end

function Quest:isCompleted(player)
	for _, mission in pairs(self.missions) do
		if not mission:isCompleted(player) then
			return false
		end
	end
	return true
end

function Mission:isCompleted(player)
	if self.ignoreEndValue then
		return player:getStorageValue(self.storageId) >= self.endValue
	end
	return player:getStorageValue(self.storageId) == self.endValue
end

function Mission:getName(player)
	if self:isCompleted(player) then
		return string.format("%s (Completed)", self.name)
	end
	return self.name
end

function Mission:getDescription(player)
	if type(self.description) == "function" then
		return self.description(player)
	end

	local value = player:getStorageValue(self.storageId)
	if self.mainDescription then
		local description = self.mainDescription:gsub("|STATE|", value)
		description = self.mainDescription:gsub("\\n", "\n")
		return description
	end

	if self.ignoreEndValue then
		for current = self.endValue, self.startValue, -1 do
			if value >= current then
				return self.descriptions[current]
			end
		end
	else
		for current = self.endValue, self.startValue, -1 do
			if value == current then
				return self.descriptions[current]
			end
		end
	end

	return "An error has occurred, please contact a gamemaster."
end

function Quest:getMissions(player)
	local missions = {}
	for _, mission in pairs(self.missions) do
		if mission:isStarted(player) then
			missions[#missions + 1] = mission
		end
	end
	return missions
end

function Quest.getQuests(player)
	local quests = {}
	for _, quest in pairs(Quest.__list) do
		if quest:isStarted(player) then
			quests[#quests + 1] = quest
		end
	end
	return quests
end

function Quest:isTracking(key, value)
	if self.storageId == key and value == self.storageValue then
		return true
	end

	for _, mission in pairs(self.missions) do
		if mission.storageId == key and value >= mission.startValue and value <= mission.endValue then
			return true
		end
	end
	return false
end

function Quest.isQuestStorage(key, value, oldValue)
	for _, quest in pairs(Quest.__list) do
		if quest.storageId == key and quest.storageValue == value then
			return true
		end
	end

	for _, mission in pairs(Mission.__list) do
		if mission.storageId == key and value >= mission.startValue and value <= mission.endValue then
			return not mission.mainDescription or oldValue < mission.startValue or oldValue > mission.endValue
		end
	end
	return false
end
