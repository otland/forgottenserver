MAX_TORNADO_AREAS = 5
MAX_MINOR_TORNADO_AREAS = 100
INTERVAL_MINOR_TORNADO = 300

-- This function will create a new random combat area based on area passed by parameter
local function generateRandomArea(base)
	local areaUpdated = {}

	for i = 1, 7 do
		areaUpdated[i] = {}

		for j = 1, 7 do
			areaUpdated[i][j] = base[i][j]

			if base[i][j] == 1 then
				areaUpdated[i][j] = math.random(0, 1)
			end
		end
	end

	return createCombatArea(areaUpdated)
end

-- Emit tornado effect randomly
local function emitTornadoEvent(cid, randomList, maxRandom)
	local randomAreaIndex = math.random(1, maxRandom)
	local pos = {x=getPlayerPosition(cid).x, y=getPlayerPosition(cid).y, z=getPlayerPosition(cid).z}

	doAreaCombatHealth(cid, COMBAT_ICEDAMAGE, pos, randomList[randomAreaIndex], 10, 20, CONST_ME_ICETORNADO)
end

-- Create random areas
local area = {}
for i = 1, MAX_TORNADO_AREAS do
	area[i] = generateRandomArea(AREA_TORNADO)
end

-- Create random minor areas
local minorArea = {}
for i = 1, MAX_MINOR_TORNADO_AREAS do
	minorArea[i] = generateRandomArea(AREA_MINOR_TORNADO)
end

function onCastSpell(creature, variant)
	addEvent(emitTornadoEvent, 0, creature.uid, area, MAX_TORNADO_AREAS)
	addEvent(emitTornadoEvent, 500, creature.uid, area, MAX_TORNADO_AREAS)
	addEvent(emitTornadoEvent, 1000, creature.uid, area, MAX_TORNADO_AREAS)

	for i = 1, 6 do
		addEvent(emitTornadoEvent, INTERVAL_MINOR_TORNADO * i, creature.uid, minorArea, MAX_MINOR_TORNADO_AREAS)
	end

	return true
end
