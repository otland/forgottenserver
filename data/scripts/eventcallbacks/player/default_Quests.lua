local lastQuestUpdate = {}

local ec = EventCallback

function ec.onStorageUpdate(player, key, value, oldValue, isLogin)
	if value == -1 or isLogin then
		return
	end

	local playerId = player:getId()
	if not lastQuestUpdate[playerId] and Quest.isQuestStorage(key, value, oldValue) then
		lastQuestUpdate[playerId] = true
		addEvent(function(playerId) lastQuestUpdate[playerId] = nil end, 100, playerId)
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Your questlog has been updated.")
	end

	local trackedQuests = TrackedQuests.__list[playerId] or {}
	for mission, quest in pairs(trackedQuests) do
		if quest:isTracking(key, value) then
			player:sendUpdateQuestTracker(mission)
		end
	end
end

ec:register()

function ec.onQuestTracker(player, missionsId)
	player:sendQuestTracker(missionsId)
end

ec:register()

function ec.onQuestLog(player)
	player:sendQuestLog()
end

ec:register()

function ec.onQuestLine(player, questId)
	local quest = Quest(questId)
	if quest then
		player:sendQuestLine(quest)
	end
end

ec:register()

local cleanTrackedQuests = CreatureEvent("cleanTrackedQuests")

function cleanTrackedQuests.onLogout(player)
	TrackedQuests.__list[player:getId()] = nil
	return true
end

cleanTrackedQuests:register()
