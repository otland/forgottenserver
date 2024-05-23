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
