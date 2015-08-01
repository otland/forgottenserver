function onTime(interval)
	-- Message broadcast
	addEvent(Game.broadcastMessage, 1000, "Rats are attacking neart Trekolt Temple!", MESSAGE_STATUS_WARNING)
	addEvent(Game.broadcastMessage, 25000, "Rats attack continues!", MESSAGE_STATUS_WARNING)

	-- Area spawns - you can use a center and radius style or using fromPos/toPos style
	local centerPos, radius = Position(94, 126, 7), 5
	for _ = 1, 3 do
		local x = math.random(centerPos.x - radius, centerPos.x + radius)
		local y = math.random(centerPos.y - radius, centerPos.y + radius)
		addEvent(Game.createMonster, 2000, "Rat", Position(x, y, 7))
	end

	-- You can generate a random amount of creatures
	local amount = math.random(4, 10)
	for _ = 1, amount do
		local x, y = math.random(89, 99), math.random(122, 130)
		addEvent(Game.createMonster, 30000, "Rat", Position(x, y, 7))
	end

	-- Single spawns
	addEvent(Game.createMonster, 15000, "Cave Rat", Position(93, 123, 7))
	addEvent(Game.createMonster, 30000, "Cave Rat", Position(98, 125, 7))
	addEvent(Game.createMonster, 30000, "Cave Rat", Position(94, 128, 7))

	return true
end
