local questII = Game.createQuest("Example Quest II", 1002, 1)

questII:createMission("Example Mission 1", 1002, 1, 10, false, function (player)
    return string.format("Example description %d by %s.", player.storage[1002], player:getName())
end)

questII:register()

-- Example static description
local questIII = Game.createQuest("Example Quest III", 1003, 1)

questIII:createMission("Example Mission III", 1003, 1, 3, true, "main Description.")

questIII:register()
