-- Classic example of XML/quest.xml transcribed to Lua
local questI = Game.createQuest("Example Quest I", 1001, 1)

questI:createMission("Example Mission 1", 1001, 1, 3, true, {
    { id = 1, description = "Example description 1" },
    { id = 2, description = "Example description 2" },
    { id = 3, description = "Example description 3" }
})

questI:createMission("Example Mission 2", 1001, 4, 5, false, {
    { id = 4, description = "Example description 1" },
    { id = 5, description = "Example description 2" }
})

questI:register()
