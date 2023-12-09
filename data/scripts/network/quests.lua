local log = PacketHandler(CLIENT_REQUEST_QUEST_LOG)

function log.onReceive(player, msg)
	player:sendQuestLog()
end

log:register()

local line = PacketHandler(CLIENT_REQUEST_QUEST_LINE)

function line.onReceive(player, msg)
	local quest = Game.getQuestById(msg:getU16())
	if quest then
		player:sendQuestLine(quest)
	end
end

line:register()

local tracker = PacketHandler(CLIENT_REQUEST_QUEST_TRACKER)

function tracker.onReceive(player, msg)
	local missionsId = {}
	for i = 1, msg:getByte() do
		missionsId[i] = msg:getU16()
	end

	player:sendQuestTracker(missionsId)
end

tracker:register()
