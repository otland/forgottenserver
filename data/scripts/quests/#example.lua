Game.createQuest("Example Quest II", {
	storageId = 1002,
	storageValue = 1,

	missions = {
		{
			name = "Example Mission 1",
			storageId = 1002,
			startValue = 1,
			endValue = 10,
			ignoreEndValue = false,
			description = function(player)
				return string.format("Example description %d by %s.", player.storage[1002], player:getName())
			end
		},
		{
			name = "Example Mission 2",
			storageId = 1003,
			startValue = 1,
			endValue = 3,
			ignoreEndValue = false,
			description = "Mission 2"
		}
	}
}):register()
