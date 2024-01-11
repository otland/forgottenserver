Quest = {}
Quest.__index = Quest

function Quest:register()
	local quests = Game.getQuests()
	local missions = Game.getMissions()

	self.id = #quests + 1

	for _, mission in pairs(self.missions) do
		mission.id = #missions + 1
		mission.questId = self.id
		missions[mission.id] = setmetatable(mission, Mission)
	end

	quests[self.id] = self
	return true
end

function Quest:isStarted(player)
	return player:getStorageValue(self.storageId) >= self.storageValue
end

function Quest:isCompleted(player)
	for _, mission in pairs(self.missions) do
		if not mission:isCompleted(player) then
			return false
		end
	end
	return true
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

Mission = {}
Mission.__index = Mission

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
	local descriptionType = type(self.description)
	if descriptionType == "function" then
		return self.description(player)
	end

	local value = player:getStorageValue(self.storageId)
	if descriptionType == "string" then
		local description = self.description:gsub("|STATE|", value)
		description = self.description:gsub("\\n", "\n")
		return description
	end

	if descriptionType == "table" then
		if self.ignoreEndValue then
			for current = self.endValue, self.startValue, -1 do
				if value >= current then
					return self.description[current]
				end
			end
		else
			for current = self.endValue, self.startValue, -1 do
				if value == current then
					return self.description[current]
				end
			end
		end
	end

	return "An error has occurred, please contact a gamemaster."
end
