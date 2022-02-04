function onThink(interval)

-- Welcome to the raid script!
-- set an interval event on globalevents.xml with a time for the script to run. keep in mind that the script can run without a raid actually occurring, so a
-- interval of 30 minutes is advised.
-- "broadcast" is if your raid has a broadcast msg, value true or false
-- "msg" is the broadcasting msg to warn the players about this raid occurring; if your raid is a silent one (without a broadcast msg), leave it blank!
-- "monsters" is the list of the one or more monsters that will be spawned
-- "haveboss" is if your raid have a boss or a unique monster to be spawned with
-- "boss" is the name boss/unique monster name to be spawned; if your raid doesn't have a boss, leave it blank!
-- "amountmin" and "amountmax" is the min and max number of monsters that will be spawned
-- "chance" is the chance of this raid of actually happening, between 1 and 100, 100 being 100% chance of occurring if the raid gets selected
-- "pos" is the center position of where your raid occurs
-- "radius" is the maximum distance xy that your monster(s) can appear from the center pos. think of it like the spawn size on map editor
-- read the example raids below for further details on how the script works

local raids = {
    [1] = {	broadcast = true, --Example raid with monsters and a boss monster
			msg = "Rats are attacking near Trekolt Temple!",
			monsters = {"Rat", "Cave Rat"},
			haveboss = true,
			boss = "Munster",
			amountmin = 7,
			amountmax = 10,
			chance = 100,
			pos = {x=94, y=126, z=7},
			radius = 3},
			
    [2] = {	broadcast = true, --Example raid with only monsters
			msg = "Orcs are attacking Rhyves walls!",
			monsters = {"Orc", "Orc Shaman", "Orc Warrior", "Orc Spearman", "Orc Rider", "Orc Berserker"},
			haveboss = false,
			boss = "",
			amountmin = 15,
			amountmax = 20,
			chance = 80,
			pos = {x=134, y=426, z=7},
			radius = 10},
			
    [3] = {	broadcast = false, --Example raid with only a boss, silent raid
			msg = "",
			monsters = {"Man in the Cave"}, --should be specified here instead on the boss var, which is only used when a raid occurs with monsters and a boss together
			haveboss = false,
			boss = "",
			amountmin = 1,
			amountmax = 1,
			chance = 50,
			pos = {x=188, y=538, z=10},
			radius = 1}
    }

	local selectRaid = math.random(1, #raids)
	local chance = math.random(1, 100)
	local raid = raids[selectRaid]

	if chance > raid.chance then
		return true
	end

	if raid.broadcast == true then
		broadcastMessage(raid.msg, MESSAGE_EVENT_ADVANCE)
	end

	if (raid.amountmax == 1) or (raid.amountmax == raid.amountmin) then
		maxi = raid.amountmax
	else
		maxi = math.random(raid.amountmin, raid.amountmax)
	end

	local pos = Position(raid.pos)
	local radius = raid.radius

	for _ = 1, maxi do
		local spawn = raid.monsters[math.random(#raid.monsters)]
		local randomx = math.random(-radius, radius)
		local randomy = math.random(-radius, radius)
		spawnRadius = Position(pos.x+randomx, pos.y+randomy, pos.z)

		monster = Game.createMonster(spawn, spawnRadius, true, false)
		if monster then
		spawnRadius:sendMagicEffect(CONST_ME_TELEPORT)
		end
	end

	if raid.haveboss == true then
		Game.createMonster(raid.boss, pos, true, true)
		pos:sendMagicEffect(CONST_ME_TELEPORT)
	end
return true
end
