local raid = GlobalEvent("Testraid")
raid:interval(1800)

local function event0()
	Game.broadcastMessage("Rats are attacking near Trekolt Temple!", MESSAGE_STATUS_WARNING)
end

local function event1()
	local center, radius, z = {x=94, y=126}, 5, 7
	for _ = 1, 3 do
		local x, y = math.random(center.x - radius, center.x + radius), math.random(center.y - radius, center.y + radius)
		Game.createMonster("Rat", Position(x, y, z))
	end
end

local function event2()
	Game.createMonster("Cave Rat", Position(93, 123, 7))
end

local function event3()
	Game.broadcastMessage("Rats attack continues!", MESSAGE_STATUS_WARNING)
end

local function event4()
	local from, to, z = {x=89, y=122}, {x=99, y=130}, 7
	for _ = 1, math.random(4, 10) do
		local x, y = math.random(from.x, to.x), math.random(from.y, to.y)
		Game.createMonster("Rat", Position(x, y, z))
	end
end

local function event5()
	Game.createMonster("Cave Rat", Position(98, 125, 7))
end

local function event6()
	Game.createMonster("Cave Rat", Position(94, 128, 7))
end

function raid.onTime(interval)
	addEvent(event0, 1000)
	addEvent(event1, 2000)
	addEvent(event2, 15000)
	addEvent(event3, 25000)
	addEvent(event4, 30000)
	addEvent(event5, 30000)
	addEvent(event6, 30000)
	return true
end

raid:register()
