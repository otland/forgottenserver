local lastQuestUpdate = {}

local ec = Event()

ec.onUpdateStorage = function(creature, key, value, oldValue, isSpawn)
	if isSpawn then
		return
	end

	local player = Player(creature:getId())
	if not player then
		return
	end

	local playerId = player:getId()
	local now = os.mtime()
	if not lastQuestUpdate[playerId] then
		lastQuestUpdate[playerId] = now
	end

	if lastQuestUpdate[playerId] - now <= 0 and Game.isQuestStorage(key, value, oldValue) then
		lastQuestUpdate[playerId] = os.mtime() + 100
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Your questlog has been updated.")
	end

	local trackedQuests = Game.getTrackedQuests()[playerId] or {}
	for mission, quest in pairs(trackedQuests) do
		if quest:isTracking(key, value) then
			player:sendUpdateQuestTracker(mission)
		end
	end
end

ec:register()
