local quest = Game.createQuest("Example Quest II", 1002, 1)

quest:createMission("Example Mission 1", 1002, 1, 10, false, function(player)
    return string.format("Example description %d by %s.", player.storage[1002], player:getName())
end)

quest:register()
