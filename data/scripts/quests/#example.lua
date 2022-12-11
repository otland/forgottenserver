local quest = Game.createQuest("Example Quest II")
quest.storageId = 1002
quest.storageValue = 1

local firstMission = quest:createMission("Example Mission 1")
firstMission.storageId = 1002
firstMission.startValue = 1
firstMission.endValue = 10
firstMission.ignoreEndValue = false
firstMission.description = function(player)
    return string.format("Example description %d by %s.", player.storage[1002], player:getName())
end

local secondMission = quest:createMission("Example Mission 2")
secondMission.storageId = 1003
secondMission.startValue = 1
secondMission.endValue = 3
secondMission.ignoreEndValue = false
secondMission.description = "Mission 2"

quest:register()
