local log = PacketHandler(0xF0)

function log.onReceive(player, msg)
	player:sendQuestLog()
end

log:register()

local line = PacketHandler(0xF1)

function line.onReceive(player, msg)
	local quest = Game.getQuestById(msg:getU16())
	if quest then
		player:sendQuestLine(quest)
	end
end

line:register()

local tracker = PacketHandler(0xD0)

function tracker.onReceive(player, msg)
	local missionsId = {}
	for i = 1, msg:getByte() do
		missionsId[i] = msg:getU16()
	end

	player:sendQuestTracker(missionsId)
end

tracker:register()
