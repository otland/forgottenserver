local bestiaryData = {
	-- dummy tables for very rare and none
	[-2] = {kills = {0, 0, 0}},
	[-1] = {kills = {2, 3, 5}},

	-- number of stars (from harmless to challenging)
	-- rarity 4 triggers rare values
	[0] = {kills = {5, 10, 25},  charmPoints = 1},
	[1] = {kills = {10, 100, 250}, charmPoints = 5},
	[2] = {kills = {25, 250, 500},  charmPoints = 15},
	[3] = {kills = {50, 500, 1000}, charmPoints = 25},
	[4] = {kills = {100, 1000, 2500}, charmPoints = 50},
	[5] = {kills = {200, 2000, 5000}, charmPoints = 100}
}

function MonsterType:getBestiaryKills()
	if self:raceId() == 0 then
		return bestiaryData[-2].kills
	end

	local info = self:bestiaryInfo()
	if info.occurrence == 3 then
		return bestiaryData[-1].kills
	end
	return bestiaryData[info.difficulty].kills
end

function MonsterType:getBestiaryCharmPoints()
	if self:raceId() == 0 then
		return 0
	end
	local info = self:bestiaryInfo()
	return bestiaryData[info.difficulty].charmPoints * (info.occurrence == 3 and 5 or 2)
end
